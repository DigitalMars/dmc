// tmlreavw.cpp : implementation of the CTMLReadView class
//
// Copyright (c) Toxidyne Corporation, 1995. All Rights Reserved.
//
//

#include "stdafx.h"
#include "tmlread.h"

#include "tmlredoc.h"
#include "tmlreavw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTMLReadView

IMPLEMENT_DYNCREATE(CTMLReadView, CView)

BEGIN_MESSAGE_MAP(CTMLReadView, CView)
	//{{AFX_MSG_MAP(CTMLReadView)
		// NOTE - the ClassExpress will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
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
