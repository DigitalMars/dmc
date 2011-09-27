// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: gdipal.c
//
//  PURPOSE: Handles the main window for the GDIPal sample.
//
//  FUNCTIONS:
//    WndProc            - Processes messages for the main window.
//    MsgCommand         - Handle the WM_COMMAND messages for the main window.
//    MsgCreate          - Handles the WM_CREATE message.  Creates the toolbar,
//                         statusbar, and client windows.
//    MsgSize            - Handles the WM_SIZE message by passing the message
//                         on to the toolbar and statusbar controls and then
//                         resizing the client window between them.
//    MsgDestroy         - Handles the WM_DESTROY message by calling
//                         PostQuitMessage().
//    MsgPaletteChanged  - Passes WM_PALETTECHANGED message to
//                         ProcessPaletteChanged
//    MsgQueryNewPalette - Passes WM_QUERYNEWPALETTE message to
//                         ProcessQueryNewPalette
//    CmdRefresh         - Forces client window to repaint.
//    CmdExit            - Handles the file exit command by calling destroy
//                         window on the main window.
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
#include "toolbar.h"            // prototypes for the tool bar
#include "statbar.h"            // prototypes for the status bar
#include "palette.h"            // prototypes for palette routines

// Global variables
HWND hWndClient;
HPALETTE  hPalette;             // App's logical palette
BOOL      bPalDevice = FALSE;


// Main window message table definition.
MSD rgmsd[] =
{
    {WM_MENUSELECT,      MsgMenuSelect},
    {WM_COMMAND,         MsgCommand},
    {WM_NOTIFY,          MsgNotify},
    {WM_TIMER,           MsgTimer},
    {WM_SIZE,            MsgSize},
    {WM_CREATE,          MsgCreate},
    {WM_DESTROY,         MsgDestroy},

    // palette messages
    {WM_PALETTECHANGED,  MsgPaletteChanged},
    {WM_QUERYNEWPALETTE, MsgQueryNewPalette}
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
    {IDM_FILENEW,     CmdFileNew},
    {IDM_EXIT,        CmdExit},

    {IDM_INFO,        CmdInfo},

    {IDM_PIXEL,       CmdDrawMode},
    {IDM_LINE,        CmdDrawMode},
    {IDM_RECT,        CmdDrawMode},
    {IDM_ELLIPSE,     CmdDrawMode},
    {IDM_BEZIER,      CmdDrawMode},
    {IDM_FILL,        CmdFill},
    {IDM_NOFILL,      CmdFill},
    {IDM_CREATEPEN,   CmdCreatePen},
    {IDM_CREATEBRUSH, CmdCreateBrush},
    {IDM_REFRESH,     CmdRefresh},

    {IDM_ABOUT,       CmdAbout}
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
//    Return TRUE if the StatusBar and ToolBar Windows could be created
//    successfully.
//
//  COMMENTS:
//    Call the CreateTSBars function with the main window's window handle
//    information (msdiMain).
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int nRet = -1;                              // Assume failure

    InitCommonControls();   // Initialize the common control library.

    // check palette capabilities

    bPalDevice = IsPaletteDevice();

    if (bPalDevice)
    {
        // create a default palette for choosing pen and brush colors
        // using the new halftone palette
        hPalette = CreateHalftonePalette(NULL);
    }

    if(CreateTBar(hwnd) && CreateSBar(hwnd))    // 1st create tool/status bars
    {
        hWndClient = CreateClientWindow(hwnd);  // then create client window
        if (hWndClient != NULL)
            nRet = 0;                           // Indicate success
    }

    // Initialize some menu options...

    CmdDrawMode(hwnd, IDM_LINE, 0, NULL);   // Set initial draw mode to Line

    CmdFill(hwnd, IDM_NOFILL, 0, NULL);     // Turn fill mode OFF
    CmdFill(hwnd, IDM_FILL, 0, NULL);       // Toggle fill state (ON)

    return nRet;
}


//
//  FUNCTION: MsgSize(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  This function resizes the toolbar and statusbar controls
//    and the Client window which covers the rest of the main window
//    client area.
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
//    receives the WM_SIZE message, it has to pass the message on to these
//    controls so that these controls can adjust their size accordingly.
//
//

LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    SendMessage(hWndToolbar, uMessage, wparam, lparam);
    SendMessage(hWndStatusbar, uMessage, wparam, lparam);

    // Re-position the panes in the status bar
    InitializeStatusBar(hwnd);

    // Re-size client window relative to the tool/status bars

    if (wparam != SIZE_MINIMIZED)
    {
        RECT rc, rcClient;

        GetClientRect(hwnd, &rcClient);

        GetWindowRect(hWndToolbar, &rc);
        ScreenToClient(hwnd, ((LPPOINT)&rc) + 1);
        rcClient.top = rc.bottom;

        GetWindowRect(hWndStatusbar, &rc);
        ScreenToClient(hwnd, (LPPOINT)&rc);
        rcClient.bottom = rc.top;

        MoveWindow(hWndClient,
                   rcClient.left,
                   rcClient.top,
                   rcClient.right-rcClient.left,
                   rcClient.bottom-rcClient.top,
                   FALSE);
    }

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
//  FUNCTION: MsgPaletteChanged(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Passes WM_PALETTECHANGED message to handler.
//
//  PARAMETERS:
//    hwnd -     Handle of application window.
//    uMessage - (unused)
//    wparam -   Handle of window that caused palette to change
//    lparam -   (unused)
//
//  RETURN VALUE:
//    Returns the value returned by ProcessPaletteChanged.
//
//  COMMENTS:
//

LRESULT MsgPaletteChanged(HWND   hwnd,
                          UINT   uMessage,
                          WPARAM wparam,
                          LPARAM lparam)
{
    return ProcessPaletteChanged(hwnd, wparam); 
}    


//
//  FUNCTION: MsgQueryNewPalette(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Passes WM_QUERYNEWPALETTE message to handler.
//
//  PARAMETERS:
//    hwnd     - Handle of application window.
//    uMessage - (unused)
//    wparam   - (unused)
//    lparam   - (unused)
//
//  RETURN VALUE:
//    Returns the value returned by ProcessQueryNewPalette.
//
//  COMMENTS:
//

LRESULT MsgQueryNewPalette(HWND   hwnd,
                           UINT   uMessage,
                           WPARAM wparam,
                           LPARAM lparam)
{
    return ProcessQueryNewPalette(hwnd); 
}


//
//  FUNCTION: CmdFileNew(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Erases current drawing
//
//  PARAMETERS:
//    hwnd     - The main window.
//    wCommand - IDM_FILENEW (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdFileNew(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    // Tell client window to purge itself
    ClientNewDrawing();
    return 0;
}


//
//  FUNCTION: CmdRefresh(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Forces repaint of entire drawing.
//
//  PARAMETERS:
//    hwnd     - The main window.
//    wCommand - IDM_REFRESH (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdRefresh(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    // Force client to repaint.
    InvalidateRect(hWndClient, NULL, TRUE);
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
