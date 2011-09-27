// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   split.c
//
//  PURPOSE:  Contains functions to browse windows on the system. 
//
//  FUNCTIONS:
//    CmdMessages          - Handles the WM_COMMAND, IDM_MESSAGES message. 
//    CmdWindowInformation - Handles the WM_COMMAND, IDM_WNDINF message. 
//    CmdWndMsgInformation - Handles the WM_COMMAND, IDM_WNDMSG message. 
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <commctrl.h>           // prototypes and defs for common controls
#include "dllskel.h"            // prototypes for DLL functions
#include "globals.h"            // prototypes specific to this application
#include "hooks.h"

//
// Global Variables for this module
//
extern HWND ghWndTreeView, hWndHeaderCtl, hWndLBoxCtl, hWndToolbar;
extern int gToolbarHeight, gStatusbarHeight, gHeaderCtlHeight;
BOOL   isWindowSplit = FALSE;

//  FUNCTION: CmdWndInformation(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays message information only.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_WNDINF          (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL                (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdWindowInformation(HWND hwnd, 
                             WORD wCommand, 
                             WORD wNotify, 
                             HWND hwndCtrl)
{
    RECT rect;


    // Since this command is generated from either a menu selection
    // or a toolbar button press, we want to make sure we indicate
    // the user's choice in both places: by checking the appropriate
    // menu item, and by pressing the appropriate toolbar button.

    CheckMenuItem (GetMenu (hwnd), IDM_WNDINF,   MF_BYCOMMAND|MF_CHECKED);
    CheckMenuItem (GetMenu (hwnd), IDM_MESSAGES, MF_BYCOMMAND|MF_UNCHECKED);
    CheckMenuItem (GetMenu (hwnd), IDM_WNDMSG,   MF_BYCOMMAND|MF_UNCHECKED);

    SendMessage (hWndToolbar, 
                 TB_CHECKBUTTON, 
                 (WPARAM)IDM_WNDINF, 
                 (LPARAM)MAKELONG (TRUE, 0));


    GetClientRect(hwnd, &rect);
    ShowWindow(hWndHeaderCtl, SW_HIDE);
    ShowWindow(hWndLBoxCtl, SW_HIDE);
    MoveWindow(ghWndTreeView, 
               0,
               gToolbarHeight,
               rect.right,
               rect.bottom - gToolbarHeight - gStatusbarHeight,
               TRUE);        
    isWindowSplit = FALSE;
    ShowWindow(ghWndTreeView, SW_SHOWNORMAL);
    return 0;

}

//  FUNCTION: CmdWndMsgInformation(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays message and Window information only.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_            (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL                (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdWndMsgInformation(HWND hwnd, 
                             WORD wCommand, 
                             WORD wNotify, 
                             HWND hwndCtrl)
{
    RECT rect;

    // Since this command is generated from either a menu selection
    // or a toolbar button press, we want to make sure we indicate
    // the user's choice in both places: by checking the appropriate
    // menu item, and by pressing the appropriate toolbar button.

    CheckMenuItem (GetMenu (hwnd), IDM_WNDMSG,   MF_BYCOMMAND|MF_CHECKED);
    CheckMenuItem (GetMenu (hwnd), IDM_WNDINF,   MF_BYCOMMAND|MF_UNCHECKED);
    CheckMenuItem (GetMenu (hwnd), IDM_MESSAGES, MF_BYCOMMAND|MF_UNCHECKED);

    SendMessage (hWndToolbar, 
                 TB_CHECKBUTTON, 
                 (WPARAM)IDM_WNDMSG, 
                 (LPARAM)MAKELONG (TRUE, 0));


    GetClientRect(hwnd, &rect); 

    isWindowSplit = TRUE;
    
    MoveWindow(ghWndTreeView, 
               0, 
               gToolbarHeight,
               (rect.right / 2) - 2,
               rect.bottom - gToolbarHeight - gStatusbarHeight,
               TRUE);
    MoveWindow(hWndHeaderCtl, 
               rect.right / 2 + 2, 
               gToolbarHeight,
               rect.right,
               gHeaderCtlHeight,
               TRUE);
    MoveWindow(hWndLBoxCtl, 
               rect.right / 2 + 2, 
               gToolbarHeight + gHeaderCtlHeight + 1,
               rect.right / 2,
               rect.bottom - 
               (gHeaderCtlHeight +1) - 
               gToolbarHeight -
               gStatusbarHeight,
               TRUE);

    ShowWindow(ghWndTreeView, SW_SHOWNORMAL); 
    ShowWindow(hWndHeaderCtl, SW_SHOWNORMAL); 
    ShowWindow(hWndLBoxCtl, SW_SHOWNORMAL); 

    return 0;

}

//  FUNCTION: CmdMessages(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays message information only.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_MESSAGES            (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL                (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdMessages(HWND hwnd, 
                    WORD wCommand, 
                    WORD wNotify, 
                    HWND hwndCtrl)
{
    RECT rect;

    // Since this command is generated from either a menu selection
    // or a toolbar button press, we want to make sure we indicate
    // the user's choice in both places: by checking the appropriate
    // menu item, and by pressing the appropriate toolbar button.

    CheckMenuItem (GetMenu (hwnd), IDM_MESSAGES, MF_BYCOMMAND|MF_CHECKED);
    CheckMenuItem (GetMenu (hwnd), IDM_WNDMSG,   MF_BYCOMMAND|MF_UNCHECKED);
    CheckMenuItem (GetMenu (hwnd), IDM_WNDINF,   MF_BYCOMMAND|MF_UNCHECKED);

    SendMessage (hWndToolbar, 
                 TB_CHECKBUTTON, 
                 (WPARAM)IDM_MESSAGES, 
                 (LPARAM)MAKELONG (TRUE, 0));


    GetClientRect(hwnd, &rect); 
    isWindowSplit = FALSE;


    MoveWindow (hWndHeaderCtl, 
                0,
                gToolbarHeight,
                rect.right,
                gHeaderCtlHeight,
                TRUE);        
 
    MoveWindow(hWndLBoxCtl, 
               0,
               gHeaderCtlHeight + 1 + gToolbarHeight,
               rect.right,
               rect.bottom - 
               gHeaderCtlHeight - 
               gToolbarHeight -
               gStatusbarHeight,
               TRUE);        


    ShowWindow (hWndHeaderCtl, SW_SHOWNORMAL);
    ShowWindow (hWndLBoxCtl, SW_SHOWNORMAL);
    ShowWindow (ghWndTreeView, SW_HIDE);
 
    return 0;

}


