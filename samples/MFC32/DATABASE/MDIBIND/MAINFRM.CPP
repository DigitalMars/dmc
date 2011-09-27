// MainFrm.cpp : implementation of the CMainFrame class
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "MDIBind.h"
#include "MDIDoc.h"
#include "Maskview.h"
#include "RDCView.h"
#include "GridView.h"

#include "DSNDlg.h"
#include "RDCDlg.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_BIND, OnWindowBind)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_BIND, OnUpdateWindowBind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
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
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
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

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	// be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnWindowBind() 
{
	CView* pActiveView=NULL;
	if(MDIGetActive()!=NULL)
		pActiveView=MDIGetActive()->GetActiveView();
    if(!pActiveView)
		return;
	CMDIBindDoc* pDocument=(CMDIBindDoc*) pActiveView->GetDocument();
	if(pActiveView->IsKindOf(RUNTIME_CLASS(CRDCView)))
	{ // dialog for RDC control
		CMcsDsc* pRDCCtl=((CRDCView*) pActiveView)->m_pRDCCtl;
		CString DSN=pRDCCtl->GetDataSourceName();
		CString Sql=pRDCCtl->GetSql();
		CString User=pRDCCtl->GetUserName();
		CString Pw=pRDCCtl->GetPassword();
		CDataSrcDlg	dlg(DSN,Sql,User,Pw);
		if(dlg.DoModal()==IDOK)
		{
			((CRDCView*) pActiveView)->Bind(dlg.m_DSN,dlg.m_Sql,dlg.m_User,dlg.m_Pw);
			// TODO bound controls are refreshed,
			// but here you might check for update of their UI managed by us
			// Grids bound to: check the SQL title
			// MsMasks bound to: check if FieldName valid any longer
		}
	}
	else
	{ // dialog for DataBound control
		COpenControlsDlg dlg(pDocument);
		if(dlg.DoModal()!=IDOK)
			return;
		if(dlg.m_pRDCView!=NULL)
		{
			// save the pointer to the new RDC & column name
			pDocument->m_pRDC=dlg.m_pRDCView->m_pRDCCtl;
			pDocument->m_boundCol=dlg.m_ColName;
			// add the RDC window title to the control window title 
			int colonpos;
			CString strTitle=pDocument->GetTitle();
			if((colonpos=strTitle.Find(':'))>0)
				strTitle=strTitle.Left(colonpos); // in case it was already bound
			CString strRDCTitle; 
			dlg.m_pRDCView->GetParent()->GetWindowText(strRDCTitle); //title of RDC window
			strTitle +=_T(":") + strRDCTitle;
			if(!dlg.m_ColName.IsEmpty())	 //the column name(simple control)
				strTitle += "-" + dlg.m_ColName;	
			pDocument->SetTitle(strTitle); // new window caption
			// rebind all controls (other views may be  affected)
			pDocument->UpdateAllViews(NULL); 
		}
	}
	
}

void CMainFrame::OnUpdateWindowBind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(MDIGetActive()!=NULL);
	
}
