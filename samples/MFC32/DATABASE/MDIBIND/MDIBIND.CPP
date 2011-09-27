// MDIBind.cpp : Defines the class behaviors for the application.
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
#include "MDIBind.h"

#include "MainFrm.h"
#include "RDCFrm.h"
#include "GridFrm.h"
#include "MaskFrm.h"
#include "MDIDoc.h"
#include "gridView.h"
#include "MaskView.h"
#include "RDCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIBindApp

BEGIN_MESSAGE_MAP(CMDIBindApp, CWinApp)
	//{{AFX_MSG_MAP(CMDIBindApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// The Doc Template for all controls (sharing the menu nIDMenu)
class CControlsDocTemplate : public CMultiDocTemplate
{
public:
	CControlsDocTemplate(HMENU nIDMenu,UINT nIDResource,
		CRuntimeClass* pDocClass, 
		CRuntimeClass* pFrameClass,
		CRuntimeClass* pViewClass) : 
		CMultiDocTemplate(nIDResource, pDocClass,
				pFrameClass, pViewClass)
			{
				m_hMenuShared = nIDMenu;
			};
protected:
	virtual ~CControlsDocTemplate(){
		m_hMenuShared = NULL;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CMDIBindApp construction
CMDIBindApp::CMDIBindApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMDIBindApp object

CMDIBindApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMDIBindApp initialization

BOOL CMDIBindApp::InitInstance()
{
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	// serve as the connection between documents, frame windows and views.
	// Load the menu shared between all templates
	m_hMenuShared = ::LoadMenu(m_hInstance,
		MAKEINTRESOURCE(IDR_CONTROLMENU));
 

	CControlsDocTemplate* pRDCTemplate;
	// we need to hold RDC template to force RDC creation at startup
	pRDCTemplate = new CControlsDocTemplate(
		m_hMenuShared,IDR_RDCTYPE,
		RUNTIME_CLASS(CMDIBindDoc),
		RUNTIME_CLASS(CRDCFrame), // RDC child frame
		RUNTIME_CLASS(CRDCView));
	AddDocTemplate(pRDCTemplate);

	AddDocTemplate(
		new CControlsDocTemplate(
		m_hMenuShared,IDR_GRIDTYPE,
		RUNTIME_CLASS(CMDIBindDoc),
		RUNTIME_CLASS(CGridFrame), // Grid child frame
		RUNTIME_CLASS(CGridView))
	);

	AddDocTemplate(
		new CControlsDocTemplate(
		m_hMenuShared,IDR_MASKTYPE,
		RUNTIME_CLASS(CMDIBindDoc),
		RUNTIME_CLASS(CMaskFrame), // Mask child frame
		RUNTIME_CLASS(CMaskView))
	);

// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

    
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{	// Don't display a new MDI child window dialog
		// but create a new RDC window during startup
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
		pRDCTemplate->OpenDocumentFile(NULL);
	}
	else	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMDIBindApp commands
int CMDIBindApp::ExitInstance() 
{
	::DestroyMenu(m_hMenuShared);	
	return CWinApp::ExitInstance();
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

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMDIBindApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

