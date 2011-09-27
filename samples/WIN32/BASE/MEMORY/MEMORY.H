
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

/*
 * Menu ID's
 */
#define MM_ABOUT        8000
#define MM_SERVER       8001
#define MM_CLIENT       8002
#define MM_RESERVE      8003

#define MM_OPT_1        7001
#define MM_OPT_2        7002
#define MM_OPT_3        7003
#define MM_OPT_4        7004
#define MM_OPT_5        7005
#define MM_OPT_6        7006
#define MM_OPT_7        7007
#define MM_OPT_8        7008

#define IDM_CASCADE     30
#define IDM_TILE        31
#define IDM_ARRANGE     32
#define IDM_CLOSEALL    33

#define SIZEOFCAPTIONTEXT       20

/*
 * Dialog ID's
 */
#define IDBTN_PAGE              101
#define IDBTN_MAP               102
#define IDBTN_OK                103

#define IDEDIT_MAPFILE          104
#define IDEDIT_MAPNAME          105


HWND        ghwndClient;
