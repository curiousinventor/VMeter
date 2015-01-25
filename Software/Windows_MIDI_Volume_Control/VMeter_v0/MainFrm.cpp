#include "stdafx.h"
#include "MainFrm.h"
#include "VMeter_v0.h"
#include "VMeter_v0Dlg.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_TRAYNOTIFY WM_USER + 100

CTrayNotifyIcon g_TrayIcon1;
CTrayNotifyIcon g_TrayIcon2;
CTrayNotifyIcon g_TrayIcon3;


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID__SHOWMENUITEM, OnShowMenu)
	ON_COMMAND(ID__HIDEMENUITEM, OnHideMenu)
	ON_COMMAND(ID__QUIT, OnQuitMenu)

	
	//
	//ON_COMMAND(ID_HAPPY, OnHappy)
	//ON_UPDATE_COMMAND_UI(ID_HAPPY, OnUpdateHappy)
	//ON_COMMAND(ID_SAD, OnSad)
	//ON_UPDATE_COMMAND_UI(ID_SAD, OnUpdateSad)
	//ON_COMMAND(ID_ANIMATED, OnAnimated)
	//ON_UPDATE_COMMAND_UI(ID_ANIMATED, OnUpdateAnimated)
	//ON_COMMAND(ID_HIDE, OnHide)
	//ON_UPDATE_COMMAND_UI(ID_HIDE, OnUpdateHide)
	//ON_COMMAND(ID_SHOW, OnShow)
	//ON_UPDATE_COMMAND_UI(ID_SHOW, OnUpdateShow)
	//ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_TRAYNOTIFY, OnTrayNotification)
	
	ON_BN_CLICKED(IDC_testhide, &CMainFrame::OnBnClickedtesthide)
END_MESSAGE_MAP()

CMainFrame::CMainFrame() {
  m_hIcon = CTrayNotifyIcon::LoadIcon(ID_VMETER_ICON);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	//Let the base class do its thing
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;

	//if (!m_TrayIcon2.Create(this, IDR_TRAYPOPUP2, _T("Animated Tray Icon"), _T("Use the tray context menu to unhide the \"first tray icon\"!"), _T("Animated Tray Icon"), 10, CTrayNotifyIcon::Info, m_hIcons, 2, 600, WM_TRAYNOTIFY, IDR_TRAYPOPUP2))
	if (!g_TrayIcon1.Create(this, IDR_MENU2, _T("VMeter"), m_hIcon, WM_TRAYNOTIFY)) {
		AfxMessageBox(_T("Failed to create tray icon"), MB_OK | MB_ICONSTOP);
		return -1;
	}
	g_TrayIcon1.SetTooltipText(_T("VMeter: right click to change settings"));
	g_TrayIcon1.SetBalloonDetails(_T("Right click to change settings"), _T("VMeter"), CTrayNotifyIcon::Info, 10, m_hIcon, TRUE, FALSE, FALSE, m_hIcon);
	//For testing purposes, change the default menu item on the first icon to be the bottom item on the context menu
	g_TrayIcon1.SetDefaultMenuItem(ID_APP_ABOUT, FALSE);


	return 0;
}

LRESULT CMainFrame::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
  //Delegate all the work back to the default implementation in
  //CTrayNotifyIcon.
  g_TrayIcon1.OnTrayNotification(wParam, lParam);
  g_TrayIcon2.OnTrayNotification(wParam, lParam);
  g_TrayIcon3.OnTrayNotification(wParam, lParam);
  return 0L;
}

void CMainFrame::OnShowMenu() {
	//g_TrayIcon1.SetTooltipText(_T("Show"));
	//g_TrayIcon1.SetBalloonDetails(_T("Show"), _T("VMeter"), CTrayNotifyIcon::Info, 2, m_hIcon, TRUE, FALSE, FALSE, m_hIcon);
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
}

void CMainFrame::OnHideMenu() {
//	g_TrayIcon1.SetTooltipText(_T("Hide"));
	g_TrayIcon1.SetBalloonDetails(_T("Right click to exit"), _T("VMeter"), CTrayNotifyIcon::Info, 2, m_hIcon, TRUE, FALSE, FALSE, m_hIcon);
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
}

void CMainFrame::OnQuitMenu() {
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}

/*void CMainFrame::OnHappy() 
//{
//	m_bHappy = !m_bHappy;
//
//  if (m_bHappy)
//  {
//    g_TrayIcon1.SetIcon(m_hIcons[0]);
//    g_TrayIcon1.SetTooltipText(_T("I'm Happy!"));
//  }
//  else
//  {
//    g_TrayIcon1.SetIcon(m_hIcons[1]);
//    g_TrayIcon1.SetTooltipText(_T("I'm Sad!"));
//  }
//}

void CMainFrame::OnUpdateHappy(CCmdUI* pCmdUI) 
{
  if (g_TrayIcon1.UsingAnimatedIcon())
    pCmdUI->Enable(FALSE);
  else
	  pCmdUI->SetCheck(m_bHappy);
}

void CMainFrame::OnSad() 
{
	OnHappy();
}

void CMainFrame::OnUpdateSad(CCmdUI* pCmdUI) 
{
  if (g_TrayIcon1.UsingAnimatedIcon())
    pCmdUI->Enable(FALSE);
  else
  	pCmdUI->SetCheck(!m_bHappy);
}

void CMainFrame::OnAnimated() 
{
	if (g_TrayIcon1.UsingAnimatedIcon())
  {
    g_TrayIcon1.SetIcon(m_hIcons[0]);
  }
  else
  {
    g_TrayIcon1.SetIcon(m_hIcons, 2, 500);
  }
}

void CMainFrame::OnUpdateAnimated(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(g_TrayIcon1.UsingAnimatedIcon());
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent) 
{
  if (m_nTimerID == nIDEvent)
  {
    //Draw into the tray icon bitmap
    CString sNum;
    sNum.Format(_T("%d"), m_nTimerCount);
    m_TrayIconDC.SetBkMode(TRANSPARENT);
    m_TrayIconDC.SetTextColor(RGB(255,255,255));
    int w = m_TrayIconDC.GetDeviceCaps(LOGPIXELSX);
    int h = m_TrayIconDC.GetDeviceCaps(LOGPIXELSY);
    CRect r;
    r.top=0;
    r.left=0;
    r.right=w;
    r.bottom=h;
    CBrush blackBrush;
    blackBrush.CreateStockObject(BLACK_BRUSH);
    m_TrayIconDC.FillRect(&r, &blackBrush);
    m_TrayIconDC.TextOut(0, 0, sNum);

    //Update it
    g_TrayIcon3.SetIcon(&m_BitmapTrayIcon);

    //Increment the count, ready for the next time aroun
    m_nTimerCount++;
    if (m_nTimerCount == 100)
      m_nTimerCount = 0;
  }
}*/

void CMainFrame::OnDestroy() 
{
  //Destroy our timer
  //KillTimer(m_nTimerID);

  //Let the parent class do its thing
	CFrameWnd::OnDestroy();
}

void CMainFrame::OnHide() 
{
  g_TrayIcon1.Hide();
}

void CMainFrame::OnUpdateHide(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!g_TrayIcon1.IsHidden());
} 

void CMainFrame::OnShow() 
{
  g_TrayIcon1.Show();
}

void CMainFrame::OnUpdateShow(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(g_TrayIcon1.IsHidden());
} 



void CMainFrame::OnBnClickedtesthide()
{
	//OnHideMenu();
	TCHAR str[255];	
	_stprintf(str, _T("tes4t"));	
	MessageBox(str, 0, 0);
}
