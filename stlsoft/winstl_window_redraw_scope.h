/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_window_redraw_scope.h (originally MWWndLck.h; ::SynesisWin)
 *
 * Purpose:     Window redraw-state scoping class.
 *
 * Created:     5th January 1996
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1996-2004, Matthew Wilson and Synesis Software
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


/// \file winstl_window_redraw_scope.h
///
/// Window redraw-state scoping class.

#ifndef WINSTL_INCL_H_WINSTL_WINDOW_REDRAW_SCOPE
#define WINSTL_INCL_H_WINSTL_WINDOW_REDRAW_SCOPE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_WINDOW_REDRAW_SCOPE_MAJOR      3
# define WINSTL_VER_H_WINSTL_WINDOW_REDRAW_SCOPE_MINOR      0
# define WINSTL_VER_H_WINSTL_WINDOW_REDRAW_SCOPE_REVISION   1
# define WINSTL_VER_H_WINSTL_WINDOW_REDRAW_SCOPE_EDIT       53
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                // Include the WinSTL root header
#endif /* WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_WINDOW_ACCESS
# include "winstl_window_access.h"  // winstl::window_access
#endif /* WINSTL_INCL_H_WINSTL_WINDOW_ACCESS */

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

// window_redraw_scope
/// Provides scoping of the redraw status of a window.
///
/// This class provides scoping of the redraw status of a window via the
/// WM_SETREDRAW message.
class window_redraw_scope
{
public:
    typedef window_redraw_scope class_type;

// Construction
public:
    /// Attempts to lock the given window from redrawing.
    ///
    /// Takes a HWND and changes it's current redraw-status, which is set back to
    /// the original state in the destructor.
    ///
    /// \param wnd The window whose redraw state is to be controlled
    /// \param bInvalidateOnUnlock Determines whether the window is invalidated upon destruction
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    template <typename W>
    ss_explicit_k window_redraw_scope(W &wnd, ws_bool_t bInvalidateOnUnlock = true)
        : m_hwnd(get_hwnd(wnd))
#else
    ss_explicit_k window_redraw_scope(HWND wnd, ws_bool_t bInvalidateOnUnlock = true)
        : m_hwnd(wnd)
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */
        , m_bInvalidateOnUnlock(bInvalidateOnUnlock)
    {
        ::SendMessage(m_hwnd, WM_SETREDRAW, false, 0L);
    }

    /// Resets the redraw status, and invalidates the window, if requested in the constructor
    ~window_redraw_scope() winstl_throw_0()
    {
        ::SendMessage(m_hwnd, WM_SETREDRAW, true, 0L);

        if(m_bInvalidateOnUnlock)
        {
            ::InvalidateRect(m_hwnd, NULL, true);
        }
    }

// Members
private:
    HWND        m_hwnd;
    ws_bool_t   m_bInvalidateOnUnlock;

// Not to be implemented
private:
    window_redraw_scope(window_redraw_scope const &rhs);
    window_redraw_scope const &operator =(window_redraw_scope const &rhs);
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

#endif /* WINSTL_INCL_H_WINSTL_WINDOW_REDRAW_SCOPE */

/* ////////////////////////////////////////////////////////////////////////// */
