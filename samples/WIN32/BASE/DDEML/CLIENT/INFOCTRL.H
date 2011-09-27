
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/


// STYLES

#define ICSTY_OWNERDRAW     0x0001    // set if the central information is not
                                      // standard text.
#define ICSTY_SHOWFOCUS     0x0002    // set to allow focus painting an movement

#define ICSTY_HASFOCUS      0x8000

#define ICN_OWNERDRAW       (WM_USER + 676)     // notifies to draw
            // wParam=id, lParam=OWNERDRAWPS FAR *
#define ICN_HASFOCUS        (WM_USER + 677)     // notifies of focus set
            // wParam=fFocus, lParam=(hMemCtrlData, hwnd)
#define ICN_BYEBYE          (WM_USER + 678)     // notifies of imminent death
            // wParam=hwnd, lParam=dwUser

#define ICM_SETSTRING       (WM_USER + 776)     // alters a string
            // wParam=index, lParam=LPSTR

#define ICSID_UL            0
#define ICSID_UC            1
#define ICSID_UR            2
#define ICSID_LL            3
#define ICSID_LC            4
#define ICSID_LR            5
#define ICSID_CENTER        6

#define GWL_USER            0
#define GWL_INFODATA        4
#define ICCBWNDEXTRA        8

HWND CreateInfoCtrl(
LPSTR szTitle,
INT x,
INT y,
INT cx,
INT cy,
HWND hwndParent,
HANDLE hInst,
LPSTR pszUL,                // NULLs here are fine.
LPSTR pszUC,
LPSTR pszUR,
LPSTR pszLL,
LPSTR pszLC,
LPSTR pszLR,
DWORD  style,
HMENU id,
DWORD dwUser);

VOID MyCascadeChildWindows(HWND hwndParent);
VOID TileChildWindows(HWND hwndParent);

typedef struct {
    PSTR pszUL;
    PSTR pszUC;
    PSTR pszUR;
    PSTR pszLL;
    PSTR pszLC;
    PSTR pszLR;
    PSTR pszCenter;
    DWORD  style;
    RECT rcFocusUL;
    RECT rcFocusUR;
    RECT rcFocusLL;
    RECT rcFocusLR;
    HANDLE hInst;
} INFOCTRL_DATA;

typedef struct {
    RECT rcBound;
    RECT rcPaint;
    HDC  hdc;
    DWORD dwUser;
} OWNERDRAWPS;

