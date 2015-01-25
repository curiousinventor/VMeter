
// VMeter_v0.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "VMeter_v0_i.h"
//#include "MainFrm.h"



class CVMeter_v0App : public CWinApp
{
private:
	//CMainFrame* pMainFrame;
public:
	CVMeter_v0App();

// Overrides
public:
	void test_func(void);
	virtual BOOL InitInstance();
	int testint;
// Implementation
//	CMainFrame* get_pMainFrame();
	DECLARE_MESSAGE_MAP()
	BOOL ExitInstance(void);
	
};

extern CVMeter_v0App theApp;