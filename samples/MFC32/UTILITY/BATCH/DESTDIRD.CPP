/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1994 - 1995	Microsoft Corporation.	All Rights Reserved.
 *
 **************************************************************************/
// destdird.cpp : implementation file
//
#ifndef UNICODE
#define OLE2ANSI
#endif

#include "stdafx.h"
#include "batch.h"
#include "destdird.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDestDirDlg dialog


CDestDirDlg::CDestDirDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDestDirDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDestDirDlg)
	m_DestDir = _T("");
	//}}AFX_DATA_INIT
}

void CDestDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDestDirDlg)
	DDX_Text(pDX, IDC_EDIT1, m_DestDir);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDestDirDlg, CDialog)
	//{{AFX_MSG_MAP(CDestDirDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDestDirDlg message handlers
