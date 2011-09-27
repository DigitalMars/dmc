// sendvw.cpp : implementation file
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
#include "sendvw.h"

#include <stdlib.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

static CString RandomString();

/////////////////////////////////////////////////////////////////////////////
// CSendView

IMPLEMENT_DYNCREATE(CSendView, CEditView)

BEGIN_MESSAGE_MAP(CSendView, CEditView)
	//{{AFX_MSG_MAP(CSendView)
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_COMMAND(ID_AUTOCHATTER, OnAutochatter)
	ON_UPDATE_COMMAND_UI(ID_AUTOCHATTER, OnUpdateAutochatter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendView construction/destruction

CSendView::CSendView()
{
	m_TimerID = 0;
}

CSendView::~CSendView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CSendView drawing

void CSendView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CSendView diagnostics

#ifdef _DEBUG
void CSendView::AssertValid() const
{
	CEditView::AssertValid();
}

void CSendView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CChatDoc* CSendView::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CChatDoc, m_pDocument);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSendView message handlers

BOOL CSendView::PreCreateWindow(CREATESTRUCT& cs) 
{
	BOOL ret = CEditView::PreCreateWindow(cs);
	cs.style = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | ES_AUTOHSCROLL |
		ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL;
	return ret;
}

void CSendView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar != VK_RETURN) || (nRepCnt!=1))
	{
		CEditView::OnChar(nChar, nRepCnt, nFlags);
		return;
	}
	else
	{
		CChatDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		CString strText;
		GetEditCtrl().GetWindowText(strText);

		pDoc->SendMsg(strText);

		strText=_T("");
		GetEditCtrl().SetWindowText(strText);
	}
}

BOOL CSendView::DestroyWindow() 
{
	if (m_TimerID != 0)
		KillTimer(m_TimerID);
		
	return CEditView::DestroyWindow();
}

void CSendView::OnTimer(UINT nIDEvent) 
{
	CChatDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_bAutoChat)
	{
		CString temp = RandomString();

		if (!temp.IsEmpty())
			pDoc->SendMsg(temp);
	}
	else
	{
		KillTimer(m_TimerID);
		m_TimerID = 0;
	}
	
	CEditView::OnTimer(nIDEvent);
}

void CSendView::OnAutochatter() 
{
	CChatDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc->m_bAutoChat)
	{
		pDoc->m_bAutoChat = TRUE;
		m_TimerID = SetTimer(1, 1000, NULL);
	}
	else
	{
		pDoc->m_bAutoChat = FALSE;
		KillTimer(m_TimerID);
	}
}

void CSendView::OnUpdateAutochatter(CCmdUI* pCmdUI) 
{		
	CChatDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->SetCheck(pDoc->m_bAutoChat);
}

static CString RandomString()
{
	CString strResult;

	UINT val1 = (rand()%36) + IDS_STATEMENT01;
	UINT val2 = (rand()%6) + IDS_ADJECTIVE01;

	CString strStatement, strAdjective;

	if(!strStatement.LoadString(val1))
		return strResult;

	if(!strAdjective.LoadString(val2))
		return strResult;

	wsprintf(strResult.GetBuffer(strStatement.GetLength()+strAdjective.GetLength()),
		(LPCTSTR)strStatement,(LPCTSTR)strAdjective);
	strResult.ReleaseBuffer();

	return strResult;
}
