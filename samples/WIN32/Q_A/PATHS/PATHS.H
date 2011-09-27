
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* paths.h - header file for the paths demonstration program. */


/* function prototypes for the window procedures. */
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void DrawPath(HDC, LPRECT);
BOOL NewAngleArc(HDC, int, int, DWORD, float, float);


/* strings to write to the window as labels. */
#define STROKEPATH        "StrokePath"
#define FILLPATH          "FillPath"
#define STROKEANDFILLWIND "StrokeAndFill, Winding"
#define STROKEANDFILLALT  "StrokeAndFill, Alternate"
#define CLIPPATHWIND      "ClipPath, Winding"
#define CLIPPATHALT       "ClipPath, Alternate"
