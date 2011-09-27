
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#ifdef WIN16
#define APIENTRY FAR PASCAL
typedef int (APIENTRY *WNDPROC)();
typedef WORD WPARAM;
typedef VOID *PVOID;
typedef WORD UINT;
#endif

#define APPCLASS   "GDIDEMO"
#define APPTITLE   "Windows GDI Demonstration"


#define APPMENU   1000
#define APPICON   1001
#define ABOUTBOX  1002

#define IDM_DEMO_POLYBEZIER 100
#define IDM_DEMO_XFORM      101
#define IDM_DEMO_MAZE       102
#define IDM_DEMO_DRAW       103
#define IDM_DEMO_BOUNCE     104

#define IDM_WINDOW_CASCADE  200
#define IDM_WINDOW_TILE     201
#define IDM_WINDOW_ICON     202
#define IDM_HELP_ABOUT      900


#define COLOR_SCALE_RED     1
#define COLOR_SCALE_GREEN   2
#define COLOR_SCALE_BLUE    3
#define COLOR_SCALE_GRAY    4

#define CLIENTWND           0


/*
** MAIN WINDOW ROUTINES (gdidemo.c)
*/
LONG  APIENTRY WndProc(HWND,UINT,WPARAM,LONG);
BOOL           CreateProc(HWND);
VOID           DestroyProc(HWND);
BOOL           CommandProc(HWND,WPARAM,LONG);
VOID           PaintProc(HWND);
DWORD FAR      lRandom(VOID);


/*
** INITIALIZATION ROUTINES (init.c)
*/
BOOL FAR RegisterAppClass(HANDLE);
VOID FAR UnregisterAppClass(HANDLE);
HWND FAR CreateAppWindow(HANDLE);
HWND FAR CreateMDIClientWindow(HWND);



/*
** DIALOG ROUTINES (dialog.c)
*/
BOOL APIENTRY AboutDlgProc(HWND,UINT,WPARAM,LONG);
int  FAR      DisplayDialogBox(HWND,LPSTR,WNDPROC,LONG);
VOID          PaintWindow(HWND,int);
HPALETTE      CreateColorScalePalette(HDC,int);


/*
** WINDOW ROUTINES (wininfo.c)
*/
BOOL  FAR AllocWindowInfo(HWND,WORD);
PVOID FAR LockWindowInfo(HWND);
BOOL  FAR UnlockWindowInfo(HWND);
BOOL  FAR FreeWindowInfo(HWND);




#ifdef TESTDEBUG
#define DEBUGOUT(lpszString) OutputDebugString(lpszString)
#else
#define DEBUGOUT(lpszString) {}
#endif


/*
** Porting macros...
*/
#if defined(_WIN32) || defined(WIN32)
#define GETINSTANCE(hWnd)   (HANDLE)GetWindowLong(hWnd,GWL_HINSTANCE)
#define GETCLASSBRUSH(hWnd) (HBRUSH)GetClassLong(hWnd,GCL_HBRBACKGROUND)
#else
#define GETINSTANCE(hWnd)   (HANDLE)GetWindowWord(hWnd,GWW_HINSTANCE)
#define GETCLASSBRUSH(hWnd) (HBRUSH)GetClassWord(hWnd,GCW_HBRBACKGROUND)
#endif
