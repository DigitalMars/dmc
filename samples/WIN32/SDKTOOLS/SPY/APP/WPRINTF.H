


/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* These functions are used to debug apps quickly */
/* NOTE windows.h must be included prior to this file */

HWND APIENTRY CreatePrintfWindow (HWND,LPSTR,BOOL,INT);
HWND APIENTRY CreatePrintfWin (HWND,HANDLE,LPSTR,DWORD,INT,INT,INT,INT,INT);
INT FAR cdecl vwprintf(HWND hWnd, LPSTR format, va_list marker);

