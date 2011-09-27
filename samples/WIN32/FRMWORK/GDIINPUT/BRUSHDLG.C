// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   brushdlg.c
//
//  PURPOSE:   Displays the "Brush Style" dialog box
//
//  FUNCTIONS:
//    BrushDlg        - Process messages for "Brush Style" dialog box.
//    MsgBrushInit    - Initialize the Brush dialog with info from lparam.
//    MsgBrushPaint   - Paint the Example Window in the Brush dialog
//    MsgBrushCommand - Process WM_COMMAND messages sent to the Brush dialog.
//    CmdBrushStyle   - Track the currently selected brush style.
//    CmdBrushHatch   - Track the currently selected hatch style.
//    CmdBrushColor   - Put up the ChooseColor dialog to select brush color.
//    CmdBrushDone    - Free the Brush dialog and related data.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include "brushdlg.h"           // Controls ID's for the Brush dialog

// global variables specific to this module
RECT rcExample;                 // location of example window in dialog

// prototypes specific to this module
LRESULT MsgBrushCommand (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgBrushInit    (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgBrushPaint   (HWND, UINT, WPARAM, LPARAM);
LRESULT CmdBrushStyle   (HWND, WORD, WORD, HWND);
LRESULT CmdBrushHatch   (HWND, WORD, WORD, HWND);
LRESULT CmdBrushColor   (HWND, WORD, WORD, HWND);
LRESULT CmdBrushDone    (HWND, WORD, WORD, HWND);

// Brush dialog message table definition.
MSD rgmsdBrush[] =
{
    {WM_COMMAND,    MsgBrushCommand},
    {WM_PAINT,      MsgBrushPaint},
    {WM_INITDIALOG, MsgBrushInit}
};

MSDI msdiBrush =
{
    sizeof(rgmsdBrush) / sizeof(MSD),
    rgmsdBrush,
    edwpNone
};

// Brush dialog command table definition.
CMD rgcmdBrush[] =
{
    {IDD_SOLIDBRUSH,    CmdBrushStyle}, // Brush Style notifications
    {IDD_NULLBRUSH,     CmdBrushStyle},
    {IDD_HATCHBRUSH,    CmdBrushStyle},
    {IDD_HATCHSTYLE,    CmdBrushHatch}, // Hatch Style notification
    {IDD_BRUSHCOLOR,    CmdBrushColor}, // Color button
    {IDOK,              CmdBrushDone},  // OK and Cancel buttons
    {IDCANCEL,          CmdBrushDone}
};

CMDI cmdiBrush =
{
    sizeof(rgcmdBrush) / sizeof(CMD),
    rgcmdBrush,
    edwpNone
};


//
//  FUNCTION: BrushDlg(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Brush Style" dialog box.
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

LRESULT CALLBACK BrushDlg(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiBrush, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgBrushCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND messages sent to the Brush dialog.
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
//    with the cmdiBrush structure defined in this file to handle
//    the command messages for the Brush dialog box.
//

LRESULT MsgBrushCommand(HWND   hwnd, 
                        UINT   uMessage, 
                        WPARAM wparam, 
                        LPARAM lparam)
{
    return DispCommand(&cmdiBrush, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgBrushInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the Brush dialog with info from lparam.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - points to LOGBRUSH structure.
//
//  RETURN VALUE:
//    Always returns TRUE
//
//  COMMENTS:
//    Sets the initial state of the controls according to the LOGBRUSH
//    passed in via lparam.
//

LRESULT MsgBrushInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int i, nIndex, nSel;
    HWND hctlHatch;
    char szTmp[32];
    LPLOGBRUSH lpLB;

    // lparam is a pointer to a LOGBRUSH structure
    lpLB = (LPLOGBRUSH)lparam;

    // Save pointer to LOGBRUSH structure in window bytes
    SetWindowLong(hdlg, DWL_USER, (LONG)lpLB);

    // Check the correct brush style button
    CheckRadioButton(hdlg,
                     IDD_BRUSHFIRST,
                     IDD_BRUSHLAST,
                     IDD_BRUSHSTYLE + lpLB->lbStyle);

    // Fill up the hatch style combobox.  The item data for each item is
    // set to the hatch style value defined by Windows (e.g. HS_DIAGCROSS).

    hctlHatch = GetDlgItem(hdlg, IDD_HATCHSTYLE);
    nSel = 0;

    for (i = IDD_HATCHFIRST; i <= IDD_HATCHLAST; i++)
    {
        LoadString(hInst, i, szTmp, sizeof(szTmp));
        nIndex = SendMessage(hctlHatch, CB_ADDSTRING, 0, (LPARAM)(LPSTR)szTmp);
        SendMessage(hctlHatch, CB_SETITEMDATA, nIndex, i - IDD_HATCHSTYLE);

        // If this item is the current style, remember it
        if (i == IDD_HATCHSTYLE + (int)lpLB->lbHatch)
            nSel = nIndex;
    }

    // Set the initial hatch selection
    SendMessage(hctlHatch, CB_SETCURSEL, nSel, 0L);

    // Center the dialog over the application window
    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));

    // Get coordinates of the example window in the dialog
    GetWindowRect(GetDlgItem(hdlg, IDD_BRUSHEXAMPLE), &rcExample);
    ScreenToClient(hdlg, (LPPOINT)&rcExample);
    ScreenToClient(hdlg, ((LPPOINT)&rcExample) + 1);

    // Reduce rect slightly so we don't paint over its frame
    InflateRect(&rcExample, -1, -1);

    return 1;
}


//
//  FUNCTION: MsgBrushPaint(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Paint the example window with the current brush style
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
//

LRESULT MsgBrushPaint(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;
    LPLOGBRUSH lpLB;
    HBRUSH hbr;

    BeginPaint(hdlg, &ps);

    // Get pointer to LOGBRUSH from window bytes
    lpLB = (LPLOGBRUSH)GetWindowLong(hdlg, DWL_USER);

    // Create brush from current LOGBRUSH
    hbr = CreateBrushIndirect(lpLB);

    // Paint the example window with the brush.
    FillRect(ps.hdc, &rcExample, hbr);

    // Don't need the brush any more
    DeleteObject(hbr);

    EndPaint(hdlg, &ps);
    return 0;
}


//
//  FUNCTION: CmdBrushStyle(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Keeps track of which style button is selected.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - Child control ID.
//    wNotify   - Child notification code (unused).
//    hwndCtrl  - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//

LRESULT CmdBrushStyle(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    // Get pointer to LOGBRUSH from window bytes
    LPLOGBRUSH lpLB = (LPLOGBRUSH)GetWindowLong(hdlg, DWL_USER);

    // Save the style of the button that was just clicked
    lpLB->lbStyle = wCommand - IDD_BRUSHSTYLE;

    // Repaint the example window
    InvalidateRect(hdlg, &rcExample, TRUE);

    return TRUE;
}


//
//  FUNCTION: CmdBrushHatch(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Keeps track of which hatch style is selected.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - Child control ID (unused).
//    wNotify   - Child notification code.
//    hwndCtrl  - Handle to the Hatch Style combobox.
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//

LRESULT CmdBrushHatch(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    LPLOGBRUSH lpLB;
    int nSel;

    // Update the hatch style selection if necessary.

    if (CBN_SELCHANGE == wNotify)
    {
        // Get pointer to LOGBRUSH from window bytes
        lpLB = (LPLOGBRUSH)GetWindowLong(hdlg, DWL_USER);

        nSel = SendMessage(hwndCtrl, CB_GETCURSEL, 0, 0L);
        if (CB_ERR != nSel)
        {
            // Set brush style to hatched
            lpLB->lbStyle = BS_HATCHED;
            CheckRadioButton(hdlg,
                             IDD_BRUSHFIRST,
                             IDD_BRUSHLAST,
                             IDD_HATCHBRUSH);

            // Save new hatch style
            lpLB->lbHatch = SendMessage(hwndCtrl, CB_GETITEMDATA, nSel, 0L);

            // Repaint the example window
            InvalidateRect(hdlg, &rcExample, TRUE);
        }
    }

    return TRUE;
}


//
//  FUNCTION: CmdBrushColor(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Puts up ChooseColor dialog to choose brush color.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - IDD_BRUSHCOLOR (unused).
//    wNotify   - Child notification code (unused).
//    hwndCtrl  - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//

LRESULT CmdBrushColor(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    LPLOGBRUSH lpLB;
    CHOOSECOLOR cc;
    static DWORD dwCustColors[16];

    // Get pointer to LOGBRUSH from window bytes
    lpLB = (LPLOGBRUSH)GetWindowLong(hdlg, DWL_USER);

    // Initialize CHOOSECOLOR struct
    cc.lStructSize      = sizeof(cc);
    cc.hwndOwner        = hdlg;
    cc.hInstance        = NULL;
    cc.rgbResult        = lpLB->lbColor;
    cc.lpCustColors     = dwCustColors;
    cc.Flags            = CC_RGBINIT;
    cc.lCustData        = 0;
    cc.lpfnHook         = NULL;
    cc.lpTemplateName   = NULL;

    if (ChooseColor(&cc))
    {
        // Save new color
        lpLB->lbColor = cc.rgbResult;

        // Repaint the example window
        InvalidateRect(hdlg, &rcExample, TRUE);
    }

    return TRUE;
}


//
//  FUNCTION: CmdBrushDone(HWND, WORD, HWND)
//
//  PURPOSE: Free the Brush dialog and related data.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - The command to be handled.
//    hwndCtrl  - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdBrushDone(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    // Exit the dialog
    EndDialog(hdlg, (IDOK == wCommand));

    return TRUE;
}
