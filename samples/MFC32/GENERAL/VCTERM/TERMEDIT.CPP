// termedit.cpp : implementation of the CTermEdit class
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
#include "vcterm.h"
#include "termedit.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTermEdit

CTermEdit::CTermEdit()
{
	m_bEcho = FALSE;
}

CTermEdit::~CTermEdit()
{
}

BEGIN_MESSAGE_MAP(CTermEdit, CEdit)
	//{{AFX_MSG_MAP(CTermEdit)
	ON_WM_CHAR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTermEdit message handlers

int CTermEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pCommCtrl = ((CMainFrame*)AfxGetMainWnd())->GetCommCtrl();
	
	return 0;
}

void CTermEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString ch(nChar);
	VARIANT var;
	CCommCtrl* pCommCtrl = ((CMainFrame*)AfxGetMainWnd())->GetCommCtrl();
	if (pCommCtrl->GetPortOpen())
	{
		var.vt = VT_BSTR;
		var.bstrVal = ch.AllocSysString();
		pCommCtrl->SetOutput(var);
	}
	
	if (m_bEcho || !pCommCtrl->GetPortOpen())
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}

