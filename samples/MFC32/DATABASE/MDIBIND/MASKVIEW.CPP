// MaskView.cpp : implementation of the CMaskView class
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
#include "MaskView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaskView

IMPLEMENT_DYNCREATE(CMaskView, CView)

BEGIN_MESSAGE_MAP(CMaskView, CView)
	//{{AFX_MSG_MAP(CMaskView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaskView construction/destruction

CMaskView::CMaskView()
{
	m_pMaskCtl = new CMsmask;
}

CMaskView::~CMaskView()
{
	delete m_pMaskCtl;
}

BOOL CMaskView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMaskView drawing

void CMaskView::OnDraw(CDC* pDC)
{
	CMDIBindDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMaskView diagnostics

#ifdef _DEBUG
void CMaskView::AssertValid() const
{
	CView::AssertValid();
}

void CMaskView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMDIBindDoc* CMaskView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDIBindDoc)));
	return (CMDIBindDoc*) m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMaskView message handlers

int CMaskView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	RECT r=  { 2,2,400,300 };

	m_pMaskCtl->Create(NULL,NULL,
					   WS_VISIBLE | WS_CHILD, r, this, 1);
	
	return 0;
}

void CMaskView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if (::IsWindow(m_pMaskCtl->m_hWnd) )
		m_pMaskCtl->MoveWindow(2,2,cx-4,cy-4,FALSE);  // do not repaint

	
}

// binds the grid control to the RDC control
void CMaskView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint )
{
   	CMDIBindDoc* pDoc=GetDocument();
	m_pMaskCtl->BindDefaultProperty(0x16,VT_BSTR,
		pDoc->m_boundCol,
		pDoc->m_pRDC); // rebind the control to new RDC & column
}



// simple DataBound control must be unbound when destroyed
void CMaskView::OnDestroy() 
{
	m_pMaskCtl->BindDefaultProperty(0x16,VT_BSTR,NULL,NULL); // unbind from RDC
	CView::OnDestroy();
}
