/*
Module : NTray.h
Purpose: Interface for a MFC class to encapsulate Shell_NotifyIcon
Created: PJN / 14-05-1997

Copyright (c) 1997 - 2010 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


/////////////////////////// Macros / Defines ///////////////////////////

#pragma once

#ifndef _NTRAY_H__
#define _NTRAY_H__

#ifndef CTRAYNOTIFYICON_EXT_CLASS
#define CTRAYNOTIFYICON_EXT_CLASS
#endif

#ifndef __ATLWIN_H__
#pragma message("CTrayNotifyIcon as of v1.51 requires ATL support to implement its functionality. If your project is MFC only, then you need to update it to include ATL support")
#endif

#ifndef _AFX
#include <atlmisc.h> //If you do want to use CTrayNotifyIcon independent of MFC, then you need to download and install WTL from http://sourceforge.net/projects/wtl
#define CTrayNotifyIconString _CSTRING_NS::CString
#else
#define CTrayNotifyIconString CString
#endif


/////////////////////////// Classes ///////////////////////////////////////////

//the actual tray notification class wrapper
class CTRAYNOTIFYICON_EXT_CLASS CTrayNotifyIcon : public CWindowImpl<CTrayNotifyIcon>
{
public:
//Enums / Typedefs
#ifndef CTRAYNOTIFYICON_NOWIN2K
  enum BalloonStyle
  {
    Warning,
    Error,
    Info,
    None,
    User
  };
#endif

//We use our own definitions of the NOTIFYICONDATA structs so that
//we can use all the functionality without requiring client code to 
//define _WIN32_IE >= 0x500
  typedef struct _NOTIFYICONDATA_1 //The version of the structure supported by Shell v4
  {
    DWORD cbSize;
    HWND hWnd;
    UINT uID;
    UINT uFlags;
    UINT uCallbackMessage;
    HICON hIcon;
    TCHAR szTip[64];
  } NOTIFYICONDATA_1;

  typedef struct _NOTIFYICONDATA_2 //The version of the structure supported by Shell v5
  {
    DWORD cbSize;
    HWND hWnd;
    UINT uID;
    UINT uFlags;
    UINT uCallbackMessage;
    HICON hIcon;
    TCHAR szTip[128];
    DWORD dwState;
    DWORD dwStateMask;
    TCHAR szInfo[256];
    union 
    {
      UINT uTimeout;
      UINT uVersion;
    } DUMMYUNIONNAME;
    TCHAR szInfoTitle[64];
    DWORD dwInfoFlags;
  } NOTIFYICONDATA_2;

  typedef struct _NOTIFYICONDATA_3 //The version of the structure supported by Shell v6
  {
    DWORD cbSize;
    HWND hWnd;
    UINT uID;
    UINT uFlags;
    UINT uCallbackMessage;
    HICON hIcon;
    TCHAR szTip[128];
    DWORD dwState;
    DWORD dwStateMask;
    TCHAR szInfo[256];
    union 
    {
      UINT uTimeout;
      UINT uVersion;
    } DUMMYUNIONNAME;
    TCHAR szInfoTitle[64];
    DWORD dwInfoFlags;
    GUID guidItem;
  } NOTIFYICONDATA_3;

  typedef struct _NOTIFYICONDATA_4 //The version of the structure supported by Shell v7
  {
    DWORD cbSize;
    HWND hWnd;
    UINT uID;
    UINT uFlags;
    UINT uCallbackMessage;
    HICON hIcon;
    TCHAR szTip[128];
    DWORD dwState;
    DWORD dwStateMask;
    TCHAR szInfo[256];
    union 
    {
      UINT uTimeout;
      UINT uVersion;
    } DUMMYUNIONNAME;
    TCHAR szInfoTitle[64];
    DWORD dwInfoFlags;
    GUID guidItem;
    HICON hBalloonIcon;
  } NOTIFYICONDATA_4;

  DECLARE_WND_CLASS(_T("TrayNotifyIconClass"))


//Constructors / Destructors
  CTrayNotifyIcon();
  virtual ~CTrayNotifyIcon();

//Create the tray icon
#ifdef _AFX
  BOOL Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, HICON hIcon, UINT nNotifyMessage, UINT uMenuID = 0, BOOL bShow = TRUE);
  BOOL Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, CBitmap* pBitmap, UINT nNotifyMessage, UINT uMenuID = 0, BOOL bShow = TRUE);
  BOOL Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, HICON* phIcons, int nNumIcons, DWORD dwDelay, UINT nNotifyMessage, UINT uMenuID = 0, BOOL bShow = TRUE);
  BOOL Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, UINT nTimeout, BalloonStyle style, HICON hIcon, UINT nNotifyMessage, UINT uMenuID = 0, BOOL bNoSound = FALSE, BOOL bLargeIcon = FALSE, BOOL bRealtime = FALSE, HICON hBalloonIcon = NULL, BOOL bQuietTime = FALSE, BOOL bShow = TRUE);                  
  BOOL Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, UINT nTimeout, BalloonStyle style, CBitmap* pBitmap, UINT nNotifyMessage, UINT uMenuID = 0, BOOL bNoSound = FALSE, BOOL bLargeIcon = FALSE, BOOL bRealtime = FALSE, HICON hBalloonIcon = NULL, BOOL bQuietTime = FALSE, BOOL bShow = TRUE);
  BOOL Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, UINT nTimeout, BalloonStyle style, HICON* phIcons, int nNumIcons, DWORD dwDelay, UINT nNotifyMessage, UINT uMenuID = 0, BOOL bNoSound = FALSE, BOOL bLargeIcon = FALSE, BOOL bRealtime = FALSE, HICON hBalloonIcon = NULL, BOOL bQuietTime = FALSE, BOOL bShow = TRUE);
#else
  BOOL Create(CWindow* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, HICON hIcon, UINT nNotifyMessage, UINT uMenuID = 0, BOOL bShow = TRUE);
  BOOL Create(CWindow* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, CBitmap* pBitmap, UINT nNotifyMessage, UINT uMenuID = 0, BOOL bShow = TRUE);
  BOOL Create(CWindow* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, HICON* phIcons, int nNumIcons, DWORD dwDelay, UINT nNotifyMessage, UINT uMenuID = 0, BOOL bShow = TRUE);
  BOOL Create(CWindow* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, UINT nTimeout, BalloonStyle style, HICON hIcon, UINT nNotifyMessage, UINT uMenuID = 0, BOOL bNoSound = FALSE, BOOL bLargeIcon = FALSE, BOOL bRealtime = FALSE, HICON hBalloonIcon = NULL, BOOL bQuietTime = FALSE, BOOL bShow = TRUE);                  
  BOOL Create(CWindow* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, UINT nTimeout, BalloonStyle style, CBitmap* pBitmap, UINT nNotifyMessage, UINT uMenuID = 0, BOOL bNoSound = FALSE, BOOL bLargeIcon = FALSE, BOOL bRealtime = FALSE, HICON hBalloonIcon = NULL, BOOL bQuietTime = FALSE, BOOL bShow = TRUE);
  BOOL Create(CWindow* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, UINT nTimeout, BalloonStyle style, HICON* phIcons, int nNumIcons, DWORD dwDelay, UINT nNotifyMessage, UINT uMenuID = 0, BOOL bNoSound = FALSE, BOOL bLargeIcon = FALSE, BOOL bRealtime = FALSE, HICON hBalloonIcon = NULL, BOOL bQuietTime = FALSE, BOOL bShow = TRUE);
#endif  

//Sets or gets the Tooltip text
  BOOL                  SetTooltipText(LPCTSTR pszTooltipText);
  BOOL                  SetTooltipText(UINT nID);
  CTrayNotifyIconString GetTooltipText() const;
  int	                  GetTooltipMaxSize();

//Sets or gets the icon displayed
  BOOL SetIcon(HICON hIcon);
  BOOL SetIcon(CBitmap* pBitmap);
  BOOL SetIcon(LPCTSTR lpIconName);
  BOOL SetIcon(UINT nIDResource);
  BOOL SetIcon(HICON* phIcons, int nNumIcons, DWORD dwDelay);
  BOOL SetStandardIcon(LPCTSTR lpIconName);
  BOOL SetStandardIcon(UINT nIDResource);
  HICON GetIcon() const;
  BOOL UsingAnimatedIcon() const;

//Sets or gets the window to send notification messages to
#ifdef _AFX
  BOOL SetNotificationWnd(CWnd* pNotifyWnd);
  CWnd* GetNotificationWnd() const;
#else
  BOOL SetNotificationWnd(CWindow* pNotifyWnd);
  CWindow* GetNotificationWnd() const;
#endif

//Modification of the tray icons
  BOOL Delete(BOOL bCloseHelperWindow = TRUE);
  BOOL Create(BOOL bShow = TRUE);
  BOOL Hide();
  BOOL Show();

//Dynamic tray icon support
  HICON BitmapToIcon(CBitmap* pBitmap);
  static BOOL GetDynamicDCAndBitmap(CDC* pDC, CBitmap* pBitmap);

//Modification of the menu to use as the context menu
  void SetMenu(HMENU hMenu);
  CMenu& GetMenu();
  void SetDefaultMenuItem(UINT uItem, BOOL fByPos);
  void GetDefaultMenuItem(UINT& uItem, BOOL& fByPos) { uItem = m_nDefaultMenuItem; fByPos = m_bDefaultMenuItemByPos; };

//Default handler for tray notification message
  virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);

//Status information
  BOOL IsShowing() const { return !IsHidden(); };
  BOOL IsHidden() const { return m_bHidden; };

//Sets or gets the Balloon style tooltip settings
  BOOL                  SetBalloonDetails(LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, BalloonStyle style, UINT nTimeout, HICON hUserIcon = NULL, BOOL bNoSound = FALSE, BOOL bLargeIcon = FALSE, BOOL bRealtime = FALSE, HICON hBalloonIcon = NULL);
  CTrayNotifyIconString GetBalloonText() const;
  CTrayNotifyIconString GetBalloonCaption() const;
  BalloonStyle          GetBalloonStyle() const;
  UINT                  GetBalloonTimeout() const;

//Other functionality
  BOOL SetVersion(UINT uVersion);
  BOOL SetFocus();

//Helper functions to load tray icon from resources
  static HICON LoadIcon(LPCTSTR lpIconName, BOOL bLargeIcon = FALSE);
  static HICON LoadIcon(UINT nIDResource, BOOL bLargeIcon = FALSE);
  static HICON LoadIcon(HINSTANCE hInstance, LPCTSTR lpIconName, BOOL bLargeIcon = FALSE);
  static HICON LoadIcon(HINSTANCE hInstance, UINT nIDResource, BOOL bLargeIcon = FALSE);

protected:
//Methods
  BOOL         CreateHelperWindow();
  void         StartAnimation(HICON* phIcons, int nNumIcons, DWORD dwDelay);
  void         StopAnimation();
  HICON        GetCurrentAnimationIcon() const;
  virtual BOOL ProcessWindowMessage(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID);
  LRESULT      OnTaskbarCreated(WPARAM wParam, LPARAM lParam);
  void         OnTimer(UINT_PTR nIDEvent);
  void         OnDestroy();
  DWORD        GetNOTIFYICONDATASizeForOS();

//Enums
  enum ShellVersion
  {
    Version4     = 0, //PreWin2k
    Version5     = 1, //Win2k
    Version6     = 2, //XP
    VersionVista = 3, //Vista
    Version7     = 4, //Windows7
  };

//Member variables
  NOTIFYICONDATA_4 m_NotifyIconData;
  BOOL             m_bCreated;
  BOOL             m_bHidden;
#ifdef _AFX  
  CWnd*            m_pNotificationWnd;
#else
  CWindow*         m_pNotificationWnd;
#endif  
  CMenu            m_Menu;
  UINT             m_nDefaultMenuItem;
  BOOL             m_bDefaultMenuItemByPos;
  ShellVersion     m_ShellVersion;
  HICON            m_hDynamicIcon; //Our cached copy of the last icon created with BitmapToIcon
  HICON*           m_phIcons;
  int              m_nNumIcons;
  UINT_PTR         m_nTimerID;
  int              m_nCurrentIconIndex;
  int              m_nTooltipMaxSize;
};

#endif //_NTRAY_H__
