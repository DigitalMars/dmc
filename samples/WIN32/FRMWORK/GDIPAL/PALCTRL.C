// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   palctrl.c
//
//  PURPOSE:  Implement the window procedure for the palette control
//            window.
//
//  FUNCTIONS:
//    RegisterPalCtrlClass    - Registers the PalCtrl window class.
//    PalCtrlProc             - Processes messages for the main window.
//    MsgPalCtrlCommand       - Handle the WM_COMMAND messages for the window.
//    MsgPalCtrlPaint         - Handles the WM_PAINT messages.
//    MsgPalCtrlLButtonDown   - Handles the WM_LBUTTONDOWN messages.
//    MsgPalCtrlLButtonDblClk - Handles the WM_LBUTTONDBLCLK messages.
//    MsgPalCtrlGetDlgCode    - Handles the WM_GETDLGCODE message.
//    MsgPalCtrlKeyDown       - Handles the WMKEYDOWN messages.
//    MsgPalCtrlDestroy       - Handles the WM_DESTROY message by calling 
//                              PostQuitMessage().
//    MsgPalCtrlGetCurSel     - Handles the PCM_GETCURSEL messages.
//    MsgPalCtrlSetCurSel     - Handles the PCM_SETCURSEL messages.
//    HighlightSquare         - Highlights a color square on the PalCtrl.
//    UnHighlightSquare       - Unhighlights a color square on the PalCtrl.
//    SetPalCtrlPalette       - Sets the palette to be displayed in the
//                              PalCtrl window.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include "palette.h"
#include "palctrl.h"


// globals
PALINFO palinfo;


// Palette control message table definition.
MSD rgmsdPalCtrl[] =
{
    {WM_CREATE,        MsgPalCtrlCreate},
    {WM_PAINT,         MsgPalCtrlPaint},
    {WM_LBUTTONDOWN,   MsgPalCtrlLButtonDown},
    {WM_LBUTTONDBLCLK, MsgPalCtrlLButtonDblClk},
    {WM_DESTROY,       MsgPalCtrlDestroy},
    {WM_GETDLGCODE,    MsgPalCtrlGetDlgCode},
    {WM_KEYDOWN,       MsgPalCtrlKeyDown},

    // PalCtrl interface messages
    {PCM_GETCURSEL,    MsgPalCtrlGetCurSel},
    {PCM_SETCURSEL,    MsgPalCtrlSetCurSel}
};

MSDI msdiPalCtrl =
{
    sizeof(rgmsdPalCtrl) / sizeof(MSD),
    rgmsdPalCtrl,
    edwpWindow
};


//
//  FUNCTION: RegisterPalCtrlClass(HINSTANCE)
//
//  PURPOSE:  Registers the palette control window class.
//
//  PARAMETERS:
//
//    hInstance - instance that owns this class
//
//  RETURN VALUE:
//
//    TRUE for success, FALSE otherwise
//
//  COMMENTS:
//

BOOL RegisterPalCtrlClass(HINSTANCE hInstance)
{
    WNDCLASS wc;

    // Fill in window class structure with parameters that describe the
    // palette control window.

    wc.style         = CS_DBLCLKS;              // Class style(s).
    wc.lpfnWndProc   = (WNDPROC)PalCtrlProc;    // Window Procedure
    wc.cbClsExtra    = 0;                       // No per-class extra data.
    wc.cbWndExtra    = PAL_CBWNDEXTRA;          // Per-window extra data.
    wc.hInstance     = hInstance;               // Owner of this class
    wc.hIcon         = NULL;                    // Icon name from .RC
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // Cursor
    wc.hbrBackground = GetStockObject(NULL_BRUSH); // Default color
    wc.lpszMenuName  = NULL;                    // Menu name from .RC
    wc.lpszClassName = "PalCtrlClass";          // Name to register as

    // Register the window class
    return RegisterClass(&wc);
}


//
//  FUNCTION: PalCtrlProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispMessage() function with the main window's message dispatch
//    information (msdiPalCtrl) and the message specific information.
//

LRESULT CALLBACK PalCtrlProc(HWND   hwnd, 
                             UINT   uMessage, 
                             WPARAM wparam, 
                             LPARAM lparam)
{
    return DispMessage(&msdiPalCtrl, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgPalCtrlCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: 
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//

LRESULT MsgPalCtrlCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HANDLE hPalCtrlInfo;

    hPalCtrlInfo = GlobalAlloc(GHND, sizeof(PALCTRLINFO));
    if (hPalCtrlInfo)
    {
        RECT          rect;
        LPPALCTRLINFO lpPalCtrlInfo;

        GetClientRect(hwnd, &rect);

        lpPalCtrlInfo              = (LPPALCTRLINFO)GlobalLock(hPalCtrlInfo);
        lpPalCtrlInfo->hPal        = NULL;
        lpPalCtrlInfo->nEntries    = 0;
        lpPalCtrlInfo->nRows       = 16;
        lpPalCtrlInfo->nCols       = 16;
        lpPalCtrlInfo->cxSquare    = (rect.right - rect.left) / 16;
        lpPalCtrlInfo->cySquare    = (rect.bottom - rect.top) / 16;
        lpPalCtrlInfo->nEntry      = 0;

        GlobalUnlock(hPalCtrlInfo);
    }

    SetWindowLong(hwnd, WL_PAL_HPALCTRLINFO, (LONG)hPalCtrlInfo);

    SetPalCtrlPalette(hwnd, GetSystemPalette());

    // notify parent
    PostMessage(GetParent(hwnd),
                WM_COMMAND,
                MAKEWPARAM((WORD)GetWindowLong(hwnd, GWL_ID), PCN_CHANGE),
                (LPARAM)hwnd);

    return 0;
}


//
//  FUNCTION: MsgPalCtrlPaint(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: 
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//

LRESULT MsgPalCtrlPaint(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    RECT          rect;
    HDC           hDC;
    PAINTSTRUCT   ps;
    int           nCols;
    int           nColors, i;
    int           cxSquare, cySquare;
    HPALETTE      hPal, hOldPal = NULL;
    HANDLE        hPalCtrlInfo;
    LPPALCTRLINFO lpPalCtrlInfo;
    int           nCurrent;

    // First do some setup.

    GetClientRect(hwnd, &rect);

    hDC      = BeginPaint(hwnd, &ps);
    hPalCtrlInfo = (HANDLE)GetWindowLong(hwnd, WL_PAL_HPALCTRLINFO);

    if (!hPalCtrlInfo)
        goto ENDPAINT;

    lpPalCtrlInfo = (LPPALCTRLINFO)GlobalLock(hPalCtrlInfo);
    hPal          = lpPalCtrlInfo->hPal;
    nColors       = lpPalCtrlInfo->nEntries;
    cxSquare      = lpPalCtrlInfo->cxSquare;
    cySquare      = lpPalCtrlInfo->cySquare;
    nCurrent      = lpPalCtrlInfo->nEntry;
    nCols         = lpPalCtrlInfo->nCols;
    GlobalUnlock(hPalCtrlInfo);

    // Let's paint -- first realize the palette.  Note that
    //  we ALWAYS realize the palette as if it were a background
    //  palette (i.e. the last parm is TRUE).  We do this, since
    //  we will already be the foreground palette if we are
    //  supposed to be (because we handle the WM_QUERYNEWPALETTE
    //  message).
    if (hPal)
        hOldPal = SelectPalette(hDC, hPal, TRUE);
    else
        goto ENDPAINT;

    RealizePalette(hDC);

    // Go through the palette displaying each color
    //  as a rectangle.
    for (i = 0;  i < nColors;  i++)
    {
        HBRUSH hBrush, hOldBrush;

        hBrush = CreateSolidBrush(PALETTEINDEX(i));
        if (hBrush)
        {
            POINT pt;

            hOldBrush = SelectObject(hDC, hBrush);

            pt.x = (i % nCols) * cxSquare;
            pt.y = (i / nCols) * cySquare;

            Rectangle(hDC,
                      pt.x,
                      pt.y,
                      pt.x + cxSquare,
                      pt.y + cySquare);

            SelectObject(hDC, hOldBrush);
            DeleteObject(hBrush);
        }
    }

    // Highlight the currently selected palette square
    HighlightSquare(hDC, 
                    hPal, 
                    nCurrent,
                    cxSquare, 
                    cySquare, 
                    nCols,
                    nColors);

    // Clean up.
    if (hOldPal)
        SelectPalette(hDC, hOldPal, FALSE);

ENDPAINT:
    EndPaint(hwnd, &ps);
    
    return 0;
}


//
//  FUNCTION: MsgPalCtrlLButtonDown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Process WM_LBUTTONDOWN messages by performing hit-testing
//            on the color grid to see which color is selected by the user.
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//

LRESULT MsgPalCtrlLButtonDown(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HDC           hDC;
    HANDLE        hPalCtrlInfo;
    LPPALCTRLINFO lpPalCtrlInfo;
    int           nRow, nCol;

    hPalCtrlInfo = (HANDLE)GetWindowLong(hwnd, WL_PAL_HPALCTRLINFO);
    if (!hPalCtrlInfo)
        return 0;

    lpPalCtrlInfo = (LPPALCTRLINFO)GlobalLock(hPalCtrlInfo);
    nRow          = (HIWORD(lparam)) / lpPalCtrlInfo->cySquare;
    nCol          = LOWORD(lparam) / lpPalCtrlInfo->cxSquare;
    hDC           = GetDC(hwnd);

    // Determine which entry is the new highlighted entry.
    // Since our actual palette grid may not fill the entire
    // control, check that we're really clicking in the palette
    // gris and not just in the control client area...
    if (nCol < 16)
        if ((nRow * lpPalCtrlInfo->nCols + nCol) < lpPalCtrlInfo->nEntries)
        {
            UnHighlightSquare(hDC, 
                              lpPalCtrlInfo->nEntry, 
                              lpPalCtrlInfo->cxSquare,
                              lpPalCtrlInfo->cySquare,
                              lpPalCtrlInfo->nCols);

            lpPalCtrlInfo->nEntry = nRow * lpPalCtrlInfo->nCols + nCol;
        
            HighlightSquare(hDC,
                            lpPalCtrlInfo->hPal, 
                            lpPalCtrlInfo->nEntry,
                            lpPalCtrlInfo->cxSquare, 
                            lpPalCtrlInfo->cySquare, 
                            lpPalCtrlInfo->nCols,
                            lpPalCtrlInfo->nEntries);
        
            // notify parent
            PostMessage(GetParent(hwnd),
                        WM_COMMAND,
                        MAKEWPARAM((WORD)GetWindowLong(hwnd, GWL_ID),
                                   PCN_CHANGE),
                        (LPARAM)hwnd);
        }

    ReleaseDC(hwnd, hDC);
    GlobalUnlock(hPalCtrlInfo);

    return 0;
}


//
//  FUNCTION: MsgPalCtrlLButtonDblClk(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Process WM_LBUTTONDBLCLK messages by sending WM_COMMAND IDOK
//            to the parent window.
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//

LRESULT MsgPalCtrlLButtonDblClk(HWND   hwnd,
                                UINT   uMessage,
                                WPARAM wparam,
                                LPARAM lparam)
{
    // send IDOK to parent window
    PostMessage(GetParent(hwnd),
                WM_COMMAND,
                MAKEWPARAM(IDOK, (WORD)GetWindowLong(hwnd, GWL_ID)),
                (LPARAM)hwnd);

    return 0;
}


//
//  FUNCTION: MsgPalCtrlGetDlgCode(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Process WM_GETDLGCODE in order for PalCtrl to receive input
//            for the arrow keys.
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Returns DLGC_WANTARROWS
//
//  COMMENTS:
//

LRESULT MsgPalCtrlGetDlgCode(HWND   hwnd,
                             UINT   uMessage,
                             WPARAM wparam,
                             LPARAM lparam)
{
    return DLGC_WANTARROWS;
}


//
//  FUNCTION: MsgPalCtrlKeyDown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Process WM_KEYDOWN messages by changing the selected color
//            accordingly.
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//

LRESULT MsgPalCtrlKeyDown(HWND   hwnd,
                          UINT   uMessage,
                          WPARAM wparam,
                          LPARAM lparam)
{
    HDC           hDC;
    HANDLE        hPalCtrlInfo;
    LPPALCTRLINFO lpPalCtrlInfo;
    int           nEntry, nMaxEntry;
    int           nCol;

    hPalCtrlInfo = (HANDLE)GetWindowLong(hwnd, WL_PAL_HPALCTRLINFO);
    if (!hPalCtrlInfo)
        return 0;

    lpPalCtrlInfo = (LPPALCTRLINFO)GlobalLock(hPalCtrlInfo);
    
    nEntry    = lpPalCtrlInfo->nEntry;
    nMaxEntry = lpPalCtrlInfo->nEntries - 1;
    nCol      = lpPalCtrlInfo->nCols;

    switch (wparam)
    {
        case VK_LEFT:
            if ((nEntry - 1) <= 0)
                nEntry = 0;
            else
                nEntry--;
            goto UpdateSelection;
            break;

        case VK_RIGHT:
            if ((nEntry + 1) >= nMaxEntry)
                nEntry = nMaxEntry;
            else
                nEntry++;
            goto UpdateSelection;
            break;

        case VK_UP:
            if ((nEntry - nCol) >= 0)
                nEntry -= nCol;
            goto UpdateSelection;
            break;

        case VK_DOWN:
            if ((nEntry + nCol) <= nMaxEntry)
                nEntry += nCol;

UpdateSelection:
            if (nEntry != lpPalCtrlInfo->nEntry)
            {
                hDC = GetDC(hwnd);

                UnHighlightSquare(hDC, 
                                  lpPalCtrlInfo->nEntry, 
                                  lpPalCtrlInfo->cxSquare,
                                  lpPalCtrlInfo->cySquare,
                                  lpPalCtrlInfo->nCols);

                lpPalCtrlInfo->nEntry = nEntry;
        
                HighlightSquare(hDC,
                                lpPalCtrlInfo->hPal, 
                                lpPalCtrlInfo->nEntry,
                                lpPalCtrlInfo->cxSquare, 
                                lpPalCtrlInfo->cySquare, 
                                lpPalCtrlInfo->nCols,
                                lpPalCtrlInfo->nEntries);
        
                // notify parent
                PostMessage(GetParent(hwnd),
                            WM_COMMAND,
                            MAKEWPARAM((WORD)GetWindowLong(hwnd, GWL_ID),
                                       PCN_CHANGE),
                            (LPARAM)hwnd);
            
                ReleaseDC(hwnd, hDC);
            }
            break;

        default:
            break;
    }

    GlobalUnlock(hPalCtrlInfo);

    return 0;
}


//
//  FUNCTION: MsgPalCtrlDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls PostQuitMessage().
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//

LRESULT MsgPalCtrlDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HANDLE    hPalCtrlInfo;
    LPPALCTRLINFO lpPalCtrlInfo;

    hPalCtrlInfo = (HANDLE)GetWindowLong(hwnd, WL_PAL_HPALCTRLINFO);
    if (!hPalCtrlInfo)
        return 0;
    
    lpPalCtrlInfo = (LPPALCTRLINFO)GlobalLock(hPalCtrlInfo);

    if (lpPalCtrlInfo->hPal)
        DeleteObject(lpPalCtrlInfo->hPal);

    GlobalUnlock(hPalCtrlInfo);
    GlobalFree(hPalCtrlInfo);
    SetWindowLong(hwnd, WL_PAL_HPALCTRLINFO, 0);
    
    return 0;
}


//
//  FUNCTION: MsgPalCtrlGetCurSel(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Fills in PALINFO structure with index and RGB values
//
//  PARAMETERS:
//
//    hwnd      - Window handle of PalCtrl
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Pointer to a PALINFO structure
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//

LRESULT MsgPalCtrlGetCurSel(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HANDLE        hPalCtrlInfo;
    LPPALCTRLINFO lpPalCtrlInfo;
    HDC           hdc;
    HPALETTE      hOldPal;
    COLORREF      crColor;

    hPalCtrlInfo = (HANDLE)GetWindowLong(hwnd, WL_PAL_HPALCTRLINFO);
    if (!hPalCtrlInfo)
        return 0;
    
    lpPalCtrlInfo = (LPPALCTRLINFO)GlobalLock(hPalCtrlInfo);

    hdc     = GetDC(hwnd);
    hOldPal = SelectPalette(hdc, lpPalCtrlInfo->hPal, TRUE);
    RealizePalette(hdc);
    crColor = GetNearestColor(hdc, PALETTEINDEX(lpPalCtrlInfo->nEntry));

    // fill in info
    ((LPPALINFO)lparam)->index = lpPalCtrlInfo->nEntry;
    ((LPPALINFO)lparam)->red   = GetRValue(crColor);
    ((LPPALINFO)lparam)->green = GetGValue(crColor);
    ((LPPALINFO)lparam)->blue  = GetBValue(crColor);

    SelectPalette(hdc, hOldPal, TRUE);
    ReleaseDC(hwnd, hdc);
    GlobalUnlock(hPalCtrlInfo);

    return 0;
}


//
//  FUNCTION: MsgPalCtrlGetCurSel(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Sets selected color for the PalCtrl
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Contains desired index to be selected
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//

LRESULT MsgPalCtrlSetCurSel(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HANDLE        hPalCtrlInfo;
    LPPALCTRLINFO lpPalCtrlInfo;
    HDC           hDC;

    hPalCtrlInfo = (HANDLE)GetWindowLong(hwnd, WL_PAL_HPALCTRLINFO);
    if (!hPalCtrlInfo)
        return 0;
    lpPalCtrlInfo = (LPPALCTRLINFO)GlobalLock(hPalCtrlInfo);

    // store selected index
    lpPalCtrlInfo->nEntry = (int)lparam;

    hDC = GetDC(hwnd);

    // Highlight the specified palette square
    HighlightSquare(hDC,
                    lpPalCtrlInfo->hPal,
                    lpPalCtrlInfo->nEntry,
                    lpPalCtrlInfo->cxSquare,
                    lpPalCtrlInfo->cySquare,
                    lpPalCtrlInfo->nCols,
                    lpPalCtrlInfo->nEntries);

    ReleaseDC(hwnd, hDC);
    
    // notify parent so appropriate updating occurs
    PostMessage(GetParent(hwnd),
                WM_COMMAND,
                MAKEWPARAM((WORD)GetWindowLong(hwnd, GWL_ID),
                           PCN_CHANGE),
                (LPARAM)hwnd);

    GlobalUnlock(hPalCtrlInfo);

    return 0;
}


//
//  FUNCTION: HighlightSquare(HDC, HPALETTE, int, int, int, int, int)
//
//  PURPOSE:  Highlight the currently selected palette entry and
//            change the info bar to reflect it.
//
//  PARAMETERS:
//    hDC       - DC where we want to highlight a pal. square.
//    hPal      - Handle to the palette we're displaying info on.
//    nEntry    - Entry to highlight.
//    cxSquare  - Width a a palette square.
//    cySquare  - Height of a palette square.
//    nCols     - # of columns currently displayed in window.
//    nColors   - # of colors in the palette.
//
//  RETURN VALUE:
//    None.
//
//  COMMENTS:
//

void HighlightSquare(HDC hDC, 
                     HPALETTE hPal,
                     int nEntry, 
                     int cxSquare, 
                     int cySquare, 
                     int nCols,
                     int nColors)
{
    RECT         rect;
    HBRUSH       hBrush;
    PALETTEENTRY pe;

    rect.left   = (nEntry % nCols) * cxSquare;
    rect.top    = (nEntry / nCols) * cySquare;
    rect.right  = rect.left + cxSquare;
    rect.bottom = rect.top  + cySquare;
    hBrush      = CreateHatchBrush(HS_BDIAGONAL,
                                   GetSysColor(COLOR_HIGHLIGHT));

    FrameRect(hDC, &rect, hBrush);

    GetPaletteEntries(hPal, nEntry, 1, &pe);

    // If the palette entry we just got is just an index into the
    //  system palette, get it's RGB value.

    if (pe.peFlags == PC_EXPLICIT)
    {
        COLORREF cref;
        HPALETTE hOldPal;

        cref       = PALETTEINDEX((WORD)pe.peRed + (pe.peGreen << 4));
        hOldPal    = SelectPalette(hDC, hPal, FALSE);
        cref       = GetNearestColor(hDC, cref);
        pe.peRed   = (BYTE)  (cref & 0x0000FF);
        pe.peGreen = (BYTE) ((cref & 0x00FF00) >> 8);
        pe.peBlue  = (BYTE) ((cref & 0xFF0000) >> 16);  

        SelectPalette(hDC, hOldPal, FALSE);
    }

    DeleteObject(hBrush);
}

//
//  FUNCTION: UnHighlightSquare(HDC, int, int, int, int)
//
//  PURPOSE:  Un-highlight a palette entry.
//
//  PARAMETERS:
//    hDC       - DC where we want to unhighlight a pal. square.
//    nEntry    - Entry to highlight.
//    cxSquare  - Width a a palette square.
//    cySquare  - Height of a palette square.
//    nCols     - # of columns currently displayed in window.
//
//  RETURN VALUE:
//    None.
//
//  COMMENTS:
//

void UnHighlightSquare(HDC hDC, 
                       int nEntry, 
                       int cxSquare, 
                       int cySquare,
                       int nCols)
{
    RECT rect;

    rect.left   = (nEntry % nCols) * cxSquare;
    rect.top    = (nEntry / nCols) * cySquare;
    rect.right  = rect.left + cxSquare;
    rect.bottom = rect.top  + cySquare;

    FrameRect(hDC, &rect, GetStockObject(BLACK_BRUSH));
}


//
//  FUNCTION: SetPalCtrlPalette(HWND, HPALETTE)
//
//  PURPOSE:  Set a palette control's hPal in its PALCTRLINFO structure.
//            This sets the palette that will be displayed in the
//            given window.  Also sets up the number of color entries
//            field in the PALCTRLINFO structure.
//
//  PARAMETERS:
//
//    hWnd      - Window we're going to display palette in.
//    hPal      - Palette to display in the window.
//
//  RETURN VALUE:
//
//    None.
//
//  COMMENTS:
//

void SetPalCtrlPalette(HWND hWnd, HPALETTE hPal)
{
    HANDLE    hPalCtrlInfo;
    LPPALCTRLINFO lpPalCtrlInfo;

    if (!hPal)
        return;

    hPalCtrlInfo = (HANDLE)GetWindowLong(hWnd, WL_PAL_HPALCTRLINFO);

    if (hPalCtrlInfo)
    {
        lpPalCtrlInfo           = (LPPALCTRLINFO)GlobalLock(hPalCtrlInfo);
        lpPalCtrlInfo->hPal     = hPal;
        lpPalCtrlInfo->nEntries = ColorsInPalette(hPal);

        GlobalUnlock(hPalCtrlInfo);
    }
}
