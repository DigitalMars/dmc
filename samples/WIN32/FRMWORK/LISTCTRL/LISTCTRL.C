// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   listctrl.c
//
//  PURPOSE:   Implement the windows procedure for the main application
//    windows.  Also implement the generic message and command dispatchers.
//
//  FUNCTIONS:
//    WndProc      - Processes messages for the main window.
//    MsgCreate    - Handle the WM_CREATE message 
//    MsgCommand   - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy   - Handles the WM_DESTROY message by calling 
//                   PostQuitMessage().
//    CmdLargeIcons- Switch main view to large icons
//    CmdSmallIcons- Switch main view to small icons
//    CmdList      - Switch main view to list view
//    CmdDetails   - Switch main view to details view
//    CmdExit      - Handles the file exit command by calling destory 
//                   window on the main window.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <commctrl.h>
#include "globals.h"            // prototypes specific to this application
#include "listview.h"
#include "resource.h"

// Main window message table definition.
MSD rgmsd[] =
{
    {WM_CREATE,         MsgCreate        },
    {WM_COMMAND,        MsgCommand       },
    {WM_SIZE,           MsgSize          },
    {WM_NOTIFY,         MsgNotify        },
    {WM_SYSCOLORCHANGE, MsgSysColorChange},
    {WM_DESTROY,        MsgDestroy       }
};


MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpWindow
};


// Main window command table definition.
CMD rgcmd[] =
{
    {IDM_EXIT,       CmdExit},
    {IDM_LARGEICONS, CmdLargeIcons},
    {IDM_SMALLICONS, CmdSmallIcons},
    {IDM_LIST,       CmdList},
    {IDM_DETAILS,    CmdDetails},
    {IDM_ABOUT,      CmdAbout}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
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
//    information (msdiMain) and the message specific information.
//

LRESULT CALLBACK WndProc(HWND   hwnd, 
                         UINT   uMessage, 
                         WPARAM wparam, 
                         LPARAM lparam)
{
    return DispMessage(&msdiMain, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_COMMAND (Unused)
//    GET_WM_COMMAND_ID(wparam, lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam, lparam) - Control handle
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispCommand() function with the main window's command dispatch
//    information (cmdiMain) and the command specific information.
//

LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiMain, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_CREATE messages for the main window.
//           Calls CreateListView() to create the listview for the main window
//
//  PARAMETERS:
//    hwnd     - window handle
//
//  RETURN VALUE:
//    Always return 0;
//    
//
//  COMMENTS:
//    
//    
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HWND hwndLV;

    if (NULL == (hwndLV = CreateListView(hwnd)))
        MessageBox(hwnd,
                   "Failed to create listview window.", 
                   SZAPPNAME, 
                   MB_OK);
   
    return 0;    
}



//
//  FUNCTION: MsgSize(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_SIZE messages for the main window.
//           
//
//  PARAMETERS:
//    hwnd     - window handle
//
//  RETURN VALUE:
//    Always return 0;
//    
//
//  COMMENTS:
//    
//    
//

LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    MoveWindow(GetDlgItem(hwnd, IDD_LISTVIEW),
               0, 0,
               LOWORD(lparam),
               HIWORD(lparam),
               FALSE);
    return 0;    
}


//
//  FUNCTION: MsgSysColorChange(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Passed the WM_SYSCOLORCHANGE message to the listview control
//
//  PARAMETERS:
//
//    hwnd      - Window handle  
//    uMessage  - Message number 
//    wparam    - Extra data     
//    lparam    - Extra data     
//
//  RETURN VALUE:
//
//    Call DefWindowProc() and return value from DWP
//
//  COMMENTS:
//
//

LRESULT MsgSysColorChange(HWND hwnd,
                          UINT uMessage,
                          WPARAM wparam,
                          LPARAM lparam)
{
    PostMessage(GetDlgItem(hwnd, IDD_LISTVIEW), 
                WM_SYSCOLORCHANGE, 
                wparam, 
                lparam);
    return DefWindowProc(hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)
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
//

LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PostQuitMessage(0);
    return 0;
}


//
//  FUNCTION: CmdLargeIcons(HWND, WORD, WORD, HWND)
//
//  PURPOSE: 
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_LARGEICONS (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//   
//

LRESULT CmdLargeIcons(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SwitchView(GetDlgItem(hwnd, IDD_LISTVIEW), LVS_ICON);
    return 0;
}


//
//  FUNCTION: CmdSmallIcons(HWND, WORD, WORD, HWND)
//
//  PURPOSE: 
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_SMALLICONS (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//   
//

LRESULT CmdSmallIcons(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SwitchView(GetDlgItem(hwnd, IDD_LISTVIEW), LVS_SMALLICON);
    return 0;
}


//
//  FUNCTION: CmdList(HWND, WORD, WORD, HWND)
//
//  PURPOSE: 
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_LIST (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//   
//

LRESULT CmdList(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SwitchView(GetDlgItem(hwnd, IDD_LISTVIEW), LVS_LIST);
    return 0;
}


//
//  FUNCTION: CmdDetails(HWND, WORD, WORD, HWND)
//
//  PURPOSE: 
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_DETAILS (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//   
//

LRESULT CmdDetails(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SwitchView(GetDlgItem(hwnd, IDD_LISTVIEW), LVS_REPORT);
    return 0;
}


//
//  FUNCTION: CmdExit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EXIT (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DestroyWindow(hwnd);
    return 0;
}


