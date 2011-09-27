
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/


#include <windows.h>
#include <string.h>

/*
 * Resource defines
 */
#define WINDOWMENU  1

#ifdef RC_INVOKED
#define ID(id) id
#else
#define ID(id) MAKEINTRESOURCE(id)
#endif

#define APPICON         1001

/*
 * Menu ID's
 */
#define MM_ABOUT        8000
#define MM_BOUNCE       8001
#define MM_SNAKE        8002
#define MM_WOBBLE       8003

#define IDM_CASCADE     30
#define IDM_TILE        31
#define IDM_ARRANGE     32
#define IDM_CLOSEALL    33

#define SIZEOFCAPTIONTEXT       20
