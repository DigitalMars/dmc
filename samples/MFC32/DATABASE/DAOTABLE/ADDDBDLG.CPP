// AddDbDlg.cpp : implementation file
// Contains functions to support the user interface for setting
// the properties of the database to be created.
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
#include "DAOTable.h"
#include "database.h"
#include "AddDbDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddDatabaseDlg dialog

// default constructor
CAddDatabaseDlg::CAddDatabaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddDatabaseDlg::IDD, pParent)
{
	// call centralized intialization
	initializer();
}

// constructor that will generally be called
// for constructing this dialog since the arguments are needed
// to do any useful database operations
//
// IN/OUT: ppDatabase--ptr to ptr to the datbase that is created via this
//         dialog
// IN: strDatabase--the name of the database file (full path or relative)
// IN: pParent--ptr to parent of this dialog
//
CAddDatabaseDlg::CAddDatabaseDlg(CDaoDatabase **ppDatabase,
								 CString strDatabaseName,
								 CWnd *pParent /*=NULL*/)
	: CDialog(CAddDatabaseDlg::IDD, pParent)
{
	// call centralized initialization
	initializer();

	// transfer incoming parameters to member variables
	m_strDatabaseName = strDatabaseName;
	m_ppDatabase = ppDatabase;
}

// initialize members that are not set to incoming arguments
void CAddDatabaseDlg::initializer()
{
	//{{AFX_DATA_INIT(CAddDatabaseDlg)
	m_strDatabaseName = _T("");
	m_bEncrypt = FALSE;
	m_nVersion = 3;
	//}}AFX_DATA_INIT
}

void CAddDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddDatabaseDlg)
	DDX_Text(pDX, IDC_EDIT_DATABASE_NAME, m_strDatabaseName);
	DDX_Check(pDX, IDC_CHECK_ENCRYPT, m_bEncrypt);
	DDX_Radio(pDX, IDC_RADIO_V10, m_nVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddDatabaseDlg, CDialog)
	//{{AFX_MSG_MAP(CAddDatabaseDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddDatabaseDlg message handlers

// user has selected the "Done" button--this means it's time to create
// the database file
void CAddDatabaseDlg::OnOK() 
{
	// transfer information from dialog controls to member variables
	UpdateData(TRUE);

	// initialize the database open options
	int dwOptions = 0;

	// user can specify encrypted or non-encrypted format
	if (m_bEncrypt)
		dwOptions |= dbEncrypt;
	else
		dwOptions |= dbDecrypt;

	// user can specify what version of access format to use
	// in creating .mdb file.  Map from radio button group
	// index to actual value to pass to database create call
	switch(m_nVersion)
	{
		case 0: dwOptions |= dbVersion10;
			   break;
		case 1: dwOptions |= dbVersion11;
			   break;
		case 2: dwOptions |= dbVersion20;
			   break;
		case 3: dwOptions |= dbVersion30;
			   break;
	}

	// try to create the database -- if failure, reinitialize pointer
	if (!createDatabase(m_ppDatabase, m_strDatabaseName, dwOptions))
		(*m_ppDatabase) = NULL;
	
	// call the base class
	CDialog::OnOK();
}
