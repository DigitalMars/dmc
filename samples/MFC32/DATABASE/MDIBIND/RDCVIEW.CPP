// RDCView.cpp : implementation of the CRDCView class
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
#include "mcsdsc.h"
#include "MDIDoc.h"
#include "RDCView.h"
#include "GridFrm.h"
#include "MaskFrm.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRDCView

IMPLEMENT_DYNCREATE(CRDCView, CView)

BEGIN_MESSAGE_MAP(CRDCView, CView)
	//{{AFX_MSG_MAP(CRDCView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRDCView construction/destruction

CRDCView::CRDCView()
{
	m_pRDCCtl = new CMcsDsc;

}

CRDCView::~CRDCView()
{
	delete m_pRDCCtl;
}

BOOL CRDCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRDCView drawing

void CRDCView::OnDraw(CDC* pDC)
{
	CMDIBindDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CRDCView diagnostics

#ifdef _DEBUG
void CRDCView::AssertValid() const
{
	CView::AssertValid();
}

void CRDCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMDIBindDoc* CRDCView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDIBindDoc)));
	return (CMDIBindDoc*) m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRDCView message handlers

int CRDCView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CMDIBindDoc* pDoc=GetDocument();
	if(pDoc->m_pRDC!=NULL)
		return -1; // the RDC already created
	
	RECT r=  { 2,2,400,300 };

	m_pRDCCtl->Create(NULL,_T("NorthWind"),
					   WS_VISIBLE | WS_CHILD, r, this, 1);

	m_pRDCCtl->SetDataSourceName(_T("NorthWind"));
	m_pRDCCtl->SetSql(_T("SELECT FirstName, LastName, BirthDate, HireDate from Employees"));
	m_pRDCCtl->SetUserName(_T("admin"));
	m_pRDCCtl->SetPassword(_T(""));
	m_pRDCCtl->SetCursorDriver(2); //server side cursor
	pDoc->m_pRDC=m_pRDCCtl; // add newly created RDC
	
	return 0;
}

void CRDCView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if (::IsWindow(m_pRDCCtl->m_hWnd) )
		m_pRDCCtl->MoveWindow(2,2,cx-4,cy-4,FALSE);	 // do not repaint
	
}

// sets the value of the RDC control
void CRDCView::Bind(CString &DSN,CString &Sql,CString &User,CString &Pw)
{
	m_pRDCCtl->SetDataSourceName(DSN);
	m_pRDCCtl->SetSql(Sql);
	m_pRDCCtl->SetUserName(User);
	m_pRDCCtl->SetPassword(Pw);
	m_pRDCCtl->SetCaption(DSN);
	// TODO here some bound controls might want to update
	m_pRDCCtl->Refresh();  // must refresh all bound controls
}


void CRDCView::OnDestroy() 
{ // must unbind all bound controls before destroying the object
	CMDIBindDoc* pDoc;
 //	 how to get all documents
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT(pMainFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	// let's do it other way: iterate through all MDI windows
	CMDIChildWnd* pMDIChild = pMainFrame->MDIGetActive();
	if(pMDIChild != NULL)
		pMDIChild = (CMDIChildWnd*) pMDIChild->GetWindow(GW_HWNDLAST);
	while (pMDIChild != NULL)
	{
		if(pMDIChild->IsKindOf(RUNTIME_CLASS(CGridFrame)) ||
			pMDIChild->IsKindOf(RUNTIME_CLASS(CMaskFrame)))
		{ // data bound control windows
			CView* pView = pMDIChild->GetActiveView();
			pDoc=(CMDIBindDoc*) pView->GetDocument();
			if(pDoc->m_pRDC==m_pRDCCtl)
			{	 // unbind this and all other controls
				pDoc->m_pRDC=NULL;
				// TODO if you want to change the title to reflect
				// unbound state, do it here
				pDoc->UpdateAllViews(NULL);
			}
		}
		pMDIChild =  (CMDIChildWnd*) pMDIChild->GetWindow(GW_HWNDPREV);
	} //while
  
	CView::OnDestroy();	
}
