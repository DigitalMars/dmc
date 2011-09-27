/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_control_functions.h
 *
 * Purpose:     Various Windows control functions.
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


/// \file winstl_control_functions.h
///
/// Various Windows control functions.

#ifndef WINSTL_INCL_H_WINSTL_CONTROL_FUNCTIONS
#define WINSTL_INCL_H_WINSTL_CONTROL_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_CONTROL_FUNCTIONS_MAJOR        2
# define WINSTL_VER_H_WINSTL_CONTROL_FUNCTIONS_MINOR        0
# define WINSTL_VER_H_WINSTL_CONTROL_FUNCTIONS_REVISION     1
# define WINSTL_VER_H_WINSTL_CONTROL_FUNCTIONS_EDIT         23
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */

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

/* COMBOBOX functions
 */

/// Adds an ANSI string to a combo-box
inline ws_int_t combobox_addstring_a(HWND hwnd, ws_char_a_t const *s)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(s)));
}

/// Adds a Unicode string to a combo-box
inline ws_int_t combobox_addstring_w(HWND hwnd, ws_char_w_t const *s)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(s)));
}

/// Adds a string (in the ambient char-encoding) to a combo-box
inline ws_int_t combobox_addstring(HWND hwnd, LPCTSTR s)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(s)));
}

/// Inserts an ANSI string into a combo-box at the given index
inline ws_int_t combobox_insertstring_a(HWND hwnd, ws_char_a_t const *s, int index)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, CB_INSERTSTRING, static_cast<WPARAM>(index), reinterpret_cast<LPARAM>(s)));
}

/// Inserts a Unicode string into a combo-box at the given index
inline ws_int_t combobox_insertstring_w(HWND hwnd, ws_char_w_t const *s, int index)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, CB_INSERTSTRING, static_cast<WPARAM>(index), reinterpret_cast<LPARAM>(s)));
}

/// Inserts a string (in the ambient char-encoding) into a combo-box at the given index
inline ws_int_t combobox_insertstring(HWND hwnd, LPCTSTR s, int index)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, CB_INSERTSTRING, static_cast<WPARAM>(index), reinterpret_cast<LPARAM>(s)));
}

/// Gets the text length of an item in a combo-box
inline ws_int_t combobox_gettextlen(HWND hwnd, ws_int_t index)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, CB_GETLBTEXTLEN, static_cast<WPARAM>(index), 0L));
}

/// Gets the text of an item in a combo-box
inline ws_int_t combobox_gettext(HWND hwnd, ws_int_t index, LPCSTR s)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, CB_GETLBTEXT, static_cast<WPARAM>(index), reinterpret_cast<LPARAM>(s)));
}

/// Gets the number of items in a combo-box
inline ws_int_t combobox_getcount(HWND hwnd)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, CB_GETCOUNT, 0, 0L));
}


/* LISTBOX functions
 */

/// Adds an ANSI string to a list-box
inline ws_int_t listbox_addstring_a(HWND hwnd, ws_char_a_t const *s)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(s)));
}

/// Adds a Unicode string to a list-box
inline ws_int_t listbox_addstring_w(HWND hwnd, ws_char_w_t const *s)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(s)));
}

/// Adds a string (in the ambient char-encoding) to a list-box
inline ws_int_t listbox_addstring(HWND hwnd, LPCTSTR s)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(s)));
}

/// Inserts an ANSI string into a list-box at the given index
inline ws_int_t listbox_insertstring_a(HWND hwnd, ws_char_a_t const *s, int index)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, LB_INSERTSTRING, static_cast<WPARAM>(index), reinterpret_cast<LPARAM>(s)));
}

/// Inserts a Unicode string into a list-box at the given index
inline ws_int_t listbox_insertstring_w(HWND hwnd, ws_char_w_t const *s, int index)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, LB_INSERTSTRING, static_cast<WPARAM>(index), reinterpret_cast<LPARAM>(s)));
}

/// Inserts a string (in the ambient char-encoding) into a list-box at the given index
inline ws_int_t listbox_insertstring(HWND hwnd, LPCTSTR s, int index)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, LB_INSERTSTRING, static_cast<WPARAM>(index), reinterpret_cast<LPARAM>(s)));
}

/// Gets the text length of an item in a list-box
inline ws_int_t listbox_gettextlen(HWND hwnd, ws_int_t index)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, LB_GETTEXTLEN, static_cast<WPARAM>(index), 0L));
}

/// Gets the text of an item in a list-box
inline ws_int_t listbox_gettext(HWND hwnd, ws_int_t index, LPCSTR s)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, LB_GETTEXT, static_cast<WPARAM>(index), reinterpret_cast<LPARAM>(s)));
}

/// Gets the number of items in a list-box
inline ws_int_t listbox_getcount(HWND hwnd)
{
    return static_cast<ws_int_t>(::SendMessage(hwnd, LB_GETCOUNT, 0, 0L));
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_control_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "control_functions", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_control_functions(test_winstl_control_functions);

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

#endif /* WINSTL_INCL_H_WINSTL_CONTROL_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
