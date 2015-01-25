// VMeter_v0Dlg.cpp : implementation file
//




#include "stdafx.h"
#include "VMeter_v0.h"
#include "VMeter_v0Dlg.h"
#include "afxdialogex.h"
#include "SoundC.h"
#include "ShortMsg.h"
#include "midi.h"
#include "Resource.h"
#include "MIDI_Devices_Dialog.h"
#include <cstdlib>
#include <Pdh.h>
//#include "RtMidi.h"

#define WM_UPDATE_MYCONTROL WM_APP + 0x10 
#define MIDI_CONTROLLER_NUM_LEDS 20
#define MIDI_CONTROLLER_NUM_BRIGHTNESS 21
#define MIDI_CONTROLLER_PARAMS 119
#define UPSIDE_DOWN_OFF 126
#define UPSIDE_DOWN_ON 125

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SCROLL_BAR_SIZE 255
#define VMETER_LED_SIZE 255

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CVMeter_v0Dlg dialog




CVMeter_v0Dlg::CVMeter_v0Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVMeter_v0Dlg::IDD, pParent)
	, m_VolText(_T(""))
	, m_sStatus(_T("Connection Status: ..."))
	, brightness_text(_T(""))
	, upside_down_box_value(FALSE)
	, m_midi_status_text(_T(""))
	, vmeter_output_type(0)
	, scale_by_system_volume(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(ID_VMETER_ICON);
	m_hMixer = NULL;
}

void CVMeter_v0Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_VOLUME, m_Volume);
	DDX_Control(pDX, IDC_VOL_SLIDER, m_VolTrack);
	//	DDX_Text(pDX, IDC_VOLUME, m_VolText);
	DDX_Text(pDX, IDC_STATUS, m_sStatus);
	DDX_Control(pDX, IDC_BRIGHTNESS_SLIDER, brightness_slider);
	DDX_Text(pDX, IDC_BRIGHTNESS_TEXT, brightness_text);
	DDX_Text(pDX, IDC_STATIC_MIDI_DATA_ACTIVITY, m_midi_status_text);
	DDX_Check(pDX, IDC_CHECK1, upside_down_box_value);
	//	DDX_Control(pDX, IDC_COMBO_POS_CTRL, m_PosOutputCtrlNum);
	//	DDX_Control(pDX, IDC_CHECK_POS_OUTPUT_EN, m_EnablePosOutputCheckBox);
	//DDX_Control(pDX, IDC_COMBO_MIDI_CHAN, m_MidiChannelCombo);
	//DDX_Control(pDX, IDC_CHECK_PRES_OUT_EN, m_EnPressureOutput);
	//DDX_Control(pDX, IDC_CHECK_ON_OFF_EN, m_EnTouchOnOff);
	//DDX_Control(pDX, IDC_CHECK_RAW_OUT_EN, m_EnRawOutput);
	//DDX_Control(pDX, IDC_CHECK_PITCH_WHEEL_EN, m_EnPitchOut);
	//DDX_Control(pDX, IDC_CHECK_TOUCH_VOL_EN, m_EnSysVolControl);
	//DDX_Control(pDX, IDC_CHECK_MIDI_LED_EN, m_EnMidiLedPosControl);
	//DDX_Control(pDX, IDC_CHECK_MIDI_BRIGHT_EN, m_EnMidiBrightessControl);
	//DDX_Control(pDX, IDC_CHECK_IGNORE_TOUCH, m_IgnoreTouch);
	//DDX_Control(pDX, IDC_CHECK_LED_SHOW_VOLUME_EN, m_LedShowVolume);
	//	DDX_Control(pDX, IDC_COMBO_PRES_CTRL, m_PresMidiCtrlNumCombo);
	//	DDX_Control(pDX, IDC_COMBO_ON_OFF_CTRL, m_TouchOnOffMidiCtrlNumCombo);
	//	DDX_Control(pDX, IDC_COMBO_LED_MIDI_POS_CTRL, m_LedPosMidiCtrlNumCombo);
	//	DDX_Control(pDX, IDC_COMBO_LED_BRIGHT_CTRL, m_BrightnessMidiCtrlNumCombo);
	//	DDX_Control(pDX, IDC_COMBO_PRES_MIN, m_PresMinCombo);
	//	DDX_Control(pDX, IDC_COMBO_PRES_MAX, m_PresMaxCombo);
	//	DDX_Control(pDX, IDC_COMBO_RAW_MIN, m_RawMinCombo);
	//	DDX_Control(pDX, IDC_COMBO_RAW_MAX, m_RawMaxCombo);

	DDX_Radio(pDX, IDC_RADIO1, vmeter_output_type);
	DDX_Check(pDX, IDC_CHECK2, scale_by_system_volume);
}

BEGIN_MESSAGE_MAP(CVMeter_v0Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BUTTON1, &CVMeter_v0Dlg::OnClickedButton1)
//	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_MESSAGE(MM_MIXM_CONTROL_CHANGE, OnMixerCtrlChange)
//	ON_BN_CLICKED(IDC_BUTTON2, &CVMeter_v0Dlg::OnBnClickedButton2)
	ON_COMMAND(ID_TOOLS_UPDATEFIRMWARE, &CVMeter_v0Dlg::OnToolsUpdatefirmware)
	ON_COMMAND(ID_FILE_EXIT, &CVMeter_v0Dlg::OnFileExit)
	ON_COMMAND(ID_TOOLS_RECALIBRATE, &CVMeter_v0Dlg::OnToolsRecalibrate)
	ON_COMMAND(ID_TOOLS_RECORDDEBUGFILE, &CVMeter_v0Dlg::OnToolsRecorddebugfile)
	ON_COMMAND(ID_HELP_ABOUT, &CVMeter_v0Dlg::OnHelpAbout)
	ON_COMMAND(ID_HELP_OPENWEBSITEHELP, &CVMeter_v0Dlg::OnHelpOpenwebsitehelp)
//	ON_BN_CLICKED(IDC_BUTTON3, &CVMeter_v0Dlg::OnBnClickedButton3)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK1, &CVMeter_v0Dlg::OnBnClickedCheck1)
	
	ON_BN_CLICKED(IDC_testhide, &CVMeter_v0Dlg::OnBnClickedHide)
	ON_COMMAND(ID_EDIT_MIDIDEVICEROUTING, &CVMeter_v0Dlg::OnEditMididevicerouting)
//	ON_BN_CLICKED(IDC_CHECK_POS_OUTPUT_EN, &CVMeter_v0Dlg::EnablePosMidiOutputChanged)
//	ON_CBN_SELCHANGE(IDC_COMBO_POS_CTRL, &CVMeter_v0Dlg::OnPosOutputChannelChange)
//	ON_CBN_SELCHANGE(IDC_COMBO_MIDI_CHAN, &CVMeter_v0Dlg::MidiOutputChannelChanged)
	
	//ON_BN_CLICKED(IDC_CHECK_PRES_OUT_EN, &CVMeter_v0Dlg::OnBnClickedCheckPresOutEn)
	//ON_BN_CLICKED(IDC_CHECK_ON_OFF_EN, &CVMeter_v0Dlg::OnBnClickedCheckOnOffEn)
	//ON_BN_CLICKED(IDC_CHECK_RAW_OUT_EN, &CVMeter_v0Dlg::OnBnClickedCheckRawOutEn)
	//ON_BN_CLICKED(IDC_CHECK_PITCH_WHEEL_EN, &CVMeter_v0Dlg::OnBnClickedCheckPitchWheelEn)
	//ON_BN_CLICKED(IDC_CHECK_TOUCH_VOL_EN, &CVMeter_v0Dlg::OnBnClickedCheckTouchVolEn)
	//ON_BN_CLICKED(IDC_CHECK_MIDI_LED_EN, &CVMeter_v0Dlg::OnBnClickedCheckMidiLedEn)
	//ON_BN_CLICKED(IDC_CHECK_MIDI_BRIGHT_EN, &CVMeter_v0Dlg::OnBnClickedCheckMidiBrightEn)
	//ON_BN_CLICKED(IDC_CHECK_IGNORE_TOUCH, &CVMeter_v0Dlg::OnBnClickedCheckIgnoreTouch)
	//ON_BN_CLICKED(IDC_CHECK_LED_SHOW_VOLUME_EN, &CVMeter_v0Dlg::OnBnClickedCheckLedShowVolumeEn)
//	ON_CBN_SELCHANGE(IDC_COMBO_PRES_CTRL, &CVMeter_v0Dlg::OnCbnSelchangeComboPresCtrl)
//	ON_CBN_SELCHANGE(IDC_COMBO_PRES_MIN, &CVMeter_v0Dlg::OnCbnSelchangeComboPresMin)
//	ON_CBN_SELCHANGE(IDC_COMBO_PRES_MAX, &CVMeter_v0Dlg::OnCbnSelchangeComboPresMax)
//	ON_CBN_SELCHANGE(IDC_COMBO_ON_OFF_CTRL, &CVMeter_v0Dlg::OnCbnSelchangeComboOnOffCtrl)
//	ON_CBN_SELCHANGE(IDC_COMBO_RAW_MIN, &CVMeter_v0Dlg::OnCbnSelchangeComboRawMin)
//	ON_CBN_SELCHANGE(IDC_COMBO_RAW_MAX, &CVMeter_v0Dlg::OnCbnSelchangeComboRawMax)
//	ON_CBN_SELCHANGE(IDC_COMBO_LED_MIDI_POS_CTRL, &CVMeter_v0Dlg::OnCbnSelchangeComboLedMidiPosCtrl)
//	ON_CBN_SELCHANGE(IDC_COMBO_LED_BRIGHT_CTRL, &CVMeter_v0Dlg::OnCbnSelchangeComboLedBrightCtrl)
	ON_MESSAGE(WM_UPDATE_MYCONTROL, &CVMeter_v0Dlg::myUpdateData)
	ON_COMMAND(ID_TOOLS_LOADDEFAULTS, &CVMeter_v0Dlg::OnToolsLoaddefaults)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CVMeter_v0Dlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON4, &CVMeter_v0Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_RADIO1, &CVMeter_v0Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CVMeter_v0Dlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_CHECK2, &CVMeter_v0Dlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CPU_RADIO, &CVMeter_v0Dlg::OnBnClickedCpuRadio)
END_MESSAGE_MAP()


// CVMeter_v0Dlg message handlers

BOOL CVMeter_v0Dlg::OnInitDialog()
{
	int j = 0;
	DWORD temp2 = 0;
	CDialogEx::OnInitDialog();
	OutputDebugString((LPCWSTR)L"test_func 255\n");
	// Add "About..." menu item to system menu.
	upside_down = 0;
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	touch_active = 0;
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	m_screen_update_counter = 0;
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	CMenu *pm = new CMenu();

    pm->LoadMenu(IDR_MENU1);

    SetMenu(pm);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	

	//for (j=0;j<CALC_POS_ARRAY_SIZE;j++) {
	//	calculated_pos_array[j] = -2;
	//}
	//calculated_pos_index = 0;
	//m_VolTrack.SetPos(0);

	lines_to_write = -1;
	led_brightness = 16;
	brightness_text.Format(_T("Brightness: %d"),led_brightness);

	//SIMPLE INTIALIZATION
	m_VolTrack.SetRange(0,SCROLL_BAR_SIZE);
	brightness_slider.SetRange(0,16);
	m_fRange = (float)m_dwMaximum;
	// get the initial value of the Master Volume control? Skipped - timer should take care
	prev_volume = 0.0;
	prev_direction = 0;
	VERIFY(this->UpdateData(FALSE));

	//handle = NULL;
	//CheckUsb();
	m_iCounter = 0;
//	OnStartTimer();


	pSysMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND);
	//pSysMenu->EnableMenuItem (SC_CLOSE, MF_BYCOMMAND|MF_DISABLED); 
	//pSysMenu->RemoveMenu(WS_MINIMIZEBOX, MF_BYCOMMAND);
	//pSysMenu->RemoveMenu(WS_MAXIMIZEBOX, MF_BYCOMMAND);

	// try opening midi:
	
	touch_state = 0;
	m_iCounter = 1;
	
	volume_changed_cycles_ago = 200;
	vmeter_touched_cycles_ago = 200;

	if (IsVistaOrLater()) {
		// peak meter init
		HRESULT hr;
		IMMDeviceEnumerator *pEnumerator = NULL;
		IMMDevice *pDevice = NULL;		
		CoInitialize(NULL);
		// Get enumerator for audio endpoint devices.
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
							  NULL, CLSCTX_INPROC_SERVER,
							  __uuidof(IMMDeviceEnumerator),
							  (void**)&pEnumerator);
		// Get peak meter for default audio-rendering device.
		hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
		 hr = pDevice->Activate(__uuidof(IAudioMeterInformation),
							   CLSCTX_ALL, NULL, (void**)&pMeterInfo);

		 vmeter_output_type = read_output_mode_from_registry();
		 if (vmeter_output_type == 0) // volume setting
			 GetDlgItem( IDC_CHECK2 )->EnableWindow(FALSE); // disable the scale check box.
		 scale_by_system_volume = read_scale_vumeter_mode_from_registry();
	}
	else {
		GetDlgItem( IDC_RADIO2 )->EnableWindow(FALSE); // disable the VU Meter display radio
		GetDlgItem( IDC_CHECK2 )->EnableWindow(FALSE); // disable scale option
	}


	// start timer to call usb_read:
	start_usb_read_timer();

	init_configuration(); // load default values or from config file
	
	//midi_has_been_initialized = 0;
	ave_pressure= 0.0;
	myhWnd = GetSafeHwnd();
	init_midi(); 
	float temp_volume = GetMasterVolume();
	DWORD VolumePos = DWORD(255 - (int)(temp_volume*255.0));
	//TCHAR str[128];
	//_stprintf(str, _T("init volume: %f\n"),temp_volume);
	//OutputDebugString(str);
	m_VolTrack.SetPos(VolumePos);
	::PostMessage(myhWnd,WM_UPDATE_MYCONTROL,0,0);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CVMeter_v0Dlg::init_midi() {
	TCHAR str[128];
	TCHAR compare_str[10];
	TCHAR compare_str2[20];
	int i;

	_stprintf(compare_str, _T("VMeter"));
	_stprintf(compare_str2, _T("USB Audio Device"));
	// attempt to open MIDI devices
	try
    {
        // If there are any MIDI output devices available, open one.
        if(midi::CMIDIOutDevice::GetNumDevs() > 0)
        {
            // try to select a device with VMeter in the name, or Audio, or just the first device.
			MIDIOUTCAPS OutCaps;
			for(i = 0; i < midi::CMIDIOutDevice::GetNumDevs(); i++)
			{
				midi::CMIDIOutDevice::GetDevCaps(i, OutCaps);
				//m_InDevsCombo.AddString(InCaps.szPname);
				_stprintf(str, _T("%s\n"),OutCaps.szPname);
				OutputDebugString(str);
				if ( _tcsncmp(compare_str,str,6) == 0 || _tcsncmp(compare_str2,str,11) == 0 ) {
					m_OutDevice.Open(i);	
					_stprintf(str, _T("found Out VMeter, open: %d\n"), m_OutDevice.IsOpen());
					OutputDebugString(str);
										
					break;
				}

			}		
        }
        // Else there are no MIDI output devices available.
      /*  else
        {
            MessageBox(L"No MIDI output devices available.", L"Warning", 
                MB_ICONWARNING | MB_OK);
        }*/

        // If there are any MIDI input devices available, open one and begin
        // recording.
        if(midi::CMIDIInDevice::GetNumDevs() > 0)
        {
			MIDIINCAPS InCaps;
			for(i = 0; i < midi::CMIDIInDevice::GetNumDevs(); i++)
			{
				midi::CMIDIInDevice::GetDevCaps(i, InCaps);
				_stprintf(str, _T("%s\n"),InCaps.szPname);
				OutputDebugString(str);
				if (_tcsncmp(compare_str,str,6) == 0 || _tcsncmp(compare_str2,str,11) == 0) {
					_stprintf(str, _T("found IN VMeter\n"));
					OutputDebugString(str);
					_stprintf(str, _T("%s\n"),InCaps.szPname); // reusing the str again for the display
					m_InDevice.Open(i);						
					m_InDevice.SetReceiver(*this);
					m_InDevice.StartRecording(); 
					connection_check = true;
					break;
				}   
			}
        }
        // Else there are no MIDI input devices available.
        /*else
        {
            MessageBox(L"No MIDI input devices available.", L"Warning", 
                MB_ICONWARNING | MB_OK);
        }*/
		if (m_OutDevice.IsOpen() && m_InDevice.IsOpen() && connection_check) {
			m_sStatus.Format(_T("Connected to %s"), str);
			// get current upside_down status, brightness
			send_midi_controller_msg(119,113); // this dumps all settings
			// send current volume
			send_midi_controller_msg(MIDI_CONTROLLER_NUM_LEDS,GetMasterVolume()*127.0);

		}
		else {
			m_sStatus.Format(_T("No VMeter connected..."));
		}
		::PostMessage(myhWnd,WM_UPDATE_MYCONTROL,0,0);

    }
    catch(const std::exception &ex)
    {
        MessageBox(L"Error opening MIDI devices.", L"Warning", 
                MB_ICONWARNING | MB_OK);
    }
}


// this function merely calls the actual function that's a method of the dialog object.  It's here
// because we can't specify a non-static (?) function for the callback in the CreateTimerQueueTimer function.
void CALLBACK TimerProc(void* lpParametar,BOOLEAN TimerOrWaitFired) {
    CVMeter_v0Dlg* obj = (CVMeter_v0Dlg*) lpParametar;
    obj->read_usb_periodically();
	//obj->UpdateData(FALSE);
} 

void CVMeter_v0Dlg::start_usb_read_timer() {
	BOOL result;
	DWORD Period = 4;
	result = CreateTimerQueueTimer(&m_timerHandle, NULL , 
	TimerProc, this, 0, 
	Period, WT_EXECUTEINTIMERTHREAD); // this last parameter specifies how what thread to use to call your function.
		// I've read that the executeintimerthread is dangerous if the calling funciton can block, but I'm not sure
		// how dangerous since there's only this function to call... i'm never doing anything else. it's ok if it's late.

}



// load default values (or from config file) into dialog
// possible sending settings out to VMeter (like upside down, or ignore touch)..
// actually that should probably happen during connect.
void CVMeter_v0Dlg::init_configuration() {
	// default config:
	// default enable states:
	pos_touch_output_enabled = 1;
	pressure_output_enabled = 0;
	touch_on_off_output_enabled =0;
	raw_output_enabled = 0;
	pitch_wheel_enable = 0;
	volume_control_enabled = 0;
	led_control_from_pos_enabled =0;
	midi_brightness_control_enabled = 0;
	ignore_touch  = 0;
	led_show_system_vol_enabled =0;

	// default ctrl and channel numbers:
	midi_channel = 0;
	pos_touch_output_ctrl_num = 0;
	pressure_output_ctrl_num = 1;
	touch_on_off_output_ctrl_num = 2;
	led_control_source_ctrl_num = 0;
	midi_brightness_ctrl_num = 1;

	pressure_output_min = 0;
	pressure_output_max = 127;
	raw_output_min = 0;
	raw_output_max = 127;


	
	// update dialog with variable state
	//m_MidiChannelCombo.SetCurSel(midi_channel);

	// default check boxes:	
	//m_EnablePosOutputCheckBox.SetCheck(pos_touch_output_enabled);	
	//m_EnPressureOutput.SetCheck(pressure_output_enabled);	
	//m_EnTouchOnOff.SetCheck(touch_on_off_output_enabled);	
	//m_EnRawOutput.SetCheck(raw_output_enabled);	
	//m_EnPitchOut.SetCheck(pitch_wheel_enable);	
	//m_EnSysVolControl.SetCheck(volume_control_enabled);	
	//m_EnMidiLedPosControl.SetCheck(led_control_from_pos_enabled);	
	//m_EnMidiBrightessControl.SetCheck(midi_brightness_control_enabled);	
	//m_IgnoreTouch.SetCheck(ignore_touch);	
	//m_LedShowVolume.SetCheck(led_show_system_vol_enabled);

	// default combo boxes:	
	//m_PosOutputCtrlNum.SetCurSel(pos_touch_output_ctrl_num);		
	//m_PresMidiCtrlNumCombo.SetCurSel(pressure_output_ctrl_num);	
	//m_TouchOnOffMidiCtrlNumCombo.SetCurSel(touch_on_off_output_ctrl_num);	
	//m_LedPosMidiCtrlNumCombo.SetCurSel(led_control_source_ctrl_num);	
	//m_BrightnessMidiCtrlNumCombo.SetCurSel(midi_brightness_ctrl_num);	
	//m_PresMinCombo.SetCurSel(pressure_output_min);
	//m_PresMaxCombo.SetCurSel(pressure_output_max);	
	//m_RawMinCombo.SetCurSel(raw_output_min);	
	//m_RawMaxCombo.SetCurSel(raw_output_max);
}

afx_msg void CVMeter_v0Dlg::OnClose(void)
{
	//if (handle)
	//	hid_close(handle);
	CloseMixer();
	
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	if (pMenu != NULL)
		delete pMenu;

	DeleteTimerQueueTimer(NULL, m_timerHandle, NULL);
//	CloseHandle (m_timerHandle);

	releaseEndPointVolumeForVista();
    CWnd::OnClose();
	_CrtDumpMemoryLeaks();
}



//void CVMeter_v0Dlg::CheckUsb() {
//	//DWORD dwVal[2];
//	//DWORD vol;
//	unsigned char buf[17] = {0};
//	char ret_val, res, i;
//	if (!handle) { // if no handle (USB device hasn't been opened yet
//		handle = hid_open(0x03eb, 0x1234, NULL);
//		if (handle) {
//			hid_get_product_string(handle, mystring, 199);
//		
//			hid_get_serial_number_string(handle, serial_string, 199);
//			m_sStatus.Format(_T("Connected: found %s, serial: %s\n"), mystring, serial_string);
//			
//			hid_set_nonblocking(handle, 1);
//			
//			
//			buf[0] = 0; // packet identifier? necessary I believe
//			buf[1] = 1; // change displayed LED column.
//			buf[2] = SCROLL_BAR_SIZE - m_VolTrack.GetPos(); // (unsigned char)(GetMasterVolume()*255.0);	
//			buf[3] = led_brightness;
//			buf[4] = upside_down;
//			ret_val = hid_write(handle, buf, 9);
//
//			// do a hid_read to store reference values for the raw sensor outputs
//			do {
//				res = hid_read(handle, (unsigned char *)buf, sizeof(buf));
//			}
//			while (res != 16);
//
//			if (res == 16) {
//				for (i=0;i<8;i++) {
//					raw_ref_values[i] = (char)buf[i+3];
//				}
//			}
//			else 
//				OutputDebugString(L"FAILED TO HID_READ in usb_check\n");
//			
//		} 
//		else {
//			m_sStatus.Format(_T("Please connect VMeter"), mystring);
//			
//		}
//		::PostMessage(myhWnd,WM_UPDATE_MYCONTROL,0,0);
//	}
//}

// When raw sensor outputs are requested, scale the current inputs to the range selected in dialog.
// We expect the input to this function is the current raw sensor value - the reference value recorded
// when the VMeter was first plugged in or re-calibrated.
// The input will therefore be negative, since touches reduce the signal

// Looking at a variety of touches, the delta usually only gets to a max of 50 for heavy one finger 
// touches.  If the whole hand is pressed down, it gets up to 60 or so.  The input can go a few negative,
// also.  We assume the input is roughly 0-50 then.

int CVMeter_v0Dlg::scale_raw_output(int in) {
	int temp;
	// need to scale from the max input to the max output range, raw_output_max - raw_output_min
	temp = (int)((float)(raw_output_max - raw_output_min) * (float)in / 50.0 + raw_output_min);	
	
	if (temp > 127)
		temp = 127;
	else if (temp > raw_output_max)
		temp = raw_output_max;
	else if (temp < raw_output_min)
		temp = raw_output_min;
	return temp;
}


// touch is enabled when the sum of differentials gets up to 33.  We're going to assume input range
// is 45 to 130.  Those values will map to the chosen output min and max.  Anything outside will be
// hard limited.
#define max_pres_in 130
#define min_pres_in 45
#define PRESSURE_FILTER_COEF .1
int CVMeter_v0Dlg::scale_pressure_output(int in) {
	int temp;
	//TCHAR str[255]; 
	float input_percentage;
	input_percentage = (float)(in - min_pres_in) / (float)(max_pres_in - min_pres_in);
	
	temp = (int)(input_percentage * (float)(pressure_output_max - pressure_output_min) + pressure_output_min);
	
	//_stprintf(str, _T("in: %d, temp pres: %d\n"),in,temp);
	//OutputDebugString(str);
	if (temp > 127)
		temp = 127;
	else if (temp > pressure_output_max)
		temp = pressure_output_max;
	else if (temp < pressure_output_min)
		temp = pressure_output_min;
	// now filter using a simple exponential average IIR filter
	ave_pressure = ave_pressure + ((float)temp - ave_pressure) * PRESSURE_FILTER_COEF;

	return (int)ave_pressure;
}


// This is the main usb_read timer that checks for new data every ~4ms.
//void CALLBACK CVMeter_v0Dlg::TimerDone2(HWND hWnd, UINT nMsg, DWORD dwTime) {
void CVMeter_v0Dlg::read_usb_periodically() {
	//unsigned char buf[20] = {0};
	////DWORD temp2
	//DWORD temp;
	//int res = -1;  
	//float weight_sum = 0.0;
	//float weighted_sum = 0.0;
	//int q = 0, i;
	////int j;
	//float f_deltas[8] = {0.0};
	//int raw_delta;
	//int raw_buf;
	//float calculated_pos = 0.0;
	//SYSTEMTIME localtim;
	//long delta_time;
	//long cur_time;
	//float calc_pos_ave = 0.0;
	//int num_to_ave = 0;
	////unsigned int weighted_sum_from_vmeter;
	//unsigned int my_int;
	//char output_string[255];
	//int string_size;
	TCHAR str[255]; 
	//int touch_input;
	static float currentVolume = 0.0;
	float tempVolume;
	static int sign_of_life_counter = 0;	
	float temp_peak;
	static float cpu_ave = 0.0;
	static float temp_cpu_ave = 0.0;
	static int cpu_counter = 0;


	if (vmeter_touched_cycles_ago < 10000)
		vmeter_touched_cycles_ago++;
	if (volume_changed_cycles_ago < 10000)
		volume_changed_cycles_ago++;
	

	tempVolume = GetMasterVolume();

	
	if (vmeter_output_type == 1) { // VU Meter display
		if (IsVistaOrLater()) {
			if (volume_changed_cycles_ago > 200 && vmeter_touched_cycles_ago > 200) {
				HRESULT hr;
				hr = pMeterInfo->GetPeakValue(&temp_peak);

				if (scale_by_system_volume)
					temp_peak = temp_peak * tempVolume;

				if (temp_peak > peak)
					peak = temp_peak;
				else
					peak = .985 * peak;
				//_stprintf(str, _T("peak: %f\toutput: %d\n"), peak, vmeter_output_type);
				//OutputDebugString(str);
				if (connection_check)
					send_midi_controller_msg(MIDI_CONTROLLER_NUM_LEDS,peak*127.0);
			}
		}	
	}
	else if (vmeter_output_type == 2) { // cpu monitor
		if (volume_changed_cycles_ago > 200 && vmeter_touched_cycles_ago > 200) {
			if (cpu_counter++ > 100) {
				temp_cpu_ave = totalcpu();		
				cpu_counter = 0;
			}
			cpu_ave = (temp_cpu_ave - cpu_ave)*.05 + cpu_ave;
			if (connection_check)
				send_midi_controller_msg(MIDI_CONTROLLER_NUM_LEDS,cpu_ave*1.270);
		}
	}
	
		

	// don't update the display or VMeter if we the volume hasn't change
	// or we recently received a command from VMeter.
	if (currentVolume != tempVolume && vmeter_touched_cycles_ago > 10) {
		currentVolume = tempVolume;
		volume_changed_cycles_ago = 0;
		peak = 0.0;
		//_stprintf(str, _T("new volume: %f\n"),currentVolume);
		//OutputDebugString(str);
		send_midi_controller_msg(MIDI_CONTROLLER_NUM_LEDS,currentVolume*127.0);
		// update the display, too
		DWORD VolumePos = DWORD(255 - (int)(currentVolume*255.0)); // does this call the OnVScroll func? NO.
		m_VolTrack.SetPos(VolumePos);
	}			
	//}
	
	
	if (sign_of_life_counter++ == 250) {
		sign_of_life_counter = 0;
		_stprintf(str, _T("connection check: %d\n"), connection_check);
		OutputDebugString(str);

		if (connection_check) { // we received an echo response since the last call.
			connection_check = false; // this should get set to true when an echo is received from the VMeter.
			send_midi_controller_msg(118,1); 
		}
		else { // lost VMeter, try to connect again?
			init_midi(); 
		}
	}


	//CheckUsb();

	//if (handle) {
	//	res = hid_read(handle, (unsigned char *)buf, sizeof(buf));
	//	// note, it always returns 16 no matter what, even when disconnected? maybe only win 7
	//	// *** see the bottom of this file for the communication protocol ***		
	//	if (res == -1) {
	//		handle = NULL;			
	//	}
	//}
	
	
		// fix the qtouch_pos var, which was read in as a signed char?
		
	//GetLocalTime(&localtim);
	//cur_time = localtim.wSecond * 1000 + localtim.wMilliseconds;
	//
	//if (m_screen_update_counter++ == 0) {
	//	m_screen_update_counter = 0;
	//	delta_time = cur_time - m_ms;
	//	m_ms = 	cur_time;
	//	//
	//	//_stprintf(str, _T("time: %ld\n"),delta_time);
	//	//OutputDebugString(str);
	//}
	//
	
	//_stprintf(str, _T("buf: %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%ld,%d\n"),buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11],buf[12],buf[13],buf[14],buf[15],delta_time,res);
	//OutputDebugString(str);

	//if (res == 16) {
	//	int val_to_send;
	//	if (raw_output_enabled) { // when VMeter is first connected or re-calibrated, 									  
	//		for (i=0;i<8;i++) {   // it stores off the current raw sensor values for reference.
	//			// the references typically vary between -90 and +30.
	//			// They change by 60 at most, in the negative direction. Because of that, we need
	//			// to protect for wrapping past -128 (the input is a char).
	//			// Since the values only go more negative (allowing for some miscalibration), 
	//			// any inputs >40 have likely wrapped. 60 is chosen arbitrarily as a wrap indication point
	//			// to provide some buffer against variations in construction.
	//			raw_buf = (char)buf[3+i];
	//			if (raw_buf >60) // it likely wrapped.  
	//				raw_buf = raw_buf - 256;			
	//			raw_delta = raw_ref_values[i] - raw_buf; 	
	//			val_to_send = scale_raw_output(raw_delta); 				
	//			send_midi_controller_msg(50+i,val_to_send);					
	//		}			
	//	}
	//	if (buf[0] == 1) { // touch is active
	//		touch_active = 1;			
	//		touch_input = (unsigned char)buf[12]; // 0 - 255
	//	
	//		// set slider:
	//		DWORD VolumePos = DWORD(255 - touch_input); // does this call the OnVScroll func? NO.
	//		m_VolTrack.SetPos(VolumePos);
	//		
	//		if (volume_control_enabled) {
	//			SetMasterVolume(((float)touch_input)/255.0);
	//		}
	//		if (pos_touch_output_enabled) {
	//			send_midi_controller_msg(pos_touch_output_ctrl_num,touch_input/2);
	//		}
	//		
	//		if (touch_state == 0) {
	//			touch_state= 1; // only send this once per touch.
	//			if (touch_on_off_output_enabled) {				
	//				send_midi_controller_msg(touch_on_off_output_ctrl_num,127);					
	//			}
	//		}

	//		if (pressure_output_enabled) {
	//			val_to_send = scale_pressure_output((unsigned int)buf[13]);
	//			//_stprintf(str, _T("buf13: %d\n"),buf[13]);
	//			//OutputDebugString(str);
	//			send_midi_controller_msg(pressure_output_ctrl_num,val_to_send);
	//		}
	//		

	//		//OutputDebugString( msg );
	//		//OutputDebugString((LPCWSTR)L"got here\n");				

	//	}// if touched
	//	else { // no touch deteced
	//			
	//		if (touch_state == 1) {
	//			send_midi_controller_msg(touch_on_off_output_ctrl_num,0);
	//			touch_state = 0; // send one off signal per release.
	//			if (pressure_output_enabled) {
	//				ave_pressure = (float)pressure_output_min;
	//				send_midi_controller_msg(pressure_output_ctrl_num,pressure_output_min);
	//			}
	//		}

	//		touch_active = 0;

	//		if (led_show_system_vol_enabled) { 
	//			//VOLUME IS BETWEEN 0.0 AND 1.0 				
	//			float CurrentVolume = GetMasterVolume();
	//	
	//			// check to see if it's changed since the last check
	//			//if ((CurrentVolume - prev_volume) > .01 || (prev_volume - CurrentVolume) > .01) { // adjust both the on-screen meter and vmeter
	//			//	DWORD VolumePos = DWORD(256 - (CurrentVolume * 256));
	//				//m_VolTrack.SetPos(VolumePos);				
	//			//	prev_volume = CurrentVolume;
	//				set_col(CurrentVolume * 255.0);
	//			//}
	//		}
	//	}	
	//	if (lines_to_write > 0) {
	//		lines_to_write--;
	//		string_size = sprintf(output_string,"%d,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u,%u,%u,%u,%u\n",buf[0],(unsigned char)buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],(unsigned char)buf[11],(unsigned char)buf[12],(unsigned char)buf[13],(unsigned char)buf[14],(unsigned char)buf[15]);
	//				
	//		bTest = WriteFile(hFile,output_string,string_size,&dwNumWritten, NULL);
	//	}
	//	else if (lines_to_write == 0) {
	//		lines_to_write = -1;
	//		bTest= CloseHandle(hFile);
	//		TCHAR str[50];
	//		_stprintf(str, _T("writing done"));	
	//		MessageBox(str, 0, 0);
	//	}
	//} // 16 sized packet

}


// about command?
/*void CVMeter_v0Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
*/
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVMeter_v0Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVMeter_v0Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




// this gets called in response to any VScross... use pScrollBar to determine which one
void CVMeter_v0Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	DWORD temp = 0;
	DWORD temp2 = 0;

	
	if (pScrollBar != NULL)
	{// which scroll bar is it?
		if (pScrollBar->GetDlgCtrlID() == IDC_BRIGHTNESS_SLIDER) {
			//set_brightness(16 - brightness_slider.GetPos());
			led_brightness =  16-brightness_slider.GetPos();
			brightness_text.Format(_T("Brightness: %d"),led_brightness);
			if (led_brightness > 0)
				send_midi_controller_msg(MIDI_CONTROLLER_NUM_BRIGHTNESS,led_brightness*8-1);
			else
				send_midi_controller_msg(MIDI_CONTROLLER_NUM_BRIGHTNESS,0);

		}
		// the touch position slider... named VOL_SLIDER due to first edition of app
		else if (pScrollBar->GetDlgCtrlID() == IDC_VOL_SLIDER) {			
			temp2 = SCROLL_BAR_SIZE - m_VolTrack.GetPos();
		
			//if (!ignore_touch)
			//	set_col(temp2); // set led column
			
			SetMasterVolume((float)temp2 / 255.0);

			send_midi_controller_msg(MIDI_CONTROLLER_NUM_LEDS,temp2/2);
			// send MIDI output
			/*if (pos_touch_output_enabled) {
				midi::CShortMsg ShortMsg(midi::CONTROL_CHANGE, midi_channel, pos_touch_output_ctrl_num, temp2/2, 0);
				ShortMsg.SendMsg(m_OutDevice);
			}*/
		}
	}
	UpdateData(FALSE);
}


void CVMeter_v0Dlg::send_midi_controller_msg(int ctrl_num, int data) {
	// last argument is time stamp... not used.
	TCHAR str[255];
	//_stprintf(str, _T("data %d\n"),data);
	//OutputDebugString(str);
	midi::CShortMsg ShortMsg(midi::CONTROL_CHANGE, midi_channel, ctrl_num, data, 0);
	ShortMsg.SendMsg(m_OutDevice);
}



// this never gets called?
// this gets called when something changes the volume.  for instance, using windows volume ctrl
// this could just be for XP?
LRESULT CVMeter_v0Dlg::OnMixerCtrlChange(WPARAM wParam, LPARAM lParam)
{
	//DWORD temp2 = 0;
	//DWORD temp3 = 0;
	//TCHAR str[100];
	//_stprintf(str, _T("mixer change\n"));
	//OutputDebugString(str);

	//_stprintf(str, _T("Mixer change on XP?"));	
	//MessageBox(str, 0, 0);

	//if (led_show_system_vol_enabled) {
	//	if (reinterpret_cast<HMIXER>(wParam) == m_hMixer &&
	//		static_cast<DWORD>(lParam) == m_dwVolumeControlID)
	//	{					
	//		if (!touch_active) {
	//			//volume_byte_size = (int)(((float)temp2)/m_fRange * (float)VMETER_LED_SIZE);	
	//			//set_col(GetMasterVolume() * 255.0);
	//		}

	//	}
	//}
	return 0;
}

void CVMeter_v0Dlg::change_ignore_touch_on_vmeter() {
	unsigned char buf[10] = {0};
	int return_val;
	buf[0] = 0;
	buf[1] = 251;
	buf[2] = !ignore_touch;
	buf[3] = led_brightness;
	//CheckUsb();
	//if (handle) {
	//	return_val = hid_write(handle, buf, 9);
	//}
}

void CVMeter_v0Dlg::OnToolsUpdatefirmware()
{
	//unsigned char buf[10];
	//// the first byte of the data sent is the hid report ID, (default 0?).  So to send 8 bytes
	//// you have to actually put 9 bytes in buf.
	//int return_val;
	//buf[0] = 0;
	//buf[1] = 254;
	//buf[2] = 0;
	//buf[3] = led_brightness;
	//buf[4] = 0;
	//buf[5] = 0;
	//buf[6] = 0;
	//buf[7] = 0;
	//buf[8] = 0;
	//// TODO: Add your control notification handler code here	
	//TCHAR str[255];
	//if (handle) {
	//	return_val = hid_write(handle, buf, 9);
	//	_stprintf(str, _T("VMeter is now prepped for a firmware update.  Please follow help instructions. %d"), return_val);	
	//	MessageBox(str, 0, 0);
	//}
	//else {
	//	_stprintf(str, _T("No VMeter detected.  Try unplugging and re-attaching."));	
	//	MessageBox(str, 0, 0);
	//}
	send_midi_controller_msg(119,127);
	
}


void CVMeter_v0Dlg::OnFileExit()
{
	CloseMixer();
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}


// takes a number 0-255 and sends it to VMeter to display on LEDs
//void CVMeter_v0Dlg::set_col(int c) { 
//	int ret;
//	unsigned char buf[10];
//	buf[0] = 0; // always required
//	buf[1] = 1; // this means buf[2] has the column height, 0-255
//	buf[2] = (unsigned char)c;
//	buf[3] = led_brightness;
//	CheckUsb();
//	if (handle) {
//		ret = hid_write(handle, buf, 9);
//	}
//}

// expects 0 to 16, sets brightness level on vmeter
//void CVMeter_v0Dlg::set_brightness(int b) { 
//	int ret;
//	led_brightness = b; // store the global brightness
//	unsigned char buf[10];
//	buf[0] = 0; // always required
//	buf[1] = 0; // will ignore buf[2] now.
//	buf[2] = 0;
//	buf[3] = led_brightness;
//	CheckUsb();
//	if (handle) {
//		ret = hid_write(handle, buf, 9);
//	}
//}

void CVMeter_v0Dlg::OnToolsRecalibrate()
{
	//unsigned char buf[17];
	//// the first byte of the data sent is the hid report ID, (default 0?).  So to send 8 bytes
	//// you have to actually put 9 bytes in buf.
	//int return_val, res, i;
	//buf[0] = 0;
	//buf[1] = 253;
	//buf[2] = 0;
	//buf[3] = led_brightness;
	//buf[4] = 0;
	//buf[5] = 0;
	//buf[6] = 0;
	//buf[7] = 0;
	//buf[8] = 0;
	// TODO: Add your control notification handler code here	
	TCHAR str[255];
	//if (handle) {
	//	return_val = hid_write(handle, buf, 9);
	//	_stprintf(str, _T("Recalibration signal sent.%d"), return_val);	
	//	MessageBox(str, 0, 0);
	//	// do a hid_read to store reference values for the raw sensor outputs
	//	do {
	//		res = hid_read(handle, (unsigned char *)buf, sizeof(buf));
	//	}
	//	while (res != 16);

	//	if (res == 16) {
	//		for (i=0;i<8;i++) {
	//			raw_ref_values[i] = (char)buf[i+3];
	//		}
	//	}
	//	else 
	//		OutputDebugString(L"FAILED TO HID_READ in recalibrate\n");
	//}
	if (m_OutDevice.IsOpen()) {
		// send MIDI recalibrate command
		send_midi_controller_msg(MIDI_CONTROLLER_PARAMS,112);
	}
	else {
		_stprintf(str, _T("No VMeter connected."));	
		MessageBox(str, 0, 0);
	}
	//MessageBox((LPCWSTR)L"Account Details",(LPCWSTR)L"Account Details2",0);
}


void CVMeter_v0Dlg::OnToolsRecorddebugfile()
{
	TCHAR str[255];
	_stprintf(str, _T("writing out debug file to c:\\vmeter_debug.txt"));
	MessageBox(str, 0, 0);
	typedef wchar_t WCHAR ;

	typedef CONST WCHAR *LPCWSTR;

	wchar_t filename[]=L"C:/vmeter_debug.txt";
	hFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE,\
                   NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	lines_to_write = 400;
}


void CVMeter_v0Dlg::OnHelpAbout()
{
	TCHAR str[255];
	_stprintf(str, _T("VMeter v1.2\n (c) 2013 Curious Inventor, LLC"));
	MessageBox(str, 0, 0);
}



void CVMeter_v0Dlg::OnHelpOpenwebsitehelp()
{
	ShellExecute(NULL, (LPCWSTR)L"open", (LPCWSTR)L"http://vmeter.net",NULL, NULL, SW_SHOWNORMAL);
}


// testing MIDI OUT
void CVMeter_v0Dlg::OnBnClickedButton3()
{
	TCHAR str[255];
	int return_val;

	midi::CShortMsg ShortMsg(midi::NOTE_ON, 0, 50, 127, 0);
    ShortMsg.SendMsg(m_OutDevice);
	
	_stprintf(str, _T("Sent test data: %d"), 0);	
	MessageBox(str, 0, 0);

}




void CVMeter_v0Dlg::OnBnClickedCheck1()
{
	UpdateData(TRUE);
	upside_down = upside_down_box_value;

	
	if (upside_down) {
		send_midi_controller_msg(MIDI_CONTROLLER_PARAMS,UPSIDE_DOWN_ON);
	}
	else {
		send_midi_controller_msg(MIDI_CONTROLLER_PARAMS,UPSIDE_DOWN_OFF);
	}	
	send_midi_controller_msg(MIDI_CONTROLLER_NUM_LEDS,GetMasterVolume() * 127.0);
	//unsigned char buf[9];
	//int return_val;
	//buf[0] = 0;
	//buf[1] = 252; // if 1, buf[2] sets lights, if 254, firmware, if 253, recalibrate, 252, change upside_down
	//buf[2] = upside_down;
	//buf[3] = led_brightness;
	//if (handle) {
	//	return_val = hid_write(handle, buf, 9);
	//}

	//// now send the current volume setting to update the lights:
	//buf[0] = 0;
	//buf[1] = 1; // if 1, buf[2] sets lights, if 254, firmware, if 253, recalibrate, 252, change upside_down
	//buf[2] = volume_byte_size;
	//buf[3] = led_brightness;
	//if (handle) {
	//	return_val = hid_write(handle, buf, 9);
	//}

}




void CVMeter_v0Dlg::OnBnClickedHide()
{
//	TCHAR str[255];	
//	_stprintf(str, _T("test"));	
//	MessageBox(str, 0, 0);
	//AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
	//AfxGetApp()->m_pMainWnd
	//m_pMainWnd is good
	//pMainFrame is hidden
	//pParent->
	//AfxGetApp()->m_pMainWnd->
	//AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
	//my_parent_win->pMainFrame->OnHideMenu();
	OutputDebugString((LPCWSTR)L"test_func 221 \n");
	theApp.test_func();
}


void CVMeter_v0Dlg::OnEditMididevicerouting()
{
	//OutputDebugString((LPCWSTR)L"OnEditMididevicerouting\n");

	 try
    {
	    MIDI_Devices_Dialog Dlg;

        //
        // Initialize MIDI device dialog box
        //

        if(m_OutDevice.IsOpen())
        {
            Dlg.m_OutDevId = m_OutDevice.GetDevID();
        }

        if(m_InDevice.IsOpen())
        {
            Dlg.m_InDevId = m_InDevice.GetDevID();
        }

        // Run dialog box
        if(Dlg.DoModal() == IDOK)
        {
            // If the client clicked OK and they chose to change the 
            // MIDI output device
            if(Dlg.m_OutChanged)
            {
                m_OutDevice.Close();
                m_OutDevice.Open(Dlg.m_OutDevId);   
                //OnSelchangeGmList();
            }

            // If the client clicked OK and they chose to change the 
            // MIDI input device
            if(Dlg.m_InChanged)
            {
                if(m_InDevice.IsOpen())
                {
                    if(m_InDevice.IsRecording())
                    {
	                    // Stop receiving MIDI events 	
                        m_InDevice.StopRecording();   	
                    }
                
                    m_InDevice.Close(); 	
                }

                m_InDevice.Open(Dlg.m_InDevId);   	
                
                // Start receiving MIDI events 	
                m_InDevice.StartRecording(); 
            }
        }	
    }
    catch(const std::exception &ex)
    {
        TCHAR str[255];
		_stprintf(str, _T("error on routing"));
		MessageBox(str, 0, 0);
    }
}

LRESULT CVMeter_v0Dlg::myUpdateData(WPARAM wParam, LPARAM lParam) {
	UpdateData(FALSE);
	return 0;
}

// Receives MIDI short message
// something to do with sync'ing multiple MIDI devices:?
// if we receive a command 240 + channel 14, that's actually an "Active Sensing" message, which
// tells all receivers they should expect another active-sensing message within 300ms, or else
// silence all outputs. It's there to keep devices from hanging once the MIDI disconnects.
void CVMeter_v0Dlg::ReceiveMsg(DWORD Msg, DWORD TimeStamp)
{
	TCHAR str[255];
    midi::CShortMsg ShortMsg(Msg, TimeStamp);
    unsigned char command = ShortMsg.GetCommand();
	unsigned char channel = ShortMsg.GetChannel();
	unsigned char data1 = ShortMsg.GetData1();
	unsigned char data2 = ShortMsg.GetData2();
    CString Text;
	static int midi_status = 0;

	// ignore system messages:
	if (command >= 0xF0) 
		return;
	// else

	// update graphic to show incoming data
	if (midi_status == 0) {
		m_midi_status_text.Format(_T("|"));
		midi_status = 1;
	}
	else if (midi_status == 1) {
		m_midi_status_text.Format(_T("/"));
		midi_status = 2;
	}
	else if (midi_status == 2) {
		m_midi_status_text.Format(_T("-"));
		midi_status = 0;
	}
	
	// ok, trying to update the dialog by posting a message to the original dialog window
	// have to do it this way due to some threading issues with the queuetimer
	

	// only process midi information on the selected channel
	
	//_stprintf(str, _T("midi status: %d\tchan: %d\tcommand: %d\tdata1:%d\tdata2: %d\n"),midi_status,channel,command,data1,data2);
	//OutputDebugString(str);

	if (channel == midi_channel) {
		if (command == midi::CONTROL_CHANGE) {
			//if (led_control_from_pos_enabled && data1 == led_control_source_ctrl_num) {
			if (data1 == 20) {
				vmeter_touched_cycles_ago = 0;
				// only move the LEDs if we're not touching or ignore touch is on
				/*if (ignore_touch || touch_state == 0) {
					set_col(data2*2);
				}*/
				// adjust volume... receive 0-127
				// set slider:
				
				DWORD VolumePos = DWORD(255 - data2*2); // does this call the OnVScroll func? NO.
				m_VolTrack.SetPos(VolumePos);
							
				SetMasterVolume(((float)data2)/127.0);
				peak = 0.0;
				
			}
			else if (data1 == 118) { // echo response
				connection_check = true;
				//_stprintf(str, _T("echo received\n"));
				//OutputDebugString(str);
			}
			else if (data1 == 119) { // receiving settings packet

				if (data2 == 85)
					settings_packet_index = 0;
				else {
					settings_packet[settings_packet_index++] = data2;
				}

				if (settings_packet_index == 12) {
					settings_packet_index = 0; // set to 0 just in case.
					// received the full packet, process
					//_stprintf(str, _T("settings, byte0: %d, byte 9: %d\n"),settings_packet[0], settings_packet[9]);
					//OutputDebugString(str);					
					upside_down_box_value = settings_packet[0] & 0x01;	
					int vmeter_brightness = (settings_packet[9]+1) / 8;
					DWORD Brightness = DWORD(16 - vmeter_brightness); // does this call the OnVScroll func? NO.
					brightness_slider.SetPos(Brightness);
					brightness_text.Format(_T("Brightness: %d"),vmeter_brightness);

				}
			}
			//if (midi_brightness_control_enabled && data1 == midi_brightness_ctrl_num) {				 
			//	set_brightness((data2+1)/8); // send a value between 0 and 16.
			//}
		}
	}
	::PostMessage(myhWnd,WM_UPDATE_MYCONTROL,0,0);
// the config packet sent back from VMeter on connection.  
//	struct CONFIG {
//	unsigned upside_down:1;
//	unsigned touch_pos_output_en:1;
//	unsigned on_off_output_en:1;
//	unsigned pres_output_en:1;
//	unsigned spacer:4;
//	
//	unsigned pitch_wheel_mode:1;
//	unsigned note_out_mode:1;
//	unsigned leds_ignore_touch:1;
//	unsigned note_vel_mode:1;
//	unsigned note_pitch_mode:1;
//	unsigned cross_fade_mode:1;
//	unsigned filter_en:1;
//	unsigned spacer3:1;
//
//	uint8_t  pos_out_ctrl_num;
//	uint8_t  on_off_out_ctrl_num;
//	uint8_t  pres_out_ctrl_num;
//	uint8_t  led_input_ctrl_num;
//	uint8_t  brightness_input_ctrl_num;
//	uint8_t  note_pitch;
//	uint8_t  note_vel;
//	uint8_t  brightness_orig;
//	unsigned midi_chan:4;
//	unsigned spacer2:4;
//	uint8_t  name;
//	uint8_t  brightness;
//};


    // 
    // If this is a note-on or note-off event, notify piano control so
    // that it can update its display
    //

  /*  if(Command == midi::NOTE_ON && ShortMsg.GetData2() > 0)
    {
        unsigned char Note = ShortMsg.GetData1();

        // Range checking
        if(Note >= m_Keys.GetLowNote() && 
            Note <= m_Keys.GetHighNote())
        {
            m_Keys.NoteOn(Note);
        }
    }
    else if(Command == midi::NOTE_OFF || (Command == midi::NOTE_ON && 
        ShortMsg.GetData2() == 0))
    {
        unsigned char Note = ShortMsg.GetData1();

        // Range checking
        if(Note >= m_Keys.GetLowNote() && 
            Note <= m_Keys.GetHighNote())
        {
            m_Keys.NoteOff(Note);
        }
    }
	*/
}


// Receives an erroneous short message
void CVMeter_v0Dlg::OnError(DWORD Msg, DWORD TimeStamp)
{
    MessageBox(L"An illegal MIDI short message was received.", L"Error",
        MB_ICONSTOP | MB_OK);
}


//
void CVMeter_v0Dlg::EnablePosMidiOutputChanged()
{
	pos_touch_output_enabled = m_EnablePosOutputCheckBox.GetCheck();
	
	TCHAR str[255];	
	_stprintf(str, _T("data2: %d\n"),pos_touch_output_enabled);
	OutputDebugString(str);
}


// touch position output channel change
//void CVMeter_v0Dlg::OnPosOutputChannelChange()
//{
//	TCHAR str[255];	
//	//MessageBox(L" touch position output channel change",0 ,MB_ICONSTOP | MB_OK);
//	pos_touch_output_ctrl_num = m_PosOutputCtrlNum.GetCurSel();
//	_stprintf(str, _T("data2: %d\n"),pos_touch_output_ctrl_num);
//	OutputDebugString(str);
//}


//void CVMeter_v0Dlg::MidiOutputChannelChanged()
//{
//	TCHAR str[255];	
//	//MessageBox(L" touch position output channel change",0 ,MB_ICONSTOP | MB_OK);
//	midi_channel = m_MidiChannelCombo.GetCurSel();
//	_stprintf(str, _T("data2: %d\n"),midi_channel);
//	OutputDebugString(str);
//}




void CVMeter_v0Dlg::OnBnClickedCheckPresOutEn()
{
	pressure_output_enabled = m_EnPressureOutput.GetCheck();
}


void CVMeter_v0Dlg::OnBnClickedCheckOnOffEn()
{
	touch_on_off_output_enabled = m_EnTouchOnOff.GetCheck();
}


void CVMeter_v0Dlg::OnBnClickedCheckRawOutEn()
{
	raw_output_enabled = m_EnRawOutput.GetCheck();
}


void CVMeter_v0Dlg::OnBnClickedCheckPitchWheelEn()
{
	pitch_wheel_enable = m_EnPitchOut.GetCheck();
}


void CVMeter_v0Dlg::OnBnClickedCheckTouchVolEn()
{
	volume_control_enabled = m_EnSysVolControl.GetCheck();
}


void CVMeter_v0Dlg::OnBnClickedCheckMidiLedEn()
{
	led_control_from_pos_enabled = m_EnMidiLedPosControl.GetCheck();
}


void CVMeter_v0Dlg::OnBnClickedCheckMidiBrightEn()
{
	midi_brightness_control_enabled = m_EnMidiBrightessControl.GetCheck();
}


void CVMeter_v0Dlg::OnBnClickedCheckIgnoreTouch()
{
	ignore_touch = m_IgnoreTouch.GetCheck();
	change_ignore_touch_on_vmeter();
}




void CVMeter_v0Dlg::OnBnClickedCheckLedShowVolumeEn()
{
	led_show_system_vol_enabled = m_LedShowVolume.GetCheck();
}


//void CVMeter_v0Dlg::OnCbnSelchangeComboPresCtrl()
//{
//	pressure_output_ctrl_num = m_PresMidiCtrlNumCombo.GetCurSel();
//}


//void CVMeter_v0Dlg::OnCbnSelchangeComboPresMin()
//{
//	pressure_output_min = m_PresMinCombo.GetCurSel();
//}


//void CVMeter_v0Dlg::OnCbnSelchangeComboPresMax()
//{
//	TCHAR ret_string[10];
//	m_PresMaxCombo.GetLBText(m_PresMaxCombo.GetCurSel(),ret_string);
//	pressure_output_max = _ttoi(ret_string);
//}


void CVMeter_v0Dlg::OnCbnSelchangeComboOnOffCtrl()
{
	touch_on_off_output_ctrl_num = m_TouchOnOffMidiCtrlNumCombo.GetCurSel();
}


void CVMeter_v0Dlg::OnCbnSelchangeComboRawMin()
{
	raw_output_min = m_RawMinCombo.GetCurSel();
}


void CVMeter_v0Dlg::OnCbnSelchangeComboRawMax()
{	
	TCHAR ret_string[10];
	m_RawMaxCombo.GetLBText(m_RawMaxCombo.GetCurSel(),ret_string);
	raw_output_max = _ttoi(ret_string);
}

//
//void CVMeter_v0Dlg::OnCbnSelchangeComboLedMidiPosCtrl()
//{
//	led_control_source_ctrl_num = m_LedPosMidiCtrlNumCombo.GetCurSel();
//}


//void CVMeter_v0Dlg::OnCbnSelchangeComboLedBrightCtrl()
//{
//	midi_brightness_ctrl_num = m_BrightnessMidiCtrlNumCombo.GetCurSel();
//}




   /*       
--------------------------------------------------     
VMeter USB Protocol
--------------------------------------------------
     
                      
The VMeter continously sends packets of 16 bytes and receives packets of 8 bytes about every 4ms (subject to change).

Some of the bytes sent from the VMeter are diagnostic and may change.

8 BYTE PACKET SENT TO VMETER:
     
packet[0]: always required, not actually sent to VMeter but must be 0.     
packet[1]: command (see below)
packet[2]: command parameter
packet[3]: brightness: 0-16, 0 is off, 16 is max brightness
packet[4]: binary value of lights 0-7 (when packet[1] == 2)
packet[5]: binary value of lights 8-15 (when packet[1] == 2)
packet[6]: binrary value of lights 16-23 (when packet[1] == 2)
packet[7]: binrary value of lights 24-31 (when packet[1] == 2)
packet[8]: binrary value of lights 32-38 (38 lights in total) (when packet[1] == 2)
     
commands:
     1: When packet[1]==1, packet[2] contains the column height, 0 thru 38. If upside-down mode is on, it draws from the top
     2: When packet[1]==2, packet[4-8] contain the binary value for each group of 8 lights. For instance, sending a 6 in packet[4]
        would turn on the 2nd and 3rd lights since 6 == 0b00000110
     251: When packet[1]==251, packet[2] sets the ignoreTouch mode. If 1, touches don't change the lights. If 0, touches override
        whatever is currently controlling the lights (MIDI for instance).
     252: packet[2] turns on / off upside-down mode.
     253: recalibrate.  Sometimes this might be necessary if you move the VMeter.
     254: Prepare to update firmware. Puts the VMEter in a special state that will accept new firmware. Requires additional software
            and firmware
     
     
    
16 BYTE PACKET RECEIVED FROM VMETER:

packet[0]: Touch state, 0 for off, 1 for on
packet[1]: not used
packet[2]: current light position being displayed, 0 - 38
packet[3]: cap sensor 1 value, closest to VMeter label and USB port, can be negative
packet[4]: cap sensor 2 value, can be negative
packet[5]: cap sensor 3 value, can be negative
packet[6]: cap sensor 4 value, can be negative
packet[7]: cap sensor 5 value, can be negative
     
packet[8]: cap sensor 6 value, can be negative
packet[9]: cap sensor 7 value, can be negative
packet[10]: cap sensor 8 value, can be negative
packet[11]: diagnostic, not specified
packet[12]: touch_pos (0-255)
packet[13]: pressure, or a sum of all the cap sensor differentials (current value - ref value). Can be negative?
packet[14]: not used
packet[15]: not used
     

     
	 */

void CVMeter_v0Dlg::OnToolsLoaddefaults()
{
	// TODO: Add your command handler code here
	TCHAR str[255];	
	_stprintf(str, _T("load defaults\n"));
	OutputDebugString(str);

	// brightness controller num: 112, 21
	send_midi_controller_msg(112,21);
	// midi channel 117, 0
	send_midi_controller_msg(117,1);
	// upside down off: 126
	send_midi_controller_msg(119,126);
	// LED column height controller num: 113, 20
	send_midi_controller_msg(113,20);
	// touch position controller num: 116, 20
	send_midi_controller_msg(116,20);
	// enable touch output, 119, 124
	send_midi_controller_msg(119,124);
	// disable pressure output 121
	send_midi_controller_msg(119,121);
	// disable on/off output 119
	send_midi_controller_msg(119,119);

	send_midi_controller_msg(MIDI_CONTROLLER_NUM_BRIGHTNESS,127);

	// disable pitch wheel
	send_midi_controller_msg(119,114);
	// disable cross fader mode
	send_midi_controller_msg(119,104);
	// lights ignore touch off
	send_midi_controller_msg(119,106);

	// disable note out
	send_midi_controller_msg(119,116);

	// store the settings
	send_midi_controller_msg(119,118);


	// get and display version!
}


void CVMeter_v0Dlg::store_output_mode_to_registry(int s) {
	TCHAR str[255];	
	HKEY hk;
	DWORD dwDisp;
	DWORD input;
	TCHAR dwData[40];
	input = s;
	_stprintf(dwData, _T("%d"), s);
	

	RegCreateKeyEx(HKEY_CURRENT_USER,
		           _T("software\\VMeter"),
				   0,
				   NULL,
				   REG_OPTION_NON_VOLATILE,
				   KEY_WRITE,
				   NULL,
				   &hk,
				   &dwDisp);

	RegSetValueEx(hk,
		          _T("output_mode"),
				  0,
				  REG_DWORD,
				  (PBYTE)&input,
				  //(PBYTE)&dwData,
				  sizeof(input));


	RegCloseKey(hk);
}

void CVMeter_v0Dlg::store_scale_vumeter_mode_to_registry(int s) {
	TCHAR str[255];	
	HKEY hk;
	DWORD dwDisp;
	DWORD input;
	TCHAR dwData[40];
	input = s;
	_stprintf(dwData, _T("%d"), s);
	

	RegCreateKeyEx(HKEY_CURRENT_USER,
		           _T("software\\VMeter"),
				   0,
				   NULL,
				   REG_OPTION_NON_VOLATILE,
				   KEY_WRITE,
				   NULL,
				   &hk,
				   &dwDisp);

	RegSetValueEx(hk,
		          _T("scale_vu_meter"),
				  0,
				  REG_DWORD,
				  (PBYTE)&input,
				  //(PBYTE)&dwData,
				  sizeof(input));


	RegCloseKey(hk);
}

int CVMeter_v0Dlg::read_output_mode_from_registry() {
	TCHAR str[255];	
	HKEY hk;
	DWORD dwDisp;
	DWORD input;
	TCHAR dwData[40];
	unsigned int return_val;
	DWORD length;
	length = sizeof(return_val);
	 HRESULT hresult;
	 LPTSTR errorText = NULL;

	DWORD dwValue;
	DWORD dwType;
	DWORD dwCount = sizeof ( DWORD );

	return_val = 0;

	hresult = RegOpenKeyEx(HKEY_CURRENT_USER,
		           _T("software\\VMeter"),
				   0,
				   KEY_QUERY_VALUE,
				   &hk);

	//hresult = ERROR_SUCCESS;
	FormatMessage(
	   // use system message tables to retrieve error text
	   FORMAT_MESSAGE_FROM_SYSTEM
	   // allocate buffer on local heap for error text
	   |FORMAT_MESSAGE_ALLOCATE_BUFFER
	   // Important! will fail otherwise, since we're not 
	   // (and CANNOT) pass insertion parameters
	   |FORMAT_MESSAGE_IGNORE_INSERTS,  
	   NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
	   hresult,
	   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	   (LPTSTR)&errorText,  // output 
	   0, // minimum size for output buffer
	   NULL);   // arguments - see note 

	if ( NULL != errorText )
	{	  
		OutputDebugString(errorText);
	   // release memory allocated by FormatMessage(
	}
	LocalFree(errorText);
	errorText = NULL;

	if (ERROR_SUCCESS != hresult) {
		_stprintf(str, _T("No key found, returning 0.\n"));
		OutputDebugString(str);		
	}
	else {
		hresult = RegQueryValueEx(hk,
						 _T("output_mode"),
						 NULL,
						  NULL,
						 (LPBYTE)&dwValue, 
						 &dwCount);
	}
	FormatMessage(
	   // use system message tables to retrieve error text
	   FORMAT_MESSAGE_FROM_SYSTEM
	   // allocate buffer on local heap for error text
	   |FORMAT_MESSAGE_ALLOCATE_BUFFER
	   // Important! will fail otherwise, since we're not 
	   // (and CANNOT) pass insertion parameters
	   |FORMAT_MESSAGE_IGNORE_INSERTS,  
	   NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
	   hresult,
	   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	   (LPTSTR)&errorText,  // output 
	   0, // minimum size for output buffer
	   NULL);   // arguments - see note 

	if ( NULL != errorText )
	{	  
		OutputDebugString(errorText);
	   // release memory allocated by FormatMessage()   
	}
	if (ERROR_SUCCESS == hresult) {
		return_val = dwValue;
	}
	LocalFree(errorText);
	errorText = NULL;
	
	RegCloseKey(hk);
	int test = 5;
	_stprintf(str, _T("\nread registry2: %d, reg ret val: %d\n\n"),test, hresult);
	OutputDebugString(str);
	return return_val;
}

int CVMeter_v0Dlg::read_scale_vumeter_mode_from_registry() {
	TCHAR str[255];	
	HKEY hk;
	DWORD dwDisp;
	DWORD input;
	TCHAR dwData[40];
	unsigned int return_val;
	DWORD length;
	length = sizeof(return_val);
	 HRESULT hresult;
	 LPTSTR errorText = NULL;

	DWORD dwValue;
	DWORD dwType;
	DWORD dwCount = sizeof ( DWORD );

	return_val = 0;

	hresult = RegOpenKeyEx(HKEY_CURRENT_USER,
		           _T("software\\VMeter"),
				   0,
				   KEY_QUERY_VALUE,
				   &hk);

	//hresult = ERROR_SUCCESS;
	FormatMessage(
	   // use system message tables to retrieve error text
	   FORMAT_MESSAGE_FROM_SYSTEM
	   // allocate buffer on local heap for error text
	   |FORMAT_MESSAGE_ALLOCATE_BUFFER
	   // Important! will fail otherwise, since we're not 
	   // (and CANNOT) pass insertion parameters
	   |FORMAT_MESSAGE_IGNORE_INSERTS,  
	   NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
	   hresult,
	   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	   (LPTSTR)&errorText,  // output 
	   0, // minimum size for output buffer
	   NULL);   // arguments - see note 

	if ( NULL != errorText )
	{	  
		OutputDebugString(errorText);
	   // release memory allocated by FormatMessage(
	}
	LocalFree(errorText);
	errorText = NULL;

	if (ERROR_SUCCESS != hresult) {
		_stprintf(str, _T("No key found, returning 0.\n"));
		OutputDebugString(str);		
	}
	else {
		hresult = RegQueryValueEx(hk,
						 _T("scale_vu_meter"),
						 NULL,
						  NULL,
						 (LPBYTE)&dwValue, 
						 &dwCount);
	}
	FormatMessage(
	   // use system message tables to retrieve error text
	   FORMAT_MESSAGE_FROM_SYSTEM
	   // allocate buffer on local heap for error text
	   |FORMAT_MESSAGE_ALLOCATE_BUFFER
	   // Important! will fail otherwise, since we're not 
	   // (and CANNOT) pass insertion parameters
	   |FORMAT_MESSAGE_IGNORE_INSERTS,  
	   NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
	   hresult,
	   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	   (LPTSTR)&errorText,  // output 
	   0, // minimum size for output buffer
	   NULL);   // arguments - see note 

	if ( NULL != errorText )
	{	  
		OutputDebugString(errorText);
	   // release memory allocated by FormatMessage()   
	}
	if (ERROR_SUCCESS == hresult) {
		return_val = dwValue;
	}
	LocalFree(errorText);
	errorText = NULL;
	
	RegCloseKey(hk);
	int test = 5;
	_stprintf(str, _T("\nread registry2: %d, reg ret val: %d\n\n"),test, hresult);
	OutputDebugString(str);
	return return_val;
}

void CVMeter_v0Dlg::OnBnClickedButtonTest()
{
	TCHAR str[255];	
	int val;
	val = read_output_mode_from_registry();
	_stprintf(str, _T("test button:\n"));
	OutputDebugString(str);
	store_output_mode_to_registry(5);
}


void CVMeter_v0Dlg::OnBnClickedButton4()
{
	// store the settings
	send_midi_controller_msg(119,118);
}


// Set vmeter display to just volume
void CVMeter_v0Dlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	vmeter_output_type = 0;
	store_output_mode_to_registry(vmeter_output_type);
	send_midi_controller_msg(MIDI_CONTROLLER_NUM_LEDS,GetMasterVolume()*127.0);
	GetDlgItem( IDC_CHECK2 )->EnableWindow(FALSE);
}


// turn on VU Meter display output
void CVMeter_v0Dlg::OnBnClickedRadio2()
{
	volume_changed_cycles_ago = 200;
	vmeter_touched_cycles_ago = 200;
	vmeter_output_type = 1;
	store_output_mode_to_registry(vmeter_output_type);
	GetDlgItem( IDC_CHECK2 )->EnableWindow(TRUE);
}

void CVMeter_v0Dlg::OnBnClickedCpuRadio()
{
	vmeter_output_type = 2;
	store_output_mode_to_registry(vmeter_output_type);
	GetDlgItem( IDC_CHECK2 )->EnableWindow(FALSE); // disable scale option on vumeter
	volume_changed_cycles_ago = 200;
	vmeter_touched_cycles_ago = 200;
}



void CVMeter_v0Dlg::OnBnClickedCheck2()
{
	UpdateData(TRUE);
	TCHAR str[255];	
	store_scale_vumeter_mode_to_registry(scale_by_system_volume);
	_stprintf(str, _T("scale: %d\n"), scale_by_system_volume);
	OutputDebugString(str);	
	// TODO: Add your control notification handler code here
}

float CVMeter_v0Dlg::totalcpu()
{
	TCHAR str[255];	
	static PDH_STATUS            status;
	static PDH_FMT_COUNTERVALUE  value;
	static HQUERY                query;
	static HCOUNTER              counter;
	static DWORD                 ret;
	static char                  runonce=1;
	float                         cpu=0.0;
	PDH_FMT_COUNTERVALUE 		query_output;

	
	if(runonce)
	{
		status = PdhOpenQuery(NULL, 0, &query);
		if(status != ERROR_SUCCESS)
		{
		  printf("PdhOpenQuery() ***Error: 0x%X\n",status);
		  return 0;
		}

		PdhAddCounter(query, TEXT("\\Processor(_Total)\\% Processor Time"),0,&counter); // A total of ALL CPU's in the system
		runonce=0; //\\Processor(_Total)\\% Processor Time
		PdhCollectQueryData(query); // No error checking here
		return 0;
	}

	

	status = PdhCollectQueryData(query);
	if(status != ERROR_SUCCESS)
	{
		_stprintf(str, _T("PdhCollectQueryData failed\n"));
		OutputDebugString(str);	
		return 0.0;
	}

	

	if((status=PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, 0, &query_output))!=ERROR_SUCCESS){
        
		_stprintf(str, _T("PdhGetFormattedCounterValue falied\n"));
		OutputDebugString(str);	
		return 0.0;
    }

	_stprintf(str, _T("CPU: %f\n"), query_output.doubleValue);
	OutputDebugString(str);	
	return query_output.doubleValue;

}

