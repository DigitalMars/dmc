// NamePage.cpp : implementation of the CNamePage class
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
#include "NamePage.h"
#include "HttpDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNamePage property page

IMPLEMENT_DYNCREATE(CNamePage, CPropertyPage)

CNamePage::CNamePage() : CPropertyPage(CNamePage::IDD)
{
}

CNamePage::CNamePage( CHttpSvrDoc* pDoc )
	: CPropertyPage(CNamePage::IDD)
{
	//{{AFX_DATA_INIT(CNamePage)
	m_strName = _T("");
	m_nNameSetting = -1;
	m_uPort = 0;
	//}}AFX_DATA_INIT
	m_pDoc = pDoc;
}

CNamePage::~CNamePage()
{
}

void CNamePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNamePage)
	DDX_Text(pDX, IDC_SVRNAME, m_strName);
	DDX_Radio(pDX, IDC_DEFNAME, m_nNameSetting);
	DDX_Text(pDX, IDC_PORT, m_uPort);
	DDV_MinMaxUInt(pDX, m_uPort, 0, 65535);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNamePage, CPropertyPage)
	//{{AFX_MSG_MAP(CNamePage)
	ON_BN_CLICKED(IDC_DEFNAME, OnDefName)
	ON_BN_CLICKED(IDC_USENAME, OnUseName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamePage message handlers

BOOL CNamePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CWnd* pwndEdit = GetDlgItem( IDC_SVRNAME );
	pwndEdit->EnableWindow( (m_nNameSetting?TRUE:FALSE) );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNamePage::OnDefName() 
{
	// disable the edit control....
	CWnd* pwndEdit = GetDlgItem( IDC_SVRNAME );
	if ( pwndEdit )
		pwndEdit->EnableWindow( FALSE );
}

void CNamePage::OnUseName() 
{
	// enable the edit control and move to it....
	CWnd* pwndEdit = GetDlgItem( IDC_SVRNAME );
	if ( pwndEdit )
	{
		pwndEdit->EnableWindow( TRUE );
		pwndEdit->SetFocus();
	}
}

void CNamePage::OnOK() 
{
	BOOL bModified = FALSE;
	CString strNewName;
	if ( m_nNameSetting && !m_strName.IsEmpty() )
		strNewName = m_strName;
	else
	{
		strNewName = ((CHttpSvrApp*)AfxGetApp())->m_strDefSvr;
		m_nNameSetting = 0;
	}
	// see if anything has changed....
	if ( m_pDoc->m_nSvrName != m_nNameSetting )
	{
		m_pDoc->m_nSvrName = m_nNameSetting;
		bModified = TRUE;
	}
	if ( strNewName.CompareNoCase(m_pDoc->m_strServer) )
	{
		m_pDoc->m_strServer = strNewName;
		bModified = TRUE;
	}

	// if the port has changed, we need to reset the listen socket....
	if ( m_pDoc->m_uPort != m_uPort )
	{
		m_pDoc->m_uPort = m_uPort;
		m_pDoc->m_bResetListen = bModified = TRUE;
	}

	if ( bModified )
		m_pDoc->SetModifiedFlag( TRUE );
	
	CPropertyPage::OnOK();
}

