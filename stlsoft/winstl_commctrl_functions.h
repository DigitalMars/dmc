/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_commctrl_functions.h
 *
 * Purpose:     Various Windows common control functions.
 *
 * Created:     13th November 2002
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2002-2004, Matthew Wilson and Synesis Software
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name(s) of Matthew Wilson and Synesis Software nor the names of
 *   any contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * ////////////////////////////////////////////////////////////////////////// */


/// \file winstl_commctrl_functions.h
///
/// Various Windows common control functions.

#ifndef WINSTL_INCL_H_WINSTL_COMMCTRL_FUNCTIONS
#define WINSTL_INCL_H_WINSTL_COMMCTRL_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_COMMCTRL_FUNCTIONS_MAJOR       2
# define WINSTL_VER_H_WINSTL_COMMCTRL_FUNCTIONS_MINOR       0
# define WINSTL_VER_H_WINSTL_COMMCTRL_FUNCTIONS_REVISION    1
# define WINSTL_VER_H_WINSTL_COMMCTRL_FUNCTIONS_EDIT        19
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#include <commctrl.h>

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::winstl */
namespace winstl
{
# else
/* Define stlsoft::winstl_project */

namespace stlsoft
{

namespace winstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_control Control Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating GUI controls

/// \defgroup winstl_control_library Control Library (WinSTL)
/// \ingroup WinSTL libraries_control
/// \brief This library provides facilities for defining and manipulating Win32 GUI controls
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

/* TREEVIEW functions
 */

/// Gets the next item in the tree view
///
/// \param hwnd The tree view window handle
/// \param hitem The tree view item
/// \param flag One of the <b>TVGN_*</b> search flags
inline HTREEITEM treeview_getnextitem(HWND hwnd, HTREEITEM hitem, UINT flag)
{
    return reinterpret_cast<HTREEITEM>(::SendMessage(hwnd, TVM_GETNEXTITEM, static_cast<WPARAM>(flag), reinterpret_cast<LPARAM>(hitem)));
}

/// Gets the tree view child item
///
/// \param hwnd The tree view window handle
/// \param hitem The tree view item whose child is to be retrieved
inline HTREEITEM treeview_getchilditem(HWND hwnd, HTREEITEM hitem)
{
    return treeview_getnextitem(hwnd, hitem, TVGN_CHILD);
}

/// Gets the tree view root item
///
/// \param hwnd The tree view window handle
inline HTREEITEM treeview_getrootitem(HWND hwnd)
{
    return treeview_getnextitem(hwnd, 0, TVGN_ROOT);
}

/// Gets the tree view caret item
///
/// \param hwnd The tree view window handle
inline HTREEITEM treeview_getcaretitem(HWND hwnd)
{
    return treeview_getnextitem(hwnd, 0, TVGN_CARET);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_commctrl_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "commctrl_functions", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_commctrl_functions(test_winstl_commctrl_functions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_control_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace winstl
# else
} // namespace winstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WINSTL_INCL_H_WINSTL_COMMCTRL_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
