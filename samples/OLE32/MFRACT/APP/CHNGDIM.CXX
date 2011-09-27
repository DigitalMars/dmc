//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       chngdim.cxx
//
//  Contents:   implementation for the Change Graph Dimension dialog box
//
//  Classes:    CChangeDim
//
//  Functions:
//
//  History:    4-20-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include "chngdim.h"


//+---------------------------------------------------------------------------
//
//  Member:     CChangeDim::DialogProc
//
//  Synopsis:   dialog proc for the change dimensions dialog box
//
//  Arguments:  [hwndDlg] - dialog's window handle
//              [uMsg]    - message
//              [wParam]  - first message parameter
//              [lParam]  - second message parameter
//
//  History:    4-20-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL CChangeDim::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    unsigned uImageWidth, uImageHeight;
    switch (uMsg)
    {
    case WM_INITDIALOG:
        _pwf->GetGraphSize(&uImageWidth, &uImageHeight);
        SetDlgItemInt(hwndDlg, IDC_HEIGHT, uImageHeight, FALSE);
        SetDlgItemInt(hwndDlg, IDC_WIDTH, uImageWidth, FALSE);
        CheckButtons(hwndDlg, uImageHeight, uImageWidth);
        return (TRUE);
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDC_VGA:
            SetDlgItemInt(hwndDlg, IDC_WIDTH, 640, FALSE);
            SetDlgItemInt(hwndDlg, IDC_HEIGHT, 480, FALSE);
            break;
        case IDC_SVGA:
            SetDlgItemInt(hwndDlg, IDC_WIDTH, 800, FALSE);
            SetDlgItemInt(hwndDlg, IDC_HEIGHT, 600, FALSE);
            break;
        case IDC_1024:
            SetDlgItemInt(hwndDlg, IDC_WIDTH, 1024, FALSE);
            SetDlgItemInt(hwndDlg, IDC_HEIGHT, 768, FALSE);
            break;
        case IDC_1280:
            SetDlgItemInt(hwndDlg, IDC_WIDTH, 1280, FALSE);
            SetDlgItemInt(hwndDlg, IDC_HEIGHT, 1024, FALSE);
            break;
        case IDC_QUICKDRAW:
            SetDlgItemInt(hwndDlg, IDC_WIDTH, QUICKWIDTH, FALSE);
            SetDlgItemInt(hwndDlg, IDC_HEIGHT, QUICKHEIGHT, FALSE);
            break;
        case IDC_HEIGHT:
        case IDC_WIDTH:
            CheckButtons(
                hwndDlg,
                GetDlgItemInt(hwndDlg, IDC_WIDTH, NULL, FALSE),
                GetDlgItemInt(hwndDlg, IDC_HEIGHT, NULL, FALSE));
            break;
        case IDC_RESET:
            uImageHeight = GetDlgItemInt(hwndDlg, IDC_HEIGHT, NULL, FALSE);
            uImageWidth = GetDlgItemInt(hwndDlg, IDC_WIDTH, NULL, FALSE);
            _pwf->SetGraphSize(uImageWidth, uImageHeight);
            EndDialog(hwndDlg,TRUE);
            return(TRUE);
        case IDC_RESIZE:
            uImageHeight = GetDlgItemInt(hwndDlg, IDC_HEIGHT, NULL, FALSE);
            uImageWidth = GetDlgItemInt(hwndDlg, IDC_WIDTH, NULL, FALSE);
            _pwf->SetGraphSize(uImageWidth, uImageHeight);
            EndDialog(hwndDlg,FALSE);
            return(TRUE);
        case IDCANCEL:
            EndDialog(hwndDlg,FALSE);
            return(TRUE);
        }
        break;
    }
    return (FALSE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CChangeDim::CheckButtons
//
//  Synopsis:   checks to correct check box in the dialog box for a given
//              set of dimensions
//
//  Arguments:  [hwndDlg] - dialog handle
//              [iWidth]   - width of the graph
//              [iHeight]  - height of the graph
//
//  History:    4-20-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CChangeDim::CheckButtons(HWND hwndDlg, int iWidth, int iHeight)
{
    if (iWidth == 640 && iHeight == 480)
    {
        CheckRadioButton(hwndDlg, IDC_QUICKDRAW, IDC_CUSTOM, IDC_VGA);
    }
    else if (iWidth == 800 && iHeight == 600)
    {
        CheckRadioButton(hwndDlg, IDC_QUICKDRAW, IDC_CUSTOM, IDC_SVGA);
    }
    else if (iWidth == 1024 && iHeight == 768)
    {
        CheckRadioButton(hwndDlg, IDC_QUICKDRAW, IDC_CUSTOM, IDC_1024);
    }
    else if (iWidth == 1280 && iHeight == 1024)
    {
        CheckRadioButton(hwndDlg, IDC_QUICKDRAW, IDC_CUSTOM, IDC_1280);
    }
    else if (iWidth == QUICKWIDTH && iHeight == QUICKHEIGHT)
    {
        CheckRadioButton(hwndDlg, IDC_QUICKDRAW, IDC_CUSTOM, IDC_QUICKDRAW);
    }
    else
    {
        CheckRadioButton(hwndDlg, IDC_QUICKDRAW, IDC_CUSTOM, IDC_CUSTOM);
    }
}

