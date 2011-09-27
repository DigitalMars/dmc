// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: printdlg.c
//
//  PURPOSE:
//    To show the use of the "Print" and "Print Setup" common dialog
//    boxes.
//
//  FUNCTIONS:
//    CmdFilePrint   - Present the print dialog to the user and process 
//                       the results.
//    CmdFilePrintSU -Present the print setup common dialog to the user.
//
//  COMMENTS:
//
//
//
//  SPECIAL INSTRUCTIONS: N/A
//

#include <windows.h>            // required for all Windows applications
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

static HANDLE hDevMode = NULL;
static HANDLE hDevNames = NULL;

//
//  FUNCTION: CmdFilePrint(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Present the print dialog to the user and process the results.
//
//  PARAMETERS:
//    hwnd     - The window handle.
//    wCommand - IDM_PRINT (Unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Command handled.
//
//  COMMENTS:
//
//

LRESULT CmdFilePrint(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    PRINTDLG pd = {0};

    static DWORD Flags     = PD_ALLPAGES | PD_SHOWHELP | PD_RETURNDC;
    static WORD  nFromPage = 0xFFFF;
    static WORD  nToPage   = 0xFFFF;
    static WORD  nCopies   = 1;

    DWORD  nPageRange = GetPageRange();

    pd.lStructSize = sizeof(pd);
    pd.hwndOwner   = hwnd;
    pd.hDevMode    = hDevMode;
    pd.hDevNames   = hDevNames;
    pd.Flags       = Flags;
    pd.nFromPage   = nFromPage;
    pd.nToPage     = nToPage;
    pd.nMinPage    = LOWORD(nPageRange);
    pd.nMaxPage    = HIWORD(nPageRange);
    pd.nCopies     = nCopies;

    if (PrintDlg(&pd))
    {
        Print(hwnd,
              pd.hDC,
              MAKEBOOL(pd.Flags & PD_PAGENUMS),
              MAKEBOOL(pd.Flags & PD_SELECTION),
              MAKEBOOL(pd.Flags & PD_COLLATE),
              MAKEBOOL(pd.Flags & PD_PRINTTOFILE),
              pd.nFromPage,
              pd.nToPage,
              pd.nCopies,
              pd.hDevNames);
        hDevMode   = pd.hDevMode;
        hDevNames  = pd.hDevNames;
        Flags      = pd.Flags;
        nFromPage  = pd.nFromPage;
        nToPage    = pd.nToPage;
    }
//    else
//    {
//        CommDlgExtendedError();
//    }

    return 0;
}


//
//  FUNCTION: CmdPrintSU(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Present the print setup common dialog to the user.
//
//  PARAMETERS:
//    hwnd     - The window handle.
//    wCommand - IDM_PRINTSU (Unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Command handled.
//
//  COMMENTS:
//
//

LRESULT CmdFilePrintSU(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    PRINTDLG pd = {0};

    pd.lStructSize = sizeof(pd);
    pd.hwndOwner   = hwnd;
    pd.hDevMode    = hDevMode;
    pd.hDevNames   = hDevNames;
    pd.Flags = PD_SHOWHELP | PD_PRINTSETUP;

    if (PrintDlg(&pd))
    {
        hDevMode = pd.hDevMode;
        hDevNames = pd.hDevNames;
    }

    return 0;
}
