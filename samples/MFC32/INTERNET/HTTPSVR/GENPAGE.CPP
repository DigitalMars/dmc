// GenPage.cpp : implementation file
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
#include "GenPage.h"
#include "HttpDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenPage property page

IMPLEMENT_DYNCREATE(CGenPage, CPropertyPage)

CGenPage::CGenPage() : CPropertyPage(CGenPage::IDD)
{
}

CGenPage::CGenPage( CHttpSvrDoc* pDoc )
	: CPropertyPage(CGenPage::IDD)
{
	//{{AFX_DATA_INIT(CGenPage)
	m_bListIcon = FALSE;
	m_bAllowListing = FALSE;
	m_bLoggingOn = FALSE;
	//}}AFX_DATA_INIT
	m_pDoc = pDoc;
}

CGenPage::~CGenPage()
{
}

void CGenPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenPage)
	DDX_Check(pDX, IDC_LISTICONS, m_bListIcon);
	DDX_Check(pDX, IDC_ALLOWLIST, m_bAllowListing);
	DDX_Check(pDX, IDC_LOGGINGON, m_bLoggingOn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGenPage)
	ON_BN_CLICKED(IDC_ALLOWLIST, OnAllowList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenPage message handlers

void CGenPage::OnOK() 
{
	BOOL bModified = FALSE;
	if ( m_pDoc->m_bListIcon != m_bListIcon )
	{
		m_pDoc->m_bListIcon = m_bListIcon;
		bModified = TRUE;
	}
	if ( m_pDoc->m_bAllowListing != m_bAllowListing )
	{
		m_pDoc->m_bAllowListing = m_bAllowListing;
		bModified = TRUE;
	}
	if ( m_pDoc->m_bLoggingOn != m_bLoggingOn )
	{
		m_pDoc->m_bLoggingOn = m_bLoggingOn;
		bModified = TRUE;
	}

	if ( bModified )
		m_pDoc->SetModifiedFlag( TRUE );

	CPropertyPage::OnOK();
}


void CGenPage::OnAllowList() 
{
	GetDlgItem(IDC_LISTICONS)->EnableWindow( 
		IsDlgButtonChecked(IDC_ALLOWLIST) );
}

BOOL CGenPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
#ifdef _MAC
	// Mac version doesn't do folder listings....
	m_bAllowListing = FALSE;
	m_bListIcons = FALSE;
	CheckDlgButton(IDC_ALLOWLIST, FALSE );
	GetDlgItem(IDC_ALLOWLIST)->EnableWindow( FALSE );
	CheckDlgButton(IDC_LISTICONS, FALSE );
	GetDlgItem(IDC_LISTICONS)->EnableWindow( FALSE );
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

