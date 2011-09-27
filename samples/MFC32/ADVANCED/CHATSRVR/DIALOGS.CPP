// dialogs.cpp : implementation file
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
#include "dialogs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiscussionDlg dialog


CDiscussionDlg::CDiscussionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiscussionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiscussionDlg)
	m_nPort = 0;
	//}}AFX_DATA_INIT
}


void CDiscussionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiscussionDlg)
	DDX_Text(pDX, IDC_EDIT1, m_nPort);
	DDV_MinMaxInt(pDX, m_nPort, 0, 99);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiscussionDlg, CDialog)
	//{{AFX_MSG_MAP(CDiscussionDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDiscussionDlg message handlers
