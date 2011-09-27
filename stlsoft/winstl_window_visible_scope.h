/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_window_visible_scope.h
 *
 * Purpose:     Window enable-state scoping class.
 *
 * Created:     26th May 2004
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2004, Matthew Wilson and Synesis Software
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


/// \file winstl_window_visible_scope.h
///
/// Window enable-state scoping class.

#ifndef WINSTL_INCL_H_WINSTL_WINDOW_VISIBLE_SCOPE
#define WINSTL_INCL_H_WINSTL_WINDOW_VISIBLE_SCOPE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_WINDOW_VISIBLE_SCOPE_MAJOR      2
# define WINSTL_VER_H_WINSTL_WINDOW_VISIBLE_SCOPE_MINOR      0
# define WINSTL_VER_H_WINSTL_WINDOW_VISIBLE_SCOPE_REVISION   1
# define WINSTL_VER_H_WINSTL_WINDOW_VISIBLE_SCOPE_EDIT       7
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

// window_visible_scope
/// Provides scoping of the visible status of a window.
///
/// This class provides scoping of the visible status of a window via the API
/// function EnableWindow()..
class window_visible_scope
{
public:
    typedef window_visible_scope class_type;

// Construction
public:
    /// \brief Toggles the window visible state
    ///
    /// Takes a HWND and changes it's current visible-status, which is set back to
    /// the original state in the destructor.
    ///
    /// \param wnd The window whose visible state is to be controlled
    ss_explicit_k window_visible_scope(HWND wnd)
        : m_hwnd(wnd)
        , m_stateOnDtor(::IsWindowVisible(m_hwnd) ? SW_SHOW : SW_HIDE)
    {
        ::ShowWindow(m_hwnd, (SW_SHOW == m_stateOnDtor) ? SW_HIDE : SW_SHOW);
    }

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    /// \brief Toggles the window visible state
    ///
    /// Takes a HWND and changes it's current visible-status, which is set back to
    /// the original state in the destructor.
    ///
    /// \param wnd The window whose visible state is to be controlled
    template <ss_typename_param_k W>
    ss_explicit_k window_visible_scope(W &wnd)
        : m_hwnd(get_hwnd(wnd))
        , m_stateOnDtor(::IsWindowVisible(m_hwnd) ? SW_SHOW : SW_HIDE)
    {
        ::ShowWindow(m_hwnd, (SW_SHOW == m_stateOnDtor) ? SW_HIDE : SW_SHOW);
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */

    /// Modifies the window visible state
    ///
    /// \param wnd The window whose visible state is to be controlled
    /// \param stateOnCtor The state to set in the constructor
    /// \param stateOnDtor The state it is reset to in the destructor
    window_visible_scope(HWND wnd, ws_int_t stateOnCtor, ws_int_t stateOnDtor)
        : m_hwnd(wnd)
        , m_stateOnDtor(stateOnDtor)
    {
        ::ShowWindow(m_hwnd, stateOnCtor);
    }
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    /// Modifies the window visible state
    ///
    /// \param wnd The window whose visible state is to be controlled
    /// \param stateOnCtor The state to set in the constructor
    /// \param stateOnDtor The state it is reset to in the destructor
    template <ss_typename_param_k W>
    window_visible_scope(W &wnd, ws_int_t stateOnCtor, ws_int_t stateOnDtor)
        : m_hwnd(get_hwnd(wnd))
        , m_stateOnDtor(stateOnDtor)
    {
        ::ShowWindow(m_hwnd, stateOnCtor);
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */

    /// Resets the visible status
    ~window_visible_scope() winstl_throw_0()
    {
        winstl_static_assert(stlsoft_raw_offsetof(class_type, m_hwnd) < stlsoft_raw_offsetof(class_type, m_stateOnDtor));

        ::ShowWindow(m_hwnd, m_stateOnDtor);
    }

// Members
private:
    HWND const      m_hwnd;
    ws_int_t const  m_stateOnDtor;

// Not to be implemented
private:
    window_visible_scope(window_visible_scope const &rhs);
    window_visible_scope const &operator =(window_visible_scope const &rhs);
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_window_visible_scope(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        RECT                    rc          =   { 0, 0, 10, 10 };
        HWND                    hwndButton  =   ::CreateWindowEx(0, "BUTTON", "A button", WS_VISIBLE, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, NULL, (HMENU)0, GetModuleHandle(NULL), NULL);

        unittest_initialiser    init(r, "WinSTL", "window_visible_scope", __FILE__);

        if(!::IsWindow(hwndButton))
        {
            r->report("failed to create test window", __LINE__);
            bSuccess = false;
        }
        else
        {
            if(!::IsWindowVisible(hwndButton))
            {
                r->report("test window is invalid (not visible)", __LINE__);
                bSuccess = false;
            }
            else
            {
                { // Enter scope
                    window_visible_scope    scope(hwndButton);

                    if(::IsWindowVisible(hwndButton))
                    {
                        r->report("test window is visible when it should be hidden", __LINE__);
                        bSuccess = false;
                    }

                } // Exit scope

                if(!::IsWindowVisible(hwndButton))
                {
                    r->report("test window is hidden when it should be visible", __LINE__);
                    bSuccess = false;
                }

                { // Enter scope
                    window_visible_scope    scope(hwndButton, SW_SHOW, SW_HIDE);

                    if(!::IsWindowVisible(hwndButton))
                    {
                        r->report("test window is hidden when it should be visible", __LINE__);
                        bSuccess = false;
                    }

                } // Exit scope

                if(::IsWindowVisible(hwndButton))
                {
                    r->report("test window is visible when it should be hidden", __LINE__);
                    bSuccess = false;
                }
            }

            DestroyWindow(hwndButton);
        }

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_window_visible_scope(test_winstl_window_visible_scope);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

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

#endif /* WINSTL_INCL_H_WINSTL_WINDOW_VISIBLE_SCOPE */

/* ////////////////////////////////////////////////////////////////////////// */
