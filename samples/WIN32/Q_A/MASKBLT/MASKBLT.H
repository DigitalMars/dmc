
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* maskblt.h - header file for the maskblt demonstration program. */


/* function prototypes for the window procedures. */
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc    (HWND, UINT, WPARAM, LPARAM);

VOID DrawGrids (HDC, int, int);


/* Top dialog item IDs */
#define  DID_X          100
#define  DID_Y          101
#define  DID_WIDTH      102
#define  DID_HEIGHT     103
#define  DID_XSRC       104
#define  DID_YSRC       105
#define  DID_XMASK      106
#define  DID_YMASK      107

#define  DID_ROP0       110
#define  DID_ROP1       111
#define  DID_ROP2       112
#define  DID_ROP3       113

#define  DID_NEWSRC     300
#define  DID_NEWMASK    301

#define  DID_DRAW       302

#define  DID_CB_MODE    150
#define  DID_CB_PATTERN 151
#define  DID_CB_ROPS    152



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
#define WM_PUTUPMASKPT   WM_USER+22
#define WM_MASKBLT       WM_USER+23



/* structure for the lookup tables.  */
typedef struct tagLookupEntry{
    int     Value;
    char    String[30];
} LookupEntry;


#define NROPS  17
LookupEntry  StandardROPs[NROPS] =
    {{ SRCCOPY     , "SRCCOPY"    },
     { SRCPAINT    , "SRCPAINT"   },
     { SRCAND      , "SRCAND"     },
     { SRCINVERT   , "SRCINVERT"  },
     { SRCERASE    , "SRCERASE"   },
     { NOTSRCCOPY  , "NOTSRCCOPY" },
     { NOTSRCERASE , "NOTSRCERASE"},
     { MERGECOPY   , "MERGECOPY"  },
     { MERGEPAINT  , "MERGEPAINT" },
     { PATCOPY     , "PATCOPY"    },
     { PATPAINT    , "PATPAINT"   },
     { PATINVERT   , "PATINVERT"  },
     { DSTINVERT   , "DSTINVERT"  },
     { BLACKNESS   , "BLACKNESS"  },
     { WHITENESS   , "WHITENESS"  },
     { 0xccaa0000  , "Transparent1"  },
     { 0xaacc0000  , "Transparent2"  }};


#define NPATTERNS  6
LookupEntry Patterns[NPATTERNS] =
    {{  0, "WHITE_BRUSH "},
     {  1, "LTGRAY_BRUSH"},
     {  2, "GRAY_BRUSH  "},
     {  3, "DKGRAY_BRUSH"},
     {  4, "BLACK_BRUSH "},
     {  5, "NULL_BRUSH  "}};
