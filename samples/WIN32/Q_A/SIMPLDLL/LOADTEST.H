
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
*                                LOADTEST.H
*
\******************************************************************************/



/******************************************************************************\
*                            SYMBOLIC CONSTANTS
\******************************************************************************/

#define IDM_LOADLIBRARY  100 /* menu identifiers */
#define IDM_FREELIBRARY  101
#define IDM_CREATETHREAD 102
#define IDM_DLLFUNCTION1 103
#define IDM_DLLFUNCTION2 104
#define IDM_DLLFUNCTION3 105
#define IDM_DLLFUNCTION4 106
#define IDM_DLLDIALOGBOX 107
#define IDM_ABOUT        108



/******************************************************************************\
*                                  TYPEDEFS
\******************************************************************************/

typedef int (*PFNDLL)();



/******************************************************************************\
*                                 GLOBAL VARS
\******************************************************************************/

HWND ghwndMain;

PFNDLL gpfnDLLFunction1 = NULL;
PFNDLL gpfnDLLFunction2 = NULL;
PFNDLL gpfnDLLFunction3 = NULL;
PFNDLL gpfnDLLFunction4 = NULL;
PFNDLL gpfnDLLDialogBox = NULL;



/******************************************************************************\
*                              FUNCTION PROTOTYPES
\******************************************************************************/

LRESULT CALLBACK MainWndProc  (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM);

void ThreadProc  (void);
void FixMenu    (UINT, HMENU);
