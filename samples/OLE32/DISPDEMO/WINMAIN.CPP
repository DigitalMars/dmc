/*** 
*dispdemo.c - IDespatch demo/sample client application.
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
*  This module is the main entry point for the sample IDispatch client,
*  dispdemo.exe.
*
*  This program is intended to demonstrate a client invoking methods
*  and referencing properties on a remote object via the IDispatch
*  interface.
*
*  The bulk of the sample can be found in the file crempoly.cpp, which
*  implements CRemPoly, the remote polygon class.
*
*Implementation Notes:
*
*****************************************************************************/

#include "dispdemo.h"

extern BOOL g_fTrace;

BOOL InitApplication(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
BOOL FAR PASCAL About(HWND, unsigned, WORD, LONG);

extern "C" {
int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT FAR PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);
}

HINSTANCE g_hInst;

TCHAR g_szDispDemoWClass[] = TSTR("DispDemoWClass");

extern "C" int PASCAL
WinMain(
    HINSTANCE hinst,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    MSG msg;


    (lpCmdLine); // UNUSED

    if(!hPrevInstance)
      if(!InitApplication(hinst))
	return FALSE;

    if(InitOle() != NOERROR)
      return FALSE;

    if(!InitInstance(hinst, nCmdShow))
      return FALSE;

    while(GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    UninitOle();

    return msg.wParam;
}


BOOL
InitApplication(HINSTANCE hinst)
{
    WNDCLASS  wc;

    wc.style		= NULL;
    wc.lpfnWndProc	= MainWndProc;
    wc.cbClsExtra	= 0;
    wc.cbWndExtra	= 0;
    wc.hInstance	= hinst;
    wc.hIcon		= LoadIcon(hinst, TSTR("DISPDEMO"));
    wc.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH); 
    wc.lpszMenuName	= TSTR("DispDemoMenu");
    wc.lpszClassName	= g_szDispDemoWClass;
    if(!RegisterClass(&wc))
      return FALSE;

    return TRUE;
}

#ifdef WIN32
#define szAppTitle TSTR("IDispatch Demo App (32-bit)")
#else //WIN32
#define szAppTitle TSTR("IDispatch Demo App")
#endif //WIN32

BOOL
InitInstance(HINSTANCE hinst, int nCmdShow)
{
    HWND hWnd;

    g_hInst = hinst;

    hWnd = CreateWindow(
      g_szDispDemoWClass,
      szAppTitle,
      WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT, 300, 100,
      NULL, NULL, hinst, NULL);

    if(!hWnd)
      return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


BOOL FAR PASCAL
About(HWND hDlg, unsigned message, WORD wParam, LONG lParam)
{
    switch(message){
    case WM_INITDIALOG:
      return TRUE;

    case WM_COMMAND:
      switch(wParam){
      case IDOK:
      case IDCANCEL:
	EndDialog(hDlg, TRUE);
	return TRUE;
      }
      break;
    }
    return FALSE;
}


extern "C" LRESULT FAR PASCAL
MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HMENU hmenu;
static DLGPROC pfnAbout;

    switch(message){
    case WM_COMMAND:
      switch(wParam){
      case IDM_TRACE:
	/* enable/disable trace */
	g_fTrace = (g_fTrace) ? FALSE : TRUE;
	hmenu = GetMenu(hwnd);
	CheckMenuItem(hmenu, IDM_TRACE, g_fTrace ? MF_CHECKED : MF_UNCHECKED);
	return 0;

      case IDM_POLY:
	DoPoly(CLSID_CPoly);
	return 0;

      case IDM_POLY2:
	DoPoly(CLSID_CPoly2);
	return 0;

      case IDM_ABOUT:
	pfnAbout = (DLGPROC)MakeProcInstance((FARPROC)About, g_hInst);
	DialogBox(g_hInst, TSTR("AboutBox"), hwnd, pfnAbout);
	FreeProcInstance(pfnAbout);
	return 0;
      }
      break;

    case WM_CLOSE:
      DestroyWindow(hwnd);
      return 0;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

