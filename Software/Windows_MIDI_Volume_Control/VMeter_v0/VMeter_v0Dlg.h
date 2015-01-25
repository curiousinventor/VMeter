
// VMeter_v0Dlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
//#include "hidapi.h"
#include "MIDIOutDevice.h"
#include "MIDIInDevice.h"

#include "LongMsg.h"
#include "ShortMsg.h"

#include <mmdeviceapi.h>
#include <endpointvolume.h>

#define CALC_POS_ARRAY_SIZE 5
#define HYSTERESIS 2

// CVMeter_v0Dlg dialog
class CVMeter_v0Dlg : public CDialogEx, public midi::CMIDIReceiver
{
// Construction
public:
	CVMeter_v0Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VMETER_V0_DIALOG };

	
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	afx_msg void OnClickedButton1();
	//void CALLBACK TimerDone2(HWND, UINT, DWORD);
	//void CALLBACK TimerProc(void* lpParametar,BOOLEAN TimerOrWaitFired);
	
	CStatic m_Volume;
	CSliderCtrl m_VolTrack;
	CString m_VolText;

	int vmeter_touched_cycles_ago;
	int volume_changed_cycles_ago;
	
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl brightness_slider;

		afx_msg void OnBnClickedButton2();
	afx_msg void OnToolsUpdatefirmware();
	afx_msg void OnFileExit();
	afx_msg void OnToolsRecalibrate();
	afx_msg void OnToolsRecorddebugfile();
	afx_msg void OnHelpAbout();
	afx_msg void OnHelpOpenwebsitehelp();
	afx_msg void OnBnClickedButton3();
	CString m_sStatus;
	afx_msg void CVMeter_v0Dlg::OnClose(void);
	CString brightness_text;
	unsigned char upside_down;
	afx_msg void OnBnClickedCheck1();
	BOOL upside_down_box_value;
	int volume_byte_size;
	CVMeter_v0App * my_parent_win;
	
	afx_msg void OnBnClickedHide();
	afx_msg void OnEditMididevicerouting();

	// Receives short messages
    void ReceiveMsg(DWORD Msg, DWORD TimeStamp);

    // Receives long messages
    void ReceiveMsg(LPSTR Msg, DWORD BytesRecorded, DWORD TimeStamp) {}

    // Called when an invalid short message is received
    void OnError(DWORD Msg, DWORD TimeStamp);

    // Called when an invalid long message is received
    void OnError(LPSTR Msg, DWORD BytesRecorded, DWORD TimeStamp) {}

	void init_configuration(void); // load default or file-stored config params into dialog

	void send_midi_controller_msg(int ctrl_num, int data);
	void start_usb_read_timer(void);
	void init_midi(void);
	void read_usb_periodically(void);
	void change_ignore_touch_on_vmeter(void);
	void set_col(int c);
	void set_brightness(int);
	LRESULT myUpdateData(WPARAM wParam, LPARAM lParam);
	void store_output_mode_to_registry(int s);
	int read_output_mode_from_registry(void);
	void store_scale_vumeter_mode_to_registry(int s);
	int read_scale_vumeter_mode_from_registry();
	float totalcpu(void);
	float peak;

// Implementation
protected:
	HICON m_hIcon;
	IAudioMeterInformation *pMeterInfo;
	midi::CMIDIOutDevice m_OutDevice;
    midi::CMIDIInDevice m_InDevice;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMixerCtrlChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	int CVMeter_v0Dlg::scale_raw_output(int in);
	int CVMeter_v0Dlg::scale_pressure_output(int in);

	bool connection_check;
	int settings_packet_index;
	unsigned char settings_packet[30];

	HANDLE   m_timerHandle;
	char midi_status;  // this is used to rotate an activity indicator when MIDI is received.
	unsigned char midi_has_been_initialized; // problem starting midi before modal dialog returns. Generating an error when "UpdateData(FALSE)" is run from the receive midi function. some sort of threading problem?
									// Now we initilize the midi after the modal init returns;

	HWND myhWnd;

private:
	UINT m_nNumMixers;
	HMIXER m_hMixer;	//DELETE
	MIXERCAPS m_mxcaps;

	//hid_device *handle;
	wchar_t mystring[200];
	wchar_t serial_string[200];
	
	float m_fVolume;
	float m_fPan;		//DELETE
	unsigned int m_dwLeft;
	unsigned int m_dwRight;
	char m_cSide;		//DELETE
	// = 0 for left, 1 for right

	int m_iCounter;
	int m_screen_update_counter;
	long m_ms;
	CString m_strDstLineName, m_strVolumeControlName;
	DWORD m_dwMinimum, m_dwMaximum;
	float m_fRange;
	DWORD m_dwVolumeControlID;	//DELETE
	UINT_PTR m_nCallbackTimer; 

	unsigned int calculated_pos_array[CALC_POS_ARRAY_SIZE];
	int calculated_pos_index;
	
	//void OnStopTimer();
	//void OnStartTimer();
	
	void CheckUsb();
	void StorePan(DWORD, DWORD);



	BOOL amdUninitialize();
	BOOL amdInitialize();
	BOOL amdGetMasterVolumeControl();
	//BOOL amdGetMasterVolumeValue(DWORD *dwVal) const;
	BOOL amdGetMasterVolumeValue(DWORD *dwVal);
	BOOL amdSetMasterVolumeValue(DWORD dwVal) const;

	char led_brightness;
	// file IO
	int lines_to_write;

	//file handle
	HANDLE hFile;
	//something to contain the number of bytes read
	DWORD dwNumWritten;
	//a boolean test variable, to test for success of reads
	BOOL bTest;
	//a buffer… can actually be of any type
	DWORD dwBuffer[256];
	char touch_active;
	float prev_volume;
	char prev_direction; // 1 is up, 0 is down

	

	// midi data
	unsigned char midi_channel;	

	unsigned char pos_touch_output_ctrl_num;
	unsigned char pos_touch_output_enabled;

	unsigned char pressure_output_enabled;
	unsigned char pressure_output_ctrl_num;
	unsigned char pressure_output_min;
	int pressure_output_max;
	float ave_pressure;

	unsigned char touch_on_off_output_enabled;
	unsigned char touch_on_off_output_ctrl_num;
	unsigned char touch_state;

	unsigned char raw_output_enabled;
	unsigned char raw_output_min;
	int raw_output_max;
	int raw_ref_values[8];

	unsigned char pitch_wheel_enable;

	unsigned char volume_control_enabled;

	// led control from midi:

	unsigned char led_control_from_pos_enabled;
	unsigned char led_control_source_ctrl_num;
	unsigned char ignore_touch;

	unsigned char midi_brightness_control_enabled;
	unsigned char midi_brightness_ctrl_num;

	unsigned char led_show_system_vol_enabled;

	




	


	
public:
	afx_msg void EnablePosMidiOutputChanged();
	afx_msg void OnPosOutputChannelChange();
	CComboBox m_PosOutputCtrlNum;
	CButton m_EnablePosOutputCheckBox;
	afx_msg void MidiOutputChannelChanged();
	CComboBox m_MidiChannelCombo;

	CButton m_EnPressureOutput;
	CButton m_EnTouchOnOff;
	CButton m_EnRawOutput;
	CButton m_EnPitchOut;
	CButton m_EnSysVolControl;
	CButton m_EnMidiLedPosControl;
	CButton m_EnMidiBrightessControl;
	CButton m_IgnoreTouch;
	CButton m_LedShowVolume;
	CComboBox m_PresMidiCtrlNumCombo;
	CComboBox m_TouchOnOffMidiCtrlNumCombo;
	CComboBox m_LedPosMidiCtrlNumCombo;
	CComboBox m_BrightnessMidiCtrlNumCombo;
	CComboBox m_PresMinCombo;
	CComboBox m_PresMaxCombo;
	CComboBox m_RawMinCombo;
	CComboBox m_RawMaxCombo;
	afx_msg void OnBnClickedCheckPresOutEn();
	afx_msg void OnBnClickedCheckOnOffEn();
	afx_msg void OnBnClickedCheckRawOutEn();
	afx_msg void OnBnClickedCheckPitchWheelEn();
	afx_msg void OnBnClickedCheckTouchVolEn();
	afx_msg void OnBnClickedCheckMidiLedEn();
	afx_msg void OnBnClickedCheckMidiBrightEn();
	afx_msg void OnBnClickedCheckIgnoreTouch();
	afx_msg void OnBnClickedCheckLedShowVolumeEn();
	afx_msg void OnCbnSelchangeComboPresCtrl();
	afx_msg void OnCbnSelchangeComboPresMin();
	afx_msg void OnCbnSelchangeComboPresMax();
	afx_msg void OnCbnSelchangeComboOnOffCtrl();
	afx_msg void OnCbnSelchangeComboRawMin();
	afx_msg void OnCbnSelchangeComboRawMax();
	afx_msg void OnCbnSelchangeComboLedMidiPosCtrl();
	afx_msg void OnCbnSelchangeComboLedBrightCtrl();
	CString m_midi_status_text;
	afx_msg void OnToolsLoaddefaults();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedRadio1();
	int vmeter_output_type;
	afx_msg void OnBnClickedRadio2();
	BOOL scale_by_system_volume;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCpuRadio();
};
