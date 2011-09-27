// tmlrdvw.cpp : implementation of the CTMLReadView class
//
// Copyright (c) XYZ Corp., 1994. All Rights Reserved.
//
//

#include "stdafx.h"
#include "tmlread.h"

#include "tmlrddoc.h"
#include "tmlrdvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTMLReadView

IMPLEMENT_DYNCREATE(CTMLReadView, CView)

BEGIN_MESSAGE_MAP(CTMLReadView, CView)
	//{{AFX_MSG_MAP(CTMLReadView)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTMLReadView construction/destruction

CTMLReadView::CTMLReadView()
{
  TRACE("CTMLReadView::CTMLReadView()\n");
	// TODO: add construction code here
}

CTMLReadView::~CTMLReadView()
{
  TRACE("CTMLReadView::~CTMLReadView()\n");
}

/////////////////////////////////////////////////////////////////////////////
// CTMLReadView drawing

void CTMLReadView::OnDraw(CDC* pDC)
{
  TRACE("CTMLReadView::OnDraw()\n");

	CTMLReadDoc* pDoc = GetDocument();

	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTMLReadView printing

BOOL CTMLReadView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTMLReadView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTMLReadView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


/////////////////////////////////////////////////////////////////////////////
// CTMLReadView diagnostics

#ifdef _DEBUG
void CTMLReadView::AssertValid() const
{
	CView::AssertValid();
}

void CTMLReadView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTMLReadDoc* CTMLReadView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTMLReadDoc)));
	return (CTMLReadDoc*) m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTMLReadView message handlers

void CTMLReadView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
    ::MessageBeep( -1);
}


void CTMLReadView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CTMLReadView::OnLButtonDown(UINT nFlags, CPoint point)
{
    AfxMessageBox( "Left button clicked!");
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonDown(nFlags, point);
}


BOOL CTMLReadView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CTMLReadView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CTMLReadView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CView::OnEraseBkgnd(pDC);
}

