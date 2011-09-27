/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_absolute_path.h
 *
 * Purpose:     Simple class that converts a relative path to an absolute one.
 *
 * Created:     20th December 2002
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


/// \file winstl_absolute_path.h
///
/// Simple class that converts a relative path to an absolute one.

#ifndef WINSTL_INCL_H_WINSTL_ABSOLUTE_PATH
#define WINSTL_INCL_H_WINSTL_ABSOLUTE_PATH

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_ABSOLUTE_PATH_MAJOR        2
# define WINSTL_VER_H_WINSTL_ABSOLUTE_PATH_MINOR        0
# define WINSTL_VER_H_WINSTL_ABSOLUTE_PATH_REVISION     1
# define WINSTL_VER_H_WINSTL_ABSOLUTE_PATH_EDIT         29
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                    // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_SYSTEM_VERSION
# include "winstl_filesystem_traits.h"  // Include the WinSTL system_version
#endif /* !WINSTL_INCL_H_WINSTL_SYSTEM_VERSION */
#ifndef WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER
# include "winstl_file_path_buffer.h"   // basic_file_path_buffer
#endif /* !WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"     // stlsoft::c_str_ptr
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef WINSTL_INCL_H_WINSTL_STRING_ACCESS
# include "winstl_string_access.h"      // winstl::c_str_ptr
#endif /* !WINSTL_INCL_H_WINSTL_STRING_ACCESS */

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

/// \weakgroup libraries_filesystem File-System Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating file-system objects

/// \weakgroup winstl_filesystem_library File-System Library (WinSTL)
/// \ingroup WinSTL libraries_filesystem
/// \brief This library provides facilities for defining and manipulating file-system objects for the Win32 API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * basic_absolute_path
 *
 * This class converts a relative path to an absolute one, and effectively acts
 * as a C-string of its value.
 */

/// Converts a relative path to an absolute path
///
/// \param C The character type
/// \param T The traits type. On translators that support default template arguments, this defaults to filesystem_traits<C>
template<   ss_typename_param_k C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k T = filesystem_traits<C>
#else
        ,   ss_typename_param_k T /* = filesystem_traits<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_absolute_path
{
public:
    /// The char type
    typedef C                           char_type;
    /// The traits type
    typedef T                           traits_type;
    /// The current parameterisation of the type
    typedef basic_absolute_path<C, T>   class_type;
    /// The size type
    typedef ws_size_t                   size_type;

// Construction
public:
    /// Constructs an absolute path from \c path
    ss_explicit_k basic_absolute_path(char_type const *path)
        : m_len(traits_type::get_full_path_name(path, winstl_num_elements(m_path), &m_path[0]))
    {}
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    /// Constructs an absolute path from \c path
    template<ss_typename_param_k S>
    ss_explicit_k basic_absolute_path(S const &path)
        : m_len(traits_type::get_full_path_name(c_str_ptr(path), winstl_num_elements(m_path), &m_path[0]))
    {}
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */

// Conversions
public:
    /// Implicit conversion to a non-mutable (const) pointer to the path
    operator char_type const *() const
    {
        return stlsoft_ns_qual(c_str_ptr)(m_path);
    }

// Attributes
public:
    /// Returns the length of the converted path
    size_type length() const
    {
        return m_len;
    }

// Members
private:
    basic_file_path_buffer<char_type>   m_path;
    size_type const                     m_len;

// Not to be implemented
private:
    basic_absolute_path(const class_type &);
    basic_absolute_path &operator =(const class_type &);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs for commonly encountered types
 */

/// Instantiation of the basic_absolute_path template for the ANSI character type \c char
typedef basic_absolute_path<ws_char_a_t, filesystem_traits<ws_char_a_t> >       absolute_path_a;
/// Instantiation of the basic_absolute_path template for the Unicode character type \c wchar_t
typedef basic_absolute_path<ws_char_w_t, filesystem_traits<ws_char_w_t> >       absolute_path_w;
/// Instantiation of the basic_absolute_path template for the Win32 character type \c TCHAR
typedef basic_absolute_path<TCHAR, filesystem_traits<TCHAR> >                   absolute_path;

/* /////////////////////////////////////////////////////////////////////////////
 * Helper functions
 */

#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
    _MSC_VER >= 1100

/// This helper function makes an absolute path variable without needing to
/// qualify the template parameter.
template<ss_typename_param_k C>
inline basic_absolute_path<C> make_absolute_path(C const *path)
{
    return basic_absolute_path<C>(path);
}

#endif /* !(_MSC_VER < 1100) */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_absolute_path(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "absolute_path", __FILE__);

        TCHAR   cwd[1 + _MAX_PATH];

        ::GetCurrentDirectory(stlsoft_num_elements(cwd), cwd);

        if(0 != lstrcmp(cwd, absolute_path(".")))
        {
            r->report("absolute path for \".\" failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_absolute_path(test_winstl_absolute_path);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_filesystem_library

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

#endif /* WINSTL_INCL_H_WINSTL_ABSOLUTE_PATH */

/* ////////////////////////////////////////////////////////////////////////// */
