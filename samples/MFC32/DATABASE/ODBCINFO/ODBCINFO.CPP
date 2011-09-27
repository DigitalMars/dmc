// OdbcInfo.cpp : Defines the class behaviors for the application.
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
#include "OdbcInfo.h"
#include "MySheet.h"
#include "catsets.h"
#include "DrvInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COdbcInfoApp

BEGIN_MESSAGE_MAP(COdbcInfoApp, CWinApp)
	//{{AFX_MSG_MAP(COdbcInfoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdbcInfoApp construction

COdbcInfoApp::COdbcInfoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COdbcInfoApp object

COdbcInfoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COdbcInfoApp initialization

BOOL COdbcInfoApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// create our property sheet
	CMyPropertySheet	sheet(_T("ODBC Info"));

	// create all our property pages
	CDriverInfo			DriverInfoPage(&m_Database);
	CFunctions			FunctionsPage(&m_Database);
	CSupportedSQL		SupportedSQLPage(&m_Database);
	CDataTypes			DataTypesPage(&m_Database);
	CIdentifiers		IdentifiersPage(&m_Database);
	CLimits				LimitsPage(&m_Database);
	CMisc1				Misc1Page(&m_Database);
	CMisc2				Misc2Page(&m_Database);

	// add the pages to our sheet
	sheet.AddPage(&DriverInfoPage);
	sheet.AddPage(&FunctionsPage);
	sheet.AddPage(&SupportedSQLPage);
	sheet.AddPage(&DataTypesPage);
	sheet.AddPage(&IdentifiersPage);
	sheet.AddPage(&LimitsPage);
	sheet.AddPage(&Misc1Page);
	sheet.AddPage(&Misc2Page);

	// the property sheet is our main window
	m_pMainWnd = &sheet;

	// For Win95: display icon in the caption bar
	// and in the taskbar button
	sheet.m_psh.dwFlags |= PSH_USEHICON;
	sheet.m_psh.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// run the app
	sheet.DoModal();

	return FALSE;
}

int COdbcInfoApp::ExitInstance() 
{
	// if a database is open, close it
	if (m_Database.IsOpen())
		m_Database.Close();
	
	return CWinApp::ExitInstance();
}
