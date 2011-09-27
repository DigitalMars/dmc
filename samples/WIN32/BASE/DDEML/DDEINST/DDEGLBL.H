
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
   ddeglbl.h
*/

// Global Declarations
HANDLE           ghModule;
HWND             ghwndMain = (HWND) NULL;
HANDLE           ghAccel;

LPSTR            szUserPath = NULL;
LPSTR            szUserGroup = NULL;

// Window Handle Globals
HWND             hwndStatus;
HWND             hwndStatusText;
HWND             hwndFileList;
HWND             hwndFileList2;
HWND             hwndAddButton;
HWND             hwndAddGroupButton;
HWND             hwndAddAll;
HWND             hwndExitButton;
HWND             hwndCombo;
HWND             hwndPathList;

// Progress Indicator Dialog
HWND             hwndDialog;

// DDEML Globals
LONG             lIdInst;
LONG             lIdInst2;


// Program Flags
BOOL             fBatch = FALSE;
