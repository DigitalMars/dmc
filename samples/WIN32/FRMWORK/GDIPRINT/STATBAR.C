// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: statbar.c
//
//  PURPOSE: Handles general routines for the Barsdi sample.
//
//  FUNCTIONS:
//    MsgTimer      - Handles the WM_TIMER messages to set the time on
//                    the status bar.
//    MsgMouseMove  - Moved to CLIENT.C
//    MsgMenuSelect - Handle the WM_MENUSELECT message. This message will
//                    enable the status bar control to update when the user
//                    moves across menu items on the main window.
//    InitializeStatusBar - Sets the pane positions in the statusbar control
//    CreateSBar    - Calls CreateStatusWindow() to create the status bar
//    UpdateStatusBar - Updates the statusbar control with appropriate text
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <commctrl.h>           // prototypes and defs for common controls
#include "globals.h"            // prototypes specific to this application
#include "statbar.h"            // prototypes specific to statbar.c
#include "resource.h"
            
// Global Variables for the status bar control.

HWND  hWndStatusbar;

//  **TODO**  Add entries to the string table in barsdi.rc for each menu
//            command.  MsgMenuSelect (below) loads these strings to display
//            information in the status bar.  MsgMenuSelect assumes that the
//            string ID is the same as the command ID and that a string
//            exists for every command.
//
// The following array contains resource string ID's for popup menus
// in the main application menu.  This array is used by MsgMenuSelect
// to display information in the status bar.
//
//  **TODO**  Add entries to this array for each popup menu in the same
//            positions as they appear in the main menu.  Remember to define
//            the ID's in globals.h and add the strings to barsdi.rc.

UINT idPopup[] =
{
    IDM_FILEMENU,    
    IDM_INFO,
//    IDM_EDITMENU,
    IDM_DRAWMENU,
    IDM_HELPMENU,
};


//
//  FUNCTION: MsgTimer(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls GetLocalTime() to set the time on the status bar
//
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
//    Every time the window procedure receives a Timer message, it calls
//    GetLocalTime() to obtain the time and then formats the time into
//    a string. The time sting is then displayed on the status bar.
//

LRESULT MsgTimer(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    char        szBuf[16];      // Temp buffer.
    SYSTEMTIME  sysTime;


    GetLocalTime(&sysTime);

    wsprintf(szBuf,
             "%2d:%02d:%02d %s",
             (sysTime.wHour == 0 ? 12 :
             (sysTime.wHour <= 12 ? sysTime.wHour : sysTime.wHour -12)),
             sysTime.wMinute,
             sysTime.wSecond,
             (sysTime.wHour < 12 ? "AM":"PM"));

    UpdateStatusBar(szBuf, 4, 0);
    return 0;
}


//
//  FUNCTION: MsgMenuSelect(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Upadates menu selections on the staus bar.
//
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Used)
//    uMessage  - Message number (Used)
//    wparam    - Extra data     (Used)
//    lparam    - Extra data     (Used)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    This message is sent when the user selects menu items by
//    by pulling down  a popup menu move the mouse around to highlite
//    different menu items.
//
//

LRESULT MsgMenuSelect(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    static char szBuffer[128];
    UINT   nStringID = 0;
    UINT   fuFlags = GET_WM_MENUSELECT_FLAGS(wparam, lparam) & 0xffff;
    UINT   uCmd    = GET_WM_MENUSELECT_CMD(wparam, lparam);
    HMENU  hMenu   = GET_WM_MENUSELECT_HMENU(wparam, lparam);


    szBuffer[0] = 0;                            // First reset the buffer


    if (fuFlags == 0xffff && hMenu == NULL)     // Menu has been closed
        nStringID = IDS_DESCRIPTION;

    else if (fuFlags & MFT_SEPARATOR)           // Ignore separators
        nStringID = 0;

    else if (fuFlags & MF_POPUP)                // Popup menu
    {
        if (fuFlags & MF_SYSMENU)               // System menu
            nStringID = IDS_SYSMENU;

        else
            // Get string ID for popup menu from idPopup array.
            nStringID = ((uCmd < sizeof(idPopup)/sizeof(idPopup[0])) ?
                            idPopup[uCmd] : 0);
    }  // for MF_POPUP

    else                                        // Must be a command item
        nStringID = uCmd;                       // String ID == Command ID

    // Load the string if we have an ID
    if (0 != nStringID)
        LoadString(hInst, nStringID, szBuffer, sizeof(szBuffer));

    // Finally... send the string to the status bar
    UpdateStatusBar(szBuffer, 0, 0);

    return 0;
}


//
//  FUNCTION: InitializeStatusBar(HWND)
//
//  PURPOSE:  Initialize statusbar control with time and mouse positions.
//
//
//  PARAMETERS:
//
//  hwndParent - Window handle of the status bar's parent
//
//
//  RETURN VALUE:  NONE
//
//
//  COMMENTS:
//
//   This function initializes the time  and mouse positions sections of
//   the statubar window. The Date for the time section is obtained by
//   calling SetTimer API. When the timer messages start comming in,
//   GetSytemTime() to fill the time section.
//   The WPARAM of SB_SETTEXT is divided into 2 parameters. The LOWORD
//   determines which section/part the text goes into, and the HIWORD
//   tells how the bar is drawn (popin or popout).
//

void InitializeStatusBar(HWND hwndParent)
{
    const cSpaceInBetween = 8;
    int   ptArray[5];   // Array defining the number of parts/sections
    SIZE  size;         // the Status bar will display.
    RECT  rect;
    HDC   hDC;

   /*
    * Fill in the ptArray...
    */

    hDC = GetDC(hwndParent);
    GetClientRect(hwndParent, &rect);

    ptArray[4] = rect.right;

    if (GetTextExtentPoint(hDC, "00:00:00 PM", 12, &size))
        ptArray[3] = ptArray[4] - (size.cx) - cSpaceInBetween;
    else
        ptArray[3] = 0;

    if (GetTextExtentPoint(hDC, "Time:", 6, &size))
        ptArray[2] = ptArray[3] - (size.cx) - cSpaceInBetween;
    else
        ptArray[2] = 0;

    if (GetTextExtentPoint(hDC, "123,123", 8, &size))
        ptArray[1] = ptArray[2] - (size.cx) - cSpaceInBetween;
    else
        ptArray[1] = 0;

    if (GetTextExtentPoint(hDC, "Cursor Pos:", 12, &size))
        ptArray[0] = ptArray[1] - (size.cx) - cSpaceInBetween;
    else
        ptArray[0] = 0;

    ReleaseDC(hwndParent, hDC);

    SendMessage(hWndStatusbar,
                SB_SETPARTS,
                sizeof(ptArray)/sizeof(ptArray[0]),
                (LPARAM)(LPINT)ptArray);

    UpdateStatusBar(SZDESCRIPTION, 0, 0);
    UpdateStatusBar("Cursor Pos:", 1, SBT_POPOUT);
    UpdateStatusBar("Time:", 3, SBT_POPOUT);
}


//
//  FUNCTION: CreateSBar(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Calls CreateStatusWindow() to create the status bar
//
//
//  PARAMETERS:
//
//  hwndParent - Window handle of the status bar's parent
//
//  RETURN VALUE:
//
//  If both controls were created successfully Return TRUE,
//  else returns FALSE.
//
//  COMMENTS:
//
//

BOOL CreateSBar(HWND hwndParent)
{
    hWndStatusbar = CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_BORDER,
                                       SZDESCRIPTION,
                                       hwndParent,
                                       IDM_STATUSBAR);
    if(hWndStatusbar)
    {
        InitializeStatusBar(hwndParent);
        SetTimer(hwndParent, IDM_TIMER, TIMER_TIMEOUT, NULL);
        return TRUE;
    }

    return FALSE;
}


//
//  FUNCTION: UpdateStatusBar(HWND)
//
//  PURPOSE:  Updates the statusbar control with appropriate text
//
//
//  PARAMETERS:
//
//  lpszStatusString - text to be displayed
//  partNumber       - which part of the status bar to display text in
//  displayFlags     - display flags
//
//
//  RETURN VALUE: NONE
//
//
//  COMMENTS:
//      None
//
//

void UpdateStatusBar(LPSTR lpszStatusString, WORD partNumber, WORD displayFlags)
{
    SendMessage(hWndStatusbar,
                SB_SETTEXT,
                partNumber | displayFlags,
                (LPARAM)lpszStatusString);
}
