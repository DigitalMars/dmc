// chkbook.cpp : Defines the class behaviors for the application.
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
#include "chkbook.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChkBookApp

BEGIN_MESSAGE_MAP(CChkBookApp, CWinApp)
	//{{AFX_MSG_MAP(CChkBookApp)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChkBookApp construction
// Place all significant initialization in InitInstance

CChkBookApp::CChkBookApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChkBookApp object

CChkBookApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CChkBookApp initialization

BOOL CChkBookApp::InitInstance()
{
	// Standard initialization
	SetDialogBkColor();        // set dialog background color to gray

	// We create two doc template objects to orchestrate the creation of two
	// distinct MDI children-hosted views of the document: (1) the check
	// check view and (2) the book view.
	//
	// We register both doc templates with the CWinApp object, by calling
	// AddDocTemplate.  However, if we were to do this and nothing else, then
	// the framework would mistakenly believe that the application supports
	// two document types.  The framework would display a File New dialog
	// that lists two document types, both which would be "Check Book".
	// We avoid this problem by removing the third string from
	// the document template for the check frame/view.  Specifically,
	// the strings for documents IDR_BOOKFRAME and IDR_CHECKFRAME are,
	// respectively:
	//
	//       "Book\nchecks\nCheck Book\n
	//        Check Book File (*.chb)\n.chb\n
	//        ChkBookFileType\nCheck Book File Type"
	// and
	//       "Check\nchecks\n\n
	//        Check Book File (*.chb)\n.chb\n
	//        ChkBookFileType\nCheck Book File Type"
	//
	// Finding no GetDocString(CDocTemplate::fileNewName) for the
	// second document template, CWinApp concludes that there is only
	// one document type supported by the application (the "Check Book"
	// document type specified in the first document template; and
	// therefore does not represent the user with a File New dialog.

	m_pBookViewTemplate = new CMultiDocTemplate(IDR_BOOKFRAME,
			RUNTIME_CLASS(CChkBookDoc),
			RUNTIME_CLASS(CCheckBookFrame),
			RUNTIME_CLASS(CBookView));
	AddDocTemplate(m_pBookViewTemplate);

	m_pCheckViewTemplate = new CMultiDocTemplate(IDR_CHECKFRAME,
			RUNTIME_CLASS(CChkBookDoc),
			RUNTIME_CLASS(CCheckBookFrame),
			RUNTIME_CLASS(CCheckView));
	AddDocTemplate(m_pCheckViewTemplate);

	// Create the main MDI frame window.

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	// The following reflects an optional user interface design
	// decision to automatically maximize the main application window
	// upon start-up.
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);

	pMainFrame->UpdateWindow();
	m_pMainWnd = pMainFrame;

	if (m_lpCmdLine[0] == '\0')
	{
		// Open file name saved in private INI file.
		CString strDocPath = GetDocPathFromIniFile();
		if (!strDocPath.IsEmpty())
			OpenDocumentFile(strDocPath);
	}
	else
	{
		OpenDocumentFile(m_lpCmdLine);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Overrides

void CChkBookApp::OnFileNew()
{
	// By default, the framework does not create an actual file for
	// a new document until the user does a File Save As.  But ChkBook
	// requires a file as soon as the user does a File New, because
	// ChkBook updates the file on a per transaction basis.  Upon File
	// New, we prompt the user with a File Open dialog, in which the
	// user specifies a new file (or if she changes her mind, she
	// can specify an existing check book file).  We call the same
	// CWinApp::DoPromptFileName that CWinApp::OnFileOpen calls.
	// But we replace the OFN_FILEMUSTEXIST flag with
	// OFN_CREATEPROMPT.

	CString strNewFileName;

	if (!(DoPromptFileName(strNewFileName, IDS_NEW_CHECKBOOK,
			OFN_HIDEREADONLY | OFN_CREATEPROMPT, TRUE, NULL)))
		return;

	// If file doesn't already exist, then create it.
	CFile file;
	CFileStatus status;
	if (!file.GetStatus(strNewFileName, status))
	{
		if (!file.Open(strNewFileName, CFile::modeCreate))
		{
			CString strMessage;
			AfxFormatString1(strMessage, IDS_CANNOT_CREATE_CHECKBOOK,
				strNewFileName);
			AfxMessageBox(strMessage);
			return;
		}
		file.Close();
	}

	// Open the file now that it has been created.
	OpenDocumentFile(strNewFileName);
}

CDocument* CChkBookApp::OpenDocumentFile(LPCSTR lpszFileName)
{
	// CWinApp::OpenDocmentFile creates the first MDI child window
	// for the book view.  This override creates the second MDI child
	// window for the check view.  Then it tiles the two MDI children
	// windows.

	CChkBookDoc* pDoc = (CChkBookDoc*)CWinApp::OpenDocumentFile(lpszFileName);
	if (pDoc == NULL)
	{
		CString strMessage;
		AfxFormatString1(strMessage, IDS_CANNOT_OPEN_CHECKBOOK,
			lpszFileName);
		AfxMessageBox(strMessage);
		return NULL;
	}

	CFrameWnd* pNewFrame = m_pCheckViewTemplate->CreateNewFrame(pDoc, NULL);
	if (pNewFrame == NULL)
		return pDoc;
	m_pCheckViewTemplate->InitialUpdateFrame(pNewFrame, pDoc);

	// Tile the two MDI children windows within the MDI frame window.

	ASSERT(pNewFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));
	CMDIFrameWnd* pMDIFrameWnd = ((CMDIChildWnd*)pNewFrame)->GetMDIFrame();
	ASSERT(pMDIFrameWnd != NULL);
	pMDIFrameWnd->MDITile(MDITILE_HORIZONTAL);

	return pDoc;
}

/////////////////////////////////////////////////////////////////////////////
// INI file implementation

static char BASED_CODE szIniFileSection[] = "Most Recent Check Book File";
static char BASED_CODE szIniFileEntry[] = "File";

void CChkBookApp::UpdateIniFileWithDocPath(const char* pszPathName)
{
	WriteProfileString(szIniFileSection, szIniFileEntry, pszPathName);
}

CString CChkBookApp::GetDocPathFromIniFile()
{
	return GetProfileString(szIniFileSection, szIniFileEntry, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CChkBookApp commands

void CChkBookApp::OnAppAbout()
{
	CDialog dlg(IDD_ABOUTBOX);
	dlg.DoModal();
}
