/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_window_functionals.h
 *
 * Purpose:     Window specific functionals.
 *
 * Created:     19th January 2001
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2001-2004, Matthew Wilson and Synesis Software
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


/// \file winstl_window_functionals.h
///
/// Window specific functionals.

#ifndef WINSTL_INCL_H_WINSTL_WINDOW_FUNCTIONALS
#define WINSTL_INCL_H_WINSTL_WINDOW_FUNCTIONALS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_WINDOW_FUNCTIONALS_MAJOR       2
# define WINSTL_VER_H_WINSTL_WINDOW_FUNCTIONALS_MINOR       0
# define WINSTL_VER_H_WINSTL_WINDOW_FUNCTIONALS_REVISION    1
# define WINSTL_VER_H_WINSTL_WINDOW_FUNCTIONALS_EDIT        18
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_WINDOW_ACCESS
# include "winstl_window_access.h"  // get_hwnd()
#endif /* !WINSTL_INCL_H_WINSTL_WINDOW_ACCESS */
#ifndef _WINSTL_WINDOW_FUNCTIONALS_NO_STD
# include <functional>
#endif /* _WINSTL_WINDOW_FUNCTIONALS_NO_STD */

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
 * Classes
 */

/// Predicate used to determine whether windows are visible
struct is_visible
#ifndef _WINSTL_WINDOW_FUNCTIONALS_NO_STD
  : public winstl_ns_qual_std(unary_function)<HWND, ws_bool_t>
#endif /* !_WINSTL_WINDOW_FUNCTIONALS_NO_STD */
{
public:
#ifndef _WINSTL_WINDOW_FUNCTIONALS_NO_STD
    ///
private:
    typedef winstl_ns_qual_std(unary_function)<HWND, ws_bool_t> parent_class_type;
public:
    /// The argument type
    typedef parent_class_type::argument_type                    argument_type;
    /// The result type
    typedef parent_class_type::result_type                      result_type;
#else
    /// The argument type
    typedef HWND                                                argument_type;
    /// The result type
    typedef ws_bool_t                                           result_type;
#endif /* _WINSTL_WINDOW_FUNCTIONALS_NO_STD */

// Operations
public:
    /// Function call operator which evaluates whether the given window is visible
    ws_bool_t operator ()(HWND hwnd)
    {
        return ::IsWindowVisible(hwnd) != false;
    }
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator which evaluates whether the given window is visible
    template <ss_typename_param_k W>
    ws_bool_t operator ()(W const &w)
    {
        return operator ()(get_hwnd(w));
    }
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
};



/// Predicate used to determine whether windows are enabled
struct is_enabled
#ifndef _WINSTL_WINDOW_FUNCTIONALS_NO_STD
  : public winstl_ns_qual_std(unary_function)<HWND, ws_bool_t>
#endif /* !_WINSTL_WINDOW_FUNCTIONALS_NO_STD */
{
public:
#ifndef _WINSTL_WINDOW_FUNCTIONALS_NO_STD
private:
    typedef winstl_ns_qual_std(unary_function)<HWND, ws_bool_t> parent_class_type;
public:
    /// The argument type
    typedef parent_class_type::argument_type                    argument_type;
    /// The result type
    typedef parent_class_type::result_type                      result_type;
#else
    /// The argument type
    typedef HWND                                                argument_type;
    /// The result type
    typedef ws_bool_t                                           result_type;
#endif /* _WINSTL_WINDOW_FUNCTIONALS_NO_STD */

// Operations
public:
    /// Function call operator which evaluates whether the given window is enabled
    ws_bool_t operator ()(HWND hwnd)
    {
        return ::IsWindowEnabled(hwnd) != false;
    }
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator which evaluates whether the given window is enabled
    template <ss_typename_param_k W>
    ws_bool_t operator ()(W const &w)
    {
        return operator ()(get_hwnd(w));
    }
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
};

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

#endif /* WINSTL_INCL_H_WINSTL_WINDOW_FUNCTIONALS */

/* ////////////////////////////////////////////////////////////////////////// */
