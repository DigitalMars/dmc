//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: dbghndlr.cpp
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "stdafx.h"


BEGIN_MESSAGE_MAP(CHandlerWindow, CFrameWnd)
  ON_WM_CREATE()
  ON_WM_INITMENU()
  ON_WM_SIZE()
  ON_WM_CLOSE()
  ON_COMMAND(IDM_ABOUT, OnAbout)
  ON_COMMAND(IDM_OPTIONS_CLEAR, OnClear)
  ON_COMMAND(IDM_OPTIONS_BREAK, OnBreak)
END_MESSAGE_MAP()

COleHandler theHandler;


BOOL
  COleHandler::InitInstance()
//----------------------------------------------------------------------------
// InitInstance:
//----------------------------------------------------------------------------
{
#if _MFC_VER == 0x0200
  // Standard initialization
  SetDialogBkColor();        // set dialog background color to gray
#endif

  return TRUE;

} /* InitInstance()
   */


int
  COleHandler::ExitInstance()
//----------------------------------------------------------------------------
// ExitInstance()
//----------------------------------------------------------------------------
{
  return CWinApp::ExitInstance();

} /* ExitInstance()
   */


CHandlerWindow::
  CHandlerWindow(LPCSTR pcszLogFileName)
//----------------------------------------------------------------------------
// Constructor:
//----------------------------------------------------------------------------
{
  const char* pszMainClass =
	AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
						theHandler.LoadStandardCursor(IDC_ARROW),
						(HBRUSH)::GetStockObject(LTGRAY_BRUSH),
						LoadIcon(AfxGetInstanceHandle(), "DbgHndlr"));

  CreateEx(NULL, pszMainClass, "Debug Handler - ",
		   WS_OVERLAPPEDWINDOW, rectDefault.left, rectDefault.top,
		   rectDefault.Width(), rectDefault.Height(), NULL,
		   LoadMenu(AfxGetInstanceHandle(), "MainMenu"));

  CMenu* pMenu = GetSystemMenu(FALSE);
  pMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);

  SetWindowText(pcszLogFileName);

} /* CHandlerWindow::CHandlerWindow()
   */


CHandlerWindow::~CHandlerWindow()
//----------------------------------------------------------------------------
// Destructor:
//----------------------------------------------------------------------------
{
} /* CHandlerWindow::~CHandlerWindow()
   */

int
  CHandlerWindow::OnCreate(LPCREATESTRUCT /* lpCreateStruct */)
//----------------------------------------------------------------------------
// OnCreate:
//----------------------------------------------------------------------------
{
  CRect rc;
  CFont font;
  GetClientRect(rc);

#if _MFC_VER == 0x0200
  m_bAutoMenuEnable = FALSE;
#endif

  m_ListBox.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
				   LBS_NOINTEGRALHEIGHT | LBS_DISABLENOSCROLL,
				   rc, this, -1);

  m_ListBox.SetFont(font.FromHandle((HFONT)GetStockObject(ANSI_FIXED_FONT)));
  m_ListBox.SendMessage(LB_SETHORIZONTALEXTENT, (WPARAM)0x800);
  return 0;

} /* OnCreate()
   */


void
  CHandlerWindow::OnInitMenu(CMenu* pMenu)
//----------------------------------------------------------------------------
// OnInitMenu:
//----------------------------------------------------------------------------
{
  /* set initial state of menu items */
  pMenu->EnableMenuItem(IDM_ABOUT, MF_ENABLED);

} /* OnInitMenu()
   */


void
  CHandlerWindow::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
//----------------------------------------------------------------------------
// OnSize:
//----------------------------------------------------------------------------
{
  CRect rc;
  GetClientRect(rc);

  m_ListBox.MoveWindow(rc, TRUE);

} /* OnSize()
   */


VOID
  CHandlerWindow::OnClose()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  DestroyWindow();

} /* OnClose()
   */


void
  CHandlerWindow::OnAbout()
//----------------------------------------------------------------------------
// OnPaint:
//----------------------------------------------------------------------------
{
  CModalDialog about("AboutBox", this);
  about.DoModal();
} /* OnAbout()
   */


void
  CHandlerWindow::OnClear(VOID)
//----------------------------------------------------------------------------
// OnClear:
//----------------------------------------------------------------------------
{
  m_ListBox.ResetContent();

} /* OnClear()
   */


void
  CHandlerWindow::OnBreak(VOID)
//----------------------------------------------------------------------------
// OnBreak;
//----------------------------------------------------------------------------
{
  CDebug db;
  db << "";
  db.DebugOutput(&m_ListBox);
  db.Reset();

  db << "++++++++++++++++++++++++++++++++" ;
  db.DebugOutput(&m_ListBox);
  db.Reset();

  db << "";
  db.DebugOutput(&m_ListBox);

} /* OnBreak()
   */
