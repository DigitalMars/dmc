/* /////////////////////////////////////////////////////////////////////////////
 * File:        drophandle_sequence_test.cpp
 *
 * Purpose:     Implementation file for the drophandle_sequence_test project.
 *
 * Created:     1st May 2003
 * Updated:     22nd April 2004
 *
 * Status:      Wizard-generated
 *
 * License:     (Licensed under the Synesis Software Open License)
 *
 *              Copyright (C) 1999-2003, Synesis Software Pty Ltd.
 *              All rights reserved.
 *
 *              www:        http://www.synesis.com.au/software
 *
 *              email:      software@synesis.com.au
 *
 *              This source code is placed into the public domain 2003
 *              by Synesis Software Pty Ltd. There are no restrictions
 *              whatsoever to your use of the software.
 *
 *              This source code is provided by Synesis Software Pty Ltd "as is"
 *              and any warranties, whether expressed or implied, including, but
 *              not limited to, the implied warranties of merchantability and
 *              fitness for a particular purpose are disclaimed. In no event
 *              shall the Synesis Software Pty Ltd be liable for any direct,
 *              indirect, incidental, special, exemplary, or consequential
 *              damages (including, but not limited to, procurement of
 *              substitute goods or services; loss of use, data, or profits; or
 *              business interruption) however caused and on any theory of
 *              liability, whether in contract, strict liability, or tort
 *              (including negligence or otherwise) arising in any way out of
 *              the use of this software, even if advised of the possibility of
 *              such damage.
 *
 *              Neither the name of Synesis Software Pty Ltd nor the names of
 *              any subdivisions, employees or agents of Synesis Software Pty
 *              Ltd, nor the names of any other contributors to this software
 *              may be used to endorse or promote products derived from this
 *              software without specific prior written permission.
 *
 * ////////////////////////////////////////////////////////////////////////// */


/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <windows.h>
#include <commctrl.h>

#ifdef __STLSOFT_COMPILER_IS_GCC
WINOLEAPI  CoInitialize(IN LPVOID pvReserved);
WINOLEAPI_(void)  CoUninitialize(void);
#endif /* __STLSOFT_COMPILER_IS_GCC */

#include <stlsoft.h>

#include <winstl.h>
#include <winstl_drophandle_sequence.h>
#include <winstl_control_functionals.h>

#include "resource.h"

#include <algorithm>

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

BOOL CALLBACK Drophandle_sequence_testDialogProc( HWND    hwnd,
                                                    UINT    uMsg,
                                                    WPARAM  wParam,
                                                    LPARAM  lParam);

/* ////////////////////////////////////////////////////////////////////////////
 * Functions
 */

int WINAPI WinMain( HINSTANCE   /* hinst */,
                    HINSTANCE   /* hinstPrev */,
                    LPSTR       /* lpszCmdLine */,
                    int         /* nCmdShow */)
{
	HINSTANCE		hinst	=	::GetModuleHandle(NULL);
    ::CoInitialize(NULL);
    ::InitCommonControls();


    ::DialogBoxParam(	hinst
					,	MAKEINTRESOURCE(IDD_DROPHANDLE_SEQUENCE_TEST)
					,	NULL
					,	(DLGPROC)Drophandle_sequence_testDialogProc
					,	0);

    ::CoUninitialize();

    return 0;
}


static BOOL Drophandle_sequence_test_OnInitDialog(HWND /* hwnd */, HWND /* hwndFocus */, LPARAM /* lParam */)
{
    return false;
}

static BOOL Drophandle_sequence_test_OnCommand(HWND hwnd, int /* code */, int idCtrl, HWND /* hwndCtrl */)
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


static void OnDropFiles(HWND hwnd, HDROP hdrop)
{
	typedef	winstl_ns_qual(drophandle_sequence_a)	drop_sequence_t;

	HWND							hwndList	=	::GetDlgItem(hwnd, IDC_FILES);
	drop_sequence_t					sequence(hdrop);
	drop_sequence_t::const_iterator	begin		=	sequence.begin();
	drop_sequence_t::const_iterator	end			=	sequence.end();

	for(; begin != end; begin++)
	{
		if(begin == end)
		{
		}
	}

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
	stlsoft_ns_qual_std(for_each)(sequence.rbegin(), sequence.rend(), winstl_ns_qual(listbox_back_inserter)(hwndList));
#else
	stlsoft_ns_qual_std(for_each)(sequence.begin(), sequence.end(), winstl_ns_qual(listbox_back_inserter)(hwndList));
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
}


BOOL CALLBACK Drophandle_sequence_testDialogProc( HWND    hwnd,
                                                    UINT    uMsg,
                                                    WPARAM  wParam,
                                                    LPARAM  lParam)
{
    BOOL    bRet    =   false;

    switch(uMsg)
    {
        case    WM_INITDIALOG:
            bRet = Drophandle_sequence_test_OnInitDialog(hwnd, (HWND)wParam, lParam);
            break;
        case    WM_COMMAND:
            bRet = Drophandle_sequence_test_OnCommand(hwnd, (int)HIWORD(wParam), (int) LOWORD(wParam), (HWND)lParam);
            break;
		case	WM_DROPFILES:
			OnDropFiles(hwnd, (HDROP)wParam);
			break;
    }

    return bRet;
}

/* ////////////////////////////////////////////////////////////////////////// */
