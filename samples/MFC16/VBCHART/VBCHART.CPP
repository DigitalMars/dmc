// vbchart.cpp : Defines the class behaviors for the application.
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.



#include "stdafx.h"
#include "vbchart.h"

#include "mainfrm.h"
#include "chartdoc.h"
#include "chartvw.h"
#include "gridentr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVbChartApp

BEGIN_MESSAGE_MAP(CVbChartApp, CWinApp)
	//{{AFX_MSG_MAP(CVbChartApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVbChartApp construction
// Place all significant initialization in InitInstance

CVbChartApp::CVbChartApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVbChartApp object

CVbChartApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CVbChartApp initialization

BOOL CVbChartApp::InitInstance()
{
	// Standard initialization
	//  (if you are not using these features and wish to reduce the size
	// of your final executable, you should remove the following initialization

	SetDialogBkColor();        // set dialog background color
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	EnableVBX();               // Initialize VBX support

	// Register document templates which serve as connection between
	//  documents and views.  Views are contained in the specified view

	AddDocTemplate(new CMultiDocTemplate(IDR_CHARTDOCTYPE,
			RUNTIME_CLASS(CChartDoc),
			RUNTIME_CLASS(CVbChartFrame),        // standard MDI child frame
			RUNTIME_CLASS(CGridEntry)));

	if (LoadVBXFile("grid.vbx") <= HINSTANCE_ERROR)
	{
		AfxMessageBox("Cannot load GRID.VBX.\nPlease place the file on the path.\n");
		return FALSE;
	}
	else
	{
		UnloadVBXFile("grid.vbx");
	}

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	m_pMainWnd = pMainFrame;

	// enable file manager drag/drop and DDE Execute open
	m_pMainWnd->DragAcceptFiles();
	EnableShellOpen();
	RegisterShellFileTypes();

	// simple command line parsing
	if (m_lpCmdLine[0] == '\0')
	{
		// create a new (empty) document
		OnFileNew();
	}
	else if ((m_lpCmdLine[0] == '-' || m_lpCmdLine[0] == '/') &&
		(m_lpCmdLine[1] == 'e' || m_lpCmdLine[1] == 'E'))
	{
		// program launched embedded - wait for DDE or OLE open
	}
	else
	{
		// open an existing document
		OpenDocumentFile(m_lpCmdLine);
	}


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg() : CDialog(CAboutDlg::IDD)
		{
			//{{AFX_DATA_INIT(CAboutDlg)
			//}}AFX_DATA_INIT
		}

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
		enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CVbChartApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// VB-Event registration
// (calls to AfxRegisterVBEvent will be placed here by ClassWizard)

//{{AFX_VBX_REGISTER_MAP()
	UINT NEAR VBN_CLICK = AfxRegisterVBEvent("CLICK");
	UINT NEAR VBN_ROWCOLCHANGE = AfxRegisterVBEvent("ROWCOLCHANGE");
	UINT NEAR VBN_SELCHANGE = AfxRegisterVBEvent("SELCHANGE");
	UINT NEAR VBN_GOTFOCUS = AfxRegisterVBEvent("GOTFOCUS");
//}}AFX_VBX_REGISTER_MAP

/////////////////////////////////////////////////////////////////////////////
// CVbChartApp commands
