// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include "penwin.h"     // MFC Apps are PenAware by default

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// other globals (internal library use)

#if !defined(_USRDLL) && !defined(_AFXCTL)
extern void (CALLBACK* _afxRegisterPenAppProc)(UINT, BOOL);
extern LRESULT CALLBACK AFX_EXPORT
	_AfxMsgFilterHook(int code, WPARAM wParam, LPARAM lParam);
#endif //!_USRDLL && !_AFXCTL

#ifdef _AFXDLL
extern BOOL WINAPI _AfxAppDataInit();       // from 'dllinit.cpp'
#endif

/////////////////////////////////////////////////////////////////////////////
// Standard init called by WinMain

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

static BOOL NEAR PASCAL RegisterWithIcon(WNDCLASS* pWndCls,
	const char FAR* pszClassName, UINT nIDIcon)
{
	pWndCls->lpszClassName = pszClassName;
	HINSTANCE hInst = AfxFindResourceHandle(
		MAKEINTRESOURCE(nIDIcon), RT_GROUP_ICON);
	if ((pWndCls->hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(nIDIcon))) == NULL)
	{
		// use default icon
		pWndCls->hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	}
	return RegisterClass(pWndCls);
}

/////////////////////////////////////////////////////////////////////////////

extern "C"
BOOL AFXAPI AfxWinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
#if !defined(_USRDLL) && !defined(_AFXCTL)
	// try to make the message queue bigger
	for (int cMsg = 96; !SetMessageQueue(cMsg) && (cMsg -= 8); )
		;
#endif //!_USRDLL && !_AFXCTL

#ifdef _AFXDLL
	if (!_AfxAppDataInit())
		return FALSE;
#endif

	// shared initialization
#if !defined(_USRDLL) && !defined(_AFXCTL)
	HINSTANCE hPenWin;
	if ((hPenWin = (HINSTANCE)GetSystemMetrics(SM_PENWINDOWS)) != NULL)
	{
		static char BASED_CODE szRegisterPenApp[] = "RegisterPenApp";
		_afxRegisterPenAppProc = (void (CALLBACK*)(UINT, BOOL))
			::GetProcAddress(hPenWin, szRegisterPenApp);
	}
#endif //!_USRDLL && !_AFXCTL

	// fill in the initial state for the application
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL)
	{
		pApp->m_hInstance = hInstance;
		pApp->m_hPrevInstance = hPrevInstance;
		pApp->m_lpCmdLine = lpCmdLine;
		pApp->m_nCmdShow = nCmdShow;
		pApp->SetCurrentHandles();
	}

	// Windows version specific initialization
#if !defined(_USRDLL) && !defined(_AFXCTL)
	if (!afxData.bWin31)
	{
		// set message filter proc
		_afxHHookOldMsgFilter = (HHOOK)::SetWindowsHook(WH_MSGFILTER,
			(HOOKPROC)_AfxMsgFilterHook);
	}
	else
	{
		// set message filter proc
		_afxHHookOldMsgFilter = ::SetWindowsHookEx(WH_MSGFILTER,
			(HOOKPROC)_AfxMsgFilterHook,
			_AfxGetHookHandle(), ::GetCurrentTask());
	}
#endif //!_USRDLL && !_AFXCTL

	if (hPrevInstance == NULL)  // one instance initialization
	{
		// register basic WndClasses
		WNDCLASS wndcls;
		memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults

		// common initialization
		wndcls.lpfnWndProc = AfxWndProc;
		wndcls.hInstance = hInstance;
		wndcls.hCursor = afxData.hcurArrow;

		// Child windows - no brush, no icon, safest default class styles
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpszClassName = _afxWnd;
		if (!::RegisterClass(&wndcls))
			return FALSE;

		// Control bar windows
		wndcls.style = 0;   // control bars don't handle double click
		wndcls.lpszClassName = _afxWndControlBar;
		wndcls.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		if (!::RegisterClass(&wndcls))
			return FALSE;

		// MDI Frame window (also used for splitter window)
		wndcls.style = CS_DBLCLKS;
		wndcls.hbrBackground = NULL;
		if (!RegisterWithIcon(&wndcls, _afxWndMDIFrame, AFX_IDI_STD_MDIFRAME))
			return FALSE;

		// SDI Frame or MDI Child windows or views - normal colors
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
		if (!RegisterWithIcon(&wndcls, _afxWndFrameOrView, AFX_IDI_STD_FRAME))
			return FALSE;
	}

#if !defined(_USRDLL) && !defined(_AFXCTL)
	// Register as a Pen aware app if penwindows installed
	if (_afxRegisterPenAppProc != NULL)
		(*_afxRegisterPenAppProc)(RPA_DEFAULT, TRUE);
#endif //!_USRDLL && !_AFXCTL

	// Handle critical errors ourself
	::SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
// CWinApp Initialization

void CWinApp::SetCurrentHandles()
{
	ASSERT(this == afxCurrentWinApp);
	ASSERT(afxCurrentInstanceHandle == NULL);
	ASSERT(afxCurrentResourceHandle == NULL);
	ASSERT(afxCurrentAppName == NULL);

	afxCurrentInstanceHandle = m_hInstance; // for instance tagging
	afxCurrentResourceHandle = m_hInstance; // for resource loading

	// get path of executable
	char szBuff[_MAX_PATH+1];
	VERIFY(::GetModuleFileName(m_hInstance, szBuff, _MAX_PATH));

	int nLen = strlen(szBuff);
	ASSERT(nLen > 4 && szBuff[nLen-4] == '.');  // must end in .EXE
	nLen -= 4;
	szBuff[nLen] = '\0';        // no suffix

	// get path of .HLP file
	if (m_pszHelpFilePath == NULL)
	{
		static char BASED_CODE szHlp[] = ".HLP";
		lstrcat(szBuff, szHlp);
		m_pszHelpFilePath = _strdup(szBuff);
		szBuff[nLen] = '\0';        // back to no suffix
	}

	// get the exe title from the full path name [no extension]
	char szExeName[_MAX_PATH];
	VERIFY(::GetFileTitle(szBuff, szExeName, _MAX_PATH) == 0);
	if (m_pszExeName == NULL)
		m_pszExeName = _strdup(szExeName); // save non-localized name

	if (m_pszProfileName == NULL)
	{
		static char BASED_CODE szIni[] = ".INI";
		lstrcat(szExeName, szIni);     // will be enough room in buffer
		m_pszProfileName = _strdup(szExeName);
	}

	// m_pszAppName is the name used to present to the user
	if (m_pszAppName == NULL)
	{
		char szTitle[256];
		if (_AfxLoadString(AFX_IDS_APP_TITLE, szTitle))
			m_pszAppName = _strdup(szTitle);             // human readable title
		else
			m_pszAppName = _strdup(m_pszExeName);       // same as EXE
	}

	afxCurrentAppName = m_pszAppName;
	ASSERT(afxCurrentAppName != NULL);
}

/////////////////////////////////////////////////////////////////////////////
