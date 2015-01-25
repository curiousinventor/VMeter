
// VMeter_v0.cpp : Defines the class behaviors for the application.
//

// memory leak detection:
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "stdafx.h"
#include "VMeter_v0.h"
#include "VMeter_v0Dlg.h"
#include <initguid.h>
#include "VMeter_v0_i.c"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVMeter_v0App

class CVMeter_v0Module :
	public ATL::CAtlMfcModule
{
public:
	DECLARE_LIBID(LIBID_VMeter_v0Lib);
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_VMETER_V0, "{72036D87-C13A-484E-95F6-FE1B399E1B1B}");};

CVMeter_v0Module _AtlModule;

BEGIN_MESSAGE_MAP(CVMeter_v0App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVMeter_v0App construction

CVMeter_v0App::CVMeter_v0App()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CVMeter_v0App object

CVMeter_v0App theApp;


// CVMeter_v0App initialization

BOOL CVMeter_v0App::InitInstance()
{
	AfxOleInit();
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	// Register class factories via CoRegisterClassObject().
	if (FAILED(_AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE)))
		return FALSE;
	#endif // !defined(_WIN32_WCE) || defined(_CE_DCOM)
	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Don't show the main window
		return TRUE;
	}
	// App was launched with /Unregserver or /Unregister switch.
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		_AtlModule.UpdateRegistryAppId(FALSE);
		_AtlModule.UnregisterServer(TRUE);
		return FALSE;
	}
	// App was launched with /Register or /Regserver switch.
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
	{
		_AtlModule.UpdateRegistryAppId(TRUE);
		_AtlModule.RegisterServer(TRUE);
		return FALSE;
	}

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("CuriousInventor"));

	WriteProfileInt(_T("Default"),_T("SomeIntValue"),100);
	int ReadValue = GetProfileInt(_T("Default"),_T("SomeIntValue"),11);

	CVMeter_v0Dlg dlg;
	//m_pMainWnd = &dlg;


	CMainFrame* pMainFrame = new CMainFrame;
	//CMainFrame* temp_MainFrame = new CMainFrame;
	//pMainFrame = temp_MainFrame;
	//testint = 5;
	//CMainFrame * test5;
	//test5 = new CMainFrame;
	//pMainFrame  = new CMainFrame;
	m_pMainWnd = pMainFrame;
	if (!pMainFrame->Create(NULL, _T("VMeter")))
		return FALSE;

	m_pMainWnd->ShowWindow(SW_HIDE);
	m_pMainWnd->UpdateWindow();


	
	/*INT_PTR nResponse = dlg.DoModal(); // display the window
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}*/
	OutputDebugString((LPCWSTR)L"got here345\n");
	if(dlg.Create( CVMeter_v0Dlg::IDD )) {
		dlg.ShowWindow( SW_HIDE );
		dlg.my_parent_win = this;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.RunModalLoop();
	}
	OutputDebugString((LPCWSTR)L"got here346\n");
	// Delete the shell manager created above.
	if (pMainFrame != NULL) {
		delete pMainFrame;
	}


	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
//	if (pMainFrame != NULL) {
//		delete pMainFrame;
//	}


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	//return FALSE;
	
	return TRUE;
}

//CMainFrame* CVMeter_v0App::get_pMainFrame() {
	//CMainFrame * test = new CMainFrame;
	//return pMainFrame;
	//return pMainFrame;
//}


BOOL CVMeter_v0App::ExitInstance(void)
{
#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	_AtlModule.RevokeClassObjects();
#endif
	return CWinApp::ExitInstance();
}

void CVMeter_v0App::test_func(void) {
	//OutputDebugString((LPCWSTR)L"test_func 341\n");
	CMainFrame* temp;
	temp = (CMainFrame*)m_pMainWnd;
	temp->OnHideMenu();
}