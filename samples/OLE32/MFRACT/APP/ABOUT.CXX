//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       about.cxx
//
//  Contents:   implementation for the about dialog box
//
//  Classes:    CAbout
//
//  Functions:
//
//  History:    4-12-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include "about.h"


//+---------------------------------------------------------------------------
//
//  Member:     CAbout::DialogProc
//
//  Synopsis:   dialog proc for the About dialog box
//
//  Arguments:  [uMsg]   - message
//              [wParam] - first message parameter
//              [lParam] - second message parameter
//
//  History:    4-12-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL CAbout::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        return(TRUE);
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK
            || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hwndDlg, TRUE);
            return(TRUE);
        }
    }
    return(FALSE);
}

