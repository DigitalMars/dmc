// srvrvw.cpp : implementation of the CServerView class
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
#include "chatsrvr.h"

#include "srvrdoc.h"
#include "srvrvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerView

IMPLEMENT_DYNCREATE(CServerView, CEditView)

BEGIN_MESSAGE_MAP(CServerView, CEditView)
	//{{AFX_MSG_MAP(CServerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerView construction/destruction

CServerView::CServerView()
{
}

CServerView::~CServerView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CServerView drawing

void CServerView::OnDraw(CDC* pDC)
{
	CServerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

}

/////////////////////////////////////////////////////////////////////////////
// CServerView diagnostics

#ifdef _DEBUG
void CServerView::AssertValid() const
{
	CEditView::AssertValid();
}

void CServerView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CServerDoc* CServerView::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CServerDoc, m_pDocument);
}
#endif //_DEBUG

void CServerView::Message(LPCTSTR lpszMessage)
{
	CString strTemp = lpszMessage;
	strTemp += _T("\r\n");
	int len = GetWindowTextLength();
	GetEditCtrl().SetSel(len,len);
	GetEditCtrl().ReplaceSel(strTemp);
}
