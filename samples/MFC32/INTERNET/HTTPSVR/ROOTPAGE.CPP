// RootPage.cpp : implementation of the CRootPage class
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

#include "HttpSvr.h"
#include "RootPage.h"
#include "HttpDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRootPage property page

IMPLEMENT_DYNCREATE(CRootPage, CPropertyPage)

CRootPage::CRootPage() : CPropertyPage(CRootPage::IDD)
{
}

CRootPage::CRootPage( CHttpSvrDoc* pDoc ) 
	: CPropertyPage(CRootPage::IDD)
{
	//{{AFX_DATA_INIT(CRootPage)
	m_strRoot = _T("");
	//}}AFX_DATA_INIT
	m_pDoc = pDoc;
}

CRootPage::~CRootPage()
{
}

void CRootPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRootPage)
	DDX_Text(pDX, IDC_ROOTDIR, m_strRoot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRootPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRootPage)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_EN_CHANGE(IDC_ROOTDIR, OnChangeRootDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRootPage message handlers

void CRootPage::OnReset() 
{
	SetDlgItemText( IDC_ROOTDIR, m_pDoc->m_strRoot );
	SetModified( FALSE );
}

void CRootPage::OnChangeRootDir() 
{
	SetModified( TRUE );
}

BOOL CRootPage::OnKillActive() 
{
	BOOL bOk = CPropertyPage::OnKillActive();
	if ( bOk )
	{
		// make sure it doesn't end in a separator char....
		if ( m_strRoot[ m_strRoot.GetLength()-1 ] == SEPCHAR )
			m_strRoot = m_strRoot.Left( m_strRoot.GetLength() - 1 );
		// resolve to complete file path....
		CString strFull;
		GetFullPathName( m_strRoot, MAX_PATH, strFull.GetBuffer(MAX_PATH+1), NULL );
		strFull.ReleaseBuffer();
		// set the control to this complete, fixed path....
		SetDlgItemText( IDC_ROOTDIR, strFull );
		m_strRoot = strFull;
		// make sure the directory is valid....
		DWORD dwAttr = GetFileAttributes( strFull );
		if ( dwAttr == -1 || 
			(dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0 )
		{
			CString strText;
			strText.LoadString( IDS_BAD_ROOT );
			MessageBox( strText, NULL, MB_OK|MB_ICONSTOP );
			bOk = FALSE;
		}
	}
	return bOk;
}

void CRootPage::OnOK() 
{
	// make sure it doesn't end in a sepchar....
	if ( m_strRoot[ m_strRoot.GetLength()-1 ] == SEPCHAR )
		m_strRoot = m_strRoot.Left( m_strRoot.GetLength()-1 );
	// copy it over....
	if ( m_pDoc->m_strRoot.CompareNoCase( m_strRoot ) )
	{
		m_pDoc->m_strRoot = m_strRoot;
		m_pDoc->SetModifiedFlag( TRUE );
	}
	CPropertyPage::OnOK();
}


