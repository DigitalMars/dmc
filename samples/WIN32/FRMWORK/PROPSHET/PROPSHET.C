// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   propshet.c
//
//  PURPOSE:   Implement a basic property sheet dialog.
//
//  FUNCTIONS:
//    WndProc      - Processes messages for the main window.
//    MsgCreate    - Handle the WM_CREATE message for the main window.
//    MsgCommand   - Handle the WM_COMMAND messages for the main window.
//    MsgPaint     - Handle the WM_PAINT message for the main window.
//    MsgDestroy   - Handles the WM_DESTROY message by calling 
//                   PostQuitMessage().
//    CmdExit      - Handles the file exit command by calling destory 
//                   window on the main window.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include "resource.h"


// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND, MsgCommand},
    {WM_PAINT,   MsgPaint},
    {WM_CREATE,  MsgCreate},
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
    {IDM_PROPSHET, CmdPropShet},
    {IDM_EXIT,  CmdExit},
    {IDM_ABOUT, CmdAbout}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};

HBRUSH ghBrushRed;  
HBRUSH ghBrushGreen;
HBRUSH ghBrushBlue; 
WORD   ghDlgBrushCtlID;

LRESULT CALLBACK BackgrndDialogProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ShapeDialogProc(HWND, UINT, WPARAM, LPARAM);
int DoPropertySheet(HWND);


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
//  PURPOSE: Handle the WM_CREATE messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_CREATE
//    wparam   - Not used
//    lparam)  - Not used
//
//  RETURN VALUE:
//    Always return 0
//
//  COMMENTS:
//    This function will create 2 brushes to be used for painting the
//    client area.
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    ghBrushRed   = CreateSolidBrush(RGB(255, 0, 0));
    ghBrushGreen = CreateSolidBrush(RGB(0, 255, 0));
    ghBrushBlue  = CreateSolidBrush(RGB(0, 0, 255));

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
//  FUNCTION: MsgPaint(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Paints the client area.
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
    HBRUSH      hBrush;
    WORD        wShape, x, y, wWidth, wHeight;

    if (GetProp(hwnd, "Brush") || GetProp(hwnd, "Shape"))
    {
        hDC = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);

        if (hBrush = (HBRUSH)GetProp(hwnd, "Brush"))
            FillRect(hDC, &rect, hBrush);

        if (wShape = (WORD)GetProp(hwnd, "Shape"))
        {
            switch(wShape)
            {
                case IDC_RECTSQUARE:   
                    wWidth  = 15;
                    wHeight = 15;
                    break;

                case IDC_TRAPEZOID:
                    wWidth  = 15;
                    wHeight = 15;
                    break;

                case IDC_RECTANGLE:
                    wWidth  = 25;
                    wHeight = 15;
                    break;

                default:
                    break;
            }

            //First draw the vertical lines...

            for (x = wWidth; x < rect.right; x += wWidth)
            {
                MoveToEx(hDC, x, 0, NULL);
                LineTo(hDC, x, rect.bottom);
                if (wShape == IDC_TRAPEZOID)
                    for (y = 0; y < rect.bottom; y += wHeight)
                    {
                        MoveToEx(hDC, x, y, NULL);
                        LineTo(hDC, x-wWidth, y+wHeight);
                    }
            }

            //Then draw the horizontal lines

            if (wShape != IDC_TRAPEZOID)
            {
                for (y = wHeight; y < rect.bottom; y += wHeight)
                {
                    MoveToEx(hDC, 0, y, NULL);
                    LineTo(hDC, rect.right, y);
                }
            }
        }

        EndPaint(hwnd, &ps);
        return 0;
    }
    else
        return DefWindowProc(hwnd, uMessage, wparam, lparam);
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
    if (GetProp(hwnd, "Brush"))
        RemoveProp(hwnd, "Brush");

    if (GetProp(hwnd, "Shape"))
        RemoveProp(hwnd, "Shape");

    if (ghBrushRed)   DeleteObject(ghBrushRed);
    if (ghBrushGreen) DeleteObject(ghBrushGreen);
    if (ghBrushBlue)  DeleteObject(ghBrushBlue);

    PostQuitMessage(0);

    return 0;
}


//
//  FUNCTION: CmdPropShet(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Bring up a property sheet dialog
//
//  PARAMETERS:
//    hwnd     - The owner window for the property sheet (main window in this case).
//    wCommand - IDM_PROPSHET (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdPropShet(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    if (-1 == DoPropertySheet(hwnd))
        MessageBox(hwnd,
                   "Failure creating Property Sheet!",
                   szAppName,
                   MB_OK);
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
//  FUNCTION: DoPropertySheet(HWND)
//
//  PURPOSE: Fills out the property sheet data structures and displays
//           the dialog with the property sheets.
//
//  PARAMETERS:
//
//    hwndOwner  - Parent window handle of the property sheets
//
//  RETURN VALUE:
//
//    Returns value from PropertySheet()
//
//  COMMENTS:
//
//

int DoPropertySheet(HWND hwndOwner)
{
    PROPSHEETPAGE psp[2];
    PROPSHEETHEADER psh;

    //Fill out the PROPSHEETPAGE data structure for the Background Color
    //sheet

    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[0].hInstance = hInst;
    psp[0].pszTemplate = MAKEINTRESOURCE(dlgbackcolor);
    psp[0].pszIcon = MAKEINTRESOURCE(IDI_BACKCOLOR);
    psp[0].pfnDlgProc = BackgrndDialogProc;
    psp[0].pszTitle = "Background Color";
    psp[0].lParam = 0;

    //Fill out the PROPSHEETPAGE data structure for the Client Area Shape
    //sheet

    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[1].hInstance = hInst;
    psp[1].pszTemplate = MAKEINTRESOURCE(dlgshape);
    psp[1].pszIcon = MAKEINTRESOURCE(IDI_SHAPES);
    psp[1].pfnDlgProc = ShapeDialogProc;
    psp[1].pszTitle = "Client Area Shape";
    psp[1].lParam = 0;

    //Fill out the PROPSHEETHEADER

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_USEICONID | PSH_PROPSHEETPAGE;
    psh.hwndParent = hwndOwner;
    psh.hInstance = hInst;
    psh.pszIcon = "";
    psh.pszCaption = (LPSTR)"Color Properties";
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.ppsp = (LPCPROPSHEETPAGE) &psp;

    //And finally display the dialog with the two property sheets.

    return PropertySheet(&psh);
}


//
//  FUNCTION: BackgrndDialogProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for background color property sheet.
//
//  PARAMETERS:
//    hdlg - window handle of the property sheet
//    wMessage - type of message
//    wparam - message-specific information
//    lparam - message-specific information
//
//  RETURN VALUE:
//    TRUE - message handled
//    FALSE - message not handled
//
//  COMMENTS:  In the interest of simplicity, we will stick to the
//             old style dialog procedure.  The point of this sample
//             is to demonstrate how to implement the PropertySheet
//             control.
//

LRESULT CALLBACK BackgrndDialogProc(HWND hdlg, 
								    UINT uMessage, 
								    WPARAM wparam, 
								    LPARAM lparam)
{
    HWND hWndApp;
    LPNMHDR lpnmhdr;

    switch (uMessage)
    {
        case WM_NOTIFY:
            lpnmhdr = (NMHDR FAR *)lparam;

            switch (lpnmhdr->code)
            {
                case PSN_SETACTIVE:
                    if (ghDlgBrushCtlID)
                        SendMessage(GetDlgItem(hdlg, ghDlgBrushCtlID),
                                    BM_SETCHECK,
                                    TRUE,
                                    0L);
                    else
                        SendMessage(GetDlgItem(hdlg, IDC_RED),
                                    BM_SETCHECK,
                                    TRUE,
                                    0L);
                    break;

                 default:
                    break;
            }

            break;

        case WM_COMMAND:
            hWndApp = GetParent(GetParent(hdlg));

            switch (wparam)
            {
                case IDC_RED:
                    ghDlgBrushCtlID = wparam;
                    SetProp(hWndApp, "Brush", (HBRUSH)ghBrushRed);
                    break;

                case IDC_GREEN:
                    ghDlgBrushCtlID = wparam;
                    SetProp(hWndApp, "Brush", (HBRUSH)ghBrushGreen);
                    break;

                case IDC_BLUE:
                    ghDlgBrushCtlID = wparam;
                    SetProp(hWndApp, "Brush", (HBRUSH)ghBrushBlue);
                    break;

                default:
                    break;
            }

            InvalidateRect(hWndApp, NULL, TRUE);
            UpdateWindow(hWndApp);
            break;

        default:
            break;
    }

    return FALSE;
}


//
//  FUNCTION: ShapeDialogProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Rectangle" property sheet.
//
//  PARAMETERS:
//    hdlg - window handle of the property sheet
//    wMessage - type of message
//    wparam - message-specific information
//    lparam - message-specific information
//
//  RETURN VALUE:
//    TRUE - message handled
//    FALSE - message not handled
//
//  COMMENTS:  In the interest of simplicity, we will stick to the
//             old style dialog procedure.  The point of this sample
//             is to demonstrate how to implement the PropertySheet
//             control.
//

LRESULT CALLBACK ShapeDialogProc(HWND hdlg, 
						 		 UINT uMessage, 
						 		 WPARAM wparam, 
								 LPARAM lparam)
{
    HWND hWndApp;
    WORD wID;
    LPNMHDR lpnmhdr;

    switch (uMessage)
    {
        case WM_NOTIFY:
            lpnmhdr = (NMHDR FAR *)lparam;
            hWndApp = GetParent(GetParent(hdlg));

            switch (lpnmhdr->code)
            {
                case PSN_SETACTIVE:
                    if (wID = (WORD)GetProp(hWndApp, "Shape"))
                        SendMessage(GetDlgItem(hdlg, wID),
                                    BM_SETCHECK,
                                    TRUE,
                                    0L);
                    break;

                 default:
                    break;
            }

            break;

        case WM_COMMAND:
            hWndApp = GetParent(GetParent(hdlg));

            switch (wparam)
            {
                case IDC_RECTSQUARE:
                case IDC_TRAPEZOID:
                case IDC_RECTANGLE:
                    SetProp(hWndApp, "Shape", (HANDLE)(WORD)wparam);
                    break;

                default:
                    break;
            }

            InvalidateRect(hWndApp, NULL, TRUE);
            UpdateWindow(hWndApp);
            break;

        default:
            break;
    }

    return FALSE;
}
