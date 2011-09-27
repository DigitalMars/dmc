// MainFrm.cpp : implementation of the CMainFrame class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// Note that your filters probably won't have an #ifdef like this one.
// The project file for this sample allows you to build the sample as
// a statically linked regular MFC DLL (that is, with _AFXDLL defined)
// or as a DLL that doesn't use MFC classes aside from the ISAPI 
// support classes (that is, without _AFXDLL defined).

#include "stdafx.h"

#include "HttpSvr.h"
#include "MainFrm.h"
#include "ReqSock.h"
#include "HttpDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_UPTIME, OnUpdateUpTime)
#ifdef IMPL_CGI
	ON_MESSAGE(WSM_CGIDONE, OnCGIDone)
#endif IMPL_CGI
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_UPTIME,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// start the uptime counter....
	CalcUpTime();
	// kick off the uptime update timer....
	m_uTimer = SetTimer( 1, 30000, NULL );

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnUpdateUpTime(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	if ( m_uTimer )
		KillTimer( m_uTimer );
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	CalcUpTime();
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::CalcUpTime()
{
	CString strUpTime;
	CHttpSvrDoc* pDoc = (CHttpSvrDoc*)GetActiveDocument();
	// only canculate if we have a doc and it's listening....
	if ( pDoc && pDoc->m_pListen )
	{
		CTime timeNow = CTime::GetCurrentTime();
		// calculate uptime and set the status bar....
		CTimeSpan upTime = timeNow - pDoc->m_timeStarted;
		UINT uFmt = upTime.GetDays() > 0 ? IDS_UPTIME_DAYS : IDS_UPTIME_DAY;
		strUpTime = upTime.Format( uFmt );
	}
	else
		strUpTime.Format( IDS_UPTIME_START );

	m_wndStatusBar.SetPaneText( 1, strUpTime, TRUE );
}

#ifdef IMPL_CGI
LRESULT CMainFrame::OnCGIDone( WPARAM, LPARAM lParam )
{
	CRequestSocket* pReqSock = (CRequestSocket*)lParam;
	pReqSock->CGIDone();
	return 0;
}
#endif // IMPL_CGI
