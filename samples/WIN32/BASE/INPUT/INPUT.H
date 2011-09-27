
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#define IDM_ABOUT 100

BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, INT);
LONG APIENTRY MainWndProc(HWND, UINT, UINT, LONG);
BOOL APIENTRY About(HWND, UINT, UINT, LONG);

#ifdef WIN32
#define GET_WM_HSCROLL_CODE(wp, lp)    LOWORD(wp)
#define GET_WM_HSCROLL_POS(wp, lp)     HIWORD(wp)
#define GET_WM_HSCROLL_HWND(wp, lp)    (HWND)(lp)
#else
#define GET_WM_HSCROLL_CODE(wp, lp)    (wp)
#define GET_WM_HSCROLL_POS(wp, lp)     LOWORD(lp)
#define GET_WM_HSCROLL_HWND(wp, lp)    (HWND)HIWORD(lp)
#endif
