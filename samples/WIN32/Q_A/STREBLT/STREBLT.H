
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* streblt.h - header file for the streblt demonstration program. */


/* function prototypes for the window procedures. */
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

VOID DrawGrids (HDC, int, int);

/* Top dialog item IDs */
#define  DID_X           100
#define  DID_Y           101
#define  DID_WIDTH       102
#define  DID_HEIGHT      103
#define  DID_XSRC        104
#define  DID_YSRC        105
#define  DID_SRCWIDTH    106
#define  DID_SRCHEIGHT   107

#define  DID_ROP0        110
#define  DID_ROP1        111
#define  DID_ROP2        112
#define  DID_NEWSRC      116

#define  DID_DRAW        120

#define  DID_CB_MODE     150
#define  DID_CB_PATTERN  151
#define  DID_CB_ROPS     152


/* accelerator IDs */
#define AID_PASTE       55
#define AID_COPY        56
#define AID_CYCLE       57



/* Misc. defines for size, color, and appearance of drawing. */
#define GRIDCOLOR  (COLORREF) 0x01000006
#define DIALOGHEIGHT  90
#define SEPARATORWIDTH 4
#define MAXCHARS      32
#define INC            5




/* other user messages sent to the main window or the dialog */
#define WM_PUTUPDESTRECT WM_USER+20
#define WM_PUTUPSRCRECT  WM_USER+21
#define WM_STRETCHBLT    WM_USER+23




/* structure for the lookup tables.  */
typedef struct tagLookupEntry{
    int     Value;
    char    String[30];
} LookupEntry;


#define NMODES  5
LookupEntry  strebltModes[NMODES] =
    {{ 1, "BLACKONWHITE"},
     { 2, "COLORONCOLOR"},
     { 3, "WHITEONBLACK"},
     { 4, "BLEND       "},
     { 5, "HALFTONE    "}};


#define NROPS  15
LookupEntry  StandardROPs[NROPS] =
    {{ 0x00CC0020, "SRCCOPY"    },
     { 0x00EE0086, "SRCPAINT"   },
     { 0x008800C6, "SRCAND"     },
     { 0x00660046, "SRCINVERT"  },
     { 0x00440328, "SRCERASE"   },
     { 0x00330008, "NOTSRCCOPY" },
     { 0x001100A6, "NOTSRCERASE"},
     { 0x00C000CA, "MERGECOPY"  },
     { 0x00BB0226, "MERGEPAINT" },
     { 0x00F00021, "PATCOPY"    },
     { 0x00FB0A09, "PATPAINT"   },
     { 0x005A0049, "PATINVERT"  },
     { 0x00550009, "DSTINVERT"  },
     { 0x00000042, "BLACKNESS"  },
     { 0x00FF0062, "WHITENESS"  }};


#define NPATTERNS  6
LookupEntry Patterns[NPATTERNS] =
    {{  0, "WHITE_BRUSH "},
     {  1, "LTGRAY_BRUSH"},
     {  2, "GRAY_BRUSH  "},
     {  3, "DKGRAY_BRUSH"},
     {  4, "BLACK_BRUSH "},
     {  5, "NULL_BRUSH  "}};
