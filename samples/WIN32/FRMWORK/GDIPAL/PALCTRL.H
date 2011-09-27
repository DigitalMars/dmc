// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for the pen style dialog.
//

// PalCtrl messages
#define PCM_GETCURSEL WM_USER+1
#define PCM_SETCURSEL WM_USER+2

// PalCtrl notification codes
#define PCN_CHANGE  1

// PalCtrl defines
#define PAL_CBWNDEXTRA      (1 * sizeof(HANDLE))  // Size of window extra bytes
#define WL_PAL_HPALCTRLINFO 0                     // Handle to PALINFO structure

// PalCtrl internal data structure
typedef struct
{
    HPALETTE hPal;                  // Handle to palette being displayed.
    int      nEntries;              // # of entries in the palette.
    int      nRows, nCols;          // # of Rows/Columns in window.
    int      cxSquare, cySquare;    // Pixel width/height of palette square.
    int      nEntry;                // Currently selected palette square.
} PALCTRLINFO, FAR *LPPALCTRLINFO;

// PalCtrl data structure for passing current selection info
typedef struct
{
    int index;
    int red;
    int green;
    int blue;
} PALINFO, FAR *LPPALINFO;

// globals
extern PALINFO palinfo;

// Msg handler prototypes
LRESULT MsgPalCtrlCreate(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPalCtrlPaint (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPalCtrlPaint (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPalCtrlLButtonDown(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPalCtrlLButtonDblClk(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPalCtrlDestroy(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPalCtrlGetDlgCode(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPalCtrlKeyDown(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPalCtrlGetCurSel(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPalCtrlSetCurSel(HWND, UINT, WPARAM, LPARAM);

// Function prototypes.
LRESULT CALLBACK PalCtrlProc(HWND, UINT, WPARAM, LPARAM);
HPALETTE GetSystemPalette(void);
HPALETTE CopyPaletteChangingFlags(HPALETTE, BYTE);
BOOL RegisterPalCtrlClass(HINSTANCE);
void SetPalCtrlPalette(HWND, HPALETTE);
int  ColorsInPalette(HPALETTE);
int  PalEntriesOnDevice(HDC);
void HighlightSquare(HDC, HPALETTE, int, int, int, int, int);
void UnHighlightSquare(HDC, int, int, int, int);
