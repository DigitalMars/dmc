/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_window_icon_scope.h (originally MWIcnScp.h; ::SynesisWin)
 *
 * Purpose:     Window icon scoping class.
 *
 * Created:     30th June 1999
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1999-2004, Matthew Wilson and Synesis Software
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


/// \file winstl_window_icon_scope.h
///
/// Window icon scoping class.

#ifndef WINSTL_INCL_H_WINSTL_WINDOW_ICON_SCOPE
#define WINSTL_INCL_H_WINSTL_WINDOW_ICON_SCOPE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_WINDOW_ICON_SCOPE_MAJOR    2
# define WINSTL_VER_H_WINSTL_WINDOW_ICON_SCOPE_MINOR    0
# define WINSTL_VER_H_WINSTL_WINDOW_ICON_SCOPE_REVISION 1
# define WINSTL_VER_H_WINSTL_WINDOW_ICON_SCOPE_EDIT     25
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                    // Include the WinSTL root header
#endif /* WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_WINDOW_ACCESS
# include "winstl_window_access.h"      // winstl::get_hwnd
#endif /* WINSTL_INCL_H_WINSTL_WINDOW_ACCESS */
#ifndef WINSTL_INCL_H_WINSTL_WINDOW_FUNCTIONS
# include "winstl_window_functions.h"   // set_window_icon
#endif /* WINSTL_INCL_H_WINSTL_WINDOW_FUNCTIONS */

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

// window_icon_scope
/// Provides scoping of the enable status of a window.
///
/// This class provides scoping of the enable status of a window via the API
/// function EnableWindow()..
class window_icon_scope
{
public:
    typedef window_icon_scope class_type;

// Construction
public:
    /// \brief Toggles the window enable state
    ///
    /// Takes a HWND and changes it's current enable-status, which is set back to
    /// the original state in the destructor.
    ///
    /// \param wnd The window whose enable state is to be controlled
    /// \param iconId The identifier of the window's icon property to be changed. Should be either \c ICON_BIG or \c ICON_SMALL
    /// \param hicon The handle of the icon to associate with the given window's identified icon property
    window_icon_scope(HWND wnd, int iconId, HICON hicon)
        : m_hwnd(wnd)
        , m_iconId(iconId)
        , m_hicon(set_window_icon(m_hwnd, iconId, hicon))
    {}

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    /// \brief Toggles the window enable state
    ///
    /// Takes a HWND and changes it's current enable-status, which is set back to
    /// the original state in the destructor.
    ///
    /// \param wnd The window whose enable state is to be controlled
    /// \param iconId The identifier of the window's icon property to be changed. Should be either \c ICON_BIG or \c ICON_SMALL
    /// \param hicon The handle of the icon to associate with the given window's identified icon property
    template <ss_typename_param_k W>
    window_icon_scope(W &wnd, int iconId, HICON hicon)
        : m_hwnd(get_hwnd(wnd))
        , m_iconId(iconId)
        , m_hicon(set_window_icon(m_hwnd, iconId, hicon))
    {}
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */

    /// Resets the enable status
    ~window_icon_scope() winstl_throw_0()
    {
        winstl_static_assert(stlsoft_raw_offsetof(class_type, m_hwnd) < stlsoft_raw_offsetof(class_type, m_hicon));

        set_window_icon(m_hwnd, m_iconId, m_hicon);
    }

// Members
private:
    HWND    m_hwnd;
    int     m_iconId;
    HICON   m_hicon;

// Not to be implemented
private:
    window_icon_scope(window_icon_scope const &rhs);
    window_icon_scope const &operator =(window_icon_scope const &rhs);
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

#endif /* WINSTL_INCL_H_WINSTL_WINDOW_ICON_SCOPE */

/* ////////////////////////////////////////////////////////////////////////// */
