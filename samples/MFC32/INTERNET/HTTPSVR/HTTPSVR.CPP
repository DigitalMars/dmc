// HttpSvr.cpp : Defines the class behaviors for the application.
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
#include "Http.h"
#include "MainFrm.h"
#include "HttpDoc.h"
#include "HttpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrApp

BEGIN_MESSAGE_MAP(CHttpSvrApp, CWinApp)
	//{{AFX_MSG_MAP(CHttpSvrApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrApp construction

CHttpSvrApp::CHttpSvrApp()
{
}

CHttpSvrApp::~CHttpSvrApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHttpSvrApp object

CHttpSvrApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrApp initialization

BOOL CHttpSvrApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	if ( !AfxSocketInit() )
	{
		AfxMessageBox( IDS_WINSOCK_FAILED, MB_OK | MB_ICONEXCLAMATION );
		return FALSE;
	}

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	SetRegistryKey( IDS_REGISTRY_KEY ); // use the registry
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	// try to get the default host name....
	int nOk =  gethostname( m_strDefSvr.GetBuffer(255), 255 );
	m_strDefSvr.ReleaseBuffer();
	if ( nOk != 0 )
	{
		// get the computer server name....
		DWORD dwLen = MAX_COMPUTERNAME_LENGTH+1;
		GetComputerName( m_strDefSvr.GetBuffer( dwLen ), &dwLen );
		m_strDefSvr.ReleaseBuffer();
		// all lower-case....
		m_strDefSvr.MakeLower();
	}

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CHttpSvrDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CHttpSvrView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);
	RegisterLogFile();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Enable drag/drop open.
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
void CHttpSvrApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrApp commands

BOOL CHttpSvrApp::OnIdle(LONG lCount) 
{
	BOOL bMore = CWinApp::OnIdle(lCount);
	// if we still need more processing time, ask for it...
	CHttpSvrDoc* pDoc = (CHttpSvrDoc*)(((CFrameWnd*)m_pMainWnd)->GetActiveDocument());
	if ( pDoc->IdleProc( lCount ) )
		bMore = TRUE;
	return bMore;
}

void AddFile( CString& strPath, UINT uStr )
{
	CString strFile;
	strFile.LoadString( uStr );
	AddFile( strPath, strFile );
}

void AddFile( CString& strPath, const CString& strFile )
{
	if ( strPath.GetAt( strPath.GetLength()-1 ) != SEPCHAR )
		strPath += SEPCHAR;
	strPath += strFile;
}

// declare this....
void AFXAPI AfxGetModuleShortFileName(HINSTANCE hInst, CString& strShortName);

void CHttpSvrApp::RegisterLogFile( void )
{
#ifndef _MAC
	// get the full path (short version)....
	CString strPath;
	AfxGetModuleShortFileName( AfxGetInstanceHandle(), strPath );
	// set the entries....
	CString strType = "HttpSvr.Logfile";
	::RegSetValue( HKEY_CLASSES_ROOT, ".hsl", REG_SZ, strType, 0 );
	::RegSetValue( HKEY_CLASSES_ROOT, strType, REG_SZ, "HttpSvr Daily Log File", 0 );
	::RegSetValue( HKEY_CLASSES_ROOT, strType+"\\DefaultIcon", REG_SZ, strPath+",2", 0 );
	::RegSetValue( HKEY_CLASSES_ROOT, strType+"\\shell\\open\\command", REG_SZ, "notepad.exe \"%1\"", 0 );
#endif // !_MAC
}
