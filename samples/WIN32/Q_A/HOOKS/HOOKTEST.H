
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

// menu commands

#define IDM_CALLWNDPROC     0
#define IDM_CBT             1
#define IDM_DEBUG           2
#define IDM_GETMESSAGE      3
#define IDM_KEYBOARD        4
#define IDM_MOUSE           5
#define IDM_MSGFILTER       6
#define IDM_EXIT            100

// function prototypes
long APIENTRY MainWndProc(HWND, UINT, UINT, LONG);
LRESULT CALLBACK CallWndProc(int, WPARAM, LPARAM);
LRESULT CALLBACK GetMsgProc(int, WPARAM, LPARAM);
LRESULT CALLBACK DebugProc(int, WPARAM, LPARAM);
LRESULT CALLBACK CBTProc(int, WPARAM, LPARAM);
LRESULT CALLBACK MouseProc(int, WPARAM, LPARAM);
LRESULT CALLBACK KeyboardProc(int, WPARAM, LPARAM);
LRESULT CALLBACK MessageProc(int, WPARAM, LPARAM);


//functions
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);

