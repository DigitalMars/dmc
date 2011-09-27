/* /////////////////////////////////////////////////////////////////////////////
 * File:        unixstl_file_path_buffer.h
 *
 * Purpose:     Contains the basic_file_path_buffer template class.
 *
 * Created:     24th May 2004
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


/// \file unixstl_file_path_buffer.h
///
/// Contains the basic_file_path_buffer template class.

#ifndef UNIXSTL_INCL_H_UNIXSTL_FILE_PATH_BUFFER
#define UNIXSTL_INCL_H_UNIXSTL_FILE_PATH_BUFFER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_FILE_PATH_BUFFER_MAJOR     2
# define UNIXSTL_VER_H_UNIXSTL_FILE_PATH_BUFFER_MINOR     0
# define UNIXSTL_VER_H_UNIXSTL_FILE_PATH_BUFFER_REVISION  1
# define UNIXSTL_VER_H_UNIXSTL_FILE_PATH_BUFFER_EDIT      13
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef UNIXSTL_INCL_H_UNIXSTL
# include "unixstl.h"                        // Include the UNIXSTL root header
#endif /* !UNIXSTL_INCL_H_UNIXSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"           // stlsoft::auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef STLSOFT_INCL_H_STLSOFT_MALLOC_ALLOCATOR
# include "stlsoft_malloc_allocator.h"      // stlsoft::malloc_allocator
#endif /* !STLSOFT_INCL_H_STLSOFT_MALLOC_ALLOCATOR */
#ifndef UNIXSTL_INCL_H_UNIXSTL_STRING_ACCESS
# include "unixstl_string_access.h"         // unixstl::c_str_ptr, etc.
#endif /* !UNIXSTL_INCL_H_UNIXSTL_STRING_ACCESS */
#include <unistd.h>

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::unixstl */
namespace unixstl
{
# else
/* Define stlsoft::unixstl_project */

namespace stlsoft
{

namespace unixstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_filesystem File-System Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating file-system objects

/// \weakgroup unixstl_filesystem_library File-System Library (UNIXSTL)
/// \ingroup UNIXSTL libraries_filesystem
/// \brief This library provides facilities for defining and manipulating file-system objects for the Win32 API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// class basic_file_path_buffer
/// Acts as a buffer with sufficient size for any drive on the host machine
///
/// This class is a non-template class primarily so that separate instantiations
/// are not created for each instantiation of the basic_file_path_buffer.
///
/// This class provides a simple function, which is to provide the maximum path
/// length for the host. This information is then cached due to the static nature
/// of the get_drivesvar_() method, although it can be reset by calling the
/// refresh() method on the buffer class.
///
/// \param C The character type
/// \param A The allocator type
template<   ss_typename_param_k C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k A = stlsoft_ns_qual(malloc_allocator)<C>
#else
        ,   ss_typename_param_k A /* = stlsoft_ns_qual(malloc_allocator)<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_file_path_buffer
{
#if defined(PATH_MAX)
    typedef stlsoft_ns_qual(auto_buffer)<C, A, 1 + PATH_MAX>    buffer_type;
#else /* ? PATH_MAX */
    typedef stlsoft_ns_qual(auto_buffer)<C, A, 1 + 512>         buffer_type;
#endif /* PATH_MAX */
public:
    /// The current parameterisation of the type
    typedef basic_file_path_buffer<C, A>                        class_type;
    /// The value type
    typedef ss_typename_type_k buffer_type::value_type          value_type;
    /// The reference type
    typedef value_type                                          &reference;
    /// The non-mutating (const) reference type
    typedef value_type const                                    &const_reference;
    /// The size type
    typedef ss_typename_type_k buffer_type::size_type           size_type;

// Construction
public:
    /// Default constructor
    basic_file_path_buffer()
#if defined(PATH_MAX)
        : m_buffer(1 + PATH_MAX)
#else /* ? PATH_MAX */
        : m_buffer(1 + pathconf("/", _PC_PATH_MAX))
#endif /* PATH_MAX */
    {}

    basic_file_path_buffer(class_type const &rhs)
        : m_buffer(rhs.size())
    {
        buffer_type::block_copy(m_buffer, rhs, rhs.size());
    }

    const basic_file_path_buffer &operator =(class_type const &rhs)
    {
        m_buffer.resize(rhs.size());
        buffer_type::block_copy(m_buffer, rhs, rhs.size());

        return *this;
    }

// Operations
public:
    void swap(class_type &rhs)
    {
        m_buffer.swap(rhs.m_buffer);
    }

// Accessors
public:
    /// Returns a pointer to a nul-terminated string
    value_type const *c_str() const
    {
        return m_buffer.data();
    }

    /// Returns a mutable (non-const) pointer to the internal buffer
    reference operator [](size_t index)
    {
        buffer_type   &this_  =   m_buffer;

        return this_[index];
    }
#if !defined(__STLSOFT_COMPILER_IS_COMO) && \
    !defined(__STLSOFT_COMPILER_IS_MWERKS)
    /// Returns a non-mutable (const) pointer to the internal buffer
    const_reference operator [](size_t index) const
    {
        unixstl_message_assert("Index out of range", !(size() < index));

        return m_buffer.data()[index];
    }
#endif /* compiler */

    /// Returns the size of the internal buffer
    size_type size() const
    {
        return m_buffer.size();
    }

// Operations
public:
    /// Causes the drives to be examined again for the next instance.
    ///
    /// \deprecated
    static void refresh() unixstl_throw_0()
    {}

// Members
private:
    buffer_type m_buffer;
};

/* Typedefs to commonly encountered types. */
/// Instantiation of the basic_file_path_buffer template for the ANSI character type \c char
typedef basic_file_path_buffer<us_char_a_t, stlsoft_ns_qual(malloc_allocator)<us_char_a_t> >    file_path_buffer_a;
/// Instantiation of the basic_file_path_buffer template for the Unicode character type \c wchar_t
typedef basic_file_path_buffer<us_char_w_t, stlsoft_ns_qual(malloc_allocator)<us_char_w_t> >    file_path_buffer_w;

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

template<   ss_typename_param_k C
        ,   ss_typename_param_k A
        >
inline C const *c_str_ptr_null(basic_file_path_buffer<C, A> const &b)
{
    return stlsoft_ns_qual(c_str_ptr_null)(b.c_str());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k A
        >
inline C const *c_str_ptr(basic_file_path_buffer<C, A> const &b)
{
    return stlsoft_ns_qual(c_str_ptr)(b.c_str());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k A
        >
inline us_size_t c_str_len(basic_file_path_buffer<C, A> const &b)
{
    return stlsoft_ns_qual(c_str_len)(b.c_str());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k A
        >
inline us_size_t c_str_size(basic_file_path_buffer<C, A> const &b)
{
    return stlsoft_ns_qual(c_str_size)(b.c_str());
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k A
        >
inline S &operator <<(S & s, basic_file_path_buffer<C, A> const &b)
{
    s << b.c_str();

    return s;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_unixstl_file_path_buffer(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "file_path_buffer", __FILE__);

        file_path_buffer_a      buf_a;

#if defined(PATH_MAX)
        if(1 + PATH_MAX != buf_a.size())
#else /* ? PATH_MAX */
        if(static_cast<ss_size_t>(1 + pathconf("/", _PC_PATH_MAX)) != buf_a.size())
#endif /* PATH_MAX */
        {
            r->report("ANSI buffer size incorrect", __LINE__);
            bSuccess = false;
        }

        file_path_buffer_w      buf_w;

#if defined(PATH_MAX)
        if(1 + PATH_MAX != buf_w.size())
#else /* ? PATH_MAX */
        if(static_cast<ss_size_t>(1 + pathconf("/", _PC_PATH_MAX)) != buf_w.size())
#endif /* PATH_MAX */
        {
            r->report("Unicode (Win9x) buffer size incorrect", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_unixstl_file_path_buffer(test_unixstl_file_path_buffer);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group unixstl_filesystem_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace unixstl
# else
} // namespace unixstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The string access shims exist either in the stlsoft namespace, or in the
 * global namespace. This is required by the lookup rules.
 *
 */

#ifndef _UNIXSTL_NO_NAMESPACE
# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
namespace stlsoft
{
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */

using ::unixstl::c_str_ptr_null;

using ::unixstl::c_str_ptr;

using ::unixstl::c_str_len;

using ::unixstl::c_str_size;

# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace stlsoft
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* UNIXSTL_INCL_H_UNIXSTL_FILE_PATH_BUFFER */

/* ////////////////////////////////////////////////////////////////////////// */
