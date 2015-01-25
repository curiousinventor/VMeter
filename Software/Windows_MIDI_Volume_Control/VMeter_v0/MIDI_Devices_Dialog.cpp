// MIDI_Devices_Dialog.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "MIDIOutDevice.h"
#include "MIDIInDevice.h"
#include "resource.h"
#include "MIDI_Devices_Dialog.h"

// MIDI_Devices_Dialog dialog

IMPLEMENT_DYNAMIC(MIDI_Devices_Dialog, CDialog)

MIDI_Devices_Dialog::MIDI_Devices_Dialog(CWnd* pParent /*=NULL*/)
	: CDialog(MIDI_Devices_Dialog::IDD, pParent)
{

}

MIDI_Devices_Dialog::~MIDI_Devices_Dialog()
{
}

void MIDI_Devices_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MIDI_OUT_DEVS, m_OutDevsCombo);
	DDX_Control(pDX, IDC_MIDI_IN_DEVS, m_InDevsCombo);

}


BEGIN_MESSAGE_MAP(MIDI_Devices_Dialog, CDialog)
END_MESSAGE_MAP()


// MIDI_Devices_Dialog message handlers


BOOL MIDI_Devices_Dialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

    //
    // Initialize output device combo box with the names of all of the 
    // MIDI output devices available
    //
	
    UINT i;
	MIDIOUTCAPS OutCaps;
    for(i = 0; i < midi::CMIDIOutDevice::GetNumDevs(); i++)
    {
        midi::CMIDIOutDevice::GetDevCaps(i, OutCaps);
        m_OutDevsCombo.AddString(OutCaps.szPname);
    }

    if(midi::CMIDIOutDevice::GetNumDevs() > 0)
    {
        m_OutDevsCombo.SetCurSel(m_OutDevId);
    }

    //
    // Initialize input device combo box with the names of all of the
    // MIDI input devices available on this system
    //

    MIDIINCAPS InCaps;
    for(i = 0; i < midi::CMIDIInDevice::GetNumDevs(); i++)
    {
        midi::CMIDIInDevice::GetDevCaps(i, InCaps);
        m_InDevsCombo.AddString(InCaps.szPname);
    }

    if(midi::CMIDIInDevice::GetNumDevs() > 0)
    {
        m_InDevsCombo.SetCurSel(m_InDevId);
    }
	
	return TRUE;  
}


// Client clicked OK
void MIDI_Devices_Dialog::OnOK() 
{    
    // Check to see if the client changed the selection for either the
    // MIDI input or output devices. If so, indicate that a new 
    // selection has been made    

    if(m_OutDevsCombo.GetCount() > 0)
    {
        if(m_OutDevId != m_OutDevsCombo.GetCurSel())
        {
	        m_OutDevId = m_OutDevsCombo.GetCurSel();
            m_OutChanged = true;
        }
    }

    if(m_InDevsCombo.GetCount() > 0)
    {
        if(m_InDevId != m_InDevsCombo.GetCurSel())
        {
            m_InDevId = m_InDevsCombo.GetCurSel();
            m_InChanged = true;
        }
    }
	
	CDialog::OnOK();
}