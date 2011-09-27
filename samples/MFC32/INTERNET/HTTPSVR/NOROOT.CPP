// NoRoot.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// Note that your filters probably won't have an #ifdef like this one.
// The project file for this sample allows you to build the sample as
// a statically linked regular MFC DLL (that is, with _AFXDLL defined)
// or as a DLL that doesn't use MFC classes aside from the ISAPI 
// support classes (that is, without _AFXDLL defined).

#include "stdafx.h"
#include "httpsvr.h"
#include "NoRoot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoRootDlg dialog


CNoRootDlg::CNoRootDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoRootDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNoRootDlg)
	m_strRoot = _T("");
	//}}AFX_DATA_INIT
}


void CNoRootDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNoRootDlg)
	DDX_Text(pDX, IDC_ROOTDIR, m_strRoot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNoRootDlg, CDialog)
	//{{AFX_MSG_MAP(CNoRootDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoRootDlg message handlers
