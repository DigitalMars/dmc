
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* plgblt.h - header file for the PlgBlt() demonstration program. */


/* function prototypes for the window procedures. */
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DlgProc    (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK About      (HWND, UINT, WPARAM, LPARAM);

VOID DrawGrids (HDC, int, int);

/* Top dialog item IDs */
#define  DID_P1X       101
#define  DID_P1Y       102
#define  DID_P2X       103
#define  DID_P2Y       104
#define  DID_P3X       105
#define  DID_P3Y       106
#define  DID_XSRC      110
#define  DID_YSRC      111
#define  DID_WIDTH     112
#define  DID_HEIGHT    113
#define  DID_XMASK     114
#define  DID_YMASK     115
#define  DID_NEWSRC    160
#define  DID_NEWMASK   161

#define  DID_DRAW    200

#define IDM_COPY            300
#define IDM_PASTE           301
#define IDM_BOTH            302
#define IDM_SPINOFF         310
#define IDM_FLIPONCE        311
#define IDM_SPIN5           312
#define IDM_SPIN10          313
#define IDM_SPIN30          314
#define IDM_SPIN60          315
#define IDM_SPIN90          316
#define IDM_SPINTOPLEFT     317
#define IDM_SPINCENTER      318
#define IDM_MODE_BLACKONWHITE    320
#define IDM_MODE_COLORONCOLOR    321
#define IDM_MODE_WHITEONBLACK    322
#define IDM_MODE_HALFTONE        323
#define IDM_ABOUT           330





/* Misc. defines for size, color, and appearance of drawing. */
#define GRIDCOLOR  (COLORREF) 0x01000006
#define DIALOGHEIGHT  90
#define SEPARATORWIDTH 4
#define MAXCHARS      32
#define INC            5




/* other user messages sent to the main window or the dialog */
#define WM_PUTUPLPOINTS  WM_USER+20
#define WM_PUTUPSRCRECT  WM_USER+21
#define WM_PUTUPMASKPT   WM_USER+22
#define WM_PLGBLT        WM_USER+23
#define WM_SPIN          WM_USER+24
