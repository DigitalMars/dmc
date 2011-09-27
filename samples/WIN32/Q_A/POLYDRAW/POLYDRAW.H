
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* polydraw.h - header file for the polydraw demonstration program. */


/* function prototypes for the window procedures. */
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

#define MAXPOINTS  16
#define NONE     -1
#define HITSIZE  10

/* Strings to be written at the bottom of the window as instructions. */
char instructions0[] =
    "Left mouse button places or moves points.  Right button erases.";
char instructions1[] = "  default -- PT_LINETO  ";
char instructions2[] = "  <shift> -- PT_MOVETO  ";
char instructions3[] = "  <ctrl>  -- PT_BEZIERTO";
