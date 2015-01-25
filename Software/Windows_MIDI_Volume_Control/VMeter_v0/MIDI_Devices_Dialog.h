#pragma once
#include "afxwin.h"


// MIDI_Devices_Dialog dialog

class MIDI_Devices_Dialog : public CDialog
{
	DECLARE_DYNAMIC(MIDI_Devices_Dialog)



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

	
public:

	MIDI_Devices_Dialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MIDI_Devices_Dialog();

// Dialog Data
	enum { IDD = IDD_MIDI_DEVICES_DIALOG };

		// midi:
	UINT m_OutDevId;
    UINT m_InDevId;

    bool m_OutChanged;
    bool m_InChanged;
	CComboBox m_OutDevsCombo;
	CComboBox m_InDevsCombo;
};
