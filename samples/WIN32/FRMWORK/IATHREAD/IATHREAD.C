// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   iathread.c
//
//  PURPOSE:  Implement the window procedure for the main application
//            window.
//
//  FUNCTIONS:
//    FrameWndProc      - Processes messages for the main window.
//    MsgCommand        - Handle the WM_COMMAND messages for the main window.
//    MsgCreate         - To create the MDI client window.
//    MsgClose          - Handles the WM_CLOSE message.
//    MsgDestroy        - Handles the WM_DESTROY message.
//    CmdForwardToActiveChild - Forwards a WM_COMMAND message to the active
//                              MDI child window.
//    CmdFileNew        - To create a new mdi child window.
//    CmdFileClose      - To close the active mdi child window    
//    CmdFileExit       - Handles the file exit command by calling destory 
//                        window on the main window.
//    CmdWindowTile     - To tile the mdi child windows.
//    CmdWindowCascade  - To cascade the mdi child windows.
//    CmdWindowIcons    - To arrage the mdi child icons.
//    CmdWindowCloseAll - To close all of the mdi child windows.
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
#include "globals.h"            // prototypes specific to this application
#include "resource.h"


// Main window message table definition.
static MSD rgmsd[] =
{
    {WM_COMMAND,    MsgCommand},
    {WM_CREATE,     MsgCreate},
    {WM_CLOSE,      MsgClose},
    {WM_DESTROY,    MsgDestroy}
};

static MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpMDIFrame
};


// Main window command table definition.
static CMD rgcmd[] =
{
    {IDM_FILE_NEW,          CmdFileNew},
    {IDM_FILE_CLOSE,        CmdFileClose},
    {IDM_FILE_EXIT,         CmdFileExit},
    {IDM_THREAD_PRIORITY,   CmdForwardToActiveChild},
    {IDM_THREAD_SUSPEND,    CmdForwardToActiveChild},
    {IDM_THREAD_RESUME,     CmdForwardToActiveChild},
    {IDM_WINDOW_TILE,       CmdWindowTile},
    {IDM_WINDOW_CASCADE,    CmdWindowCascade},
    {IDM_WINDOW_ICONS,      CmdWindowIcons},
    {IDM_WINDOW_CLOSEALL,   CmdWindowCloseAll},
    {IDM_HELP_ABOUT,        CmdAbout}

};

static CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpMDIFrame
};


// Global variable definitions

UINT gcOpenChildWnds = 0;         // Number of child windows open.


//
//  FUNCTION: FrameWndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependent on message number)
//    lparam   - additional information (dependent on message number)
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

LRESULT CALLBACK FrameWndProc(HWND   hwnd, 
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
//    hwnd      - window handle
//    uMessage  - WM_COMMAND (Unused)
//    wParam    - Message-specific parameter.
//    lParam    - Message-specific parameter.
//
//  RETURN VALUE:
//    The return value depends on the command number.  If the command
//    is implemented in the command dispatch table, the return value is
//    the value returned by the command handling function.  Otherwise,
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
//    hwnd     - The window handing the message.
//    uMessage - WM_CREATE             (unused)
//    wparam   - Message specific data (unused).
//    lparam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    CLIENTCREATESTRUCT ccs;

    // Find window menu where children will be listed
    ccs.hWindowMenu  = GetSubMenu(GetMenu(hwnd), INITWINDOWMENU);
    ccs.idFirstChild = IDW_FIRST_CHILD;

    // Create the MDI client filling the client area
    ghwndMDIClient = CreateWindow("mdiclient",
                                  NULL,
                                  WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL |
                                  WS_HSCROLL |WS_VISIBLE,
                                  0, 0, 0, 0,
                                  hwnd,
                                  (HMENU)IDW_MDICLIENT,
                                  hInst,
                                  (LPSTR)&ccs);

    // If client window couldn't be created, signal failure.
    if (ghwndMDIClient == NULL)
        return -1;

    return 0;
}


//
//  FUNCTION: MsgClose(HWND, UINT, WPARAM, LPARAM)
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
//

LRESULT MsgClose(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    // Destroy unattached menu.
    if (GetMenu(hwnd) == ghInitMenu)
        DestroyMenu(ghDocMenu);
    else
        DestroyMenu(ghInitMenu);
   
    DestroyWindow(hwnd);
    return 0;
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
//  FUNCTION: CmdForwardToActiveChild(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To forward a command to the active MDI child window.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - Depends on command.
//    wNotify   - Notification number.
//    hwndCtrl  - NULL.
//
//  RETURN VALUE:                         
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdForwardToActiveChild(HWND hwnd, WORD wCommand, WORD wNotify,
                                HWND hwndCtrl)
{
    HWND hChildWnd = NULL;

    // Ask the MDI client to tell us which child window is active.  If there
    // is an active child window, forward the message; otherwise, don't.
    hChildWnd = (HWND)SendMessage(ghwndMDIClient, WM_MDIGETACTIVE, 0, 0);

    if (IsWindow(hChildWnd))
        SendMessage(hChildWnd, 
                    WM_COMMAND, 
                    GET_WM_COMMAND_MPS(wCommand, hwndCtrl, wNotify));
    return 0;
}


//
//  FUNCTION: CmdFileNew(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To create a new mdi child window.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - IDM_FILE_NEW        (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL                (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdFileNew(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    static int      snChildWndNum = 0;
    MDICREATESTRUCT mcs           = {0};
    HWND            hwndChild     = NULL;
    char            szTitle[DEFAULT_STR_LEN];
    char            szChildName[DEFAULT_STR_LEN];
	DWORD           dwVersion;

    // Reset child window # if no child windows exist
    if (gcOpenChildWnds == 0)       
        snChildWndNum = 0;

    LoadString(hInst, IDS_CHILDNAME, szChildName, sizeof(szChildName));
    wsprintf(szTitle, "Thread Window %d", snChildWndNum);

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
        mcs.szTitle = szTitle;          // window title
        mcs.hOwner  = hInst;            // owner
        mcs.x       = CW_USEDEFAULT;    // x position
        mcs.y       = CW_USEDEFAULT;    // y position
        mcs.cx      = CW_USEDEFAULT;    // width
        mcs.cy      = CW_USEDEFAULT;    // height
        mcs.style   = 0;                // window style
        mcs.lParam  = 0;                // lparam

        hwndChild = (HWND) SendMessage(ghwndMDIClient,
                                       WM_MDICREATE,
                                       0,
                                       (LPARAM)(LPMDICREATESTRUCT) &mcs);
    }
    else
    {
        // This method will only work with Windows 95, not Windows NT or Win32s
        hwndChild = CreateWindowEx(WS_EX_MDICHILD,  // EX window style
                                   szChildName,     // window class name
                                   szTitle,         // window title
                                   0,               // window style
                                   CW_USEDEFAULT,   // x position
                                   CW_USEDEFAULT,   // y position
                                   CW_USEDEFAULT,   // width
                                   CW_USEDEFAULT,   // height
                                   ghwndMDIClient,  // parent
                                   NULL,            // menu (child ID)
                                   hInst,           // owner
                                   0);              // lparam
    }

    if (hwndChild != NULL)
    {
        ShowWindow(hwndChild, SW_SHOW);
        snChildWndNum++;
    }

    return 0;
}


//
//  FUNCTION: CmdFileClose(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To close the active MDI child window.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - IDM_FILE_CLOSE      (unused).
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL                (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdFileClose(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    HWND hwndActive;

    // Ask the MDI client window which MDI child window is active.  If 
    // there is an active child window, destroy it; otherwise, do nothing.

    hwndActive = (HWND)SendMessage(ghwndMDIClient, WM_MDIGETACTIVE, 0, 0);
    if (hwndActive != NULL)
        SendMessage(ghwndMDIClient, WM_MDIDESTROY, (WPARAM)hwndActive, 0);

    return 0;
}


//
//  FUNCTION: CmdFileExit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window handling the command.
//    wCommand - IDM_FILE_EXIT       (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL                (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdFileExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SendMessage(hwnd, WM_CLOSE, 0, 0L);
    return 0;
}


//
//  FUNCTION: CmdWindowTile(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To tile the mdi child windows.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - IDM_WINDOW_TILE     (unused).
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL                (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdWindowTile(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SendMessage(ghwndMDIClient, WM_MDITILE, 0, 0);

    return 0;
}

//
//  FUNCTION: CmdWindowCascade(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To cascade the mdi child windows.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - IDM_WINDOW_CASCADE  (unused).
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL                (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdWindowCascade(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SendMessage(ghwndMDIClient, WM_MDICASCADE, 0, 0);

    return 0;
}

//
//  FUNCTION: CmdWindowIcons(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To arrage the mdi child icons.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - IDM_WINDOW_ICONS    (unused).
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL                (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdWindowIcons(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SendMessage(ghwndMDIClient, WM_MDIICONARRANGE, 0, 0);

    return 0;
}

//
//  FUNCTION: CmdWindowCloseAll(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To close all of the MDI child windows.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - IDM_WINDOW_CLOSEALL (unused).
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL                (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdWindowCloseAll(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    HWND hwndChild;

    // As long as the MDI client has a child, destroy it
    while (hwndChild = GetWindow(ghwndMDIClient, GW_CHILD))
    {
        // Skip the icon and title windows
        while (hwndChild && GetWindow(hwndChild, GW_OWNER))
            hwndChild = GetWindow(hwndChild, GW_HWNDNEXT);

        if (IsWindow(hwndChild))
            SendMessage(ghwndMDIClient, WM_MDIDESTROY, (WPARAM)hwndChild, 0);
        else
            break;
    }

    return 0;
}
