// dllhusk.cpp : Defines the class behaviors for the application.
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
#include "dllhusk.h"

#include "mainfrm.h"
#include "testdll1.h"   // classes exported from TESTDLL1
#include "testdll2.h"   // classes exported from TESTDLL2

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHuskApp

BEGIN_MESSAGE_MAP(CHuskApp, CWinApp)
	//{{AFX_MSG_MAP(CHuskApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_DUMP_CLASSES, OnDumpClasses)
	ON_COMMAND(ID_DUMP_DOCTEMPLATES, OnDumpDocTemplates)
	ON_COMMAND(ID_DUMP_OBJECTS, OnDumpObjects)
	ON_COMMAND(ID_DUMP_DLLS, OnDumpDLLs)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHuskApp construction
// Place all significant initialization in InitInstance

CHuskApp::CHuskApp()
{
	m_pListOut = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHuskApp object

CHuskApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CHuskApp initialization

BOOL CHuskApp::InitInstance()
{
	// Standard initialization

	SetDialogBkColor();        // set dialog background color
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register document templates


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	m_pMainWnd = pMainFrame;

	// Initialize DLLs - will add doc templates and make other class
	//   implementations available to this application
	InitTestDLL1();
	InitTestDLL2();

	CreateListOutput();

	if (m_lpCmdLine[0] == '\0')
	{
		if (m_pListOut != NULL)
			m_pListOut->AddString("Attempting to create a new document");
		// create a new (empty) document
		OnFileNew();
	}
	else
	{
		if (m_pListOut != NULL)
			m_pListOut->AddString("Attempting to open an existing document");
		// open an existing document
		OpenDocumentFile(m_lpCmdLine);
	}

#ifdef _DEBUG
	if (m_pListOut != NULL)
	{
		m_pListOut->AddString("");
		m_pListOut->AddString("Click the Right-Mouse-Button over");
		m_pListOut->AddString(" the main TitleBar for diagnostic menu");
		m_pListOut->AddString("");
	}
#endif
	return TRUE;
}

// Example of calling a class in a DLL
BOOL CHuskApp::CreateListOutput()
{
	m_pListOut = new CListOutputFrame;
			// all objects are allocated on the application's heap,
			//  even CListOutputFrame which is defined in another DLL

	ASSERT(m_pListOut->IsKindOf(RUNTIME_CLASS(CListOutputFrame)));

	CRect rectOriginal(250, 0, 600, 300);
			// hard-coded initial position

	if (!m_pListOut->Create("List Output",
			WS_OVERLAPPEDWINDOW | WS_CHILD | WS_VISIBLE,
			rectOriginal, (CMDIFrameWnd*)m_pMainWnd))
	{
		AfxMessageBox("Failed to create ListOutput Window");
		m_pListOut = NULL;  // just in case (Create will delete the C++ object)
		return FALSE;
	}

	m_pListOut->SetBackpointer(&m_pListOut);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

void CHuskApp::OnAppAbout()
{
	if (m_pListOut != NULL)
		m_pListOut->AddString("CHuskApp::OnAppAbout called");
	CDialog(IDD_ABOUTBOX).DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CHuskApp debug menu dump commands

#ifdef _DEBUG
static void DumpObjectProc(CObject* pObject, void* pContext)
{
	CListOutputFrame* pListOut = (CListOutputFrame*)pContext;
	char szT[128];
	wsprintf(szT, "    a %s at $%08lX",
		pObject->GetRuntimeClass()->m_lpszClassName, (long)(void*)pObject);
	pListOut->AddString(szT);
}

static void DumpClassProc(const CRuntimeClass* pClass, void* pContext)
{
	CListOutputFrame* pListOut = (CListOutputFrame*)pContext;
	char szT[128];
	wsprintf(szT, "    %s", pClass->m_lpszClassName);
	pListOut->AddString(szT);
}
#endif

void CHuskApp::OnDumpClasses()
{
#ifdef _DEBUG
	if (m_pListOut == NULL && !CreateListOutput())
		return; // no output window
	m_pListOut->AddString("Dump of all classes:");
	AfxDoForAllClasses(DumpClassProc, m_pListOut);
	m_pListOut->AddString("");
#endif
}

void CHuskApp::OnDumpDocTemplates()
{
#ifdef _DEBUG
	if (m_pListOut == NULL && !CreateListOutput())
		return; // no output window
	m_pListOut->AddString("Dump of all Doc Templates:");
	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		CString str;
		char szT[128];
		if (pTemplate->GetDocString(str, CDocTemplate::fileNewName))
			wsprintf(szT, "    Template for %s documents", (const char*) str);
		else
			wsprintf(szT, "    Unknown DocTemplate at $%08lX",
				(long)(void*)pTemplate);
		m_pListOut->AddString(szT);
	}
	m_pListOut->AddString("");
#endif
}

void CHuskApp::OnDumpObjects()
{
#ifdef _DEBUG
	if (m_pListOut == NULL && !CreateListOutput())
		return; // no output window
	m_pListOut->AddString("Dump of all heap Objects");
	AfxDoForAllObjects(DumpObjectProc, m_pListOut);
	m_pListOut->AddString("");
#endif
}

void CHuskApp::OnDumpDLLs()
{
#ifdef _DEBUG
	if (m_pListOut == NULL && !CreateListOutput())
		return; // no output window
	m_pListOut->AddString("Dump of DLLs in resource search order");
	CDynLinkLibrary* pDLL;
	for (pDLL = _AfxGetAppData()->pFirstDLL; pDLL != NULL;
		pDLL = pDLL->m_pNextDLL)
	{
		char szName[64];
		GetModuleFileName(pDLL->m_hModule, szName, sizeof(szName));
		int nClass = 0;
		CRuntimeClass* pClass;
		for (pClass = pDLL->m_pFirstSharedClass; pClass != NULL;
		  pClass = pClass->m_pNextClass)
			nClass++;
		char szT[256];
		wsprintf(szT, "    Module %s has %d classes", szName, nClass);
		m_pListOut->AddString(szT);
	}
	m_pListOut->AddString("");
#endif
}

/////////////////////////////////////////////////////////////////////////////
