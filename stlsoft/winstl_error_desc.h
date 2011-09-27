/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_error_desc.h
 *
 * Purpose:     Converts a Win32 error code to a printable string.
 *
 * Created:     13th July 2003
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


/// \file winstl_error_desc.h
///
/// Converts a Win32 error code to a printable string.

#ifndef WINSTL_INCL_H_WINSTL_ERROR_DESC
#define WINSTL_INCL_H_WINSTL_ERROR_DESC

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_ERROR_DESC_MAJOR    2
# define WINSTL_VER_H_WINSTL_ERROR_DESC_MINOR    0
# define WINSTL_VER_H_WINSTL_ERROR_DESC_REVISION 1
# define WINSTL_VER_H_WINSTL_ERROR_DESC_EDIT     28
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_FUNCTIONS
# include "winstl_functions.h"              // winstl::FormatMessage()
#endif /* !WINSTL_INCL_H_WINSTL_FUNCTIONS */
#ifndef WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS
# include "winstl_filesystem_traits.h"      // load_library
#endif /* !WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS */
#ifndef WINSTL_INCL_H_WINSTL_STRING_ACCESS
# include "winstl_string_access.h"          // c_str_ptr, etc.
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

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup winstl_error_library Error Library
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// String form of a Win32 error
///
/// This class
template<   ss_typename_param_k C
        ,   ss_typename_param_k T = filesystem_traits<C>
        >
class basic_error_desc
{
public:
    /// The character type
    typedef C                       char_type;
    /// The traits_type
    typedef T                       traits_type;
    /// The current parameterisation of the type
    typedef basic_error_desc<C, T>  class_type;

// Construction
public:
    /// Constructor
    ///
    /// \note This uses the calling thread's error current value, obtained by
    /// calling GetLastError()
    ss_explicit_k basic_error_desc(ws_dword_t error = GetLastError(), char_type const *paths = NULL);
public:
    /// Destructor
    ~basic_error_desc() winstl_throw_0();

// Attributes
public:
    /// The error description
    char_type const *get_description() const;

// Accessors
public:
    /// The error description
    operator char_type const *() const;

// Members
private:
    char_type   *m_message;

// Not to be implemented
public:
    basic_error_desc(class_type const &);
    basic_error_desc &operator =(class_type const &);
};

/* Typedefs to commonly encountered types. */
/// Instantiation of the basic_error_desc template for the ANSI character type \c char
typedef basic_error_desc<ws_char_a_t>   error_desc_a;
/// Instantiation of the basic_error_desc template for the Unicode character type \c wchar_t
typedef basic_error_desc<ws_char_w_t>   error_desc_w;
/// Instantiation of the basic_error_desc template for the Win32 character type \c TCHAR
typedef basic_error_desc<TCHAR>         error_desc;

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline basic_error_desc<C, T>::basic_error_desc(ws_dword_t error /* = ::GetLastError() */, char_type const *paths /* = NULL */)
    : m_message(NULL)
{
    ws_dword_t  cch =   0;

    if(NULL != paths)
    {
        HINSTANCE   hinstSource =   traits_type::load_library(paths);

        if(NULL != hinstSource)
        {
            cch =   FormatMessage(error, hinstSource, &m_message);

            traits_type::free_library(hinstSource);
        }
    }

    if(NULL == m_message)
    {
        cch = FormatMessage(error, NULL, &m_message);
    }

    if(0 == cch)
    {
        m_message = NULL;
    }
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline basic_error_desc<C, T>::~basic_error_desc() winstl_throw_0()
{
    if(m_message != NULL)
    {
        ::LocalFree(m_message);
    }
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_error_desc<C, T>::char_type const *basic_error_desc<C, T>::get_description() const
{
    return (NULL != m_message) ? m_message : "";
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
#if defined(__STLSOFT_COMPILER_IS_GCC)
inline basic_error_desc<C, T>::operator C const *() const
#else /* ? compiler */
inline basic_error_desc<C, T>::operator ss_typename_type_k basic_error_desc<C, T>::char_type const *() const
#endif /* compiler */
{
    return get_description();
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * String access shims
 */

#ifndef __STLSOFT_CF_TEMPLATE_SHIMS_NOT_SUPPORTED

/// \brief Returns the corresponding C-string pointer of \c e
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline C const *c_str_ptr_null(basic_error_desc<C, T> const &e)
{
    C const *p  =   e;

    return ('\0' != *e) ? p : NULL;
}

/// \brief Returns the corresponding C-string pointer of \c e
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline C const *c_str_ptr(basic_error_desc<C, T> const &e)
{
    return e;
}

/// \brief Returns the corresponding C-string pointer of \c e
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline C const *get_ptr(basic_error_desc<C, T> const &e)
{
    return e;
}

#endif /* !__STLSOFT_CF_TEMPLATE_SHIMS_NOT_SUPPORTED */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_error_desc(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "error_desc", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_error_desc(test_winstl_error_desc);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_error_library

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

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The string access shims exist either in the stlsoft namespace, or in the
 * global namespace. This is required by the lookup rules.
 *
 */

#ifndef _WINSTL_NO_NAMESPACE
# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
namespace stlsoft
{
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */

using ::winstl::c_str_ptr_null;

using ::winstl::c_str_ptr;

using ::winstl::get_ptr;

# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace stlsoft
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WINSTL_INCL_H_WINSTL_ERROR_DESC */

/* ////////////////////////////////////////////////////////////////////////// */
