/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1994 - 1995	Microsoft Corporation.	All Rights Reserved.
 *
 **************************************************************************/
// batch.cpp : Defines the class behaviors for the application.
//

#define INC_OLE2
#include "stdafx.h"
#include "batch.h"

#include "mainfrm.h"
#include "batchdoc.h"
#include "batchvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// For Thread Local Storage in the AVISave callback
DWORD gdwTlsCookie;

/////////////////////////////////////////////////////////////////////////////
// CBatchApp

BEGIN_MESSAGE_MAP(CBatchApp, CWinApp)
	//{{AFX_MSG_MAP(CBatchApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_OPTIONS_COMPRESSALL, OnOptionsCompressAll)
	ON_COMMAND(ID_OPTIONS_STOP, OnOptionsStop)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_STOP, OnUpdateStop)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_COMPRESSALL, OnUpdateCompressAll)
	ON_UPDATE_COMMAND_UI(ID_APP_EXIT, OnUpdateAppExit)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchApp construction

CBatchApp::CBatchApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBatchApp object

CBatchApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBatchApp initialization

BOOL CBatchApp::InitInstance()
{
	// Initialize TLS
	gdwTlsCookie = TlsAlloc();

	// Initialize the AVIFile junk
	AVIFileInit();

	Enable3dControls();

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_BATCHTYPE,
		RUNTIME_CLASS(CBatchDoc),
		RUNTIME_CLASS(CMDIChildWnd),          // standard MDI child frame
		RUNTIME_CLASS(CBatchView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

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

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CBatchApp::ExitInstance()
{
	// Free our AVIFile stuff

	AVIFileExit();
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

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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
void CBatchApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CBatchApp commands
			
//
// Compress everything.  Walk through all the documents in the application
// and if they're not already compressing, compress that list.
//
void CBatchApp::OnOptionsCompressAll() 
{
	CWinApp* pApp = AfxGetApp();
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = pApp->GetNextDocTemplate(pos);
	CBatchDoc *pDoc;

	
	pos = pTemplate->GetFirstDocPosition();
	while (pos != NULL) {
		pDoc = (CBatchDoc *)pTemplate->GetNextDoc(pos);
		if (!pDoc->fCompressing)
			pDoc->CompressList();
	}
}

//
// Enable the Compress All option if there's any document window that isn't
// already compressing.
//
void CBatchApp::OnUpdateCompressAll(CCmdUI* pCmdUI) 
{
	CWinApp* pApp = AfxGetApp();
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = pApp->GetNextDocTemplate(pos);
	CBatchDoc *pDoc;
	BOOL fCanCompress = FALSE;
	
	pos = pTemplate->GetFirstDocPosition();
	while (pos != NULL) {
		pDoc = (CBatchDoc *)pTemplate->GetNextDoc(pos);
		if (!pDoc->fCompressing)
			fCanCompress = TRUE;
	}
	pCmdUI->Enable(fCanCompress);
}

// 
// Stop any compressions in progress
//
void CBatchApp::OnOptionsStop() 
{
	CWinApp* pApp = AfxGetApp();
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = pApp->GetNextDocTemplate(pos);
	CBatchDoc *pDoc;
	
	pos = pTemplate->GetFirstDocPosition();
	while (pos != NULL) {
		pDoc = (CBatchDoc *)pTemplate->GetNextDoc(pos);
		if (pDoc->fCompressing)
			pDoc->fStopPlease = TRUE;
	}	
}

//
// Enable the Stop Compressing option if any document window is compressing.
//
void CBatchApp::OnUpdateStop(CCmdUI* pCmdUI) 
{
	CWinApp* pApp = AfxGetApp();
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = pApp->GetNextDocTemplate(pos);
	CBatchDoc *pDoc;
	BOOL fCanStop = FALSE;
	
	pos = pTemplate->GetFirstDocPosition();
	while (pos != NULL) {
		pDoc = (CBatchDoc *)pTemplate->GetNextDoc(pos);
		if (pDoc->fCompressing)
			fCanStop = TRUE;
	}
	pCmdUI->Enable(fCanStop);	
}

//
// If anybody is compressing, don't allow FileExit
//
void CBatchApp::OnUpdateAppExit(CCmdUI* pCmdUI) 
{
	CWinApp* pApp = AfxGetApp();
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = pApp->GetNextDocTemplate(pos);
	CBatchDoc *pDoc;
	
	pos = pTemplate->GetFirstDocPosition();
	while (pos != NULL) {
		pDoc = (CBatchDoc *)pTemplate->GetNextDoc(pos);
		if (pDoc->fCompressing) {
			pCmdUI->Enable(FALSE);
			return;
		}
	}	
	pCmdUI->Enable(TRUE);
}
