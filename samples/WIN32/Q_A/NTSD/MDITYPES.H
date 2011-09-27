/******************************Module*Header*******************************\
* Module Name: mditypes.h
*
* Contains typedefs for the MDI project
*
* Created: 24-Feb-1992 21:04:30
* Author: Petrus Wong
*
* Copyright (c) 1990 Microsoft Corporation
*
* (General description of its use)
*
* Dependencies:
*
*   (#defines)
*   (#includes)
*
\**************************************************************************/
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
