// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
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

// Global Variables for the status and toolbar controls.

HWND    hWndToolbar;


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
    TBBUTTON tbButton[] =       // Array defining the toolbar buttons
    {
    {0, IDM_WNDINF,     TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, 0},
    {1, IDM_MESSAGES,   TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, 0},
    {2, IDM_WNDMSG,     TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, 0},
    {0, 0,              TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
    {3, IDM_CLEARWND,   TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {0, 0,              TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
    {4, IDM_WINDOW,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {5, IDM_CAPON,      TBSTATE_ENABLED, 
                        TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {6, IDM_CAPOFF,     TBSTATE_ENABLED,
                        TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {0, 0,              TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
    {7, IDM_PPTIES,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {8, IDM_FONT,       TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {9, IDM_ABOUT,      TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0}
};

    hWndToolbar = CreateToolbarEx(hwnd,
                  WS_CHILD |
                  WS_VISIBLE |
                  TBSTYLE_TOOLTIPS |
                  CCS_ADJUSTABLE,
                  IDM_TOOLBAR,
                  10,        // # of images in bitmap
                  hInst,
                  IDB_BMP,
                  tbButton,
                  sizeof(tbButton)/sizeof(TBBUTTON),
                  0,        // default button width
                  0,        // default button height
                  18,       // width of image in bitmap
                  18,       // height of bitmap
                  sizeof(TBBUTTON));

    if (!hWndToolbar)
    return FALSE;

    return TRUE;
}

//
//  FUNCTION: MsgNotifyToolbar(HWND, UINT, WPARAM, LPARAM)
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

LRESULT MsgNotifyToolbar(HWND hwnd, 
                         UINT uMessage, 
                         WPARAM wparam, 
                         LPARAM lparam)
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
