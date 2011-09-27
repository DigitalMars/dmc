/*** 
*main.cpp
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
*  This module is the main entry point of the sample IDispatch
*  calculator, dspcalc2.exe.
*
*  This program is intended to demonstrate an implementation of
*  the IDispatch interface.
*
*Implementation Notes:
*
*****************************************************************************/

#include "dspcalc2.h"

TCHAR g_szAppName[] = TSTR("DspCalc2");

BOOL InitApplication(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);


extern "C" {
long FAR PASCAL WndProc(HWND, UINT, WPARAM, LPARAM);
int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
}


extern "C" int PASCAL
WinMain(
    HINSTANCE hinst,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    MSG msg;

    if(!hPrevInstance)
      if(!InitApplication(hinst))
	return FALSE;

    if(InitOle() != NOERROR)
      return FALSE;

    if(!InitInstance(hinst, nCmdShow)){
      UninitOle();
      return FALSE;
    }

    while(GetMessage(&msg, NULL, NULL, NULL)) {
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

    wc.style		= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc	= WndProc;
    wc.cbClsExtra	= 0;
    wc.cbWndExtra	= DLGWINDOWEXTRA;
    wc.hInstance	= hinst;
    wc.hIcon		= LoadIcon(hinst, g_szAppName);
    wc.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE+1);
    wc.lpszMenuName	= NULL;
    wc.lpszClassName	= g_szAppName;

    if(!RegisterClass(&wc))
      return FALSE;

    return TRUE;
}


BOOL
InitInstance(HINSTANCE hinst, int nCmdShow)
{
    g_pcalc->m_hwnd = CreateDialog(hinst, g_szAppName, 0, NULL);

    ShowWindow(g_pcalc->m_hwnd, nCmdShow);

    g_pcalc->m_arith.Display();

    return TRUE;
}


extern "C" long FAR PASCAL
WndProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch(message){
    case WM_COMMAND:
      switch(wParam){
      case IDC_ZERO:
      case IDC_ONE:
      case IDC_TWO:
      case IDC_THREE:
      case IDC_FOUR:
      case IDC_FIVE:
      case IDC_SIX:
      case IDC_SEVEN:
      case IDC_EIGHT:
      case IDC_NINE:
      case IDC_PLUS:
      case IDC_MINUS:
      case IDC_MULT:
      case IDC_DIV:
      case IDC_CLEAR:
      case IDC_EQUALS:
	g_pcalc->m_arith.ButtonPush(wParam);
	return 0;
      }
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}


#if defined(WIN32)

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
