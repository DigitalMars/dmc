// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for the status bar module.
//

//-------------------------------------------------------------------------
#define IDM_STATUSBAR       501
#define IDM_TIMER           701
#define TIMER_TIMEOUT       1000


// System Menu string ID's
#define IDS_SYSMENU         900
#define IDS_MDISYSMENU      901

#define IDS_SCSIZE          SC_SIZE
#define IDS_SCMOVE          SC_MOVE
#define IDS_SCMINIMIZE      SC_MINIMIZE
#define IDS_SCMAXIMIZE      SC_MAXIMIZE
#define IDS_SCNEXTWINDOW    SC_NEXTWINDOW
#define IDS_SCPREVWINDOW    SC_PREVWINDOW
#define IDS_SCCLOSE         SC_CLOSE
#define IDS_SCRESTORE       SC_RESTORE
#define IDS_SCTASKLIST      SC_TASKLIST


// Global variable for status bar window
extern HWND hWndStatusbar;

// Function prototypes for status bar creation
BOOL CreateSBar(HWND);
void InitializeStatusBar(HWND);
void UpdateStatusBar(LPSTR, WORD, WORD);
