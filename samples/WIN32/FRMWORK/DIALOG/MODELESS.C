// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   modeless.c
//
//  PURPOSE:   Displays the "modeless" dialog box
//
//  FUNCTIONS:
//    CmdModeless        - Displays the "Modeless" dialog box
//    Modeless           - Processes messages for "Modeless" dialog box.
//    MsgModelessInit    - To initialize the modeless box with version info
//                         from resources.
//    MsgModelessCommand - Process WM_COMMAND message sent to the modeless
//                         box.
//    CmdModelessDone    - Free the modeless box and related data.
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "modeless.h"

LRESULT MsgModelessInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgModelessCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdModelessDone(HWND, WORD, WORD, HWND);
LRESULT CmdModelessColorChange(HWND, WORD, WORD, HWND);

// Modeless dialog message table definition.
MSD rgmsdModeless[] =
{
    {WM_COMMAND,    MsgModelessCommand},
    {WM_INITDIALOG, MsgModelessInit}
};

MSDI msdiModeless =
{
    sizeof(rgmsdModeless) / sizeof(MSD),
    rgmsdModeless,
    edwpNone
};

// Modeless dialog command table definition.
CMD rgcmdModeless[] =
{
    {IDD_RED,         CmdModelessColorChange},
    {IDD_GREEN,       CmdModelessColorChange},
    {IDD_BLUE,        CmdModelessColorChange},
    {IDOK,            CmdModelessDone},
    {IDCANCEL,        CmdModelessDone}
};

CMDI cmdiModeless =
{
    sizeof(rgcmdModeless) / sizeof(CMD),
    rgcmdModeless,
    edwpNone
};

// Module specific globals

HWND    hwndModeless=0; //Window handle to modeless dialog
HBRUSH  hBkgndBrush;    //Brush for background color of main window
DLGPROC lpProcModeless;
WORD    wIDColor;       //Dialog ID value for current color.  Used to set
                        //the radio button when modeless dialog is invoked.

//
//  FUNCTION: Modeless(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Modeless" dialog box.
//
//  PARAMETERS:
//    hdlg - window handle of the dialog box
//    wMessage - type of message
//    wparam - message-specific information
//    lparam - message-specific information
//
//  RETURN VALUE:
//    TRUE - message handled
//    FALSE - message not handled
//
//  COMMENTS:
//
//     Display version information from the version section of the
//     application resource.
//
//     Wait for user to click on "Ok" button, then close the dialog box.
//

LRESULT CALLBACK Modeless(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiModeless, hdlg, uMessage, wparam, lparam);
}

//
//  FUNCTION: MsgModelessCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the modeless box.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    Uses this DipsCommand function defined in wndproc.c combined
//    with the cmdiModeless structure defined in this file to handle
//    the command messages for the modeless dialog box.
//

LRESULT MsgModelessCommand(HWND hwnd,
                           UINT uMessage,
                           WPARAM wparam,
                           LPARAM lparam)
{
    return DispCommand(&cmdiModeless, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgModelessInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the modeless box with version info from resources.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    Uses the version apis to retrieve version information for
//    each of the static text boxes in the modeless box.
//

LRESULT MsgModelessInit(HWND hdlg,
                        UINT uMessage,
                        WPARAM wparam,
                        LPARAM lparam)
{
    // Center the dialog over the application window
    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));
    if (hBkgndBrush)
        CheckRadioButton(hdlg, IDD_RED, IDD_BLUE, wIDColor);

    return TRUE;
}


//
//  FUNCTION: MsgPaint(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Fills the windows background with a specific color,
//           if background brush exists.  Brush is created via the 
//           modeless dialog
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

LRESULT MsgPaint(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{                          
    PAINTSTRUCT ps;
    HDC         hDC;
    RECT        rect;

    if (hBkgndBrush)
    {
        hDC = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);
        FillRect(hDC, &rect, hBkgndBrush);
        EndPaint(hwnd, &ps);
        return 0;
    }
    else
        return DefWindowProc(hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: CmdModelessColorChange(HWND, WORD, HWND)
//
//  PURPOSE: Update the background color of the main application
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Creates a new brush and invalidates the main window
//

LRESULT CmdModelessColorChange(HWND hdlg,
                               WORD wCommand,
                               WORD wNotify,
                               HWND hwndCtrl)
{
    COLORREF rgbColor;

    if (wCommand == wIDColor)
        return(TRUE);

    wIDColor = wCommand;

    if (hBkgndBrush)
        DeleteObject(hBkgndBrush);

    switch(wIDColor)
    {
        case IDD_RED:
            rgbColor = RGB(255, 0, 0);
            break;

        case IDD_GREEN:
            rgbColor = RGB(0, 255, 0);
            break;

        case IDD_BLUE:
            rgbColor = RGB(0, 0, 255);
            break;

        default:
            break;
    }

    hBkgndBrush = CreateSolidBrush(rgbColor);

    InvalidateRect(GetWindow(hdlg, GW_OWNER), NULL, TRUE);

    return TRUE;
}

//
//  FUNCTION: CmdModelessDone(HWND, WORD, HWND)
//
//  PURPOSE: Free the modeless box and related data.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls DestroyWindow to finish the dialog session.
//

LRESULT CmdModelessDone(HWND hdlg,
                        WORD wCommand,
                        WORD wNotify,
                        HWND hwndCtrl)
{
    DestroyWindow(hdlg);          // Exit the dialog
    FreeProcInstance(lpProcModeless);
    lpProcModeless = NULL;
    hwndModeless = NULL;
    return TRUE;
}
