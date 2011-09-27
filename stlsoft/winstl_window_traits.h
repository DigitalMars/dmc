/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_window_traits.h
 *
 * Purpose:     Contains the window_traits template class, and ANSI
 *              and Unicode specialisations thereof.
 *
 * Created:     24th August 2003
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2003-2004, Matthew Wilson and Synesis Software
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


/// \file winstl_window_traits.h
///
/// Contains the window_traits template class, and ANSI and Unicode specialisations thereof.

#ifndef WINSTL_INCL_H_WINSTL_WINDOW_TRAITS
#define WINSTL_INCL_H_WINSTL_WINDOW_TRAITS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_WINDOW_TRAITS_MAJOR        2
# define WINSTL_VER_H_WINSTL_WINDOW_TRAITS_MINOR        0
# define WINSTL_VER_H_WINSTL_WINDOW_TRAITS_REVISION     1
# define WINSTL_VER_H_WINSTL_WINDOW_TRAITS_EDIT         9
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"    // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"                 // stlsoft::string_access
#endif /* WINSTL_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef WINSTL_INCL_H_WINSTL_STRING_ACCESS
# include "winstl_string_access.h"                  // winstl::string_access
#endif /* WINSTL_INCL_H_WINSTL_STRING_ACCESS */

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

stlsoft_ns_using(c_str_ptr_a)
stlsoft_ns_using(c_str_ptr_w)

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup winstl_reg_library Registry Library
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifdef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/// Traits for accessing the correct registry functions for a given character type
///
/// window_traits is a traits class for determining the correct registry
/// structures and functions for a given character type.
///
/// \param C The character type
template <ss_typename_param_k C>
struct window_traits
{
public:
    /// The character type
    typedef C               char_type;
    /// The size type
    typedef ws_size_t       size_type;
    /// The difference type
    typedef ws_ptrdiff_t    difference_type;

public:
    // General string handling

    /// Copies the contents of \c src to \c dest
    static char_type    *str_copy(char_type *dest, char_type const *src);
    /// Appends the contents of \c src to \c dest
    static char_type    *str_cat(char_type *dest, char_type const *src);
    /// Comparies the contents of \c src and \c dest
    static ws_int_t     str_compare(char_type *dest, char_type const *src);
    /// Evaluates the length of \c src
    static size_type    str_len(char_type const *src);

    // Window functions

    /// Gets the number of characters of text for the given window
    static ws_int_t     get_window_text_length(HWND hwnd);

    /// Retrieves the text for the given window
    static ws_int_t     get_window_text(HWND hwnd, char_type *buffer, ws_int_t cchBuff);

    /// Sets the text for the given window
    static ws_bool_t    set_window_text(HWND hwnd, char_type const *buffer);
}

#else

template <ss_typename_param_k C>
struct window_traits;

STLSOFT_TEMPLATE_SPECIALISATION
struct window_traits<ws_char_a_t>
{
public:
    typedef ws_char_a_t         char_type;
    typedef ws_size_t           size_type;
    typedef ws_ptrdiff_t        difference_type;

public:
    static char_type *str_copy(char_type *dest, char_type const *src)
    {
        return lstrcpyA(dest, src);
    }

    static char_type *str_cat(char_type *dest, char_type const *src)
    {
        return lstrcatA(dest, src);
    }

    static ws_int_t str_compare(char_type const *s1, char_type const *s2)
    {
        return lstrcmpA(s1, s2);
    }

    static size_type str_len(char_type const *src)
    {
        return static_cast<size_type>(lstrlenA(src));
    }


    static ws_int_t get_window_text_length(HWND hwnd)
    {
        return ::GetWindowTextLength(hwnd);
    }

    static ws_int_t get_window_text(HWND hwnd, char_type *buffer, ws_int_t cchBuff)
    {
        return ::GetWindowTextA(hwnd, buffer, cchBuff);
    }

    static ws_bool_t set_window_text(HWND hwnd, char_type const *s)
    {
        return 0 != ::SetWindowTextA(hwnd, s);
    }

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    template <ss_typename_param_k S>
    static ws_bool_t set_window_text(HWND hwnd, S const &s)
    {
        return 0 != ::SetWindowTextA(hwnd, c_str_ptr_a(s));
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */
};

STLSOFT_TEMPLATE_SPECIALISATION
struct window_traits<ws_char_w_t>
{
public:
    typedef ws_char_w_t         char_type;
    typedef ws_size_t           size_type;
    typedef ws_ptrdiff_t        difference_type;

public:
    static char_type *str_copy(char_type *dest, char_type const *src)
    {
        return lstrcpyW(dest, src);
    }

    static char_type *str_cat(char_type *dest, char_type const *src)
    {
        return lstrcatW(dest, src);
    }

    static ws_int_t str_compare(char_type const *s1, char_type const *s2)
    {
        return lstrcmpW(s1, s2);
    }

    static size_type str_len(char_type const *src)
    {
        return static_cast<size_type>(lstrlenW(src));
    }


    static ws_int_t get_window_text_length(HWND hwnd)
    {
        return ::GetWindowTextLength(hwnd);
    }

    static ws_int_t get_window_text(HWND hwnd, char_type *buffer, ws_int_t cchBuff)
    {
        return ::GetWindowTextW(hwnd, buffer, cchBuff);
    }

    static ws_bool_t set_window_text(HWND hwnd, char_type const *s)
    {
        return 0 != ::SetWindowTextW(hwnd, s);
    }

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    template <ss_typename_param_k S>
    static ws_bool_t set_window_text(HWND hwnd, S const &s)
    {
        return 0 != ::SetWindowTextW(hwnd, c_str_ptr_w(s));
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */
};

#endif /* __STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_reg_library

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

#endif /* WINSTL_INCL_H_WINSTL_WINDOW_TRAITS */

/* ////////////////////////////////////////////////////////////////////////// */
