// oldbavw.cpp : implementation of the COldbarsView class
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
#include "oldbars.h"

#include "oldbadoc.h"
#include "oldbavw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COldbarsView

IMPLEMENT_DYNCREATE(COldbarsView, CView)

BEGIN_MESSAGE_MAP(COldbarsView, CView)
	//{{AFX_MSG_MAP(COldbarsView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COldbarsView construction/destruction

COldbarsView::COldbarsView()
{
	// TODO: add construction code here

}

COldbarsView::~COldbarsView()
{
}

/////////////////////////////////////////////////////////////////////////////
// COldbarsView drawing

void COldbarsView::OnDraw(CDC* pDC)
{
	COldbarsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// COldbarsView printing

BOOL COldbarsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COldbarsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COldbarsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// COldbarsView diagnostics

#ifdef _DEBUG
void COldbarsView::AssertValid() const
{
	CView::AssertValid();
}

void COldbarsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COldbarsDoc* COldbarsView::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(COldbarsDoc, m_pDocument);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COldbarsView message handlers
