
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/******************************************************************************\
*
*                                 LINKTEST.H
*
\******************************************************************************/



/******************************************************************************\
*                              SYMBOLIC CONSTANTS
\******************************************************************************/

#define IDM_CREATETHREAD 100 /* menu identifiers */
#define IDM_DLLFUNCTION1 101
#define IDM_DLLFUNCTION2 102
#define IDM_DLLFUNCTION3 103
#define IDM_DLLFUNCTION4 104
#define IDM_DLLDIALOGBOX 105
#define IDM_ABOUT        106



/******************************************************************************\
*                                  GLOBAL VARS
\******************************************************************************/

HWND ghwndMain;



/******************************************************************************\
*                              FUNCTION PROTOTYPES
\******************************************************************************/

LRESULT CALLBACK MainWndProc  (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM);

void ThreadProc   (void);
