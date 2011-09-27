/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_system_directory.h
 *
 * Purpose:     Simple class that gets, and makes accessible, the system
 *              directory.
 *
 * Created:     10th December 2002
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


/// \file winstl_system_directory.h
///
/// Simple class that gets, and makes accessible, the system directory.

#ifndef WINSTL_INCL_H_WINSTL_SYSTEM_DIRECTORY
#define WINSTL_INCL_H_WINSTL_SYSTEM_DIRECTORY

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_SYSTEM_DIRECTORY_MAJOR     2
# define WINSTL_VER_H_WINSTL_SYSTEM_DIRECTORY_MINOR     0
# define WINSTL_VER_H_WINSTL_SYSTEM_DIRECTORY_REVISION  1
# define WINSTL_VER_H_WINSTL_SYSTEM_DIRECTORY_EDIT      35
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                    // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS
# include "winstl_filesystem_traits.h"  // filesystem_traits
#endif /* !WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS */
#ifndef WINSTL_INCL_H_WINSTL_STRING_ACCESS
# include "winstl_string_access.h"                  // winstl::string_access
#endif /* WINSTL_INCL_H_WINSTL_STRING_ACCESS */
#ifndef STLSOFT_INCL_H_STLSOFT_CONSTRAINTS
# include "stlsoft_constraints.h"       // stlsoft::not_implicitly_comparable
#endif /* !STLSOFT_INCL_H_STLSOFT_CONSTRAINTS */

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

/// \weakgroup libraries_filesystem File-System Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating file-system objects

/// \weakgroup winstl_filesystem_library File-System Library (WinSTL)
/// \ingroup WinSTL libraries_filesystem
/// \brief This library provides facilities for defining and manipulating file-system objects for the Win32 API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * basic_system_directory
 *
 * This class wraps the GetSystemDirectory() API function, and effectively acts
 * as a C-string of its value.
 */

/// Represents the system directory
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
class basic_system_directory
{
public:
    /// The char type
    typedef C                       char_type;
    /// The traits type
    typedef T                       traits_type;
    /// The current parameterisation of the type
    typedef basic_system_directory<C, T>  class_type;
    /// The size type
    typedef ws_size_t               size_type;

// Operations
public:
    /// Gets the system directory into the given buffer
    static size_type   get_path(ws_char_a_t *buffer, size_type cchBuffer);
    /// Gets the system directory into the given buffer
    static size_type   get_path(ws_char_w_t *buffer, size_type cchBuffer);

// Attributes
public:
    /// Returns a non-mutable (const) pointer to the path
    char_type const *get_path() const;
    /// Returns a pointer to a nul-terminated string
    char_type const *c_str() const;
    /// Returns the length of the converted path
    size_type       length() const;

// Conversions
public:
    /// Implicit conversion to a non-mutable (const) pointer to the path
    operator char_type const *() const
    {
        return get_path();
    }

// Implementation
private:
    struct Information
    {
        char_type   m_dir[_MAX_PATH];
        size_type   m_len;

        Information()
            : m_len(get_path(m_dir, winstl_num_elements(m_dir)))
        {}
    };

#if !defined(__STLSOFT_COMPILER_IS_DMC)
    static Information const &get_information_()
    {
        static Information  s_info;

        return s_info;
    }
#else
    // Digital Mars gets an internal compiler error when the
    // preferred implementation is used, so we provide this
    // slightly less efficient implementation.
    Information m_information;
    Information const &get_information_() const
    {
        return m_information;
    }
#endif /* __STLSOFT_COMPILER_IS_DMC */
};

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs for commonly encountered types
 */

/// Instantiation of the basic_system_directory template for the ANSI character type \c char
typedef basic_system_directory<ws_char_a_t, filesystem_traits<ws_char_a_t> >    system_directory_a;
/// Instantiation of the basic_system_directory template for the Unicode character type \c wchar_t
typedef basic_system_directory<ws_char_w_t, filesystem_traits<ws_char_w_t> >    system_directory_w;
/// Instantiation of the basic_system_directory template for the Win32 character type \c TCHAR
typedef basic_system_directory<TCHAR, filesystem_traits<TCHAR> >                system_directory;

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline /* static */ ss_typename_type_k basic_system_directory<C, T>::size_type basic_system_directory<C, T>::get_path(ws_char_a_t *buffer, ss_typename_type_k basic_system_directory<C, T>::size_type cchBuffer)
{
    return static_cast<size_type>(::GetSystemDirectoryA(buffer, cchBuffer));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline /* static */ ss_typename_type_k basic_system_directory<C, T>::size_type basic_system_directory<C, T>::get_path(ws_char_w_t *buffer, ss_typename_type_k basic_system_directory<C, T>::size_type cchBuffer)
{
    return static_cast<size_type>(::GetSystemDirectoryW(buffer, cchBuffer));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_system_directory<C, T>::char_type const *basic_system_directory<C, T>::get_path() const
{
#if defined(__STLSOFT_COMPILER_IS_DMC)
    return get_information_().m_dir;
#else
    static char_type const    *s_sz  =   get_information_().m_dir;

    return s_sz;
#endif /* __STLSOFT_COMPILER_IS_DMC */
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_system_directory<C, T>::char_type const *basic_system_directory<C, T>::c_str() const
{
    return get_path();
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_system_directory<C, T>::size_type basic_system_directory<C, T>::length() const
{
    return get_information_().m_len;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_system_directory(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "system_directory", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_system_directory(test_winstl_system_directory);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_filesystem_library

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

/// \weakgroup concepts STLSoft Concepts

/// \weakgroup concepts_shims Shims
/// \ingroup concepts

/// \weakgroup concepts_shims_string_access String Access Shims
/// \ingroup concepts_shims
/// \brief These \ref concepts_shims "shims" retrieve the C-string for arbitrary types

/// \weakgroup winstl_string_access_shims String Access Shims (WinSTL)
/// \ingroup WinSTL concepts_shims_string_access
/// \brief These \ref concepts_shims "shims" retrieve the C-string for arbitrary types
/// @{

/// \brief Returns the corresponding C-string pointer of the basic_system_directory \c sd, or a null pointer
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline C const *c_str_ptr_null(basic_system_directory<C, T> const &sd)
{
    return sd;
}

/// \brief Returns the corresponding C-string pointer of the basic_system_directory \c sd
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline C const *c_str_ptr(basic_system_directory<C, T> const &sd)
{
    return sd;
}

/// \brief Returns the length (in characters) of the basic_system_directory \c sd, <b><i>not</i></b> including the null-terminating character
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ws_size_t c_str_len(basic_system_directory<C, T> const &sd)
{
    return sd.length();
}

/// \brief Returns the size (in bytes) of the basic_system_directory \c sd, <b><i>not</i></b> including the null-terminating character
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ws_size_t c_str_size(basic_system_directory<C, T> const &sd)
{
    return sd.length() * sizeof(C);
}

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_string_access_shims

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

using ::winstl::c_str_len;

using ::winstl::c_str_size;

# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace stlsoft
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WINSTL_INCL_H_WINSTL_SYSTEM_DIRECTORY */

/* ////////////////////////////////////////////////////////////////////////// */
