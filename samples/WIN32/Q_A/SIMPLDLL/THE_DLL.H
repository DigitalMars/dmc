
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
*                                 THE_DLL.H
*
\******************************************************************************/



/******************************************************************************\
*                              SYMBOLIC CONSTANTS
\******************************************************************************/

#define BUFSIZE 80



/******************************************************************************\
*                               GLOBAL VARIABLES
\******************************************************************************/

HANDLE ghMod;



/******************************************************************************\
*                              FUNCTION PROTOTYPES
\******************************************************************************/

INT DLLFunction1 (void);
INT DLLFunction2 (int);
INT DLLFunction3 (HANDLE);
INT DLLFunction4 (HWND);

INT  DLLDialogBox (HWND);

BOOL CALLBACK DLLDlgProc (HWND, UINT, WPARAM, LPARAM);
