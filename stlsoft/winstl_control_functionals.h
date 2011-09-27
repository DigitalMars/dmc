/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_control_functionals.h
 *
 * Purpose:     Functionals for application to controls.
 *
 * Created:     8th October 2002
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


/// \file winstl_control_functionals.h
///
/// Functionals for application to controls.

#ifndef WINSTL_INCL_H_WINSTL_CONTROL_FUNCTIONALS
#define WINSTL_INCL_H_WINSTL_CONTROL_FUNCTIONALS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_CONTROL_FUNCTIONALS_MAJOR      2
# define WINSTL_VER_H_WINSTL_CONTROL_FUNCTIONALS_MINOR      0
# define WINSTL_VER_H_WINSTL_CONTROL_FUNCTIONALS_REVISION   1
# define WINSTL_VER_H_WINSTL_CONTROL_FUNCTIONALS_EDIT       35
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_GCC: __GNUC__<3
__STLSOFT_COMPILER_IS_MSVC: _MSC_VER<1200
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                    // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */

#if defined(__STLSOFT_COMPILER_IS_GCC) && \
    __GNUC__ < 3
# error winstl_control_functionals.h is not compatible with GNU C++ prior to 3.0
#endif /* compiler */
#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200
# error winstl_control_functionals.h is not compatible with Visual C++ 5.0 or earlier
#endif /* compiler */

#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"     // stlsoft::c_str_ptr
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef WINSTL_INCL_H_WINSTL_STRING_ACCESS
# include "winstl_string_access.h"      // winstl::c_str_ptr
#endif /* !WINSTL_INCL_H_WINSTL_STRING_ACCESS */
#ifndef WINSTL_INCL_H_WINSTL_CHAR_CONVERSIONS
# include "winstl_char_conversions.h"      // winstl::a2w, w2a
#endif /* !WINSTL_INCL_H_WINSTL_CHAR_CONVERSIONS */
#ifndef WINSTL_INCL_H_WINSTL_CONTROL_FUNCTIONS
# include "winstl_control_functions.h"     // winstl::listbox_add_string, ...
#endif /* !WINSTL_INCL_H_WINSTL_CONTROL_FUNCTIONS */

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

#if !defined(__STLSOFT_COMPILER_IS_MWERKS)
stlsoft_ns_using(c_str_ptr)
#endif /* compiler */

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
 * Classes
 */

/// Function object used to insert items at the front of list-box
struct listbox_front_inserter
{
public:
    /// Construct with the target list-box window
    ss_explicit_k listbox_front_inserter(HWND hwndListbox)
        : m_hwndListbox(hwndListbox)
        , m_bUnicode(::IsWindowUnicode(hwndListbox))
    {}

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    template <ss_typename_param_k S>
    void operator ()(S const &s)
    {
        insert(c_str_ptr(s));
    }
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    void operator ()(ws_char_a_t const *s)
    {
        insert(s);
    }
    /// Function call operator taking the item string
    void operator ()(ws_char_w_t const *s)
    {
        insert(s);
    }

// Implementation
private:
    void insert(ws_char_a_t const *s)
    {
        if(m_bUnicode)
        {
            listbox_insertstring_w(m_hwndListbox, a2w(s), 0);
        }
        else
        {
            listbox_insertstring_a(m_hwndListbox, s, 0);
        }
    }
    void insert(ws_char_w_t const *s)
    {
        if(m_bUnicode)
        {
            listbox_insertstring_w(m_hwndListbox, s, 0);
        }
        else
        {
            listbox_insertstring_a(m_hwndListbox, w2a(s), 0);
        }
    }

private:
    HWND        m_hwndListbox;
    ws_int32_t  m_bUnicode;
};

/// Function object used to add items to a list-box
struct listbox_add_inserter
{
public:
    /// Construct with the target list-box window
    ss_explicit_k listbox_add_inserter(HWND hwndListbox)
        : m_hwndListbox(hwndListbox)
        , m_bUnicode(::IsWindowUnicode(hwndListbox))
    {}

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    template <ss_typename_param_k S>
    void operator ()(S const &s)
    {
        add(c_str_ptr(s));
    }
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    void operator ()(ws_char_a_t const *s)
    {
        add(s);
    }
    /// Function call operator taking the item string
    void operator ()(ws_char_w_t const *s)
    {
        add(s);
    }

// Implementation
private:
    void add(ws_char_a_t const *s)
    {
        if(m_bUnicode)
        {
            listbox_addstring_w(m_hwndListbox, a2w(s));
        }
        else
        {
            listbox_addstring_a(m_hwndListbox, s);
        }
    }
    void add(ws_char_w_t const *s)
    {
        if(m_bUnicode)
        {
            listbox_addstring_w(m_hwndListbox, s);
        }
        else
        {
            listbox_addstring_a(m_hwndListbox, w2a(s));
        }
    }

private:
    HWND        m_hwndListbox;
    ws_int32_t  m_bUnicode;
};

/// Function object used to insert items to the back of a list-box
struct listbox_back_inserter
{
public:
    /// Construct with the target list-box window
    ss_explicit_k listbox_back_inserter(HWND hwndListbox)
        : m_hwndListbox(hwndListbox)
        , m_bUnicode(::IsWindowUnicode(hwndListbox))
    {}

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    template <ss_typename_param_k S>
    void operator ()(S const &s)
    {
        insert(c_str_ptr(s));
    }
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    void operator ()(ws_char_a_t const *s)
    {
        insert(s);
    }
    /// Function call operator taking the item string
    void operator ()(ws_char_w_t const *s)
    {
        insert(s);
    }

// Implementation
private:
    void insert(ws_char_a_t const *s)
    {
        if(m_bUnicode)
        {
            listbox_insertstring_w(m_hwndListbox, a2w(s), -1);
        }
        else
        {
            listbox_insertstring_a(m_hwndListbox, s, -1);
        }
    }
    void insert(ws_char_w_t const *s)
    {
        if(m_bUnicode)
        {
            listbox_insertstring_w(m_hwndListbox, s, -1);
        }
        else
        {
            listbox_insertstring_a(m_hwndListbox, w2a(s), -1);
        }
    }

private:
    HWND        m_hwndListbox;
    ws_int32_t  m_bUnicode;
};



/// Function object used to insert items at the front of combo-box
struct combobox_front_inserter
{
public:
    /// Construct with the target combo-box window
    ss_explicit_k combobox_front_inserter(HWND hwndListbox)
        : m_hwndListbox(hwndListbox)
        , m_bUnicode(::IsWindowUnicode(hwndListbox))
    {}

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    template <ss_typename_param_k S>
    void operator ()(S const &s)
    {
        insert(c_str_ptr(s));
    }
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    void operator ()(ws_char_a_t const *s)
    {
        insert(s);
    }
    /// Function call operator taking the item string
    void operator ()(ws_char_w_t const *s)
    {
        insert(s);
    }

// Implementation
private:
    void insert(ws_char_a_t const *s)
    {
        if(m_bUnicode)
        {
            combobox_insertstring_w(m_hwndListbox, a2w(s), 0);
        }
        else
        {
            combobox_insertstring_a(m_hwndListbox, s, 0);
        }
    }
    void insert(ws_char_w_t const *s)
    {
        if(m_bUnicode)
        {
            combobox_insertstring_w(m_hwndListbox, s, 0);
        }
        else
        {
            combobox_insertstring_a(m_hwndListbox, w2a(s), 0);
        }
    }

private:
    HWND        m_hwndListbox;
    ws_int32_t  m_bUnicode;
};

/// Function object used to add items to a combo-box
struct combobox_add_inserter
{
public:
    /// Construct with the target combo-box window
    ss_explicit_k combobox_add_inserter(HWND hwndListbox)
        : m_hwndListbox(hwndListbox)
        , m_bUnicode(::IsWindowUnicode(hwndListbox))
    {}

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    template <ss_typename_param_k S>
    void operator ()(S const &s)
    {
        add(c_str_ptr(s));
    }
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    void operator ()(ws_char_a_t const *s)
    {
        add(s);
    }
    /// Function call operator taking the item string
    void operator ()(ws_char_w_t const *s)
    {
        add(s);
    }

// Implementation
private:
    void add(ws_char_a_t const *s)
    {
        if(m_bUnicode)
        {
            combobox_addstring_w(m_hwndListbox, a2w(s));
        }
        else
        {
            combobox_addstring_a(m_hwndListbox, s);
        }
    }
    void add(ws_char_w_t const *s)
    {
        if(m_bUnicode)
        {
            combobox_addstring_w(m_hwndListbox, s);
        }
        else
        {
            combobox_addstring_a(m_hwndListbox, w2a(s));
        }
    }

private:
    HWND        m_hwndListbox;
    ws_int32_t  m_bUnicode;
};

/// Function object used to insert items to the back of a combo-box
struct combobox_back_inserter
{
public:
    /// Construct with the target combo-box window
    ss_explicit_k combobox_back_inserter(HWND hwndListbox)
        : m_hwndListbox(hwndListbox)
        , m_bUnicode(::IsWindowUnicode(hwndListbox))
    {}

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    template <ss_typename_param_k S>
    void operator ()(S const &s)
    {
        insert(c_str_ptr(s));
    }
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    void operator ()(ws_char_a_t const *s)
    {
        insert(s);
    }
    /// Function call operator taking the item string
    void operator ()(ws_char_w_t const *s)
    {
        insert(s);
    }

// Implementation
private:
    void insert(ws_char_a_t const *s)
    {
        if(m_bUnicode)
        {
            combobox_insertstring_w(m_hwndListbox, a2w(s), -1);
        }
        else
        {
            combobox_insertstring_a(m_hwndListbox, s, -1);
        }
    }
    void insert(ws_char_w_t const *s)
    {
        if(m_bUnicode)
        {
            combobox_insertstring_w(m_hwndListbox, s, -1);
        }
        else
        {
            combobox_insertstring_a(m_hwndListbox, w2a(s), -1);
        }
    }

private:
    HWND        m_hwndListbox;
    ws_int32_t  m_bUnicode;
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_control_functionals(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "control_functionals", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_control_functionals(test_winstl_control_functionals);

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

#endif /* WINSTL_INCL_H_WINSTL_CONTROL_FUNCTIONALS */

/* ////////////////////////////////////////////////////////////////////////// */
