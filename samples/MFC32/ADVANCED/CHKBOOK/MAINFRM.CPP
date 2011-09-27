// mainfrm.cpp : implementation of the CMainFrame class
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

#include "stdafx.h"
#include "chkbook.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_COMMAND(ID_VIEW_CHECK, OnViewCheck)
	ON_COMMAND(ID_VIEW_BOOK, OnViewBook)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_EDIT_NEW_CHECK,
	ID_EDIT_COMMIT_CHECK,
		ID_SEPARATOR,
	ID_PREV_CHECK,
	ID_NEXT_CHECK,
		ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_APP_ABOUT,
};

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
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
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}


void CMainFrame::CreateOrActivateFrame(CDocTemplate* pTemplate,
	CRuntimeClass* pViewClass)
{
	// If a check view or book view (specified by pViewClass)
	// already exists, then activate the MDI child window containing
	// the view.  Otherwise, create a new view for the document.

	CMDIChildWnd* pMDIActive = MDIGetActive();
	ASSERT(pMDIActive != NULL);
	CDocument* pDoc = pMDIActive->GetActiveDocument();
	ASSERT(pDoc != NULL);

	CView* pView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(pViewClass))
		{
			pView->GetParentFrame()->ActivateFrame();
			return;
		}
	}

	CMDIChildWnd* pNewFrame
		= (CMDIChildWnd*)(pTemplate->CreateNewFrame(pDoc, NULL));
	if (pNewFrame == NULL)
		return;     // not created
	ASSERT_KINDOF(CMDIChildWnd, pNewFrame);
	pTemplate->InitialUpdateFrame(pNewFrame, pDoc);
	MDITile(MDITILE_HORIZONTAL);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame commands

void CMainFrame::OnViewCheck()
{
	CreateOrActivateFrame(theApp.m_pCheckViewTemplate,
		RUNTIME_CLASS(CCheckView));
}

void CMainFrame::OnViewBook()
{
	CreateOrActivateFrame(theApp.m_pBookViewTemplate,
		RUNTIME_CLASS(CBookView));
}
