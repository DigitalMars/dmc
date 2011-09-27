// DaoView.cpp : Defines the class behaviors for the application.
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

#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaoViewApp

BEGIN_MESSAGE_MAP(CDaoViewApp, CWinApp)
	//{{AFX_MSG_MAP(CDaoViewApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaoViewApp construction

CDaoViewApp::CDaoViewApp()
{
	m_bShowSystemObjects = FALSE;
	m_bShowWarnings = TRUE;
	m_bOpenODBC = FALSE;
	m_nMaxRecords = 100;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDaoViewApp object

CDaoViewApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDaoViewApp initialization

BOOL CDaoViewApp::InitInstance()
{
	if(!AfxOleInit()){
		AfxMessageBox(_T("Ole Initialization failed"));
		return FALSE;
	}
 
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	LoadStdProfileSettings(6);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDaoViewDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDaoTreeView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDaoViewApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDaoViewApp commands

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CDaoWorkspace ws;
	try
	{
		ws.Create(_T("VersionWorkspace"),_T("Admin"),_T(""));
		GetDlgItem(IDC_VERSION)->SetWindowText(ws.GetVersion());
	}
	catch (CDaoException* e)
	{
		e->ReportError();
		e->Delete();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CDaoViewApp commands

void DisplayDaoException(CDaoException* e)
{
	CString strMsg;
	if (e->m_pErrorInfo!=NULL)
	{
		strMsg.Format(
			_T("%s   (%d)\n\n")
			_T("Would you like to see help?"),
			(LPCTSTR)e->m_pErrorInfo->m_strDescription,
			e->m_pErrorInfo->m_lErrorCode);

		if (AfxMessageBox(strMsg, MB_YESNO) == IDYES)
		{
			WinHelp(GetDesktopWindow(),
					e->m_pErrorInfo->m_strHelpFile,
					HELP_CONTEXT,
					e->m_pErrorInfo->m_lHelpContext);
		}
	}
	else
	{
		strMsg.Format(
			_T("ERROR:CDaoException\n\n")
			_T("SCODE_CODE		=%d\n")	
			_T("SCODE_FACILITY	=%d\n")	
			_T("SCODE_SEVERITY	=%d\n")	
			_T("ResultFromScode	=%d\n"),
			SCODE_CODE		(e->m_scode),
			SCODE_FACILITY	(e->m_scode),
			SCODE_SEVERITY	(e->m_scode),
			ResultFromScode (e->m_scode));
		AfxMessageBox(strMsg);
	}
}

