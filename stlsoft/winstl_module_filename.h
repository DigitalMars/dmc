/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_module_filename.h (originally MWModFNm.h; ::SynesisWin)
 *
 * Purpose:     Preposterously simple class that gets, and makes accessible, the
 *              module filename.
 *
 * Created:     31st March 2002
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


/// \file winstl_module_filename.h
///
/// Preposterously simple class that gets, and makes accessible, the module filename.

#ifndef WINSTL_INCL_H_WINSTL_MODULE_FILENAME
#define WINSTL_INCL_H_WINSTL_MODULE_FILENAME

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_MODULE_FILENAME_MAJOR      2
# define WINSTL_VER_H_WINSTL_MODULE_FILENAME_MINOR      0
# define WINSTL_VER_H_WINSTL_MODULE_FILENAME_REVISION   1
# define WINSTL_VER_H_WINSTL_MODULE_FILENAME_EDIT       40
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_MSVC: _MSC_VER<1200
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200
# error winstl_module_filename.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS
# include "winstl_filesystem_traits.h"  // filesystem_traits
#endif /* !WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS */
#ifndef WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER
# include "winstl_file_path_buffer.h"   // basic_file_path_buffer
#endif /* !WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER */

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
 * basic_module_filename
 *
 * This class wraps the GetModuleFilename() API function, and effectively acts
 * as an adaptor between HINSTANCE and a C-string to the name.
 */

/// Represents a module filename
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
class basic_module_filename
{
public:
    /// The char type
    typedef C                           char_type;
    /// The traits type
    typedef T                           traits_type;
    /// The current parameterisation of the type
    typedef basic_module_filename<C, T> class_type;
    /// The size type
    typedef ws_size_t                   size_type;

// Construction
public:
    /// Default constructor - represent the filename of the current process
    basic_module_filename();
    /// Represent the filename of the given instance
    ss_explicit_k basic_module_filename(HINSTANCE hinst);

// Operations
public:
    /// Copy the module filename to the given buffer
    size_type           get_filename(ws_char_a_t *buffer, size_type cchBuffer) const;
    /// Copy the module filename to the given buffer
    size_type           get_filename(ws_char_w_t *buffer, size_type cchBuffer) const;
    /// Copy the module filename to the given buffer
    static size_type    get_filename(HINSTANCE hinst, ws_char_a_t *buffer, size_type cchBuffer);
    /// Copy the module filename to the given buffer
    static size_type    get_filename(HINSTANCE hinst, ws_char_w_t *buffer, size_type cchBuffer);

// Attributes
public:
    /// Returns a non-mutable (const) pointer to the path
    char_type const *get_filename() const;
    /// Returns the length of the converted path
    size_type       length() const;

// Conversions
public:
    /// Implicit conversion to a non-mutable (const) pointer to the path
    operator char_type const *() const
    {
        return get_filename();
    }

// Members
private:
    basic_file_path_buffer<char_type>   m_path;
    HINSTANCE const                     m_hinst;
    size_type const                     m_len;

// Not to be implemented
private:
    basic_module_filename(class_type const &);
    basic_module_filename &operator =(class_type const &);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs for commonly encountered types
 */

/// Instantiation of the basic_module_filename template for the ANSI character type \c char
typedef basic_module_filename<ws_char_a_t, filesystem_traits<ws_char_a_t> >     module_filename_a;
/// Instantiation of the basic_module_filename template for the Unicode character type \c wchar_t
typedef basic_module_filename<ws_char_w_t, filesystem_traits<ws_char_w_t> >     module_filename_w;
/// Instantiation of the basic_module_filename template for the Win32 character type \c TCHAR
typedef basic_module_filename<TCHAR, filesystem_traits<TCHAR> >                 module_filename;

/* //////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_module_filename(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "module_filename", __FILE__);

        TCHAR   mfn[1 + _MAX_PATH];

        ::GetModuleFileName(NULL, &mfn[0], stlsoft_num_elements(mfn));

        if(0 != lstrcmp(mfn, module_filename()))
        {
            r->report("module filename failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_module_filename(test_winstl_module_filename);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline basic_module_filename<C, T>::basic_module_filename()
    : m_hinst(::GetModuleHandle(NULL))
    , m_len(get_filename(&m_path[0], m_path.size()))
{
    stlsoft_static_assert(stlsoft_raw_offsetof(class_type, m_hinst) < stlsoft_raw_offsetof(class_type, m_len));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline basic_module_filename<C, T>::basic_module_filename(HINSTANCE hinst)
    : m_hinst(hinst)
    , m_len(get_filename(hinst, &m_path[0], m_path.size()))
{
    stlsoft_static_assert(stlsoft_raw_offsetof(class_type, m_hinst) < stlsoft_raw_offsetof(class_type, m_len));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_module_filename<C, T>::size_type basic_module_filename<C, T>::get_filename(ws_char_a_t *buffer, ss_typename_type_k basic_module_filename<C, T>::size_type cchBuffer) const
{
    return static_cast<size_type>(::GetModuleFileNameA(m_hinst, buffer, cchBuffer));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_module_filename<C, T>::size_type basic_module_filename<C, T>::get_filename(ws_char_w_t *buffer, ss_typename_type_k basic_module_filename<C, T>::size_type cchBuffer) const
{
    return static_cast<size_type>(::GetModuleFileNameW(m_hinst, buffer, cchBuffer));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline /* static */ ss_typename_type_k basic_module_filename<C, T>::size_type basic_module_filename<C, T>::get_filename(HINSTANCE hinst, ws_char_a_t *buffer, ss_typename_type_k basic_module_filename<C, T>::size_type cchBuffer)
{
    return static_cast<size_type>(::GetModuleFileNameA(hinst, buffer, cchBuffer));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline /* static */ ss_typename_type_k basic_module_filename<C, T>::size_type basic_module_filename<C, T>::get_filename(HINSTANCE hinst, ws_char_w_t *buffer, ss_typename_type_k basic_module_filename<C, T>::size_type cchBuffer)
{
    return static_cast<size_type>(::GetModuleFileNameW(hinst, buffer, cchBuffer));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_module_filename<C, T>::char_type const *basic_module_filename<C, T>::get_filename() const
{
    return stlsoft_ns_qual(c_str_ptr)(m_path);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_module_filename<C, T>::size_type basic_module_filename<C, T>::length() const
{
    return m_len;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * String access shims
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ws_char_a_t const *c_str_ptr_a_null(basic_module_filename<C, T> const &mfn)
{
    return mfn; // Can never be empty, so no need to cater for NULL return
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ws_char_w_t const *c_str_ptr_w_null(basic_module_filename<C, T> const &mfn)
{
    return mfn; // Can never be empty, so no need to cater for NULL return
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ws_char_a_t const *c_str_ptr_a(basic_module_filename<C, T> const &mfn)
{
    return mfn;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ws_char_w_t const *c_str_ptr_w(basic_module_filename<C, T> const &mfn)
{
    return mfn;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_module_filename<C, T>::size_type c_str_len_a(basic_module_filename<C, T> const &mfn)
{
    return mfn.length();
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_module_filename<C, T>::size_type c_str_size(basic_module_filename<C, T> const &mfn)
{
    return mfn.length() * sizeof(ss_typename_type_k basic_module_filename<C, T>::char_type);
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

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

#endif /* WINSTL_INCL_H_WINSTL_MODULE_FILENAME */

/* ////////////////////////////////////////////////////////////////////////// */
