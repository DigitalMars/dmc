// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   blandmdi.c
//
//  PURPOSE:  Implement the window procedure for the main application
//            window.
//
//  FUNCTIONS:
//    WndProc           - Processes messages for the main window.
//    MsgCreate         - To create the MDI client window.
//    MsgCommand        - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy        - Handles the WM_DESTROY message by calling 
//                        PostQuitMessage().
//    CmdFileNew        - To create a new mdi child window.
//    CmdWindowTile     - To tile the mdi child windows.
//    CmdWindowCascade  - To cascade the mdi child windows.
//    CmdWindowIcons    - To arrage the mdi child icons.
//    CmdWindowCloseAll - To close all of the mdi child windows.
//    CmdExit           - Handles the file exit command by calling destory 
//                        window on the main window.
//
//  COMMENTS:
//    Message dispatch table -
//      For every message to be handled by the main window procedure
//      place the message number and handler function pointer in
//      rgmsd (the message dispatch table).  Place the prototype
//      for the function in globals.h and the definition of the
//      function in the appropriate module.
//    Command dispatch table -
//      For every command to be handled by the main window procedure
//      place the command number and handler function pointer in
//      rgcmd (the command dispatch table).  Place the prototype
//      for the function in globals.h and the definition of the
//      function in the appropriate module.
//    Globals.h Contains the definitions of the structures and dispatch.c
//      contains the functions that use these structures.
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

// Main window message table definition.
MSD rgmsd[] =
{
    {WM_INITMENU,   MsgInitMenu},
    {WM_COMMAND,    MsgCommand},
    {WM_CREATE,     MsgCreate},
    {WM_DESTROY,    MsgDestroy}
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpMDIFrame
};


// Main window command table definition.
CMD rgcmd[] =
{
    {IDM_FILENEW,       CmdFileNew},
    {IDM_FILECLOSE,     CmdFileClose},
    {IDM_EXIT,          CmdExit},
    {IDM_ABOUT,         CmdAbout},
    {IDM_WINDOWTILE,    CmdWindowTile},
    {IDM_WINDOWCASCADE, CmdWindowCascade},
    {IDM_WINDOWICONS,   CmdWindowIcons},
    {IDM_WINDOWCLOSEALL,CmdWindowCloseAll}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpMDIFrame
};

// Module specific globals

HWND hwndMDIClient = NULL;      // The MDI client window handle.
UINT cUntitled = 1;
UINT cOpen = 0;
extern char szChildName[];


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
//  PURPOSE: To create an MDI client window.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - WM_CREATE (unused)
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    CLIENTCREATESTRUCT ccs = {0};

    // Find window menu where children will be listed
    ccs.hWindowMenu  = GetSubMenu(GetMenu(hwnd), WINDOWMENU);
    ccs.idFirstChild = IDM_WINDOWCHILD;

    // Create the MDI client filling the client area
    hwndMDIClient = CreateWindow("mdiclient",
                                 NULL,
                                 WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL |
                                 WS_HSCROLL,
                                 0, 0, 0, 0,
                                 hwnd,
                                 (HMENU)0xCAC,
                                 hInst,
                                 (LPVOID)&ccs);

    ShowWindow(hwndMDIClient, SW_SHOW);

    return 0;
}


//
//  FUNCTION: MsgInitMenu(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Enable/Disable the close menu command.
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - HMENU - The menu about to be activated
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    0 - The message was handled.
//    1 - The message was not handled - wrong menu.
//
//  COMMENTS:
//
//

LRESULT MsgInitMenu(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    if (GetMenu(hwnd) == (HMENU)wparam)
    {
        UINT mf = (cOpen) ? MF_ENABLED : MF_GRAYED;

        EnableMenuItem((HMENU)wparam, IDM_FILECLOSE, mf);

        return 0;
    }
    else
    {
        return 1;
    }
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
//  FUNCTION: CmdFileNew(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To create a new mdi child window.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - IDM_FILENEW (unused)
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdFileNew(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    HWND hwndChild;
    char rgch[15];
    DWORD dwVersion;

    wsprintf(rgch,"Untitled%d", cUntitled);

    // Create the MDI child window
        
    // Windows NT and Windows 95 present different options for creating
    // an MDI child window.  While using the WM_MDICREATE message will
    // work on both Windows versions, Windows 95 presents a new window
    // style which simplifies the process.  Here the function uses the
    // method apropriate for the system it's running on.
    dwVersion = GetVersion();
    if ((dwVersion < 0x80000000) || (LOBYTE(LOWORD(dwVersion)) < 4))
    {
        // This is Windows NT or Win32s, so use the WM_MDICREATE message
        MDICREATESTRUCT mcs;

        mcs.szClass = szChildName;      // window class name
        mcs.szTitle = rgch;             // window title
        mcs.hOwner  = hInst;            // owner
        mcs.x       = CW_USEDEFAULT;    // x position
        mcs.y       = CW_USEDEFAULT;    // y position
        mcs.cx      = CW_USEDEFAULT;    // width
        mcs.cy      = CW_USEDEFAULT;    // height
        mcs.style   = 0;                // window style
        mcs.lParam  = 0;                // lparam

        hwndChild = (HWND) SendMessage(hwndMDIClient, 
                                       WM_MDICREATE, 
                                       0,
                                       (LPARAM)(LPMDICREATESTRUCT) &mcs);
    }
    else
    {
        hwndChild = CreateWindowEx(WS_EX_MDICHILD,  // EX window style
                                   szChildName,     // window class name
                                   rgch,            // window title
                                   0,               // window style
                                   CW_USEDEFAULT,   // x position
                                   CW_USEDEFAULT,   // y position
                                   CW_USEDEFAULT,   // width
                                   CW_USEDEFAULT,   // height
                                   hwndMDIClient,   // parent
                                   NULL,            // menu (child ID)
                                   hInst,           // owner
                                   0);              // lparam
    }

    if (hwndChild != NULL)
        ShowWindow(hwndChild, SW_SHOW);

    cUntitled++;
    cOpen++;
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


//
//  FUNCTION: CmdFileClose(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To close all of the mdi child windows.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - IDM_FILECLOSE (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdFileClose(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    HWND hwndT;

    hwndT = (HWND)SendMessage(hwndMDIClient, WM_MDIGETACTIVE, 0, 0L);
    if (hwndT != NULL)
        SendMessage(hwndMDIClient, WM_MDIDESTROY, (WPARAM)hwndT, 0L);

    return 0;
}


//
//  FUNCTION: CmdWindowTile(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To tile the mdi child windows.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - IDM_WINDOWTILE (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdWindowTile(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SendMessage(hwndMDIClient, WM_MDITILE, 0, 0L);

    return 0;
}

//
//  FUNCTION: CmdWindowCascade(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To cascade the mdi child windows.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - IDM_WINDOWCASCADE (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdWindowCascade(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SendMessage(hwndMDIClient, WM_MDICASCADE, 0, 0L);

    return 0;
}

//
//  FUNCTION: CmdWindowIcons(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To arrage the mdi child icons.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - IDM_WINDOWICONS (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdWindowIcons(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SendMessage(hwndMDIClient, WM_MDIICONARRANGE, 0, 0L);

    return 0;
}

//
//  FUNCTION: CmdWindowCloseAll(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To close all of the mdi child windows.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - IDM_WINDOWCLOSEALL (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdWindowCloseAll(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    HWND hwndT;

    // As long as the MDI client has a child, destroy it
    while (hwndT = GetWindow(hwndMDIClient, GW_CHILD))
    {

        // Skip the icon and title windows
        while (hwndT && GetWindow(hwndT, GW_OWNER))
            hwndT = GetWindow(hwndT, GW_HWNDNEXT);

        if (hwndT)
            SendMessage(hwndMDIClient, WM_MDIDESTROY, (WPARAM)hwndT, 0L);
        else
            break;
    }

    return 0;
}

