
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#include "mdi.h"

//
// Instance data for each MDI child window
//
typedef struct _PerWndInfo {
    HWND    hParent;
    HWND    hTextWnd;
    RECT    rcClient;
    char    CaptionBarText[SIZEOFCAPTIONTEXT];
} INFO, *PINFO;
