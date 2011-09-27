// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: print.c
//
//  PURPOSE: Handle the application specific printing commands based on
//    parameters gathered from the common printer dialog box.
//
//  FUNCTIONS:
//    Print - To print a text document based on information from the common
//      print dialog.
//    AbortProc - Processes messages for the Abort Dialog box.
//    AbortDlg - Processes messages for printer abort dialog box.
//
//
//  COMMENTS:
//
//  SPECIAL INSTRUCTIONS: N/A
//


#include <windows.h>            // required for all Windows applications
#include "globals.h"            // prototypes specific to this application
#include "resource.h"
#include <richedit.h>
#include <string.h>


BOOL CALLBACK AbortProc(HDC, int);
LRESULT CALLBACK AbortDlg(HWND, UINT, WPARAM, LPARAM);

static HWND hdlgAbort = NULL;   // Abort dialog hanlde
static BOOL fAbort = FALSE;     // Abort Flag
static UINT LineSpace = 0;      // Height of line
static UINT clnPage = 0;        // Number of lines per page

//
//  FUNCTION:
//    Print(HWND, HDC, BOOL, BOOL, BOOL, BOOL, UINT, UINT, UINT, HGLOBAL)
//
//  PURPOSE: To print a text document based on information from the
//    common print dialog.
//
//  PARAMETERS:
//    hwnd - The window that the print message came from.
//    hdc - The display context to print to.
//    fPageNums - Print the page range specified by n[From|To]Page.
//    fSelection - Print the current selection.
//    fCollate - Print the pages in a collated order.
//    fFile - Print to a file.
//    nFromPage - First page to print.  (Valid only when fPageNums == TRUE)
//    nToPage - Last page to print.     (Valid only when fPageNums == TRUE)
//    hDevNames -
//
//  RETURN VALUE:
//    None.
//
//  COMMENTS:
//
//

VOID Print(HWND hwnd,
           HDC  hdc,
           BOOL fPageNums,
           BOOL fSelection,
           BOOL fCollate,
           BOOL fFile,
           UINT nFromPage,
           UINT nToPage,
           UINT nCopies,
           HGLOBAL hDevNames)
{
    HCURSOR hcurSave;
    DOCINFO di;
    UINT ipg;
    LONG lTextLength, lTextPrinted;
    FORMATRANGE fr;
    HDC hTargetDC;

    hcurSave = SetCursor(hcursHourGlass);

    fAbort = FALSE; // Clears the abort flag

    // Define the abort function
    SetAbortProc(hdc, (DLGPROC)AbortProc);

    // Start the printing session
    ZeroMemory(&di, sizeof(DOCINFO));
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = "PrntFile text";
    di.lpszOutput = NULL;

    if (StartDoc(hdc, &di) < 0)
    {
        DWORD dwErr = GetLastError();
        char szErr[128];
        wsprintf(szErr, "Unable to start print job- Error %ld, hdc = %ld", dwErr, (DWORD)hdc);
        SetCursor(hcurSave);    // Remove the hourglass
        MessageBox(hwnd, 
                   szErr, 
                   szAppName, 
                   MB_OK | MB_ICONHAND);
        DeleteDC(hdc);
        return;
    }

    // Create the Abort dialog box (modeless)
    hdlgAbort = CreateDialog(hInst, "AbortDlg", hwnd, (DLGPROC)AbortDlg);

    if (!hdlgAbort)
    {
        SetCursor(hcurSave);    // Remove the hourglass
        MessageBox(hwnd, 
                   "NULL Abort window handle", 
                   szAppName, 
                   MB_OK | MB_ICONHAND);
        DeleteDC(hdc);
        return;
    }

    // Now show Abort dialog
    ShowWindow (hdlgAbort, SW_NORMAL);

    // Disable the main window to avoid reentrancy problems
    EnableWindow(hwnd, FALSE);
    SetCursor(hcurSave);      // Remove the hourglass

    ipg = 1; // First page

    // Find out real size of document in characters
    lTextLength = SendMessage(GetEditWindow(NULL), WM_GETTEXTLENGTH, 0, 0);

    // Rendering to the same DC we are measuring

    hTargetDC = GetDC(GetEditWindow(NULL));

    fr.hdc = hdc;
    fr.hdcTarget = hTargetDC;

    // Set up a 7x9" printing area
    fr.rc.left = fr.rc.top = 0;
    fr.rc.right = 7*1440;  // 6" wide
    fr.rc.bottom = 9*1440; // 9" down
    fr.rcPage = fr.rc;

    // Default the range of text to print as the entire document
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;

    if (fSelection)
    {
        SendMessage (GetEditWindow(NULL), EM_EXGETSEL, 0, (LPARAM)&fr.chrg);
        if (fr.chrg.cpMax >= fr.chrg.cpMin)
            lTextLength = fr.chrg.cpMax;
    }

    StartPage (hdc);

    do
    {
        // Print as much as will fit on a page. The return value
        // is the index of the first character on the next page.
        lTextPrinted = SendMessage(GetEditWindow(NULL), 
                                   EM_FORMATRANGE, 
                                   FALSE,
                                   (LPARAM)&fr);

        if (!(fPageNums && ipg < nFromPage))
        {
            SendMessage(GetEditWindow(NULL), 
                        EM_DISPLAYBAND, 
                        0,
                        (LPARAM)&fr.rc);
        }

        // If there is more text to print, spit this page from the printer
        // and start another one.
        if (lTextPrinted < lTextLength)
        {
            if (!(fPageNums && (ipg < nFromPage || ipg >= nToPage)))
            {
                EndPage(hdc);
                StartPage(hdc);
            }

            // Adjust the range of characters to print starting at the
            // first character of the next page
            fr.chrg.cpMin = lTextPrinted;
        }
        ipg++;

        if (fPageNums && ipg > nToPage)
            break;
    }
    while ((lTextPrinted < lTextLength) && (!fAbort));

    // Restore the RTF Edit control to its original, upright position
    SendMessage (GetEditWindow(NULL), EM_FORMATRANGE, TRUE, (LPARAM)NULL);

    if (!fAbort)
    {
        EndPage(hdc);
        EndDoc(hdc);
    }

    ReleaseDC(GetEditWindow(NULL), hTargetDC);

    EnableWindow(hwnd, TRUE);

    // Destroy the Abort dialog box

    DestroyWindow(hdlgAbort);
    DeleteDC(hdc);
}

//
//  FUNCTION: AbortProc(HDC, int)
//
//  PURPOSE: Processes messages for the Abort Dialog box.
//
//  PARAMETERS:
//    hdc - The printer device context used to calculate the sizes.
//    Code -  The error code
//
//  RETURN VALUE:
//    Returns FALSE if the user has aborted, TRUE otherwise.
//
//  COMMENTS:
//
//

BOOL CALLBACK AbortProc(HDC hdc, int Code)
{
    MSG msg;

    if (!hdlgAbort)              // If the abort dialog isn't up yet
        return TRUE;

    // Process messages intended for the abort dialog box

    while (!fAbort && PeekMessage(&msg, NULL, 0, 0, TRUE))
        if (!IsDialogMessage(hdlgAbort, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    // fAbort is TRUE (return is FALSE) if the user has aborted

    return !fAbort;
}

//
//  FUNCTION: AbortDlg(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for printer abort dialog box.
//
//  PARAMETERS:
//    hdlg   - The dialog handle.
//    msg    - The message number.
//    wparam - Message specific data.
//    lparam -  Message specific data.
//
//  RETURN VALUE:
//    Depends on the message number.
//
//  COMMENTS:
//      This dialog box is created while the program is printing, and allows
//      the user to cancel the printing process.
//

LRESULT CALLBACK AbortDlg(HWND hdlg, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
        // Watch for Cancel button, RETURN key, ESCAPE key, or SPACE BAR

        case WM_COMMAND:
            return fAbort = TRUE;

        case WM_INITDIALOG:

            // Set the focus to the Cancel box of the dialog

            SetFocus(GetDlgItem(hdlg, IDCANCEL));
            SetDlgItemText(hdlg, IDC_FILENAME, (LPCTSTR)GetFName());
            return TRUE;
    }

    return FALSE;
}

