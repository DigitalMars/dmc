/*** 
*winmain.cpp
*
*  This is a part of the Microsoft Source Code Samples.
*
*  Copyright (C) 1992-1995 Microsoft Corporation. All rights reserved.
*
*  This source code is only intended as a supplement to Microsoft Development
*  Tools and/or WinHelp documentation.  See these sources for detailed
*  information regarding the Microsoft samples programs.
*
*Purpose:
*  This module is the main entry point for the sample IDispatch polygon
*  server, spoly.exe.
*
*  This program is intended to demonstrate an implementation of the IDispatch
*  interface. Spoly is a very simple app, that implements two simple objects,
*  CPoly and CPoint and exposes their properties and methods for programatic
*  and cross-process access via IDispatch.
*
*Implementation Notes:
*
*****************************************************************************/

#include <stdio.h>
#include <string.h>

#include "spoly.h"
#include "cpoint.h"
#include "cpoly.h"


HINSTANCE g_hinst = 0;

HWND g_hwndFrame = 0;
HWND g_hwndClient = 0;

TCHAR g_szFrameWndClass[] = TSTR("FrameWClass");

CStatBar FAR* g_psb = NULL;


BOOL InitApplication(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

extern "C" int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
extern "C" long FAR PASCAL FrameWndProc(HWND, UINT, WPARAM, LPARAM);


extern "C" int PASCAL
WinMain(
    HINSTANCE hinst,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    MSG msg;
    int retval;
    HRESULT hresult;

    if(!hPrevInstance)
      if(!InitApplication(hinst))
	return FALSE;

    if((hresult = InitOle()) != NOERROR)
      return FALSE;

    if(!InitInstance(hinst, nCmdShow)){
      retval = FALSE;
      goto LExit;
    }

    while(GetMessage(&msg, NULL, NULL, NULL)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    g_psb->Release();
    CPoly::PolyTerm();

    retval = msg.wParam;

LExit:;
    UninitOle();

    return retval;
}


BOOL
InitApplication(HINSTANCE hinst)
{
    WNDCLASS  wc;

    wc.style		= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc	= FrameWndProc;
    wc.cbClsExtra	= 0;
    wc.cbWndExtra	= 0;
    wc.hInstance	= hinst;
    wc.hIcon		= LoadIcon(hinst, TSTR("SPOLY"));
    wc.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH) (COLOR_APPWORKSPACE+1);
    wc.lpszMenuName	= TSTR("SPolyMenu");
    wc.lpszClassName	= g_szFrameWndClass;

    if(!RegisterClass(&wc))
      return FALSE;

    return TRUE;
}

#ifdef WIN32
#define szAppTitle TSTR("IDispatch Polygon Server (32-bit)")
#else //WIN32
#define szAppTitle TSTR("IDispatch Polygon Server")
#endif //WIN32

BOOL
InitInstance(HINSTANCE hinst, int nCmdShow)
{
    g_hinst = hinst;

    // Create a main frame window
    //
    g_hwndFrame = CreateWindow(
      g_szFrameWndClass,
      szAppTitle,
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      NULL,
      NULL,
      hinst,
      NULL);
    if(!g_hwndFrame)
      return FALSE;

    g_hwndClient = GetWindow(g_hwndFrame, GW_CHILD);
    if(!g_hwndClient)
      return FALSE;

    // create the status bar
    //
    g_psb = CStatBar::Create(g_hinst, g_hwndFrame);
    if(!g_psb)
      return FALSE;

    // initialize and show the status bar
    //
    g_psb->SetHeight(GetSystemMetrics(SM_CYCAPTION) - 1);
    g_psb->SetFont((HFONT)GetStockObject(SYSTEM_FONT));
    g_psb->SetText(OLESTR(""));
    g_psb->Show();

    ShowWindow(g_hwndFrame, nCmdShow);

    UpdateWindow(g_hwndFrame);

    return TRUE;
}


void
FrameWndOnCreate(HWND hwnd)
{
    CLIENTCREATESTRUCT ccs;

    ccs.hWindowMenu = NULL;
    ccs.idFirstChild = IDM_FIRSTCHILD;

    g_hwndClient = CreateWindow(
      TSTR("MDICLIENT"),
      0,
      WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
      0, 0, 0, 0,
      hwnd,
      (HMENU) 1,
      g_hinst,
      &ccs);
}


void
FrameWndOnSize(HWND hwnd)
{
    RECT rc;
    int height;

    // Get the client rectangle for the frame window
    GetClientRect(hwnd, &rc);

    height = g_psb->GetHeight();

    // adjust the client win to make room for the status bar.
    //
    MoveWindow(
      g_hwndClient,
      rc.left,
      rc.top,
      rc.right - rc.left,
      rc.bottom - rc.top - height,
      TRUE);

    // move the status bar to the bottom of the newly positioned window.
    //
    g_psb->SetX(rc.left);
    g_psb->SetY(rc.bottom - height),
    g_psb->SetWidth(rc.right - rc.left);
    g_psb->Move();
}


extern "C" long FAR PASCAL
FrameWndProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch(message){
    case WM_COMMAND:
      switch(wParam){
      case IDM_DUMP:
	CPoly::PolyDump();
	return 0;

      case IDM_CLEAR:
	InvalidateRect(g_hwndClient, NULL, TRUE);
	return 0;
      }
      break;

    case WM_CREATE:
      FrameWndOnCreate(hwnd);
      break;

    case WM_SIZE:
      FrameWndOnSize(hwnd);
      return 1;

    case WM_PAINT:
      CPoly::PolyDraw();
      break;

    case WM_CLOSE:
      DestroyWindow(hwnd);
      return 0;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    }
    return DefFrameProc(hwnd, g_hwndClient, message, wParam, lParam);
}


#if defined(WIN32)

extern "C" OLECHAR FAR*
ConvertStrAtoW(char FAR* strIn, OLECHAR FAR* buf, UINT size)
{
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, 
                       strIn, -1, buf, size) ; 
  return buf;
}

extern "C" OLECHAR FAR*
WideString(char FAR* strIn)
{
  static OLECHAR buf[256];
  
  return (ConvertStrAtoW(strIn, buf, 256));
}

extern "C" char FAR*
ConvertStrWtoA(OLECHAR FAR* strIn, char FAR* buf, UINT size)
{
  int badConversion = FALSE;
  
  WideCharToMultiByte(CP_ACP, NULL, 
	              strIn, -1, 
		      buf, size, 
		      NULL, &badConversion);
  return buf;
}

extern "C" char FAR*
AnsiString(OLECHAR FAR* strIn)
{
  static char buf[256];
  
  return (ConvertStrWtoA(strIn, buf, 256));	
}

#endif

