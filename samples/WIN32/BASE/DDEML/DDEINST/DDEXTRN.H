
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
   ddeextrn.h
*/

// Global Declarations
extern HANDLE           ghModule;
extern HWND             ghwndMain;
extern HANDLE           ghAccel;

extern LPSTR            szUserPath;
extern LPSTR            szUserGroup;

// Window Handle Globals
extern HWND             hwndStatus;
extern HWND             hwndStatusText;
extern HWND             hwndFileList;
extern HWND             hwndFileList2;
extern HWND             hwndAddButton;
extern HWND             hwndAddGroupButton;
extern HWND             hwndAddAll;
extern HWND             hwndExitButton;
extern HWND             hwndCombo;
extern HWND             hwndPathList;

// Dialog Box handle
extern HWND             hwndDialog;

// DDEML Globals
extern LONG             lIdInst;
extern LONG             lIdInst2;

// Program Flags
extern BOOL             fBatch;
