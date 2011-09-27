// mainfrm.cpp : implementation of the CMainFrame class
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CMainFrame message map and member functions
// Notes:
//   Our only addition to AppExpress's CMainFrame is the
//     use of rectWindRect to set up, store, and save window
//     size and position.  We have also modified the toolbar
//     button array to suit our custom toolbar.

#include "stdafx.h"
#include "dibapi.h"

#include "TMLRead.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

////
//// Message Map
////

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
  //{{AFX_MSG_MAP(CMainFrame)
  ON_WM_CREATE()
  ON_WM_MOVE()
  ON_WM_SIZE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

////
//// array used to initialize toolbar
////
static UINT BASED_CODE buttons[] =
{
  // same order as in the bitmap 'toolbar.bmp'
  ID_FILE_OPEN,
  IDC_PREVFILE,
  ID_FILE_PRINT,
    ID_SEPARATOR,
  ID_APP_ABOUT,
    ID_SEPARATOR,
  ID_APP_EXIT
};

////
//// array used to initialize status bar
////
static UINT BASED_CODE indicators[] =
{
  ID_SEPARATOR,     // status line indicator
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
};

////
//// Constructor
////
CMainFrame::CMainFrame()
{
  TRACE ("CMainFrame Constructor\n");
}

////
//// Destructor
////   Writes window position and size to profile
////
CMainFrame::~CMainFrame()
{
  TRACE ("CMainFrame Destructor\n");

  AfxGetApp()->WriteProfileInt("PositionAndSize", "LastX", rectWindRect.left);
  AfxGetApp()->WriteProfileInt("PositionAndSize", "LastY", rectWindRect.top);
  AfxGetApp()->WriteProfileInt("PositionAndSize", "LastCX", rectWindRect.Width());
  AfxGetApp()->WriteProfileInt("PositionAndSize", "LastCY", rectWindRect.Height());
}

////
//// PreCreateWindow
////  Sets up window position and size from profile
////
BOOL
CMainFrame::PreCreateWindow(CREATESTRUCT &cs)
{
  cs.x =  AfxGetApp()->GetProfileInt("PositionAndSize", "LastX", 10);
  cs.y =  AfxGetApp()->GetProfileInt("PositionAndSize", "LastY", 10);
  cs.cx = AfxGetApp()->GetProfileInt("PositionAndSize", "LastCX", 300);
  cs.cy = AfxGetApp()->GetProfileInt("PositionAndSize", "LastCY", 400);

  return CFrameWnd::PreCreateWindow (cs);
}

////
//// OnCreate
////  Sets up toolbar and status bar
////
int
CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  TRACE ("CMainFrame::OnCreate()\n");

  // call base class to create window
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  // create and attach toolbar
#if (_MFC_VER >= 0x0300)
  if (!m_wndToolBar.Create(this,
        WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY) ||
#else
  if (!m_wndToolBar.Create(this) ||
#endif
        !m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
        !m_wndToolBar.SetButtons(buttons,
                                 sizeof(buttons)/sizeof(UINT))) {
    TRACE("CMainFrame::OnCreate() failed to create toolbar\n");
    return -1;    // fail to create
  }

  // create and attach status bar
  if (!m_wndStatusBar.Create(this) ||
      !m_wndStatusBar.SetIndicators(indicators,
                                    sizeof(indicators)/sizeof(UINT))) {
    TRACE("CMainFrame::OnCreate() failed to create status bar\n");
    return -1;    // fail to create
  }

#if (_MFC_VER >= 0x0300)
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);
#endif

  return 0;
}

////
//// CMainFrame diagnostics
////
#ifdef _DEBUG
void
CMainFrame::AssertValid() const
{
  CFrameWnd::AssertValid();
}

void
CMainFrame::Dump(CDumpContext& dc) const
{
  CFrameWnd::Dump(dc);
}
#endif //_DEBUG

////
//// OnMove
////  Saves position and size
////
void
CMainFrame::OnMove(int x, int y)
{
  TRACE ("CMainFrame::OnMove()\n");

  CFrameWnd::OnMove(x, y); // call base class handler

  GetWindowRect (&rectWindRect);
}

////
//// OnMove
////  Saves position and size
////
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
  TRACE ("CMainFrame::OnSize()\n");

  CFrameWnd::OnSize(nType, cx, cy); // call base class handler

  GetWindowRect (&rectWindRect);
}
