// dlltrace.cpp : Defines the class behaviors for the application.
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// This version of hello demonstrates communication with
// a dynamic link library (DLL) implemented with the _USRDLL
//version of the Microsoft Foundation Classes.

#include <afxwin.h>
#include "resource.h"
#include "traceapi.h"

// Note: This application doesn't really serve any useful purpose in
//  either debug or release builds (it does not modify AFX.INI like
//  the TRACER application does).  In debug builds, it is possible to
//  tell that calling the dialog box actually changes the flags, since
//  TRACE output is either enabled or disabled.  In release builds,
//  the tracing facility of MFC is disabled unconditionally, so there
//  is no way to tell that the flags have been changed, except for
//  the fact that they are retained across calls to display the dialog
//  box.

#ifndef _DEBUG
static BOOL afxTraceEnabled;
static int afxTraceFlags;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWindow

class CMainWindow : public CFrameWnd
{
public:
	CMainWindow();

	//{{AFX_MSG(CMainWindow)
	afx_msg void OnPaint();
	afx_msg void OnAbout();
	afx_msg void OnTraceFlags();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMainWindow, CFrameWnd)
	//{{AFX_MSG_MAP(CMainWindow)
	ON_WM_PAINT()
	ON_COMMAND(ID_APP_ABOUT, OnAbout)
	ON_COMMAND(ID_TRACE_FLAGS, OnTraceFlags)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CMainWindow::CMainWindow()
{
	VERIFY(Create(NULL, _T("Hello Foundation Application"),
		WS_OVERLAPPEDWINDOW, rectDefault, NULL,
		MAKEINTRESOURCE(IDR_MAINMENU)));
}

void CMainWindow::OnPaint()
{
	CString s = "Hello, Windows! (with DLL support)";
	CPaintDC dc(this);
	CRect rect;

	GetClientRect(rect);
	dc.SetTextAlign(TA_BASELINE | TA_CENTER);
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(rect.right / 2, rect.bottom / 2, s);
}

void CMainWindow::OnAbout()
{
	CDialog(IDD_ABOUTBOX).DoModal();
}

void CMainWindow::OnTraceFlags()
{
	struct TracerData data;
	data.bEnabled = afxTraceEnabled;
	data.flags = afxTraceFlags;

	TRACE0("Calling Tracer DLL\n");
	if (PromptTraceFlags(this->m_hWnd, &data))
	{
		TRACE0("Changing trace flags\n");
		afxTraceEnabled = data.bEnabled;
		afxTraceFlags = data.flags;
		TRACE0("Changed trace flags\n");
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTheApp

class CTheApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle(LONG lCount);
};

BOOL CTheApp::InitInstance()
{
	// standard initialization of a main window
	Enable3dControls();

	m_pMainWnd = new CMainWindow;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

BOOL CTheApp::PreTranslateMessage(MSG* pMsg)
{
	// special filter for DLL
	if (CWinApp::PreTranslateMessage(pMsg))
		return TRUE;

	return FilterDllMsg(pMsg);
}

BOOL CTheApp::OnIdle(LONG lCount)
{
	if (CWinApp::OnIdle(lCount))
		return TRUE;

	ProcessDllIdle();
	return FALSE;   // no more for me to do
}

CTheApp NEAR theApp; // application object

/////////////////////////////////////////////////////////////////////////////
