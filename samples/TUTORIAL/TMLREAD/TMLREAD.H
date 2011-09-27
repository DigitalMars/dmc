// TMLRead.h
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CTMLReadApp class declaration
//   CAboutDlg class declaration
// Notes:
//   CTMLReadApp is the application class.  A single instance of this class
//     is defined in TMLRead.cpp.
//   CAboutDlg is the About dialog box class, used by the application when the
//     'About' menu item is chosen.

#include "resource.h"

#include "tmlword.h"
#include "tmllink.h"
#include "tmlparse.h"

//// CTMLReadApp
////  Standard SDI application class

class CTMLReadApp : public CWinApp
{
  public:
    CTMLReadApp();

  // Overrides
    virtual BOOL InitInstance();

  // Implementation

  //{{AFX_MSG(CTMLReadApp)
  afx_msg void OnAppAbout();
    // NOTE - the ClassExpress will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

public:
  ~CTMLReadApp();                             // for SetCursor
  HCURSOR LoadHyperlinkCursor();              // for SetCursor
    
private:
  HCURSOR hcurHyperlink;                      // for SetCursor
};

////
//// CAboutDlg
////   Standard About dialog box
////

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
    virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
    //{{AFX_MSG(CAboutDlg)
      // No message handlers
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//////////////////// VB-Event extern declarations

//{{AFX_VBX_REGISTER()
//}}AFX_VBX_REGISTER
