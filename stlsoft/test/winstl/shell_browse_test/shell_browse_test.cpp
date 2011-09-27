/* /////////////////////////////////////////////////////////////////////////////
 * File:        Shell_browse_test.cpp
 *
 * Purpose:     Implementation file for the shell_browse_test project.
 *
 * Created:     10th December 2002
 * Updated:     22nd April 2004
 * Author:      Synesis Software (Pty) Ltd (C) 2002. All rights reserved.
 *              (www.synesis-group.com/software).
 *
 * Status:      Third-party
 *
 * Copyright:   Synesis Software (Pty) Ltd provides the source code contained in
 *              this file/document free-of-charge according to the following
 *              conditions:
 *
 *              (i) No warranty, expressed or implied, is given as to the
 *              correctness or efficiency of the code.
 *              (ii) No part of this code may be modified or copied, by whatever
 *              means without including a copy of this notice in its entirety
 *              (this means the all content between the <<START:>> and <<:END>>
 *              tokens inclusive, including, and not limited to, purpose, date
 *              of creation, date of last update, authorship, address, copyright
 *              and status).
 *              (iii) This code, in whole or part, remains the property of
 *              Synesis Software (Pty) Ltd, whether incorporated as-is, or in
 *              modified form.
 *
 * ////////////////////////////////////////////////////////////////////////// */


/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <winstl.h>

#include <winstl_shell_browse.h>

winstl_ns_using(shell_browse)

#include <winstl_windows_type_conversions.h>

winstl_ns_using(Id2StrT)
winstl_ns_using(WPARAM2HWND)
winstl_ns_using(LPARAM2HWND)

#include "resource.h"

/* ////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

/* ////////////////////////////////////////////////////////////////////////////
 * Constants & definitions
 */

/* ////////////////////////////////////////////////////////////////////////////
 * Statics & globals
 */

/* ////////////////////////////////////////////////////////////////////////////
 * Forward declarations
 */

static BOOL CALLBACK Shell_browse_testDialogProc(	HWND    hwnd,
													UINT    uMsg,
													WPARAM  wParam,
													LPARAM  lParam);

/* ////////////////////////////////////////////////////////////////////////////
 * Functions
 */

int WINAPI WinMain( HINSTANCE   hinst,
                    HINSTANCE   /* hinstPrev */,
                    LPSTR       /* lpszCmdLine */,
                    int         /* nCmdShow */)
{
    ::DialogBoxParam(hinst, Id2StrT(IDD_SHELL_BROWSE_TEST), NULL, Shell_browse_testDialogProc, 0);

    return 0;
}


static BOOL Shell_browse_test_OnInitDialog(HWND /* hwnd */, HWND /* hwndFocus */, LPARAM /* lParam */)
{
    return false;
}

static void OnBrowse(HWND hwnd)
{
	TCHAR	szPath[_MAX_PATH + 1];

//	if(shell_browse("Select a folder", szPath))
	if(shell_browse("Select a folder", szPath, (UINT)0, hwnd))
	{
		::SetDlgItemText(hwnd, IDC_PATH, szPath);
	}
}

static BOOL Shell_browse_test_OnCommand(HWND hwnd, int /* code */, int idCtrl, HWND /* hwndCtrl */)
{
    BOOL    bRet    =   false;

    switch(idCtrl)
    {
        case    IDOK:
			/* Fall through */
        case    IDCANCEL:
            ::EndDialog(hwnd, idCtrl);
            bRet = true;
            break;
		case	IDC_BROWSE:
			OnBrowse(hwnd);
			break;
    }

    return bRet;
}



BOOL CALLBACK Shell_browse_testDialogProc(  HWND    hwnd,
                                            UINT    uMsg,
                                            WPARAM  wParam,
                                            LPARAM  lParam)
{
    BOOL    bRet    =   false;

    switch(uMsg)
    {
        case    WM_INITDIALOG:
            bRet = Shell_browse_test_OnInitDialog(hwnd, WPARAM2HWND(wParam), lParam);
            break;
        case    WM_COMMAND:
            bRet = Shell_browse_test_OnCommand(hwnd, (int)HIWORD(wParam), (int) LOWORD(wParam), LPARAM2HWND(lParam));
            break;
    }

    return bRet;
}

/* ////////////////////////////////////////////////////////////////////////// */
