// OptionsD.cpp : implementation file
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
#include "DaoView.h"
#include "OptionsD.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


COptionsDlg::COptionsDlg(UINT nIDCaption /*=0*/)
	: CPropertyPage(COptionsDlg::IDD, nIDCaption)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	m_bShowSystemObjects = FALSE;
	m_bShowWarnings = FALSE;
	m_bOpenODBC = FALSE;
	m_nMaxRecords = 0;
	//}}AFX_DATA_INIT
}


void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Check(pDX, IDC_BSYSTEMOBJECTS, m_bShowSystemObjects);
	DDX_Check(pDX, IDC_BWARNINGS, m_bShowWarnings);
	DDX_Check(pDX, IDC_BOPENODBC, m_bOpenODBC);
	DDX_Text(pDX, IDC_MAXRECS, m_nMaxRecords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlg, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

