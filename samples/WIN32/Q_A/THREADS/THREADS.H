
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#define IDM_SUSPENDGREEN     501
#define IDM_SUSPENDRED       502
#define IDM_RESUMEGREEN      601
#define IDM_RESUMERED        602

#define IDM_GIDLE            700
#define IDM_GLOW             701
#define IDM_GBNORM           702
#define IDM_GNORM            703
#define IDM_GANORM           704
#define IDM_GHIGH            705
#define IDM_GTC              706

#define IDM_RIDLE            750
#define IDM_RLOW             751
#define IDM_RBNORM           752
#define IDM_RNORM            753
#define IDM_RANORM           754
#define IDM_RHIGH            755
#define IDM_RTC              756

#define GREEN    0x00007F00
#define RED      0x0000007F

LRESULT CALLBACK MainWndProc (HWND, UINT, WPARAM, LPARAM);
VOID ThreadProc (LPVOID *);
