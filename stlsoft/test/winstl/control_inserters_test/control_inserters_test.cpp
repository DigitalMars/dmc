/* /////////////////////////////////////////////////////////////////////////////
 * File:        control_inserters_test.cpp
 *
 * Purpose:     Implementation file for the control_inserters_test project.
 *
 * Created:     8th October 2002
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
#include <commctrl.h>

#include <algorithm>
#include <functional>

#include <winstl_searchpath_sequence.h>
#include <winstl_control_functionals.h>
#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
    _MSC_VER >= 1100
# include <winstl_commctrl_functionals.h>
#endif /* !(_MSC_VER < 1100) */

#include <winstl_windows_type_conversions.h>

winstl_ns_using(Id2StrT)
winstl_ns_using(Ptr2LPARAM)
winstl_ns_using(WPARAM2HWND)
winstl_ns_using(LPARAM2HWND)

#include "resource.h"

/* ////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

winstl_ns_using(searchpath_sequence_a)

winstl_ns_using(listbox_front_inserter)
winstl_ns_using(listbox_back_inserter)
//winstl_ns_using(listbox_indexed_inserter)
winstl_ns_using(listbox_add_inserter)

winstl_ns_using(combobox_front_inserter)
winstl_ns_using(combobox_back_inserter)
//winstl_ns_using(combobox_indexed_inserter)
winstl_ns_using(combobox_add_inserter)

winstl_ns_using(listview_front_inserter)
winstl_ns_using(listview_back_inserter)

/* ////////////////////////////////////////////////////////////////////////////
 * Constants & definitions
 */

/* ////////////////////////////////////////////////////////////////////////////
 * Statics & globals
 */

/* ////////////////////////////////////////////////////////////////////////////
 * Forward declarations
 */

static BOOL CALLBACK Inserters_testDialogProc(	HWND    hwnd,
												UINT    uMsg,
												WPARAM  wParam,
												LPARAM  lParam);

/* ////////////////////////////////////////////////////////////////////////////
 * Functions
 */

#if defined(_STLSOFT_CF_COMPILER_IS_BORLAND) && \
    __BORLANDC__ >= 0x0560
int WINAPI __WinMain( HINSTANCE   hinst,
                    HINSTANCE   /* hinstPrev */,
                    LPSTR       /* lpszCmdLine */,
                    int         /* nCmdShow */)
#else
int WINAPI WinMain( HINSTANCE   hinst,
                    HINSTANCE   /* hinstPrev */,
                    LPSTR       /* lpszCmdLine */,
                    int         /* nCmdShow */)
#endif
{
    ::InitCommonControls();

	if(hinst == NULL)
	{
		hinst = ::GetModuleHandle(NULL);
	}

    ::DialogBoxParam(	hinst,
						Id2StrT(IDD_INSERTERS_TEST),
						NULL,
#ifdef STRICT
						Inserters_testDialogProc,
#else
						reinterpret_cast<DLGPROC>(Inserters_testDialogProc),
#endif /* STRICT */
						0);

    return 0;
}


static void InsertListbox(HWND hwndDlg, HWND hwndListbox)
{
	searchpath_sequence_a	sequence;

	::SendMessage(hwndListbox, LB_RESETCONTENT, 0, 0L);

	if(::IsDlgButtonChecked(hwndDlg, IDC_LISTBOX_FRONT))
	{
		stlsoft_ns_qual_std(for_each)(sequence.begin(), sequence.end(), listbox_front_inserter(hwndListbox));
	}
	else if(::IsDlgButtonChecked(hwndDlg, IDC_LISTBOX_BACK))
	{
		stlsoft_ns_qual_std(for_each)(sequence.begin(), sequence.end(), listbox_back_inserter(hwndListbox));
	}
	else if(::IsDlgButtonChecked(hwndDlg, IDC_LISTBOX_ADD))
	{
		stlsoft_ns_qual_std(for_each)(sequence.begin(), sequence.end(), listbox_add_inserter(hwndListbox));
	}
}

static void InsertCombobox(HWND hwndDlg, HWND hwndCombobox)
{
	searchpath_sequence_a	sequence;

	::SendMessage(hwndCombobox, CB_RESETCONTENT, 0, 0L);

	if(::IsDlgButtonChecked(hwndDlg, IDC_COMBOBOX_FRONT))
	{
		stlsoft_ns_qual_std(for_each)(sequence.begin(), sequence.end(), combobox_front_inserter(hwndCombobox));
	}
	else if(::IsDlgButtonChecked(hwndDlg, IDC_COMBOBOX_BACK))
	{
		stlsoft_ns_qual_std(for_each)(sequence.begin(), sequence.end(), combobox_back_inserter(hwndCombobox));
	}
	else if(::IsDlgButtonChecked(hwndDlg, IDC_COMBOBOX_ADD))
	{
		stlsoft_ns_qual_std(for_each)(sequence.begin(), sequence.end(), combobox_add_inserter(hwndCombobox));
	}
}

#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
    _MSC_VER >= 1100
static void InsertListview(HWND hwndDlg, HWND hwndListview)
{
	searchpath_sequence_a	sequence;

	::SendMessage(hwndListview, LVM_DELETEALLITEMS, 0, 0L);

	if(::IsDlgButtonChecked(hwndDlg, IDC_LISTVIEW_FRONT))
	{
		stlsoft_ns_qual_std(for_each)(sequence.begin(), sequence.end(), listview_front_inserter(hwndListview));
	}
	else if(::IsDlgButtonChecked(hwndDlg, IDC_LISTVIEW_BACK))
	{
		stlsoft_ns_qual_std(for_each)(sequence.begin(), sequence.end(), listview_back_inserter(hwndListview));
	}
}
#endif /* !(_MSC_VER < 1100) */


static BOOL Inserters_test_OnInitDialog(HWND hwnd, HWND /* hwndFocus */, LPARAM /* lParam */)
{
	LV_COLUMN	column;

	column.mask		=	LVCF_TEXT | LVCF_WIDTH;
	column.cx		=	100;
	column.pszText	=	"Items";

	::SendMessage(::GetDlgItem(hwnd, IDC_LISTVIEW), LVM_INSERTCOLUMNA, 0, Ptr2LPARAM(&column));

	::CheckDlgButton(hwnd, IDC_LISTBOX_FRONT, 1);
	::CheckDlgButton(hwnd, IDC_COMBOBOX_FRONT, 1);
	::CheckDlgButton(hwnd, IDC_LISTVIEW_FRONT, 1);

    return false;
}

static BOOL Inserters_test_OnCommand(HWND hwnd, int /* code */, int idCtrl, HWND /* hwndCtrl */)
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
		case	IDC_LISTBOX_BTN:
			InsertListbox(hwnd, ::GetDlgItem(hwnd, IDC_LISTBOX));
			break;
		case	IDC_COMBOBOX_BTN:
			InsertCombobox(hwnd, ::GetDlgItem(hwnd, IDC_COMBOBOX));
			break;
#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
    _MSC_VER >= 1100
		case	IDC_LISTVIEW_BTN:
			InsertListview(hwnd, ::GetDlgItem(hwnd, IDC_LISTVIEW));
			break;
#endif /* !(_MSC_VER < 1100) */
    }

    return bRet;
}


BOOL CALLBACK Inserters_testDialogProc(	HWND    hwnd,
                                        UINT    uMsg,
                                        WPARAM  wParam,
                                        LPARAM  lParam)
{
    BOOL    bRet    =   false;

    switch(uMsg)
    {
        case    WM_INITDIALOG:
            bRet = Inserters_test_OnInitDialog(hwnd, WPARAM2HWND(wParam), lParam);
            break;
        case    WM_COMMAND:
            bRet = Inserters_test_OnCommand(hwnd, (int)HIWORD(wParam), (int) LOWORD(wParam), LPARAM2HWND(lParam));
            break;
    }

    return bRet;
}

/* ////////////////////////////////////////////////////////////////////////// */
