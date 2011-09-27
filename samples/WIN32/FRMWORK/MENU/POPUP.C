// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: popup.c
//
//  PURPOSE: Example of a popup menu.
//
//  FUNCTIONS:
//    MsgButtonUp - Display and track the popup menu on button click.
//    CmdFont - Displays a message box for font menu items.
//    CmdSize - Displays a message box for size menu items.
//    CmdStyle - Displays a message box for style menu items.
//
//  COMMENTS:
//


#include <windows.h>            // required for all Windows applications
#include "globals.h"            // prototypes specific to this application

// Porting macro
#define LONG2POINT(l, pt) ((pt).x = (SHORT)LOWORD(l), (pt).y = (SHORT)HIWORD(l))


//
//  FUNCTION: MsgButtonUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display and track the popup menu on button click.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_LBUTTONUP   (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Coordinates where the mouse was pressed
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgButtonUp(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    RECT  rc;
    POINT pt;

    // Draw the "floating" popup in the app's client area
    GetClientRect(hwnd, &rc);

    // Temporary porting macro
    LONG2POINT(lparam, pt);
    if (PtInRect(&rc, pt))
    {
        HMENU hmenu;
        HMENU hmenuTrackPopup;

        // Get the menu for the popup from the resource file.
        hmenu = LoadMenu(hInst, "PopupMenu");
        if (!hmenu)
           return 0;

        // Get the first menu in it which we will use for the call to
        // TrackPopup(). This could also have been created on the fly using
        // CreatePopupMenu and then we could have used InsertMenu() or
        // AppendMenu.
        hmenuTrackPopup = GetSubMenu(hmenu, 0);

        // Convert the mouse point to screen coordinates since that is what
        // TrackPopup expects.
        ClientToScreen(hwnd, &pt);

        // Draw and track the "floating" popup
        TrackPopupMenuEx(hmenuTrackPopup,
                         TPM_LEFTALIGN | TPM_TOPALIGN |     // default values
                         TPM_LEFTBUTTON | TPM_HORIZONTAL |  // equivalent to 0
                         TPM_RIGHTBUTTON,                   // Right selection
                         pt.x, pt.y,
                         hwnd,
                         NULL);

        // Destroy the menu since were are done with it.
        DestroyMenu(hmenu);
    }
    return 0;
}

//
//  FUNCTION: CmdFont(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays a message box for font menu items.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_FONT            (Unused)
//    wNotify   - Notification number (Unused)
//    hwndCtrl  - NULL.               (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT CmdFont(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    MessageBox(hwnd,
               "A font was selected",
               "Popup Menu Alert",
               MB_APPLMODAL | MB_OK);

    return 0;
}

//
//  FUNCTION: CmdSize(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays a message box for size menu items.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_SIZE            (Unused)
//    wNotify   - Notification number (Unused)
//    hwndCtrl  - NULL.               (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT CmdSize(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    MessageBox(hwnd,
               "A size was selected",
               "Popup Menu Alert",
               MB_APPLMODAL | MB_OK);

    return 0;
}


//
//  FUNCTION: CmdStyle(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays a message box for style menu items.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_STYLE           (Unused)
//    wNotify   - Notification number (Unused)
//    hwndCtrl  - NULL.               (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT CmdStyle(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    MessageBox(hwnd,
               "A style was selected",
               "Popup Menu Alert",
               MB_APPLMODAL | MB_OK);

    return 0;
}
