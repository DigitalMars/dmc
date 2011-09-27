// mfccalc.cpp : Defines the class behaviors for the application.
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
#include "mfccalc.h"
#include "calcdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcApp

BEGIN_MESSAGE_MAP(CCalcApp, CWinApp)
	//{{AFX_MSG_MAP(CCalcApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcApp construction

CCalcApp::CCalcApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCalcApp object

CCalcApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCalcApp initialization

BOOL CCalcApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	Enable3dControls();

	// Initialize OLE 2.0 libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// Parse the command line to see if launched as OLE server
	if (RunEmbedded() || RunAutomated())
	{
	    // Register all OLE server (factories) as running.  This enables the 
	    //	OLE 2.0 libraries to create objects from other applications.
		COleTemplateServer::RegisterAll();

		// Do not continue with rest of initialization.  Wait for
		// client to create a CCalcDlg object instead.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//	to update the system registry in case it has been damaged.
	COleObjectFactory::UpdateRegistryAll();

	// create a modeless dialog as the main window of the application
	CCalcDlg* pDlg = new CCalcDlg;
	pDlg->SetVisible(TRUE);
	if (!pDlg->GetVisible())
	{
		AfxMessageBox(IDP_UNABLE_TO_SHOW_CALC);
		return FALSE;
	}
	pDlg->RegisterActive();

	// We are using a modeless dialog so we can translate accelerator keys 
	// against the dialog.  In order to run the main message pump, 
	// InitInstance must return TRUE even though this application is
	// a dialog-based application.
	return TRUE;
}
