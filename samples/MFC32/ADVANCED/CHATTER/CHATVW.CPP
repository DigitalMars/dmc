// chatvw.cpp : implementation of the CChatView class
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
#include "chatter.h"

#include "chatdoc.h"
#include "chatvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatView

IMPLEMENT_DYNCREATE(CChatView, CEditView)

BEGIN_MESSAGE_MAP(CChatView, CEditView)
	//{{AFX_MSG_MAP(CChatView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatView construction/destruction

CChatView::CChatView()
{
}

CChatView::~CChatView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CChatView drawing

void CChatView::OnDraw(CDC* pDC)
{
	CChatDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CChatView diagnostics

#ifdef _DEBUG
void CChatView::AssertValid() const
{
	CEditView::AssertValid();
}

void CChatView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CChatDoc* CChatView::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CChatDoc, m_pDocument);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChatView message handlers

BOOL CChatView::PreCreateWindow(CREATESTRUCT& cs) 
{
	BOOL ret = CEditView::PreCreateWindow(cs);
	cs.style = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | ES_AUTOHSCROLL |
		ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_READONLY;
	return ret;
}

void CChatView::Message(LPCTSTR lpszMessage)
{
	CString strTemp = lpszMessage;
	strTemp += _T("\r\n");
	int len = GetWindowTextLength();
	GetEditCtrl().SetSel(len,len);
	GetEditCtrl().ReplaceSel(strTemp);
}

