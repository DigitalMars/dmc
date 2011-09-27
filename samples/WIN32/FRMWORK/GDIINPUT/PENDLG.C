// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   pendlg.c
//
//  PURPOSE:  Displays the "Pen Style" dialog box
//
//  FUNCTIONS:
//    PenDlg          - Process messages for "Pen Style" dialog box.
//    MsgPenInit      - Initialize the Pen dialog with info from lparam.
//    MsgPenPaint     - Paint the Example Window in the Pen dialog
//    MsgPenCommand   - Process WM_COMMAND messages sent to the Pen dialog.
//    CmdPenStyle     - Track the currently selected pen style.
//    CmdPenColor     - Put up the ChooseColor dialog to select pen color.
//    CmdPenDone      - Free the Pen dialog and related data.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include "pendlg.h"             // Controls ID's for the Pen dialog

// global variables specific to this module
RECT rcExample;                 // location of example window in dialog

// prototypes specific to this module
LRESULT MsgPenInit    (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPenPaint   (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPenCommand (HWND, UINT, WPARAM, LPARAM);
LRESULT CmdPenStyle   (HWND, WORD, WORD, HWND);
LRESULT CmdPenColor   (HWND, WORD, WORD, HWND);
LRESULT CmdPenDone    (HWND, WORD, WORD, HWND);

// Pen dialog message table definition.
MSD rgmsdPen[] =
{
    {WM_COMMAND,    MsgPenCommand},
    {WM_PAINT,      MsgPenPaint},
    {WM_INITDIALOG, MsgPenInit}
};

MSDI msdiPen =
{
    sizeof(rgmsdPen) / sizeof(MSD),
    rgmsdPen,
    edwpNone
};

// Pen dialog command table definition.
CMD rgcmdPen[] =
{
    {IDD_PENSTYLE,  CmdPenStyle},   // Pen Style notification msg
    {IDD_PENCOLOR,  CmdPenColor},   // Color button
    {IDOK,          CmdPenDone},    // OK and Cancel buttons
    {IDCANCEL,      CmdPenDone}
};

CMDI cmdiPen =
{
    sizeof(rgcmdPen) / sizeof(CMD),
    rgcmdPen,
    edwpNone
};


//
//  FUNCTION: PenDlg(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Pen Style" dialog box.
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
//

LRESULT CALLBACK PenDlg(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiPen, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgPenInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the Pen dialog with info from lparam.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - points to LOGPEN structure.
//
//  RETURN VALUE:
//    Always returns TRUE
//
//  COMMENTS:
//    Sets the initial state of the controls according to the LOGPEN
//      passed in via lparam.
//

LRESULT MsgPenInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int i, nIndex, nSel;
    HWND hctlStyle;
    char szTmp[32];
    LPLOGPEN lpLP;

    // lparam is a pointer to a LOGPEN structure
    lpLP = (LPLOGPEN)lparam;

    // Save pointer to LOGPEN structure in window bytes
    SetWindowLong(hdlg, DWL_USER, (LONG)lpLP);

    // Center the dialog over the application window
    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));

    // Fill up the style combobox.  The item data is set to the
    // corresponding pen style defined in windows.h (e.g. PS_SOLID).

    hctlStyle = GetDlgItem(hdlg, IDD_PENSTYLE);
    nSel = 0;

    for (i = IDD_PENFIRST; i <= IDD_PENLAST; i++)
    {
        LoadString(hInst, i, szTmp, sizeof(szTmp));
        nIndex = SendMessage(hctlStyle, CB_ADDSTRING, 0, (LPARAM)(LPSTR)szTmp);
        SendMessage(hctlStyle, CB_SETITEMDATA, nIndex, i - IDD_PENSTYLE);

        // If this item is the current style, remember it
        if (i == IDD_PENSTYLE + (int)lpLP->lopnStyle)
            nSel = nIndex;
    }

    // Set the initial style selection
    SendMessage(hctlStyle, CB_SETCURSEL, nSel, 0L);

    // Initialize the Width edit control
    SetDlgItemInt(hdlg, IDD_PENWIDTH, lpLP->lopnWidth.x, FALSE);

    // Example window always uses 0 width (so styles work)
    lpLP->lopnWidth.x = 0;

    // Get coordinates of the example window in the dialog
    GetWindowRect(GetDlgItem(hdlg, IDD_PENEXAMPLE), &rcExample);
    ScreenToClient(hdlg, (LPPOINT)&rcExample);
    ScreenToClient(hdlg, ((LPPOINT)&rcExample) + 1);

    // Reduce rect slightly so we don't paint over its frame
    InflateRect(&rcExample, -1, -1);

    return TRUE;
}


//
//  FUNCTION: MsgPenPaint(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Paint the example window with examples of the current pen
//
//  PARAMETERS:
//    hwnd - The window handling the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns TRUE
//
//  COMMENTS:
//    Sets the initial state of the controls according to the LOGPEN
//    passed in via lparam.
//

LRESULT MsgPenPaint(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;
    HPEN hpen, hpenBlack;
    LPLOGPEN lpLP;
    int i, y, dy;

    BeginPaint(hdlg, &ps);
    SetBkMode(ps.hdc, TRANSPARENT);
    SelectObject(ps.hdc, GetStockObject(NULL_BRUSH));

    // Get pointer to LOGPEN from window bytes
    lpLP = (LPLOGPEN)GetWindowLong(hdlg, DWL_USER);


    // Draw some stuff in the example window with pens based on
    // the current LOGPEN (vary the width starting at 0).

    #define NUMLINES    5

    y = rcExample.top;
    dy = (rcExample.bottom - rcExample.top) / (NUMLINES + 1);
    lpLP->lopnWidth.x = 0;
    hpenBlack = GetStockObject(BLACK_PEN);

    for (i = 0; i < NUMLINES; i++)
    {
        lpLP->lopnWidth.x = i*3;
        hpen = CreatePenIndirect(lpLP);
        SelectObject(ps.hdc, hpen);

        MoveToEx(ps.hdc, rcExample.left + 10, y += dy, NULL);
        LineTo(ps.hdc, rcExample.right - 10, y);

        // Deselect and delete the pen
        SelectObject(ps.hdc, hpenBlack);
        DeleteObject(hpen);
    }

    EndPaint(hdlg, &ps);
    return 0;
}


//
//  FUNCTION: MsgPenCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND messages sent to the PenStyle box.
//
//  PARAMETERS:
//    hwnd      - The window handing the message.
//    uMessage  - The message number. (unused).
//    wparam    - Message specific data (unused).
//    lparam    - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    Uses this DipsCommand function defined in wndproc.c combined
//    with the cmdiPen structure defined in this file to handle
//    the command messages for the Pen dialog box.
//

LRESULT MsgPenCommand(HWND   hwnd, 
                      UINT   uMessage, 
                      WPARAM wparam, 
                      LPARAM lparam)
{
    return DispCommand(&cmdiPen, hwnd, wparam, lparam);
}


//
//  FUNCTION: CmdPenStyle(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Keeps track of which style button is selected.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - Child control ID (unused).
//    wNotify   - Child notification code.
//    hwndCtrl  - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//

LRESULT CmdPenStyle(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    LPLOGPEN lpLP;
    int nSel;

    // Get pointer to LOGPEN from window bytes
    lpLP = (LPLOGPEN)GetWindowLong(hdlg, DWL_USER);

    // Update the style selection if necessary.
    if (CBN_SELCHANGE == wNotify)
    {
        nSel = SendMessage(hwndCtrl, CB_GETCURSEL, 0, 0L);
        if (CB_ERR != nSel)
        {
            // Save new style
            lpLP->lopnStyle = SendMessage(hwndCtrl, CB_GETITEMDATA, nSel, 0L);

            // Repaint the example window
            InvalidateRect(hdlg, &rcExample, TRUE);
        }
    }

    return TRUE;
}


//
//  FUNCTION: CmdPenColor(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Puts up ChooseColor dialog to choose pen color.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - IDD_PENCOLOR (unused).
//    wNotify   - Child notification code (unused).
//    hwndCtrl  - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//

LRESULT CmdPenColor(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    LPLOGPEN lpLP;
    CHOOSECOLOR cc;
    static DWORD dwCustColors[16];

    // Get pointer to LOGPEN from window bytes
    lpLP = (LPLOGPEN)GetWindowLong(hdlg, DWL_USER);

    // Initialize CHOOSECOLOR struct
    cc.lStructSize      = sizeof(cc);
    cc.hwndOwner        = hdlg;
    cc.hInstance        = NULL;
    cc.rgbResult        = lpLP->lopnColor;
    cc.lpCustColors     = dwCustColors;
    cc.Flags            = CC_RGBINIT;
    cc.lCustData        = 0;
    cc.lpfnHook         = NULL;
    cc.lpTemplateName   = NULL;

    if (ChooseColor(&cc))
    {
        // Save new color
        lpLP->lopnColor = cc.rgbResult;

        // Repaint the example window
        InvalidateRect(hdlg, &rcExample, TRUE);
    }

    return TRUE;
}


//
//  FUNCTION: CmdPenDone(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Free the PenStyle box and related data.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - The command to be handled.
//    wNotify   - Notification code (unused).
//    hwndCtrl  - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdPenDone(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    LPLOGPEN lpLP;
    UINT nWidth;
    BOOL bTranslated;

    if (IDOK == wCommand)
    {
        // Get pointer to LOGPEN from window bytes
        lpLP = (LPLOGPEN)GetWindowLong(hdlg, DWL_USER);

        // Get pen width from edit control
        nWidth = GetDlgItemInt(hdlg, IDD_PENWIDTH, &bTranslated, FALSE);

        if (bTranslated)                // Did GetDlgItemInt succeed?
        {
            lpLP->lopnWidth.x = nWidth; // Yes, save new width.
            EndDialog(hdlg, TRUE);      // Exit the dialog (success)
        }
        else                            // Error
        {
            MessageBox(hdlg, "Pen Width must be a non-negative integer.",
                        "Pen Style", MB_ICONINFORMATION | MB_OK);
            SetDlgItemInt(hdlg, IDD_PENWIDTH, lpLP->lopnWidth.x, FALSE);
            SetFocus(GetDlgItem(hdlg, IDD_PENWIDTH));
        }
    }
    else    // Just close the dialog (cancel)
        EndDialog(hdlg, FALSE);

    return TRUE;
}
