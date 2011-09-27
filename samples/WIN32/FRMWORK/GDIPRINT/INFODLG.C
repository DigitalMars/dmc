// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   info.c
//
//  PURPOSE:   Displays the "Info" dialog box
//
//  FUNCTIONS:
//    CmdInfo        - Displays the "Info" dialog box
//    InfoProc       - Processes messages for "Info" dialog box.
//    MsgInfoInit    - To initialize the about box with version info
//                     from resources.
//    MsgInfoCommand - Process WM_COMMAND message sent to the about box.
//    CmdInfoDone    - Free the about box and related data.
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include "infodlg.h"


LRESULT CALLBACK InfoProc(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgInfoInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgInfoCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdInfoDone(HWND, WORD, WORD, HWND);


// Info dialog message table definition.
MSD rgmsdInfo[] =
{
    {WM_COMMAND,    MsgInfoCommand},
    {WM_INITDIALOG, MsgInfoInit}
};

MSDI msdiInfo =
{
    sizeof(rgmsdInfo) / sizeof(MSD),
    rgmsdInfo,
    edwpNone
};

// Info dialog command table definition.
CMD rgcmdInfo[] =
{
    {IDOK,     CmdInfoDone},
    {IDCANCEL, CmdInfoDone}
};

CMDI cmdiInfo =
{
    sizeof(rgcmdInfo) / sizeof(CMD),
    rgcmdInfo,
    edwpNone
};

// Module specific "globals"  Used when a variable needs to be
// accessed in more than on handler function.


//
//  FUNCTION: CmdInfo(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the "Info" dialog box
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_INFO (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    To process the IDM_INFO message, call DialogBox() to display the
//    InfoDlg dialog box.

LRESULT CmdInfo(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DialogBox(hInst, "InfoDlg", hwnd, (DLGPROC)InfoProc);
    return 0;
}


//
//  FUNCTION: InfoProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Info" dialog box.
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
//     Display video device information
//
//     Wait for user to click on "Ok" button, then close the dialog box.
//

LRESULT CALLBACK InfoProc(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiInfo, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgInfoInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the about box with version info from resources.
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
//    Uses GetDeviceCaps to query the driver for info regarding
//    the display device's color and palette capabilities.
//

LRESULT MsgInfoInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HDC  hDC;
    char szText[64];

    // Center the dialog over the application window
    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));

    // get screen DC
    hDC = GetDC(NULL);
    
    // get various DeviceCaps
    
    // Palette-based device?
    wsprintf(szText, "%s", (bPalDevice ? "Yes": "No"));
    SetDlgItemText(hdlg, IDC_RCPALETTE, szText);

    if (bPalDevice)
    {
        wsprintf(szText, "%u", GetDeviceCaps(hDC, SIZEPALETTE));
        SetDlgItemText(hdlg, IDC_SIZEPALETTE, szText);

        // This is the number of static colors reserved by Windows
        wsprintf(szText, "%u", GetDeviceCaps(hDC, NUMRESERVED));
        SetDlgItemText(hdlg, IDC_NUMRESERVED, szText);
        
        // This is the actual number of bits the device can use
        // to represent colors. For example, the 8514/A uses
        // 18 bit DACs: 6 bits for red, 6 for green, and 6 for blue.
        wsprintf(szText, "%u", GetDeviceCaps(hDC, COLORRES));
        SetDlgItemText(hdlg, IDC_COLORRES, szText);        
    }
    else
    {
        // not a palette device, so these aren't applicable
        wsprintf(szText, "%s", "N/A");
        SetDlgItemText(hdlg, IDC_SIZEPALETTE, szText);
        SetDlgItemText(hdlg, IDC_NUMRESERVED, szText);
        SetDlgItemText(hdlg, IDC_COLORRES, szText);
    }

    wsprintf(szText, "%u", GetDeviceCaps(hDC, NUMCOLORS));
    SetDlgItemText(hdlg, IDC_NUMCOLORS, szText);

    wsprintf(szText, "%u", GetDeviceCaps(hDC, BITSPIXEL));    
    SetDlgItemText(hdlg, IDC_BITSPIXEL, szText);
    
    wsprintf(szText, "%u", GetDeviceCaps(hDC, PLANES));
    SetDlgItemText(hdlg, IDC_PLANES, szText);
                    
    ReleaseDC(NULL, hDC);

    return TRUE;
}

//
//  FUNCTION: MsgInfoCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the about box.
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
//    with the cmdiInfo structure defined in this file to handle
//    the command messages for the about dialog box.
//

LRESULT MsgInfoCommand(HWND   hwnd, 
                        UINT   uMessage, 
                        WPARAM wparam, 
                        LPARAM lparam)
{
    return DispCommand(&cmdiInfo, hwnd, wparam, lparam);
}

//
//  FUNCTION: CmdInfoDone(HWND, WORD, HWND)
//
//  PURPOSE: Free the about box and related data.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdInfoDone(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    EndDialog(hdlg, TRUE);          // Exit the dialog
    return TRUE;
}
