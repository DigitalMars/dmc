
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

// Porting macro
#ifdef WIN32
#define LONG2POINT(l, pt)  ((pt).x = (SHORT)LOWORD(l), (pt).y = (SHORT)HIWORD(l))
#else
#define LONG2POINT(l, pt)  ((pt).x = (INT)LOWORD(l), (pt).y = (INT)HIWORD(l))
#endif

/* Defines for top-level menu positions */
#define IDFILE_POS     0
#define IDCOLORS_POS   1
#define IDSTATES_POS   2
#define IDHELP_POS     3

#define MEASUREITEMWIDTH  40
#define MEASUREITEMHEIGHT 40

/* Dialog ID for the About.. dialog */
#define ID_ABOUT 100

/* Defines for menu items */
#define IDM_EXIT   202
#define IDM_ABOUT  203

/* Colors menu IDs */
#define IDM_COLOROWNERDR 500
#define IDM_BLACK        501
#define IDM_BLUE         502
#define IDM_RED          503
#define IDM_GREEN        504

/* Floating popup menu IDs */
#define IDM_FONT         600
#define IDM_SIZE         601
#define IDM_STYLE        602

/* States menu IDs */
#define IDM_STATE1       610
#define IDM_STATE2       611
#define IDM_STATE3       612
#define IDM_STATE4       613

/* BitBlt Raster operation */
#define ROP_PSDPxax     0x00B8074AL

BOOL NEAR PASCAL MenuInit(HANDLE);
LONG APIENTRY MenuWndProc(HWND, UINT, UINT, LONG);
BOOL APIENTRY About(HWND, UINT, UINT, LONG);
