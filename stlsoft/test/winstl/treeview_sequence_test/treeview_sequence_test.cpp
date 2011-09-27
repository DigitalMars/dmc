/* /////////////////////////////////////////////////////////////////////////////
 * File:        Treeview_sequence_test.cpp
 *
 * Purpose:     Implementation file for the treeview_sequence_test project.
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
#include <commctrl.h>

#include <algorithm>

winstl_ns_using_std(for_each)

#include <functional>

#include <winstl_treeview_sequence.h>

winstl_ns_using(treeview_peer_sequence)
winstl_ns_using(treeview_child_sequence)
//winstl_ns_using(treeview_visible_sequence)

#include <winstl_control_functionals.h>

winstl_ns_using(listbox_back_inserter)

#include <winstl_windows_type_conversions.h>

winstl_ns_using(Id2StrT)
winstl_ns_using(WPARAM2HWND)
winstl_ns_using(LPARAM2HWND)
winstl_ns_using(LONG2HTREEITEM)

#include <stlsoft_string_access.h>

stlsoft_ns_using(c_str_ptr)

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

static BOOL CALLBACK Treeview_sequence_testDialogProc(  HWND    hwnd,
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
    ::InitCommonControls();

    ::DialogBoxParam(	hinst,
						Id2StrT(IDD_TREEVIEW_SEQUENCE_TEST),
						0,
						Treeview_sequence_testDialogProc,
						0);

    return 0;
}


static BOOL Treeview_sequence_test_OnInitDialog(HWND hwnd, HWND /* hwndFocus */, LPARAM /* lParam */)
{
	HWND			hwndTree	=	::GetDlgItem(hwnd, IDC_TREE);
	TV_INSERTSTRUCT	tvis;

	tvis.hParent		=	TVI_ROOT;
	tvis.hInsertAfter	=	TVI_LAST;
	tvis.item.mask		=	TVIF_TEXT;

	tvis.item.pszText	=	"Root item";
	tvis.hParent = TreeView_InsertItem(hwndTree, &tvis);

	tvis.item.pszText	=	"Item 1";
	TreeView_InsertItem(hwndTree, &tvis);

	tvis.item.pszText	=	"Item 2";
	TreeView_InsertItem(hwndTree, &tvis);

	tvis.item.pszText	=	"Item 3";
	TreeView_InsertItem(hwndTree, &tvis);

	tvis.item.pszText	=	"Item 4";
	tvis.hParent = TreeView_InsertItem(hwndTree, &tvis);

	tvis.item.pszText	=	"Item 4.1";
	TreeView_InsertItem(hwndTree, &tvis);

	tvis.item.pszText	=	"Item 4.2";
	TreeView_InsertItem(hwndTree, &tvis);

	tvis.item.pszText	=	"Item 4.3";
	TreeView_InsertItem(hwndTree, &tvis);

	tvis.item.pszText	=	"Item 4.4";
	TreeView_InsertItem(hwndTree, &tvis);

	::CheckDlgButton(hwnd, IDC_CHILDREN, 1);

    return false;
}

template <typename F>
struct htreeitem_adaptor
{
public:
	htreeitem_adaptor(F f, HWND hwndTree)
		: m_f(f)
		, m_hwndTree(hwndTree)
	{}

// Operations
public:
	void operator ()(HTREEITEM hitem)
	{
		CHAR	sz[201];
		TV_ITEM	item;

		item.mask		=	TVIF_TEXT;
		item.pszText	=	sz;
		item.cchTextMax	=	stlsoft_num_elements(sz);
		item.hItem		=	hitem;

		TreeView_GetItem(m_hwndTree, &item);

		m_f(sz);
	}

// Members
protected:
	F		m_f;
	HWND	m_hwndTree;
};

template <typename F, typename A>
inline htreeitem_adaptor<F> make_htreeitem_adaptor(F f, A a)
{
	return htreeitem_adaptor<F>(f, a);
}

static void DoTransfer(HWND hwnd)
{
	HWND	hwndList	=	::GetDlgItem(hwnd, IDC_LIST);
	HWND	hwndTree	=	::GetDlgItem(hwnd, IDC_TREE);

	::SendMessage(hwndList, LB_RESETCONTENT, 0, 0L);

	if(::IsDlgButtonChecked(hwnd, IDC_CHILDREN))
	{
		// Enumerating elements normally, so take the children of the root item
		HTREEITEM				hItem	=	LONG2HTREEITEM(::SendMessage(hwndTree, TVM_GETNEXTITEM, TVGN_CARET, 0));

		treeview_child_sequence	treeItems(hwndTree, hItem);

		for_each(treeItems.begin(), treeItems.end(), make_htreeitem_adaptor(listbox_back_inserter(hwndList), hwndTree));
	}
	else
	{
		// For some reason, using this at the global level makes GCC unhappy, so
		// do it here
		winstl_ns_using(treeview_visible_sequence)

		// Doing visible items, so no need to specify a root item
		treeview_visible_sequence	treeItems(hwndTree);

		for_each(treeItems.begin(), treeItems.end(), make_htreeitem_adaptor(listbox_back_inserter(hwndList), hwndTree));
	}
}

static BOOL Treeview_sequence_test_OnCommand(HWND hwnd, int /* code */, int idCtrl, HWND /* hwndCtrl */)
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
		case	IDC_TX:
			DoTransfer(hwnd);
			break;
    }

    return bRet;
}



BOOL CALLBACK Treeview_sequence_testDialogProc( HWND    hwnd,
                                                UINT    uMsg,
                                                WPARAM  wParam,
                                                LPARAM  lParam)
{
    BOOL    bRet    =   false;

    switch(uMsg)
    {
        case    WM_INITDIALOG:
            bRet = Treeview_sequence_test_OnInitDialog(hwnd, WPARAM2HWND(wParam), lParam);
            break;
        case    WM_COMMAND:
            bRet = Treeview_sequence_test_OnCommand(hwnd, (int)HIWORD(wParam), (int) LOWORD(wParam), LPARAM2HWND(lParam));
            break;
    }

    return bRet;
}

/* ////////////////////////////////////////////////////////////////////////// */
