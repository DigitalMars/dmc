// GridView.cpp : implementation of the CGridView class
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
#include "GridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridView

IMPLEMENT_DYNCREATE(CGridView, CView)

BEGIN_MESSAGE_MAP(CGridView, CView)
	//{{AFX_MSG_MAP(CGridView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridView construction/destruction

CGridView::CGridView()
{
	m_pGridCtl = new CMsDgridCtrl;

}

CGridView::~CGridView()
{
	delete m_pGridCtl;
}

BOOL CGridView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGridView drawing

void CGridView::OnDraw(CDC* pDC)
{
	CMDIBindDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGridView diagnostics

#ifdef _DEBUG
void CGridView::AssertValid() const
{
	CView::AssertValid();
}

void CGridView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMDIBindDoc* CGridView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDIBindDoc)));
	return (CMDIBindDoc*) m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridView message handlers

int CGridView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	RECT r=  { 2,2,400,300 };

	m_pGridCtl->Create(NULL,_T(""),
					   WS_VISIBLE | WS_CHILD, r, this, 1);
	
	return 0;
}

void CGridView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if (::IsWindow(m_pGridCtl->m_hWnd) )
		m_pGridCtl->MoveWindow(2,2,cx-4,cy-4,FALSE); // do not repaint

	
}

// binds the grid control to the RDC control
// sets the Grid Caption to the SQL string from RDC control
void CGridView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint )
{
	CMDIBindDoc* pDoc=GetDocument();
	if(pDoc->m_pRDC!=NULL)
	{	// bind the control
		LPUNKNOWN pCursor=pDoc->m_pRDC->GetDSCCursor();
		ASSERT(pCursor!=NULL);
		// bind the grid to the actual RDC and RDC to the Grid
		m_pGridCtl->SetDataSource(pCursor);
		m_pGridCtl->BindProperty(0x9,pDoc->m_pRDC);
		// display the SQL caption
		m_pGridCtl->SetCaption(pDoc->m_pRDC->GetSql());	
	}
	else // unbind
	{
		m_pGridCtl->SetDataSource(NULL);
		m_pGridCtl->BindProperty(0x9,NULL);
		m_pGridCtl->SetCaption(NULL);	
	}			  
}

