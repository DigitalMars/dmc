/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_window_functions.h (formerly MWBase.h, ::SynesisWin)
 *
 * Purpose:     Window functions.
 *
 * Created:     7th May 2000
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2000-2004, Matthew Wilson and Synesis Software
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


/// \file winstl_window_functions.h
///
/// Window functions..

#ifndef WINSTL_INCL_H_WINSTL_WINDOW_FUNCTIONS
#define WINSTL_INCL_H_WINSTL_WINDOW_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_WINDOW_FUNCTIONS_MAJOR     2
# define WINSTL_VER_H_WINSTL_WINDOW_FUNCTIONS_MINOR     0
# define WINSTL_VER_H_WINSTL_WINDOW_FUNCTIONS_REVISION  1
# define WINSTL_VER_H_WINSTL_WINDOW_FUNCTIONS_EDIT      23
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                // Include the WinSTL root header
#endif /* WINSTL_INCL_H_WINSTL */

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

/// \weakgroup libraries_window Window Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating GUI windows

/// \defgroup winstl_window_library Window Library (WinSTL)
/// \ingroup WinSTL libraries_window
/// \brief This library provides facilities for defining and manipulating Win32 GUI windows
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

/** Gets the style of the window */
inline ws_long_t GetStyle(HWND h)
{
    return GetWindowLong(h, GWL_STYLE);
}

/** Gets the extended style of the window */
inline ws_long_t GetExStyle(HWND h)
{
    return GetWindowLong(h, GWL_EXSTYLE);
}

/** Sets the style of the window */
inline ws_long_t SetStyle(HWND h, ws_long_t s)
{
    return SetWindowLong(h, GWL_STYLE, (ws_long_t)s);
}

/** Sets the extended style of the window */
inline ws_long_t SetExStyle(HWND h, ws_long_t x)
{
    return SetWindowLong(h, GWL_EXSTYLE, (ws_long_t)x);
}

/** Modifies the style of the window */
inline ws_long_t ModifyStyle(HWND h, ws_long_t sRem, ws_long_t sAdd)
{
    return SetStyle(h, (GetStyle(h) & ~sRem) | sAdd);
}

/** Modifies the extended style of the window */
inline ws_long_t ModifyExStyle(HWND h, ws_long_t sRem, ws_long_t sAdd)
{
    return SetExStyle(h, (GetExStyle(h) & ~sRem) | sAdd);
}

/** Tests whether the given window has the given window class */
inline ws_bool_t IsWindowClass(HWND hwnd, ws_char_a_t const *name)
{
    winstl_assert(NULL != hwnd);
    winstl_assert(NULL != name);

    ws_char_a_t szName[256];

    (void)::GetClassNameA(hwnd, szName, stlsoft_num_elements(szName));

    return 0 == lstrcmpiA(szName, name);
}

/** Tests whether the given window has the given window class */
inline ws_bool_t IsWindowClass(HWND hwnd, ws_char_w_t const *name)
{
    winstl_assert(NULL != hwnd);
    winstl_assert(NULL != name);

    ws_char_w_t szName[256];

    (void)::GetClassNameW(hwnd, szName, stlsoft_num_elements(szName));

    return 0 == lstrcmpiW(szName, name);
}

/** Enables/disable a dialog item
 */
inline void EnableDlgItem(HWND hwnd, int id, ws_bool_t bEnable)
{
    ::EnableWindow(::GetDlgItem(hwnd, id), bEnable);
}

/** Elicits the enable status of a dialog item
 */
inline ws_bool_t IsDlgItemEnabled(HWND hwnd, int id)
{
    return ::IsWindowEnabled(::GetDlgItem(hwnd, id)) != 0;
}

/** Gets the text length of a dialog item's window contents
 */
inline int GetDlgItemTextLength(HWND hwnd, int id)
{
    return ::GetWindowTextLength(::GetDlgItem(hwnd, id));
}

/** Gets the HINSTANCE associated with a given window
 */
#ifdef GetWindowInstance
# undef GetWindowInstance
#endif /* GetWindowInstance */
inline HINSTANCE GetWindowInstance(HWND hwnd)
{
    return reinterpret_cast<HINSTANCE>(::GetWindowLong(hwnd, GWL_HINSTANCE));
}

inline HICON set_window_icon(HWND hwnd, int iconId, HICON hicon)
{
    winstl_assert(ICON_BIG == iconId || ICON_SMALL == iconId);

    return (HICON)::SendMessage(hwnd, WM_SETICON, (WPARAM)iconId, (LPARAM)hicon);
}

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_window_library

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

#endif /* WINSTL_INCL_H_WINSTL_WINDOW_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
