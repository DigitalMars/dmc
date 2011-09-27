/* /////////////////////////////////////////////////////////////////////////////
 * File:        Window_update_scope.cpp
 *
 * Purpose:     Implementation file for the window_update_scope_test project.
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

#include <winstl_window_update_scope.h>

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

static BOOL CALLBACK Window_update_scopeDialogProc(	HWND    hwnd,
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
    ::DialogBoxParam(hinst, Id2StrT(IDD_WINDOW_UPDATE_SCOPE), NULL, Window_update_scopeDialogProc, 0);

    return 0;
}


static BOOL Window_update_scope_OnInitDialog(HWND /* hwnd */, HWND /* hwndFocus */, LPARAM /* lParam */)
{
    return false;
}

static BOOL Window_update_scope_OnCommand(HWND hwnd, int /* code */, int idCtrl, HWND /* hwndCtrl */)
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
    }

    return bRet;
}



BOOL CALLBACK Window_update_scopeDialogProc(HWND    hwnd,
                                            UINT    uMsg,
                                            WPARAM  wParam,
                                            LPARAM  lParam)
{
    BOOL    bRet    =   false;

    switch(uMsg)
    {
        case    WM_INITDIALOG:
            bRet = Window_update_scope_OnInitDialog(hwnd, WPARAM2HWND(wParam), lParam);
            break;
        case    WM_COMMAND:
            bRet = Window_update_scope_OnCommand(hwnd, (int)HIWORD(wParam), (int) LOWORD(wParam), LPARAM2HWND(lParam));
            break;
    }

    return bRet;
}

/* ////////////////////////////////////////////////////////////////////////// */
