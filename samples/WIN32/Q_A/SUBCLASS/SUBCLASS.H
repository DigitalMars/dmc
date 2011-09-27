
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/


#define   IDM_BUTTON        101
#define   IDM_EDIT          102
#define   IDM_LIST          103
#define   IDM_TEST          110



/* function prototypes for the window procedures. */
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SubclassWndProc(HWND, UINT, WPARAM, LPARAM);

VOID QueryWindowPos (HWND, LPRECT);
VOID PaintRect (HWND, LPRECT);
VOID SubclassWindow (HWND, WNDPROC);
DWORD ToggleMenu (HMENU, UINT);
LONG ShiftlParam(HWND, HWND, LONG);



#define WM_SC_CREATE  WM_USER+1



/* Different "Tracking actions." */
#define ACTIONNONE    0x0000
#define ACTIONMOVE    0x0001
#define ACTIONSIZEX   0x0002
#define ACTIONSIZEY   0x0004
#define ACTIONSIZEXY  0x0006


/* misc. defines for drawing and formating */
#define INC   5
#define EPSILON  (float) 0.0001
#define MAXCHARS 32
#define MINSIZE 5


/* Global variables. */
HWND   hwndMain;
HANDLE hInst;
BOOL   fTestMode;


/* structure pointed at by the extra user bytes. */
typedef struct tagExtraBytes{
    WNDPROC    pfnOldProc;
    RECT       rect;
    int        Action;   /* ACTIONSIZEXY, ACTIONMOVE, ... */
} ExtraBytes, *PExtraBytes;
