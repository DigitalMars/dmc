// ilbsvvw.cpp : implementation of the CIlbsvrView class
//

#include "stdafx.h"
#include "ilbsvr.h"

#include "ilbsvdoc.h"
#include "ilbsvvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrView

IMPLEMENT_DYNCREATE(CIlbsvrView, CView)

BEGIN_MESSAGE_MAP(CIlbsvrView, CView)
	//{{AFX_MSG_MAP(CIlbsvrView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrView construction/destruction

CIlbsvrView::CIlbsvrView()
{
	// TODO: add construction code here
}

CIlbsvrView::~CIlbsvrView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrView drawing

void CIlbsvrView::OnDraw(CDC* pDC)
{
	CIlbsvrDoc* pDoc = GetDocument();

	// TODO: add draw code here
}



/////////////////////////////////////////////////////////////////////////////
// CIlbsvrView diagnostics

#ifdef _DEBUG
void CIlbsvrView::AssertValid() const
{
	CView::AssertValid();
}

void CIlbsvrView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIlbsvrDoc* CIlbsvrView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIlbsvrDoc)));
	return (CIlbsvrDoc*) m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrView message handlers
