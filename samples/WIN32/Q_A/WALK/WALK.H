
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* definitions */

#define BUFSIZE 300

CHAR chPathName[BUFSIZE];

/* procedure definitions */

HANDLE FindFirstDirectory(LPTSTR, LPWIN32_FIND_DATA);
VOID Walk(WORD);
VOID main(VOID);
