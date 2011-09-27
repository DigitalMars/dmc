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
// interlea.cpp : implementation file
//
#ifndef UNICODE
#define OLE2ANSI
#endif

#include "stdafx.h"
#include "batch.h"
#include "interlea.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInterleaveDlg dialog


CInterleaveDlg::CInterleaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInterleaveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInterleaveDlg)
	m_InterleaveEvery = 0;
	//}}AFX_DATA_INIT
}

void CInterleaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInterleaveDlg)
	DDX_Text(pDX, IDC_EDIT1, m_InterleaveEvery);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInterleaveDlg, CDialog)
	//{{AFX_MSG_MAP(CInterleaveDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInterleaveDlg message handlers
