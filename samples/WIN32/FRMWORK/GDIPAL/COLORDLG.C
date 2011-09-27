// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:    colordlg.c
//
//  PURPOSE:   Displays the "Color" dialog box
//
//  FUNCTIONS:
//    CmdColor        - Displays the "Color" dialog box
//    Color           - Processes messages for "Color" dialog box.
//    MsgColorInit    - To initialize the color box with version info
//                      from resources.
//    MsgColorCommand - Process WM_COMMAND message sent to the color box.
//    CmdColorOK      - Free the color box and related data. Do OK processing.
//    CmdColorCancel  - Free the color box and related data. Do Cancel
//                      processing.
//    CmdColorPalCtrl - Handles notifications from the PalCtrl.
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include "colordlg.h"

LRESULT MsgColorInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgColorCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdColorOK(HWND, WORD, WORD, HWND);
LRESULT CmdColorCancel(HWND, WORD, WORD, HWND);
LRESULT CmdColorPalCtrl(HWND, WORD, WORD, HWND);

// Color dialog message table definition.
MSD rgmsdColor[] =
{
    {WM_COMMAND,    MsgColorCommand},
    {WM_INITDIALOG, MsgColorInit}
};

MSDI msdiColor =
{
    sizeof(rgmsdColor) / sizeof(MSD),
    rgmsdColor,
    edwpNone
};

// Color dialog command table definition.
CMD rgcmdColor[] =
{
    {IDOK,        CmdColorOK},
    {IDCANCEL,    CmdColorCancel},

    // PalCtrl notifications
    {IDD_PALCTRL, CmdColorPalCtrl}
};

CMDI cmdiColor =
{
    sizeof(rgcmdColor) / sizeof(CMD),
    rgcmdColor,
    edwpNone
};

// Module specific "globals"  Used when a variable needs to be
// accessed in more than on handler function.


//
//  FUNCTION: Color(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Color" dialog box.
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

LRESULT CALLBACK Color(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiColor, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgColorInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the Color box with version info from resources.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Return FALSE (when calling SetFocus processing WM_INITDIALOG)
//
//  COMMENTS:
//

LRESULT MsgColorInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    // Center the dialog over the application window
    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));

    // give the PalCtrl focus
    SetFocus(GetDlgItem(hdlg, IDD_PALCTRL));

    // Set color selection in PalCtrl, LPARAM contains the desired index
    SendDlgItemMessage(hdlg, IDD_PALCTRL, PCM_SETCURSEL, wparam, lparam);

    return FALSE;
}

//
//  FUNCTION: MsgColorCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the Color box.
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
//    with the cmdiColor structure defined in this file to handle
//    the command messages for the Color dialog box.
//

LRESULT MsgColorCommand(HWND   hwnd, 
                        UINT   uMessage, 
                        WPARAM wparam, 
                        LPARAM lparam)
{
    return DispCommand(&cmdiColor, hwnd, wparam, lparam);
}


//
//  FUNCTION: CmdColorOK(HWND, WORD, HWND)
//
//  PURPOSE: Free the Color box and related data.
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
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdColorOK(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    // Get the current color selection
    SendMessage(hwndCtrl, PCM_GETCURSEL, (WPARAM)NULL, (LPARAM)&palinfo);

    EndDialog(hdlg, TRUE);          // Exit the dialog
    return TRUE;
}


//
//  FUNCTION: CmdColorCancel(HWND, WORD, HWND)
//
//  PURPOSE: Free the Color box and related data.
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
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdColorCancel(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    // user cancelled, return -1 in index field of palinfo
    palinfo.index = -1;
    palinfo.red = palinfo.green = palinfo.blue = 0;
    
    EndDialog(hdlg, TRUE);          // Exit the dialog
    return TRUE;
}


//
//  FUNCTION: CmdColorPalCtrl(HWND, WORD, HWND)
//
//  PURPOSE: Handles notifications from the PalCtrl.
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
//

LRESULT CmdColorPalCtrl(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    switch (wNotify)
    {
        case PCN_CHANGE:
            {
                // Get the current color selection
                SendMessage(hwndCtrl, PCM_GETCURSEL, (WPARAM)NULL, (LPARAM)&palinfo);

                // Update index and RGB edit controls
                SetDlgItemInt(hdlg, IDD_INDEX, palinfo.index, FALSE);
                SetDlgItemInt(hdlg, IDD_RED, palinfo.red, FALSE);
                SetDlgItemInt(hdlg, IDD_GREEN, palinfo.green, FALSE);
                SetDlgItemInt(hdlg, IDD_BLUE, palinfo.blue, FALSE);
            }
            break;

        default:
            break;
    }

    return TRUE;
}
