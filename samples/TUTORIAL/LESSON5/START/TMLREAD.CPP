// TMLRead.cpp
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CTMLReadApp message map and member functions
//   CAboutDlg message map and member functions
// Notes:
//   CTMLReadApp is the application class.  A single instance of this class
//     is defined below
//   CAboutDlg is the About dialog box class, used by the application when the
//     'About' menu item is chosen.

#include "stdafx.h"
#include "dibapi.h"

#include "TMLRead.h"

#include "tmlword.h"
#include "tmllink.h"
#include "tmlimage.h"
#include "tmlparse.h"
#include "tmlvobjs.h"

#include "mainfrm.h"
#include "ptrstack.h"
#include "tmlrddoc.h"
#include "tmlrdvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

////
////  Message Map
////    The unimplemented messages have been removed
////

BEGIN_MESSAGE_MAP(CTMLReadApp, CWinApp)
  //{{AFX_MSG_MAP(CTMLReadApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    // NOTE - the ClassExpress will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG_MAP
  // Standard file based document commands
  ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

////
//// Constructor
////
CTMLReadApp::CTMLReadApp()
{
  TRACE ("CTMLReadApp Constructor\n");
}

//// 
//// CESIViewerApp destruction
//// for SetCursor

CTMLReadApp::~CTMLReadApp()
{
    if ( hcurHyperlink )
        DestroyCursor( hcurHyperlink );
}

//// 
//// for SetCursor
//// 
HCURSOR CTMLReadApp::LoadHyperlinkCursor()
{
    if ( ! hcurHyperlink )
        hcurHyperlink = LoadCursor( IDC_HYPERLNK );

    return hcurHyperlink;
}

////
//// The one and only CTMLReadApp object
////
CTMLReadApp NEAR theApp;

////
//// InitInstance
////   Perform some standard initialization
////
BOOL
CTMLReadApp::InitInstance()
{
  TRACE ("CTMLReadApp::InitInstance()\n");

#if (_MFC_VER >= 0x0300)
  Enable3dControls();
#else
  SetDialogBkColor();        // Set dialog background color to gray
#endif

  LoadStdProfileSettings();  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views.
  AddDocTemplate(new CSingleDocTemplate(IDR_MAINFRAME,
      RUNTIME_CLASS(CTMLReadDoc),
      RUNTIME_CLASS(CMainFrame),     // main SDI frame window
      RUNTIME_CLASS(CTMLReadView)));

  LoadHyperlinkCursor();             // for SetCursor

  // simple command line parsing
  if (m_lpCmdLine[0] == '\0')
    OnFileNew();                     // create a new (empty) document
  else
    OpenDocumentFile(m_lpCmdLine);   // open an existing document

  return TRUE;
}

////
//// OnAppAbout
////  Runs the About dialog
////
void
CTMLReadApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

//////////// About dialog box class methods

////
//// Constructor
////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  TRACE ("CAboutDlg::Constructor\n");

  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

////
//// DoDataExchange
////
void
CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  TRACE ("CAboutDlg::DoDataExchange()\n");

  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
}

////
//// Message Map
////

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////
// VB-Event registration
// (calls to AfxRegisterVBEvent will be placed here by ClassExpress)

//{{AFX_VBX_REGISTER_MAP()
//}}AFX_VBX_REGISTER_MAP

/////////////////////////////////////////////////////////////////////////////
// CTMLReadApp commands
