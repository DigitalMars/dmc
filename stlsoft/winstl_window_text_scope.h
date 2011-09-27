/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_window_text_scope.h (originally MWTxtScp.h; ::SynesisWin)
 *
 * Purpose:     Window text scoping class.
 *
 * Created:     21st August 2003
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


/// \file winstl_window_text_scope.h
///
/// Window text scoping class.

#ifndef WINSTL_INCL_H_WINSTL_WINDOW_TEXT_SCOPE
#define WINSTL_INCL_H_WINSTL_WINDOW_TEXT_SCOPE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_WINDOW_TEXT_SCOPE_MAJOR        2
# define WINSTL_VER_H_WINSTL_WINDOW_TEXT_SCOPE_MINOR        0
# define WINSTL_VER_H_WINSTL_WINDOW_TEXT_SCOPE_REVISION     1
# define WINSTL_VER_H_WINSTL_WINDOW_TEXT_SCOPE_EDIT         12
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                                // Include the WinSTL root header
#endif /* WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_WINDOW_ACCESS
# include "winstl_window_access.h"                  // winstl::window_access
#endif /* WINSTL_INCL_H_WINSTL_WINDOW_ACCESS */
#ifndef WINSTL_INCL_H_WINSTL_WINDOW_TRAITS
# include "winstl_window_traits.h"                  // winstl::window_traits
#endif /* WINSTL_INCL_H_WINSTL_WINDOW_TRAITS */
#ifndef WINSTL_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"                   // stlsoft::auto_buffer
#endif /* WINSTL_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR
# include "winstl_window_processheap_allocator.h"   // winstl::processheap_allocator
#endif /* WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR */

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

// window_text_scope
/// Provides scoping of the text of a window.
///
/// This class provides scoping of the text of a window via the API
/// function SetWindowText().
template <ss_typename_param_k S>
class window_text_scope
{
public:
    /// The string type
    typedef S                                           string_type;
    /// The type of the current instantiation
    typedef window_text_scope<S>                        class_type;
    /// The character type
    typedef ss_typename_type_k string_type::value_type  char_type;
    /// The traits type
    typedef window_traits<char_type>                    traits_type;

// Construction
public:
    /// \brief Toggles the window text
    ///
    /// Takes a HWND and changes sets its text, which is set back to
    /// the original in the destructor.
    ///
    /// \param wnd The window whose text is to be modified
    /// \param s The new, temporary, window text
    ss_explicit_k window_text_scope(HWND wnd, char_type const *s)
        : m_hwnd(wnd)
    {
        typedef processheap_allocator<char_type>                    ator_t;
        typedef stlsoft_ns_qual(auto_buffer)<char_type,  ator_t>    buffer_t;

        buffer_t    buffer(1 + traits_type::get_window_text_length(m_hwnd));

        if(0 < buffer.size()) // Some allocators return NULL, rather than throw
        {
            buffer[buffer.size() - 1] = char_type(0);

            traits_type::get_window_text(m_hwnd, buffer, buffer.size());

            m_oldText = buffer.data();

            traits_type::set_window_text(m_hwnd, s);
        }
    }

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    /// \brief Toggles the window text
    ///
    /// Takes a HWND and changes sets its text, which is set back to
    /// the original in the destructor.
    ///
    /// \param wnd The window whose text is to be modified
    /// \param s The new, temporary, window text
#if 0
    template <ss_typename_param_k S>
    ss_explicit_k window_text_scope(HWND wnd, S const &s)
        : m_hwnd(wnd)
    {
        typedef stlsoft_ns_qual(auto_buffer)<char_type, processheap_allocator<char_type> > buffer_t;

        buffer_t    buffer(1 + traits_type::get_window_text_length(m_hwnd));

        if(0 < buffer.size()) // Some allocators return NULL, rather than throw
        {
            buffer[buffer.size() - 1] = char_type(0);

            traits_type::get_window_text(m_hwnd, buffer, buffer.size());

            m_oldText = buffer.data();

            traits_type::set_window_text(m_hwnd, s);
        }
    }
#endif /* 0 */

    /// \brief Toggles the window text
    ///
    /// Takes a HWND and changes sets its text, which is set back to
    /// the original in the destructor.
    ///
    /// \param wnd The window whose text is to be modified
    /// \param s The new, temporary, window text
    template<   ss_typename_param_k W
            ,   ss_typename_param_k S1
            >
    ss_explicit_k window_text_scope(W &wnd, S1 const &s)
        : m_hwnd(get_hwnd(wnd))
    {
        typedef stlsoft_ns_qual(auto_buffer)<char_type, processheap_allocator<char_type> > buffer_t;

        buffer_t    buffer(1 + traits_type::get_window_text_length(m_hwnd));

        if(0 < buffer.size()) // Some allocators return NULL, rather than throw
        {
            buffer[buffer.size() - 1] = char_type(0);

            traits_type::get_window_text(m_hwnd, buffer, buffer.size());

            m_oldText = buffer.data();

            traits_type::set_window_text(m_hwnd, s);
        }
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */

    /// Resets the window text
    ~window_text_scope() winstl_throw_0()
    {
        traits_type::set_window_text(m_hwnd, m_oldText);
    }

// Members
private:
    HWND        m_hwnd;
    string_type m_oldText;

// Not to be implemented
private:
    window_text_scope(window_text_scope const &rhs);
    window_text_scope const &operator =(window_text_scope const &rhs);
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

#endif /* WINSTL_INCL_H_WINSTL_WINDOW_TEXT_SCOPE */

/* ////////////////////////////////////////////////////////////////////////// */
