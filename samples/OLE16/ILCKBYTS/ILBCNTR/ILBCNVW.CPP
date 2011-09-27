// ilbcnvw.cpp : implementation of the CIlbcntrView class
//

#include "stdafx.h"
#include "ilbcntr.h"

#include "ilbcndoc.h"
#include "ilbcnvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrView

IMPLEMENT_DYNCREATE(CIlbcntrView, CView)

BEGIN_MESSAGE_MAP(CIlbcntrView, CView)
	//{{AFX_MSG_MAP(CIlbcntrView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrView construction/destruction

CIlbcntrView::CIlbcntrView()
{
	// TODO: add construction code here
}

CIlbcntrView::~CIlbcntrView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrView drawing

void CIlbcntrView::OnDraw(CDC* pDC)
{
	CIlbcntrDoc* pDoc = GetDocument();

	// TODO: add draw code here
}



/////////////////////////////////////////////////////////////////////////////
// CIlbcntrView diagnostics

#ifdef _DEBUG
void CIlbcntrView::AssertValid() const
{
	CView::AssertValid();
}

void CIlbcntrView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIlbcntrDoc* CIlbcntrView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIlbcntrDoc)));
	return (CIlbcntrDoc*) m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrView message handlers
