// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: toolbar.c
//
//  PURPOSE: Handles general routines for the Toolbar control
//
//  FUNCTIONS:
//    CreateTBar    - Creates the Toolbar control for the sample.
//    MsgNotify     - Handles the WM_NOTIFY message that gets sent to
//                    the parent window to get ToolTip Text.
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <commctrl.h>           // prototypes and defs for common controls
#include "globals.h"            // prototypes specific to this application
#include "toolbar.h"            // prototypes and #defines for toolbar.c
#include "resource.h"

// Global Variable for the toolbar control.

HWND    hWndToolbar;

//  **TODO**  Change the following values to match your toolbar bitmap
//
// NUMIMAGES    = Number of images in toolbar.bmp.  Note that this is not
//                the same as the number of elements on the toolbar.
// IMAGEWIDTH   = Width of a single button image in toolbar.bmp
// IMAGEHEIGHT  = Height of a single button image in toolbar.bmp
// BUTTONWIDTH  = Width of a button on the toolbar (zero = default)
// BUTTONHEIGHT = Height of a button on the toolbar (zero = default)

#define NUMIMAGES       18
#define IMAGEWIDTH      18
#define IMAGEHEIGHT     17
#define BUTTONWIDTH     0
#define BUTTONHEIGHT    0

//  **TODO**  Add/remove entries in the following array to define the 
//            toolbar buttons (see documentation for TBBUTTON).

TBBUTTON tbButton[] =
{
    { 0, IDM_FILENEW,     TBSTATE_ENABLED,       TBSTYLE_BUTTON,     0, 0},
    { 1, IDM_FILEOPEN,    TBSTATE_ENABLED,       TBSTYLE_BUTTON,     0, 0},
    { 2, IDM_FILESAVE,    TBSTATE_ENABLED,       TBSTYLE_BUTTON,     0, 0},
    { 6, IDM_FILEPRINT,   TBSTATE_ENABLED,       TBSTYLE_BUTTON,     0, 0},
    { 0, 0,               TBSTATE_ENABLED,       TBSTYLE_SEP,        0, 0},
//    { 3, IDM_EDITCUT,     TBSTATE_ENABLED,       TBSTYLE_BUTTON,     0, 0},
//    { 4, IDM_EDITCOPY,    TBSTATE_ENABLED,       TBSTYLE_BUTTON,     0, 0},
//    { 5, IDM_EDITPASTE,   TBSTATE_ENABLED,       TBSTYLE_BUTTON,     0, 0},
//    { 0, 0,               TBSTATE_ENABLED,       TBSTYLE_SEP,        0, 0},
    { 9, IDM_PIXEL,       TBSTATE_ENABLED,       TBSTYLE_CHECKGROUP, 0, 0},
    {10, IDM_LINE,        TBSTATE_ENABLED,       TBSTYLE_CHECKGROUP, 0, 0},
    {11, IDM_RECT,        TBSTATE_ENABLED,       TBSTYLE_CHECKGROUP, 0, 0},
    {12, IDM_ELLIPSE,     TBSTATE_ENABLED,       TBSTYLE_CHECKGROUP, 0, 0},
    {13, IDM_BEZIER,      TBSTATE_ENABLED,       TBSTYLE_CHECKGROUP, 0, 0},
    { 0, 0,               TBSTATE_ENABLED,       TBSTYLE_SEP,        0, 0},
    {14, IDM_FILL,        TBSTATE_ENABLED,       TBSTYLE_CHECKGROUP, 0, 0},
    {15, IDM_NOFILL,      TBSTATE_ENABLED,       TBSTYLE_CHECKGROUP, 0, 0},
    { 0, 0,               TBSTATE_ENABLED,       TBSTYLE_SEP,        0, 0},
    {16, IDM_CREATEPEN,   TBSTATE_ENABLED,       TBSTYLE_BUTTON,     0, 0},
    {17, IDM_CREATEBRUSH, TBSTATE_ENABLED,       TBSTYLE_BUTTON,     0, 0},
    { 0, 0,               TBSTATE_ENABLED,       TBSTYLE_SEP,        0, 0},
    { 7, IDM_ABOUT,       TBSTATE_ENABLED,       TBSTYLE_BUTTON,     0, 0},
};


//
//  FUNCTION: CreateTBar(HWND)
//
//  PURPOSE:  Calls CreateToolBarEx()
//
//
//  PARAMETERS:
//
//  hwnd - Window handle : Used for the hWndParent parameter of the control.
//
//  RETURN VALUE:
//
//  If toolbar control was created successfully Return TRUE,
//  else returns FALSE.
//
//  COMMENTS:
//
//

BOOL CreateTBar(HWND hwnd)
{
    hWndToolbar = CreateToolbarEx(hwnd,
                                  WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS, 
                                  IDM_TOOLBAR,
                                  NUMIMAGES,
                                  hInst,
                                  IDB_BMP,
                                  tbButton,
                                  sizeof(tbButton)/sizeof(TBBUTTON),
                                  BUTTONWIDTH,
                                  BUTTONHEIGHT,
                                  IMAGEWIDTH,
                                  IMAGEHEIGHT,
                                  sizeof(TBBUTTON));

    return (hWndToolbar != NULL);
}

//
//  FUNCTION: MsgNotify(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  WM_NOTIFY is sent to the parent window to get the
//            tooltip text assoc'd with that toolbar button.
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - TOOLTIPTEXT FAR*
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//
//  COMMENTS:
//    This message fills in the lpszText field of the TOOLTIPTEXT
//    structure if code == TTN_NEEDTEXT
//

LRESULT MsgNotify(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    LPTOOLTIPTEXT lpToolTipText;
    static char   szBuffer[64];

    lpToolTipText = (LPTOOLTIPTEXT)lparam;
    if (lpToolTipText->hdr.code == TTN_NEEDTEXT)
    {
        LoadString(hInst,
                   lpToolTipText->hdr.idFrom,   // string ID == command ID
                   szBuffer,
                   sizeof(szBuffer));

        lpToolTipText->lpszText = szBuffer;
    }
    return 0;
}
