
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*******************************************************************************
 *                                                                             *
 *  MODULE      : Print.c                                                      *
 *                                                                             *
 *  DESCRIPTION : Routines used for printing.                                  *
 *                                                                             *
 *  FUNCTIONS   : GetPrinterDC()   - Gets default printer from WIN.INI and     *
 *                                   creates a DC for it.                      *
 *                                                                             *
 *                InitPrinting()   - Initializes print job.                    *
 *                                                                             *
 *                TermPrinting()   - Terminates print job.                     *
 *                                                                             *
 *                PrintDlgProc()   - Dialog function for the "Cancel Printing" *
 *                                   dialog.                                   *
 *                                                                             *
 *                AbortProc()      - Peeks at message queue for messages from  *
 *                                   the print dialog.                         *
 *                                                                             *
 *******************************************************************************/

#include <windows.h>
#include <string.h>
#include <commdlg.h>
#include "showdib.h"

FARPROC  lpfnAbortProc    = NULL;
FARPROC  lpfnPrintDlgProc = NULL;
HWND     hWndParent       = NULL;
HWND     hDlgPrint        = NULL;
BOOL     bError;
BOOL     bUserAbort;


BOOL APIENTRY AbortProc (HDC, SHORT);
BOOL APIENTRY PrintDlgProc (HWND, WORD, UINT, DWORD);

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : GetPrinterDC()                                             *
 *                                                                          *
 *  PURPOSE    : Read WIN.INI for default printer and create a DC for it.   *
 *                                                                          *
 *  RETURNS    : A handle to the DC if successful or NULL otherwise.        *
 *                                                                          *
 ****************************************************************************/
HDC PASCAL GetPrinterDC() {

    PRINTDLG pd;

    pd.lStructSize = sizeof(PRINTDLG);
    pd.hDevMode = (HANDLE) NULL;
    pd.hDevNames = (HANDLE) NULL;
    pd.Flags = PD_RETURNDC;
    pd.hwndOwner = hWndApp;
    pd.hDC = (HDC) NULL;
    pd.nFromPage = 1;
    pd.nToPage = 1;
    pd.nMinPage = 0;
    pd.nMaxPage = 0;
    pd.nCopies = 1;
    pd.hInstance = (HANDLE) NULL;
    pd.lCustData = 0L;
    pd.lpfnPrintHook = (LPPRINTHOOKPROC) NULL;
    pd.lpfnSetupHook = (LPSETUPHOOKPROC) NULL;

    pd.lpPrintTemplateName = (LPSTR) NULL;
    pd.lpSetupTemplateName = (LPSTR)  NULL;
    pd.hPrintTemplate = (HANDLE) NULL;
    pd.hSetupTemplate = (HANDLE) NULL;

    // Display the PRINT dialog box. */

    if( PrintDlg(&pd) == TRUE ) {
        return( pd.hDC );        
    }
    else
        return( NULL );

}

//
// Getting the printer DC the old fashion way...
// called to enable or disable the IDM_PRINT menu item.
//
HDC PASCAL GetPrinterDC1()
{
    static CHAR szPrinter [80];
    CHAR    *szDevice, *szDriver, *szOutput;

    GetProfileString ("windows", "device", "", szPrinter, sizeof(szPrinter));

    if ((szDevice = strtok (szPrinter, "," )) &&
        (szDriver = strtok (NULL,      ", ")) &&
        (szOutput = strtok (NULL,      ", ")))

        return CreateDC (szDriver, szDevice, szOutput, NULL) ;

    return NULL;
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : InitPrinting(HDC hDC, HWND hWnd, HANDLE hInst, LPSTR msg)  *
 *                                                                          *
 *  PURPOSE    : Makes preliminary driver calls to set up print job.        *
 *                                                                          *
 *  RETURNS    : TRUE  - if successful.                                     *
 *               FALSE - otherwise.                                         *
 *                                                                          *
 ****************************************************************************/
BOOL PASCAL InitPrinting(HDC hDC, HWND hWnd, HANDLE hInst, LPSTR msg)
{
    DOCINFO         DocInfo;

    bError     = FALSE;     /* no errors yet */
    bUserAbort = FALSE;     /* user hasn't aborted */

    hWndParent = hWnd;      /* save for Enable at Term time */

    lpfnPrintDlgProc = (DLGPROC) MakeProcInstance (PrintDlgProc, hInst);
    lpfnAbortProc    = (WNDPROC) MakeProcInstance (AbortProc, hInst);

    hDlgPrint = CreateDialog (hInst, "PRTDLG", hWndParent, 
    	(DLGPROC)lpfnPrintDlgProc);

    if (!hDlgPrint)
        return FALSE;

    SetWindowText (hDlgPrint, msg);
    EnableWindow (hWndParent, FALSE);        /* disable parent */

//
// Use new printing APIs...Petrus Wong 12-May-1993
//
    if (SetAbortProc(hDC, (ABORTPROC)lpfnAbortProc) <= 0) {
        bError = TRUE;
        return FALSE;
    }

    DocInfo.cbSize      = sizeof(DOCINFO);
    DocInfo.lpszDocName = (LPTSTR) msg;
    DocInfo.lpszOutput  = NULL;

    if (StartDoc(hDC, &DocInfo) <= 0) {
        bError = TRUE;
        return FALSE;
    }
    bError = FALSE;

    /* might want to call the abort proc here to allow the user to
     * abort just before printing begins */
    return TRUE;
}
/****************************************************************************
 *                                                                          *
 *  FUNCTION   :  TermPrinting(HDC hDC)                                     *
 *                                                                          *
 *  PURPOSE    :  Terminates print job.                                     *
 *                                                                          *
 ****************************************************************************/
VOID PASCAL TermPrinting(HDC hDC)
{
//
// Use new printing APIs...Petrus Wong 12-May-1993
//
    if (!bError)
        EndDoc(hDC);

    if (bUserAbort)
        AbortDoc(hDC);
    else {
        EnableWindow(hWndParent, TRUE);
        DestroyWindow(hDlgPrint);
    }

    FreeProcInstance(lpfnAbortProc);
    FreeProcInstance(lpfnPrintDlgProc);
}
/****************************************************************************
 *                                                                          *
 *  FUNCTION   :PrintDlgProc (HWND, unsigned , WORD , DWORD )               *
 *                                                                          *
 *  PURPOSE    :Dialog function for the "Cancel Printing" dialog. It sets   *
 *              the abort flag if the user presses <Cancel>.                *
 *                                                                          *
 ****************************************************************************/
BOOL APIENTRY PrintDlgProc (HWND hDlg, WORD iMessage, UINT wParam, DWORD lParam)
{
    switch (iMessage) {
    case WM_INITDIALOG:

            EnableMenuItem (GetSystemMenu (hDlg, FALSE), (WORD)SC_CLOSE, (WORD)MF_GRAYED);
            break;

    case WM_COMMAND:
            bUserAbort = TRUE;
            EnableWindow (hWndParent, TRUE);
            DestroyWindow (hDlg);
            hDlgPrint = 0;
            break;

    default:
            return FALSE;
    }
    return TRUE;
        UNREFERENCED_PARAMETER(wParam);
        UNREFERENCED_PARAMETER(lParam);
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   :AbortProc (HDC hPrnDC, short nCode)                         *
 *                                                                          *
 *  PURPOSE    :Checks message queue for messages from the "Cancel Printing"*
 *              dialog. If it sees a message, (this will be from a print    *
 *              cancel command), it terminates.                             *
 *                                                                          *
 *  RETURNS    :Inverse of Abort flag                                       *
 *                                                                          *
 ****************************************************************************/
BOOL APIENTRY AbortProc (HDC hPrnDC, SHORT nCode)
{
    MSG   msg;

    while (!bUserAbort && PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
        if (!hDlgPrint || !IsDialogMessage(hDlgPrint, &msg)) {
            TranslateMessage (&msg);
            DispatchMessage (&msg);
        }
    }
    return !bUserAbort;
        UNREFERENCED_PARAMETER(hPrnDC);
        UNREFERENCED_PARAMETER(nCode);
}
