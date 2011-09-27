// DSNDlg.cpp : implementation file
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
#include "MDIBind.h"
#include "DSNDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSrcDlg dialog


CDataSrcDlg::CDataSrcDlg(CString &DSN,CString &Sql,CString &User,CString &Pw,
						 CWnd* pParent /*=NULL*/)
	: CDialog(CDataSrcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataSrcDlg)
	m_DSN = DSN;
	m_User = User;
	m_Sql = Sql;
	m_Pw = Pw;
	//}}AFX_DATA_INIT
}


void CDataSrcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataSrcDlg)
	DDX_Text(pDX, IDC_DATASRC, m_DSN);
	DDX_Text(pDX, IDC_USER, m_User);
	DDX_Text(pDX, IDC_SQL, m_Sql);
	DDX_Text(pDX, IDC_PASWORD, m_Pw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataSrcDlg, CDialog)
	//{{AFX_MSG_MAP(CDataSrcDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataSrcDlg message handlers
