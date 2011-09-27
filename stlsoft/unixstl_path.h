/* /////////////////////////////////////////////////////////////////////////////
 * File:        unixstl_path.h (formerly MLPath.h, ::SynesisStd)
 *
 * Purpose:     Simple class that represents a path.
 *
 * Created:     1st May 1993
 * Updated:     12th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1993-2004, Matthew Wilson and Synesis Software
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


/// \file unixstl_path.h
///
/// Simple class that represents a path

#ifndef UNIXSTL_INCL_H_UNIXSTL_PATH
#define UNIXSTL_INCL_H_UNIXSTL_PATH

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_PATH_MAJOR     4
# define UNIXSTL_VER_H_UNIXSTL_PATH_MINOR     0
# define UNIXSTL_VER_H_UNIXSTL_PATH_REVISION  1
# define UNIXSTL_VER_H_UNIXSTL_PATH_EDIT      149
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef UNIXSTL_INCL_H_UNIXSTL
# include "unixstl.h"                       // Include the UNIXSTL root header
#endif /* !UNIXSTL_INCL_H_UNIXSTL */
#ifndef UNIXSTL_INCL_H_UNIXSTL_SYSTEM_VERSION
# include "unixstl_filesystem_traits.h"     // filesystem_traits
#endif /* !UNIXSTL_INCL_H_UNIXSTL_SYSTEM_VERSION */
#ifndef UNIXSTL_INCL_H_UNIXSTL_FILE_PATH_BUFFER
# include "unixstl_file_path_buffer.h"      // basic_file_path_buffer
#endif /* !UNIXSTL_INCL_H_UNIXSTL_FILE_PATH_BUFFER */
#ifndef UNIXSTL_INCL_H_STLSOFT_MALLOC_ALLOCATOR
# include "stlsoft_malloc_allocator.h"      // malloc_allocator
#endif /* !UNIXSTL_INCL_H_STLSOFT_MALLOC_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"         // stlsoft::c_str_ptr
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef UNIXSTL_INCL_H_UNIXSTL_STRING_ACCESS
# include "unixstl_string_access.h"         // unixstl::c_str_ptr
#endif /* !UNIXSTL_INCL_H_UNIXSTL_STRING_ACCESS */

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

#if !defined(__STLSOFT_COMPILER_IS_MWERKS)
stlsoft_ns_using(c_str_ptr)
#endif /* compiler */

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
 * basic_path
 *
 * This class represents a path, and effectively acts as a C-string of its value.
 */

/// \brief Represents a path
///
/// \param C The character type
/// \param T The traits type. On translators that support default template arguments, this defaults to filesystem_traits<C>
/// \param A The allocator type. On translators that support default template arguments, this defaults to malloc_allocator<C>
///
/// \note This class derives from the Synesis Software class Path, but has been influenced
/// by other, later, ideas. The idea of using the / operator for path concatenation was
/// sparked by the Boost implementation (although the details were not investigated prior
/// to this implementation, so the two may have significant semantic differences). This
/// has been added without requiring any major fundamental changes to the original
/// <code>push/pop<code>-based interface
template<   ss_typename_param_k C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k T = filesystem_traits<C>
        ,   ss_typename_param_k A = malloc_allocator<C>
#else
        ,   ss_typename_param_k T /* = filesystem_traits<C> */
        ,   ss_typename_param_k A /* = malloc_allocator<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_path
{
/// \name Types
/// @{
public:
    /// The char type
    typedef C                           char_type;
    /// The traits type
    typedef T                           traits_type;
    /// The allocator type
    typedef A                           allocator_type;
    /// The current parameterisation of the type
    typedef basic_path<C, T, A>         class_type;
    /// The size type
    typedef us_size_t                   size_type;

// TODO: Use the slice string, and provide iterators over the directory parts

/// @}

/// \name Construction
/// @{
public:
    /// Constructs a path from \c path
    ss_explicit_k basic_path(char_type const *path);
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    /// Constructs a path from \c path
    template<ss_typename_param_k S>
    ss_explicit_k basic_path(S const &s)
    {
        traits_type::str_copy(&m_buffer[0], stlsoft_ns_qual(c_str_ptr)(s));

        m_len = stlsoft_ns_qual(c_str_len)(s);
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */
    /// Constructs a path from \c cch characters in \c path
    basic_path(char_type const *path, size_type cch);

    basic_path(class_type const &rhs);

    class_type &operator =(class_type const &rhs);
    class_type &operator =(char_type const *rhs);

    // Creates a root path
    static class_type root(char_type const *s);
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    // Creates a root path
    template<ss_typename_param_k S>
    static class_type root(S const &s)
    {
        return root(stlsoft_ns_qual(c_str_ptr)(s));
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */
/// @}

/// \name Operations
/// @{
public:
    class_type &push(class_type const &rhs, us_bool_t bAddPathNameSeparator = false);
    class_type &push(char_type const *rhs, us_bool_t bAddPathNameSeparator = false);
    class_type &push_ext(class_type const &rhs, us_bool_t bAddPathNameSeparator = false);
    class_type &push_ext(char_type const *rhs, us_bool_t bAddPathNameSeparator = false);
    class_type &push_sep();
    class_type &pop();
    class_type &pop_sep();

    class_type &operator /=(class_type const &rhs);
    class_type &operator /=(char_type const *rhs);

    class_type &make_absolute(us_bool_t bRemoveTrailingPathNameSeparator = true);
    class_type &canonicalise(us_bool_t bRemoveTrailingPathNameSeparator = true);

/// @}

/// \name Attributes
/// @{
public:
    /// Returns the length of the converted path
    size_type length() const;
    /// Conversion to a non-mutable (const) pointer to the path
    char_type const *c_str() const;
/// @}

/// \name Comparison
/// @{
public:
    us_bool_t equivalent(char_type const *rhs) const;
    us_bool_t equivalent(class_type const &rhs) const;

    us_bool_t equal(char_type const *rhs) const;
    us_bool_t equal(class_type const &rhs) const;
/// @}

/// \name Iteration
/// @{
public:
#if 0
    directory_iterator  dir_begin() const;
    directory_iterator  dir_end() const;
#endif /* 0 */
/// @}

// Implementation
private:
    void        swap(class_type &rhs);
    class_type  &concat_(char_type const *rhs);
    class_type  &concat_(class_type const &rhs);

// Members
private:
    typedef basic_file_path_buffer<char_type>   buffer_type;

    buffer_type m_buffer;
    size_type   m_len;
};

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs for commonly encountered types
 */

/// Instantiation of the basic_path template for the ANSI character type \c char
typedef basic_path<us_char_a_t, filesystem_traits<us_char_a_t> >       path_a;
/// Instantiation of the basic_path template for the Unicode character type \c wchar_t
typedef basic_path<us_char_w_t, filesystem_traits<us_char_w_t> >       path_w;

/* /////////////////////////////////////////////////////////////////////////////
 * Operators
 */

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline us_bool_t operator ==(basic_path<C, T, A> const &lhs, ss_typename_type_k basic_path<C, T, A>::char_type const *rhs)
{
    return lhs.equal(rhs);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline us_bool_t operator !=(basic_path<C, T, A> const &lhs, ss_typename_type_k basic_path<C, T, A>::char_type const *rhs)
{
    return !lhs.equal(rhs);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline us_bool_t operator ==(ss_typename_type_k basic_path<C, T, A>::char_type const *lhs, basic_path<C, T, A> const &rhs)
{
    return rhs.equal(lhs);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline us_bool_t operator !=(ss_typename_type_k basic_path<C, T, A>::char_type const *lhs, basic_path<C, T, A> const &rhs)
{
    return !rhs.equal(lhs);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline us_bool_t operator ==(basic_path<C, T, A> const &lhs, basic_path<C, T, A> const &rhs)
{
    return lhs.equal(rhs);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline us_bool_t operator !=(basic_path<C, T, A> const &lhs, basic_path<C, T, A> const &rhs)
{
    return !lhs.equal(rhs);
}

// operator /
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> operator /(basic_path<C, T, A> const &lhs, ss_typename_type_k basic_path<C, T, A>::char_type const *rhs)
{
    return basic_path<C, T, A>(lhs) /= rhs;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> operator /(ss_typename_type_k basic_path<C, T, A>::char_type const *lhs, basic_path<C, T, A> const &rhs)
{
    return basic_path<C, T, A>(lhs) /= rhs;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> operator /(basic_path<C, T, A> const &lhs, basic_path<C, T, A> const &rhs)
{
    return basic_path<C, T, A>(lhs) /= rhs;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Helper functions
 */

#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
    _MSC_VER >= 1100

/// This helper function makes a path variable without needing to
/// qualify the template parameter.
template<ss_typename_param_k C>
inline basic_path<C> make_path(C const *path)
{
    return basic_path<C>(path);
}

#endif /* !(_MSC_VER < 1100) */

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline C const *c_str_ptr_null(basic_path<C, T, A> const &b)
{
    return stlsoft_ns_qual(c_str_ptr_null)(b.c_str());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline C const *c_str_ptr(basic_path<C, T, A> const &b)
{
    return stlsoft_ns_qual(c_str_ptr)(b.c_str());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline us_size_t c_str_len(basic_path<C, T, A> const &b)
{
    return stlsoft_ns_qual(c_str_len)(b.c_str());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline us_size_t c_str_size(basic_path<C, T, A> const &b)
{
    return stlsoft_ns_qual(c_str_size)(b.c_str());
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline S &operator <<(S & s, basic_path<C, T, A> const &b)
{
    s << b.c_str();

    return s;
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_unixstl_path(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "path", __FILE__);

        path_a  path1("..");
        path_a  path2(".\\..");

        if(!path1.equivalent(path2))
        {
            r->report("equivalence test failed ", __LINE__);
            bSuccess = false;
        }

        path_a  path3(path1);

        if(path1 != path3)
        {
            r->report("copy-construction failed ", __LINE__);
            bSuccess = false;
        }

        path3 /= "sub1";

        path_a  path4("..\\sub1");

        if(path4 != path3)
        {
            r->report("concatenation failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_unixstl_path(test_unixstl_path);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

////////////////////////////////////////////////////////////////////////////////
// Implementation

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline void basic_path<C, T, A>::swap(basic_path<C, T, A> &rhs)
{
    m_buffer.swap(rhs.m_buffer);
    std::swap(m_len, rhs.m_len);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_param_k basic_path<C, T, A>::class_type &basic_path<C, T, A>::concat_(ss_typename_param_k basic_path<C, T, A>::char_type const *rhs)
{
    m_len = traits_type::str_len(traits_type::str_cat(&m_buffer[0], rhs));

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_param_k basic_path<C, T, A>::class_type &basic_path<C, T, A>::concat_(basic_path<C, T, A> const &rhs)
{
    m_len = traits_type::str_len(traits_type::str_cat(&m_buffer[0], .c_str()));

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* ss_explicit_k */ basic_path<C, T, A>::basic_path(ss_typename_type_k basic_path<C, T, A>::char_type const *path)
    : m_len(0)
{
    if(NULL != path)
    {
        us_size_t   cch =   traits_type::str_len(path);

        unixstl_assert(cch < m_buffer.size());

        traits_type::str_copy(&m_buffer[0], path);

        m_len = cch;
    }
}

template<   ss_typename_param_k C
    ,   ss_typename_param_k T
    ,   ss_typename_param_k A
    >
inline basic_path<C, T, A>::basic_path(ss_typename_type_k basic_path<C, T, A>::char_type const *path, ss_typename_type_k basic_path<C, T, A>::size_type cch)
    : m_len(cch)
{
    stlsoft_assert((NULL == path) == (0 == cch));

    if(0 != cch)
    {
        unixstl_assert(cch < m_buffer.size());

        traits_type::str_n_copy(&m_buffer[0], path, cch);
        m_buffer[cch] = '\0';
    }
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A>::basic_path(basic_path<C, T, A> const &rhs)
    : m_len(rhs.m_len)
{
    traits_type::str_copy(&m_buffer[0], stlsoft_ns_qual(c_str_ptr)(rhs.m_buffer));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::operator =(basic_path<C, T, A> const &path)
{
    class_type  newPath(path);

    swap(newPath);

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::operator =(ss_typename_type_k basic_path<C, T, A>::char_type const *path)
{
    class_type  newPath(path);

    swap(newPath);

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ ss_typename_type_k basic_path<C, T, A>::class_type basic_path<C, T, A>::root(ss_typename_type_k basic_path<C, T, A>::char_type const *s)
{
    return class_type(s);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::push(class_type const &rhs, us_bool_t bAddPathNameSeparator /* = false */)
{
    return push(rhs.c_str(), bAddPathNameSeparator);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::push(char_type const *rhs, us_bool_t bAddPathNameSeparator /* = false */)
{
    class_type  newPath(*this);

    newPath.push_sep();
    newPath.concat_(rhs);
    if(bAddPathNameSeparator)
    {
        newPath.push_sep();
    }

    swap(newPath);

    return *this;
}

#if 0
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::push_ext(class_type const &rhs, us_bool_t bAddPathNameSeparator /* = false */)
{
}
#endif /* 0 */

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::push_ext(char_type const *rhs, us_bool_t bAddPathNameSeparator /* = false */)
{
    unixstl_assert(NULL != rhs);

    class_type  newPath(*this);

    newPath.pop_sep();
    if('.' != *rhs)
    {
        newPath.concat_(".");
    }
    newPath.concat_(rhs);
    if(bAddPathNameSeparator)
    {
        newPath.push_sep();
    }

    swap(newPath);

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::push_sep()
{
    if(0 != m_len)
    {
        if(traits_type::path_name_separator() != m_buffer[m_len - 1])
        {
            unixstl_assert(m_len + 1 < m_buffer.size());

            m_buffer[m_len]     =   '\\';
            m_buffer[m_len + 1] =   '\0';
            ++m_len;
        }
    }

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::pop();

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::pop_sep()
{
    if(0 != m_len)
    {
        char_type *last = &m_buffer[m_len - 1];

        if(traits_type::has_dir_end(last))
        {
            m_buffer[m_len-- - 1] = '\0';
        }
    }

    return *this;
}


template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::operator /=(basic_path<C, T, A> const &path)
{
    return push(path);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::operator /=(ss_typename_type_k basic_path<C, T, A>::char_type const *path)
{
    return push(path);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::make_absolute(us_bool_t bRemoveTrailingPathNameSeparator /* = true */)
{
    buffer_type buffer;
    us_dword_t  cch = traits_type::get_full_path_name(c_str(), buffer.size(), &buffer[0]);
    class_type  newPath(stlsoft_ns_qual(c_str_ptr)(buffer), cch);

    if(bRemoveTrailingPathNameSeparator)
    {
        newPath.pop_sep();
    }

    swap(newPath);

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_path<C, T, A> &basic_path<C, T, A>::canonicalise(us_bool_t bRemoveTrailingPathNameSeparator /* = true */)
{
    class_type  newPath(*this);

#if 0

    us_dword_t  cchRhs  =   traits_type::get_full_path_name(rhs, rhs_.size(), &rhs_[0]);

#if 0
    if( cchLhs == cchRhs + 1 &&
        !traits_type)
#endif /* 0 */

    return cchLhs == cchRhs && 0 == traits_type::str_compare_no_case(stlsoft_ns_qual(c_str_ptr)(lhs_), stlsoft_ns_qual(c_str_ptr)(rhs_));
#endif /* 0 */

    if(bRemoveTrailingPathNameSeparator)
    {
        newPath.pop_sep();
    }

    swap(newPath);

    return *this;
}



template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_path<C, T, A>::size_type basic_path<C, T, A>::length() const
{
    return m_len;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_path<C, T, A>::char_type const *basic_path<C, T, A>::c_str() const
{
    return stlsoft_ns_qual(c_str_ptr)(m_buffer);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline us_bool_t basic_path<C, T, A>::equivalent(basic_path<C, T, A> const &rhs) const
{
    return equivalent(rhs.c_str());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline us_bool_t basic_path<C, T, A>::equivalent(ss_typename_type_k basic_path<C, T, A>::char_type const *rhs) const
{
    class_type  lhs_(*this);
    class_type  rhs_(rhs);

    return lhs_.canonicalise(false).make_absolute(true) == rhs_.canonicalise(false).make_absolute(true);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline us_bool_t basic_path<C, T, A>::equal(basic_path<C, T, A> const &rhs) const
{
    return equal(rhs.c_str());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline us_bool_t basic_path<C, T, A>::equal(ss_typename_type_k basic_path<C, T, A>::char_type const *rhs) const
{
    return 0 == traits_type::str_compare_no_case(stlsoft_ns_qual(c_str_ptr)(m_buffer), stlsoft_ns_qual(c_str_ptr)(rhs));
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

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

#endif /* UNIXSTL_INCL_H_UNIXSTL_PATH */

/* ////////////////////////////////////////////////////////////////////////// */
