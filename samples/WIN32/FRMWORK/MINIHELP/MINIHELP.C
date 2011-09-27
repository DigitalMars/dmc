// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: minihelp.c
//
//  PURPOSE: Implements commands in the help menu.
//
//   
//  FUNCTIONS:
//    WndProc - Processes messages for the main window.
//    MsgCommand - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy - Handles the WM_DESTROY message by calling 
//                 PostQuitMessage().
//    CmdExit - Handles the file exit command by calling destory 
//              window on the main window.
//    CmdHelpContents - Displays the help contents for the sample.
//    CmdHelpSearch - Displays the help index for the sample.
//    CmdHelpHelp - Displays the help on help.
//
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
//  SPECIAL INSTRUCTIONS: N/A
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
    {WM_COMMAND, MsgCommand},
    {WM_DESTROY, MsgDestroy}
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
    {IDM_HELPCONTENTS,  CmdHelpContents},
    {IDM_HELPSEARCH,    CmdHelpSearch},
    {IDM_HELPHELP,      CmdHelpHelp},
    {IDM_EXIT,          CmdExit},
    {IDM_ABOUT,         CmdAbout}
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
//    GET_WM_COMMAND_ID(wparam,lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam,lparam) - Control handle
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
//  FUNCTION: CmdHelpContents(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the help contents for the sample.
//
//  PARAMETERS:
//    hwnd - The window handle
//    wCommand - IDM_HELPCONTENTS (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT CmdHelpContents(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    if (!WinHelp (hwnd, SZAPPNAME ".HLP", HELP_KEY,(DWORD)(LPSTR)"CONTENTS"))
    {
        MessageBox(GetFocus(),
                   "Unable to activate help",
                   szAppName,
                   MB_SYSTEMMODAL|MB_OK|MB_ICONHAND);
    }

    return 0;
}


//
//  FUNCTION: CmdHelpSearch(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the help index for the sample.
//
//  PARAMETERS:
//    hwnd - The window handle
//    wCommand - IDM_HELPSEARCH (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT CmdHelpSearch(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    if (!WinHelp(hwnd, SZAPPNAME ".HLP", HELP_PARTIALKEY, (DWORD)(LPSTR)""))
    {
        MessageBox(GetFocus(),
                   "Unable to activate help",
                   szAppName,
                   MB_SYSTEMMODAL|MB_OK|MB_ICONHAND);
    }

    return 0;
}


//
//  FUNCTION: CmdHelpHelp(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the help on help.
//
//  PARAMETERS:
//    hwnd - The window handle
//    wCommand - IDM_HELPHELP (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT CmdHelpHelp(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    if(!WinHelp(hwnd,NULL, HELP_HELPONHELP, 0))
    {
        MessageBox(GetFocus(),
                    "Unable to activate help",
                    szAppName,
                    MB_SYSTEMMODAL|MB_OK|MB_ICONHAND);
    }

    return 0;
}
