#include "ntray.h"
#include "resource.h" //main symbols

class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();
	virtual ~CMainFrame();

protected:
	DECLARE_DYNCREATE(CMainFrame)

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowMenu();
	
	afx_msg void OnQuitMenu();
	afx_msg void OnHappy();
	afx_msg void OnUpdateHappy(CCmdUI* pCmdUI);
	afx_msg void OnSad();
	afx_msg void OnUpdateSad(CCmdUI* pCmdUI);
	afx_msg void OnAnimated();
	afx_msg void OnUpdateAnimated(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateShow(CCmdUI* pCmdUI);
	afx_msg void OnShow();
	afx_msg void OnUpdateHide(CCmdUI* pCmdUI);
	afx_msg void OnHide();
	afx_msg void OnTestCaption();
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	

	DECLARE_MESSAGE_MAP()

//Member variables
  HICON    m_hIcon;
  CDC      m_TrayIconDC;
  CBitmap  m_BitmapTrayIcon;
public:
	afx_msg void Test2_function();
	afx_msg void OnHideMenu();
	afx_msg void OnBnClickedtesthide();
};

