// TableSettings.cpp : implementation file
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
#include "catalog2.h"
#include "Settings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableSettings property page

IMPLEMENT_DYNCREATE(CTableSettings, CPropertyPage)

CTableSettings::CTableSettings() : CPropertyPage(CTableSettings::IDD)
{
	//{{AFX_DATA_INIT(CTableSettings)
	m_bSynonyms = FALSE;
	m_bSystemTables = FALSE;
	m_bViews = FALSE;
	//}}AFX_DATA_INIT
}

CTableSettings::~CTableSettings()
{
}

void CTableSettings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTableSettings)
	DDX_Check(pDX, IDC_ALIAI, m_bSynonyms);
	DDX_Check(pDX, IDC_SYSTABLES, m_bSystemTables);
	DDX_Check(pDX, IDC_VIEWS, m_bViews);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTableSettings, CPropertyPage)
	//{{AFX_MSG_MAP(CTableSettings)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableSettings message handlers
