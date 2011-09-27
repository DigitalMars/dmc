// InstrDlg.cpp : implementation file
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
#include "InstrDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInstructorPage property page

// IMPLEMENT_DYNCREATE(CInstructorPage, CPropertyPage)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CInstructorPage::CInstructorPage() : CPropertyPage(CInstructorPage::IDD)
{
	//{{AFX_DATA_INIT(CInstructorPage)
	//}}AFX_DATA_INIT
}

CInstructorPage::~CInstructorPage()
{
}

void CInstructorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInstructorPage)
	//}}AFX_DATA_MAP
}

BOOL CInstructorPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SetModified(TRUE);  // allow the APPLY button to become active

	return TRUE;
}


BEGIN_MESSAGE_MAP(CInstructorPage, CPropertyPage)
	//{{AFX_MSG_MAP(CInstructorPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstructorPage message handlers

