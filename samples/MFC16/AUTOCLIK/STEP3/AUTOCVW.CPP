// autocvw.cpp : implementation of the CClikView class
//

#include "stdafx.h"
#include "autoclik.h"

#include "autocdoc.h"
#include "autocvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClikView

IMPLEMENT_DYNCREATE(CClikView, CView)

BEGIN_MESSAGE_MAP(CClikView, CView)
	//{{AFX_MSG_MAP(CClikView)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClikView construction/destruction

CClikView::CClikView()
{
	// TODO: add construction code here
}

CClikView::~CClikView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CClikView drawing

void CClikView::OnDraw(CDC* pDC)
{
	CClikDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDC->TextOut(pDoc->m_pt.x, pDoc->m_pt.y, pDoc->m_str);
}

/////////////////////////////////////////////////////////////////////////////
// CClikView printing

BOOL CClikView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CClikView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CClikView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CClikView diagnostics

#ifdef _DEBUG
void CClikView::AssertValid() const
{
	CView::AssertValid();
}

void CClikView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CClikDoc* CClikView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClikDoc)));
	return (CClikDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CClikView message handlers

void CClikView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClikDoc* pDoc = GetDocument();
	pDoc->m_pt = point;
	pDoc->Refresh();

	CView::OnLButtonDown(nFlags, point);
}
