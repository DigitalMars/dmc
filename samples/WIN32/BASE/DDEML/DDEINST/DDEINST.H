
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*
   ddeinst.h -- include file for DDEML Installer.
*/

// Window Procedure function prototypes.
LRESULT APIENTRY    MainWndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT APIENTRY    PathDialogWndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT APIENTRY    ProgressDialogWndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT APIENTRY    StatusBarWndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT APIENTRY    ListBoxWndProc (HWND, UINT, WPARAM, LPARAM);
WNDPROC             DefListBoxWndProc;

// Utility functions
BOOL                DecodeOptions (LPSTR);
BOOL                InitializeApp (void);
void                CleanUpApp (void);

// Path traversal functions
BOOL                StartTraverseThread (LPSTR);
BOOL                TraversePath (LPSTR);

// DDEML Group information retrieval functions
BOOL                StartGroupRetrievalThread (void);
BOOL                GroupRetrieval ();

// DDEML Execute functions
BOOL                StartAddThread (void);
BOOL                CreateGroup (void);
BOOL                AddProgramItems (LPSTR);
HDDEDATA CALLBACK   GroupDDECallback (UINT, UINT, HANDLE, HSZ, HSZ, HDDEDATA,
                          LONG, LONG);


// Dialog Window Procedures
LRESULT APIENTRY    AboutBoxWndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT APIENTRY    ProgressDlgWndProc (HWND, UINT, WPARAM, LPARAM);

#define ACCEL_ID        1024
#define APPICON         2048

// The Menu identifiers
#define MainMenu        10
#define DI_EXIT         11
#define DI_ABOUT        13

// The Child control identifiers
#define ID_STATUSBAR    2
#define ID_FILELIST     3
#define ID_FILELIST2    4
#define ID_ADDBUTTON    5
#define ID_ADDALLBUTTON 6
#define ID_EXITBUTTON   7
#define ID_COMBOBOX     8
#define ID_PATHLIST     9
#define ID_ADDGROUP     10


// DDEML Starter flags
#define WM_USER_GET_GROUPS      (WM_USER+1)
#define WM_USER_GET_APPS        (WM_USER+2)


// Dialog Box update message
#define WM_USER_UPDATE_DIALOG   (WM_USER+10)
#define WM_USER_CLOSE_DIALOG    (WM_USER+12)
#define WM_USER_THREAD_DONE     (WM_USER+14)

// DDEML Progress message for Status Bar
#define WM_USER_UPDATE_STATUS   (WM_USER+16)

#define ID_DDEML_CONNECT        1
#define ID_DDEML_RETRIEVING     2
#define ID_DDEML_DISCONNECT     3
#define ID_DDEML_COMPLETE       4
#define ID_DDEML_CREATE         5
#define ID_DDEML_ADD            6
#define ID_DDEML_ACTIVATE       7
