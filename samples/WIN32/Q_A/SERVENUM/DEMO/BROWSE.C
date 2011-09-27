// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   browse.c
//
//  PURPOSE:  Implement the windows procedure for the main application
//            windows.
//
//  FUNCTIONS:
//    WndProc      - Processes messages for the main window.
//    MsgCreate    - Handle the WM_CREATE messages for the main window.
//    MsgCommand   - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy   - Handles the WM_DESTROY message by calling
//                   PostQuitMessage().
//    CmdExit      - Handles the file exit command by calling destory
//                   window on the main window.
//    CmdEnum      - Calls ServerEnumDialog
//    CmdEnumHelp  - Calls ServerEnumDialog with a Help Procedure
//    HelpProc     - Help procedure for callback
//
//  COMMENTS:
//
//  AUTHOR: Craig Link - Microsoft Developer Support
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>

#include "globals.h"            // prototypes specific to this application
#include "servenum.h"

// typedef's for dynamic linking to functions in servenum.dll
typedef DWORD (*FPSERVENUMDLG)(HWND, LPTSTR, LPDWORD, FARPROC);

int HelpProc(VOID);

// Main window message table definition.
MSD rgmsd[] =
{
    {WM_CREATE,   MsgCreate},
    {WM_COMMAND,  MsgCommand},
    {WM_DESTROY,  MsgDestroy}
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
    {IDM_ENUM, CmdEnumDlg},
    {IDM_ENUMHELP, CmdEnumHelpDlg},
    {IDM_EXIT, CmdExit},
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};

// Application "globals"

// Module specific "globals"  Used when a variable needs to be
// accessed in more than on handler function.

HWND g_hwnd;
HINSTANCE g_hLibrary;
FPSERVENUMDLG g_lpfnServEnumDlg;

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
//  FUNCTION: MsgCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Initialized main window
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    Creates a listbox the size of the window and fills it
//    with a list of RAS connections.
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    DWORD dwError;
    TCHAR szError[128];

    // Store the main window handle
    g_hwnd = hwnd;

    // Attempt to dynamically load the servenum.dll
    g_hLibrary = LoadLibrary(TEXT("servenum.dll"));
    if (!g_hLibrary)
    {
        dwError = GetLastError();
        wsprintf(szError, TEXT("Error loading SERVENUM.DLL.  ")
                 TEXT("GetLastError() == %d."), dwError);
        MessageBox(hwnd, szError, TEXT("Server Enum"), MB_ICONEXCLAMATION | MB_OK);
        return(-1);
    }

    // The DLL is loaded, get a pointer to the exported function
#ifdef UNICODE
    g_lpfnServEnumDlg = (FPSERVENUMDLG) GetProcAddress(g_hLibrary,
            "ServerEnumDialogW");
#else
    g_lpfnServEnumDlg = (FPSERVENUMDLG) GetProcAddress(g_hLibrary,
            "ServerEnumDialogA");
#endif

    if (!g_lpfnServEnumDlg)
    {
        dwError = GetLastError();
        wsprintf(szError, TEXT("Error loading SERVENUM.DLL.  ")
                 TEXT("GetLastError() == %d."), dwError);
        MessageBox(hwnd, szError, TEXT("Server Enum"), MB_ICONEXCLAMATION | MB_OK);
        return(-1);
    }

    return 0;
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
//    wCommand - IDM_EXIT
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
//  FUNCTION: CmdEnumDlg(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Calls the ServerEnumDialog API
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_ENUM
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
LRESULT CmdEnumDlg(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    TCHAR szBuf[64];
    TCHAR szServer[MAX_COMPUTERNAME_LENGTH+1];
    DWORD cchServer = MAX_COMPUTERNAME_LENGTH+1;
    DWORD dwRet;

    dwRet = g_lpfnServEnumDlg( hwnd, szServer, &cchServer, NULL );

    if ( dwRet == ERROR_SUCCESS )
        wsprintf( szBuf, TEXT("Selected Server: %s"), szServer );

    else if ( dwRet == ERROR_CANCELLED )
        _tcscpy( szBuf, TEXT("ServerEnumDialog Cancelled" ) );

    else
        wsprintf( szBuf, TEXT("ServerEnumDialog return error 0x%x" ), dwRet );

    MessageBox( hwnd, szBuf, szAppName, MB_OK );

    return 0;
}


//
//  FUNCTION: CmdEnumHelpDlg(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Calls the ServerEnumDialog API with a HelpProcedure
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_ENUMHELP
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
LRESULT CmdEnumHelpDlg(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    TCHAR szBuf[64];
    TCHAR szServer[MAX_COMPUTERNAME_LENGTH+1];
    DWORD cchServer = MAX_COMPUTERNAME_LENGTH+1;
    DWORD dwRet;

    dwRet = g_lpfnServEnumDlg(hwnd, szServer, &cchServer, (FARPROC) HelpProc);

    if ( dwRet == ERROR_SUCCESS )
        wsprintf( szBuf, TEXT("Selected Server: %s"), szServer );

    else if ( dwRet == ERROR_CANCELLED )
        _tcscpy( szBuf, TEXT("ServerEnumDialog Cancelled" ) );

    else
        wsprintf( szBuf, TEXT("ServerEnumDialog return error 0x%x" ), dwRet );

    MessageBox( hwnd, szBuf, szAppName, MB_OK );

    return 0;
}


//
//  FUNCTION: HelpProc( VOID )
//
//  PURPOSE: Called by the ServerEnumDialog API
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
int HelpProc( VOID )
{
    MessageBox( g_hwnd, TEXT("Help Procedure Called"), szAppName, MB_OK );
    return 0;
}
