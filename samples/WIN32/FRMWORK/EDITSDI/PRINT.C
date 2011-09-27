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
//    CalcPageSize - To calculate the number of lines per page & height of a
//      line.
//    PrintPage - To print a single page.
//    GetPageRange - To get the range of pages of a document.
//    AbortProc - Processes messages for the Abort Dialog box.
//    AbortDlg - Processes messages for printer abort dialog box.
//
//
//  COMMENTS:
//
//  SPECIAL INSTRUCTIONS: N/A
//


#include <windows.h>            // required for all Windows applications
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "resource.h"
#include <string.h>


BOOL CALLBACK AbortProc(HDC, int);
LRESULT CALLBACK AbortDlg(HWND, UINT, WPARAM, LPARAM);
VOID CalcPageSize(HDC);
VOID PrintPage(HDC, UINT, UINT, UINT, UINT);

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
    UINT    ilnStartSel = 0;
    UINT    cln;
    UINT    ipg;
    UINT    ipgMin;
    UINT    ipgMax;

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
        SetCursor(hcurSave);    // Remove the hourglass
        MessageBox(hwnd, 
                   "Unable to start print job", 
                   NULL, 
                   MB_OK | MB_ICONHAND);
        DeleteDC(hdc);
    }

    // Create the Abort dialog box (modeless)
    hdlgAbort = CreateDialog(hInst, "AbortDlg", hwnd, (DLGPROC)AbortDlg);

    if (!hdlgAbort)
    {
        SetCursor(hcurSave);    // Remove the hourglass
        MessageBox(hwnd, 
                   "NULL Abort window handle", 
                   NULL, 
                   MB_OK | MB_ICONHAND);
        DeleteDC(hdc);
        return;
    }

    // Now show Abort dialog
    ShowWindow (hdlgAbort, SW_NORMAL);

    // Disable the main window to avoid reentrancy problems
    EnableWindow(hwnd, FALSE);
    SetCursor(hcurSave);      // Remove the hourglass

    CalcPageSize(hdc);

    // You can output only one line at a time, so you need a count of the
    //  number of lines to print.  You can retrieve the count sending the
    //  EM_GETLINECOUNT message to the edit control.

    // Calculate the first and last page to be printed

    if (fPageNums)
    {
        ipgMin = nFromPage - 1;
        ipgMax = nToPage;
        cln = ipgMax * clnPage;
    }
    else if (fSelection)
    {
        UINT ichStart;
        UINT ichEnd;

        GETSEL(ichStart, ichEnd);

        ilnStartSel = (UINT) SendMessage(GetEditWindow(), 
                                         EM_LINEFROMCHAR, 
                                         ichStart, 
                                         0L);
        ipgMin = ilnStartSel / clnPage;
        cln = (UINT) SendMessage(GetEditWindow(), 
                                 EM_LINEFROMCHAR, 
                                 ichEnd, 
                                 0L) - ilnStartSel + 1;
        ilnStartSel = ilnStartSel % clnPage;
        ipgMax = ipgMin + cln / clnPage + 1;
    }
    else
    {
        cln = (UINT)SendMessage(GetEditWindow(), EM_GETLINECOUNT, 0, 0L);
        ipgMin = 0;
        ipgMax = cln / clnPage + 1;
    }

    do
    {
        for (ipg = ipgMin; ipg < ipgMax && !fAbort; ipg++)
        {
            // If we are printing the entire document, or just a range
            // of pages, ilnStartSel will be zero.  Otherwise, it's
            // the 0 based offset (from the top of the given page) to 
            // the first line to print 

            UINT ilnFirst = ilnStartSel;
            UINT ilnLast  = ilnFirst + clnPage - 1;

            // Special case last page
            if (ipg == ipgMax-1)
            {
                ilnLast = ilnFirst + ((cln-1) % clnPage);
            }

            PrintPage(hdc, ipg, ilnFirst, ilnLast, fCollate ? 1 : nCopies);
        }
    } while (fCollate && --nCopies > 0);
    EndDoc(hdc);

    EnableWindow(hwnd, TRUE);

    // Destroy the Abort dialog box

    DestroyWindow(hdlgAbort);
    DeleteDC(hdc);
}

//
//  FUNCTION: CalcPageSize(HDC)
//
//  PURPOSE: To calculate the number of lines per page & height of a line.
//
//  PARAMETERS:
//    hdc - The printer device context used to calculate the sizes.
//
//  RETURN VALUE:
//    NONE
//
//  COMMENTS:
//    Sets the module static variable LineSpace and clnPage to the
//    Height of a line and the number of lines per page, respectively.
//

VOID CalcPageSize(HDC hdc)
{
    TEXTMETRIC tm;
    UINT nPageSize;

    // Since you may have more than one line, you need to
    //   compute the spacing between lines.  You can do that by
    //   retrieving the height of the characters you are printing
    //   and advancing their height plus the recommended external
    //   leading height.

    GetTextMetrics(hdc, &tm);
    LineSpace = tm.tmHeight + tm.tmExternalLeading;

    // Since you may have more lines than can fit on one page, you need
    //  to compute the number of lines you can print per page.  You can
    //  do that by retrieving the dimensions of the page and dividing
    //  the height by the line spacing.

    nPageSize = GetDeviceCaps(hdc, VERTRES);
    clnPage = nPageSize / LineSpace - 1;
}

//
//  FUNCTION: PrintPage(HDC, UING, UINJT, UINT, UINT)
//
//  PURPOSE: To print a single page.
//
//  PARAMETERS:
//    hdc - The printer device context to print on.
//    ipg - The zero based index of the page to print.
//    iln - The zero based index of the first line on the page to print.
//    ilnEnd - The zero based index of the last line on the page to print.
//    nCopies - The number of copies to print.
//
//  RETURN VALUE:
//    NONE
//
//  COMMENTS:
//
//

VOID PrintPage(HDC hdc, UINT ipg, UINT ilnStart, UINT ilnEnd, UINT nCopies)
{
    UINT ilnBase = ipg * clnPage;

    while (nCopies-- > 0)
    {
        UINT iln = 0;

        StartPage(hdc);
        while(iln + ilnStart <= ilnEnd)
        {
            char szLine[128];
            int  cchLine;

            szLine[0] = sizeof(szLine)-1;          // Maximum buffer size
            szLine[1] = 0;
            cchLine = (int)SendMessage(GetEditWindow(),
                                       EM_GETLINE,
                                       ilnBase + iln + ilnStart,
                                       (DWORD)(LPSTR)szLine);
            TextOut(hdc, 0, iln*LineSpace, (LPSTR)szLine, cchLine);
            iln += 1;
        }
        EndPage(hdc);
    }
}

//
//  FUNCTION: GetPageRange(VOID);
//
//  PURPOSE: To get the range of pages of a document.
//
//  PARAMETERS:
//
//  RETURN VALUE:
//
//  COMMENTS:
//
//

DWORD GetPageRange(VOID)
{
    return (DWORD) MAKELONG(1,0xFFFF);
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
#ifdef WIN16
            SetDlgItemText(hdlg, IDC_FILENAME, (LPSTR) GetFName());
#else
            SetDlgItemText(hdlg, IDC_FILENAME, (LPCTSTR) GetFName());
#endif
            return TRUE;
    }
    return FALSE;
}
