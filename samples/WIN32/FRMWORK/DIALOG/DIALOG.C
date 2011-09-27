// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: dialog.c
//
//  PURPOSE: Handles general routines for the dialog sample.
//
//  FUNCTIONS:
//    WndProc - Processes messages for the main window.
//    MsgCommand - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy   - Handles the WM_DESTROY message by calling 
//                   PostQuitMessage().
//    CmdModal     - Brings up the modal dialog
//    CmdModeless  - Brings up the modeless dialog
//    CmdExit      - Handles the file exit command by calling destory 
//                   window on the main window.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "modal.h"
#include "modeless.h"
#include "resource.h"

// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND, MsgCommand},
    {WM_DESTROY, MsgDestroy},
    {WM_PAINT,   MsgPaint}       //For ecapsulation purposes, this is in
                                 //modeless.c
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
    {IDM_EXIT,     CmdExit},
    {IDM_MODAL,    CmdModal},
    {IDM_MODELESS, CmdModeless},
    {IDM_ABOUT,    CmdAbout}
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
//  FUNCTION: CmdModal(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the "Modal" dialog box
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_ABOUT (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    To process the IDM_ABOUT message, call MakeProcInstance() to get the
//    current instance address of the Modal() function.  Then call Dialog
//    box which will create the box according to the information in your
//    sample.rc file and turn control over to the Modal() function.  When
//    it returns, free the intance address.
//
//

LRESULT CmdModal(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DLGPROC lpProcModal = MakeProcInstance((FARPROC)Modal, hInst);

    DialogBox(hInst, MAKEINTRESOURCE(IDD_MODALDIALOG), hwnd, lpProcModal);

    FreeProcInstance(lpProcModal);

    return 0;
}


//
//  FUNCTION: CmdModeless(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the "Modeless" dialog box
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_ABOUT (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    To process the IDM_ABOUT message, call MakeProcInstance() to get the
//    current instance address of the Modeless() function.  Then call Dialog
//    box which will create the box according to the information in your
//    sample.rc file and turn control over to the Modeless() function.  When
//    it returns, free the intance address.
//
//

LRESULT CmdModeless(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    if (!lpProcModeless && !hwndModeless)
    {
        lpProcModeless = MakeProcInstance((FARPROC)Modeless, hInst);

        hwndModeless = CreateDialog(hInst, 
                                    MAKEINTRESOURCE(IDD_MODELESSDIALOG), 
                                    hwnd, 
                                    lpProcModeless);
    }
    else
        SetActiveWindow(hwndModeless);

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
