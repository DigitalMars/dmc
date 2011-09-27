// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: barsdi.c
//
//  PURPOSE: Handles general routines for the Barsdi sample.
//
//  FUNCTIONS:
//    WndProc     - Processes messages for the main window.
//    MsgCommand  - Handle the WM_COMMAND messages for the main window.
//    MsgCreate   - Handle the WM_CREATE messages for the main window.
//    MsgSize     - Handles the WM_SIZE message by calling SendMessage() to pass
//                  the WM_SIZE message onto the status bar and tool bar controls. 
//    MsgDestroy  - Handles the WM_DESTROY message by calling 
//                  PostQuitMessage().
//    CmdExit     - Handles the file exit command by calling destory 
//                  window on the main window.
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
#include <commctrl.h>           // prototypes and defs for common controls
#include "globals.h"            // prototypes specific to this application
#include "statbar.h"            // prototypes specific to statbar.c
#include "toolbar.h"            // prototypes specific to toolbar.c
#include "resource.h"


// Main window message table definition.
MSD rgmsd[] =
{
    {WM_CREATE,     MsgCreate    },
    {WM_SIZE,       MsgSize      },
    {WM_TIMER,      MsgTimer     },
    {WM_MOUSEMOVE,  MsgMousemove },
    {WM_COMMAND,    MsgCommand   },
    {WM_NOTIFY,     MsgNotify    },
    {WM_MENUSELECT, MsgMenuSelect},
    {WM_DESTROY,    MsgDestroy   }
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
    {IDM_FILENEW,     CmdStub},
    {IDM_FILEOPEN,    CmdStub},
    {IDM_FILESAVE,    CmdStub},
    {IDM_FILESAVEAS,  CmdStub},
    {IDM_FILECLOSE,   CmdStub},
    {IDM_FILEPRINT,   CmdStub},
    {IDM_FILEPAGESU,  CmdStub},
    {IDM_FILEPRINTSU, CmdStub},
    {IDM_EXIT,        CmdExit},

    {IDM_EDITUNDO,    CmdStub},
    {IDM_EDITCUT,     CmdStub},
    {IDM_EDITCOPY,    CmdStub},
    {IDM_EDITPASTE,   CmdStub},
    {IDM_EDITCLEAR,   CmdStub},

    {IDM_ABOUT,       CmdAbout},
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
//           and call InitCommonControls() API to initialize the
//           common control library. 
//
//  PARAMETERS:
//    hwnd     - window handle
//
//  RETURN VALUE:
//    Return 0 if the StatusBar and ToolBar Windows could be created
//    successfully. Otherwise, returns -1 to abort the main window
//    creation.
//
//  COMMENTS:
//    Call the CreateTSBars function with the main window's window handle
//    information (msdiMain). 
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
   InitCommonControls() ; // Initialize the common control library.

   if(CreateTBar(hwnd) && CreateSBar(hwnd))
       return 0;    // Tool and status bars were created, so return 0.
   else 
       return -1;   // Tool and status bars were not created, so return -1.
}


//
//  FUNCTION: MsgSize(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  This function resizes the toolbar and statusbar controls. 
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
//
//    When the window procdure that has the status and tool bar controls
//    receive the WM_SIZE message, it has to pass the message on to these 
//    controls so that these controls can adjust their size accordingly. 
//   
//

LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam) 
{
    SendMessage(hWndStatusbar,  uMessage, wparam, lparam);
    SendMessage(hWndToolbar, uMessage, wparam, lparam);

    // Re-position the panes in the status bar
    InitializeStatusBar(hwnd);

    return 0 ;
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
    char szBuffer[50];   
    int  cbWritten = 0;

    cbWritten = LoadString(hInst, wCommand, szBuffer, sizeof(szBuffer)); 
    if(cbWritten == 0) 
        lstrcpy(szBuffer, "Unknown Command");

    UpdateStatusBar(szBuffer, 0, 0);
    MessageBox(hwnd, 
               "EXIT APPLICATION:", 
               "EXIT", 
               MB_OK | MB_ICONEXCLAMATION);
 
    DestroyWindow(hwnd);
    return 0;
}
