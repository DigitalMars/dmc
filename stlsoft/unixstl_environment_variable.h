/* /////////////////////////////////////////////////////////////////////////////
 * File:        unixstl_environment_variable.h
 *
 * Purpose:     Simple class that provides access to an environment variable.
 *
 * Created:     2nd November 2003
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


/// \file unixstl_environment_variable.h
///
/// Simple class that provides access to an environment variable.

#ifndef UNIXSTL_INCL_H_UNIXSTL_ENVIRONMENT_VARIABLE
#define UNIXSTL_INCL_H_UNIXSTL_ENVIRONMENT_VARIABLE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_ENVIRONMENT_VARIABLE_MAJOR     2
# define UNIXSTL_VER_H_UNIXSTL_ENVIRONMENT_VARIABLE_MINOR     0
# define UNIXSTL_VER_H_UNIXSTL_ENVIRONMENT_VARIABLE_REVISION  1
# define UNIXSTL_VER_H_UNIXSTL_ENVIRONMENT_VARIABLE_EDIT      27
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef UNIXSTL_INCL_H_UNIXSTL
# include "unixstl.h"                        // Include the UNIXSTL root header
#endif /* !UNIXSTL_INCL_H_UNIXSTL */
#ifndef UNIXSTL_INCL_H_UNIXSTL_SYSTEM_VERSION
# include "unixstl_filesystem_traits.h"      // Include the UNIXSTL get_environment_variable
#endif /* !UNIXSTL_INCL_H_UNIXSTL_SYSTEM_VERSION */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"         // stlsoft::c_str_ptr
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef UNIXSTL_INCL_H_UNIXSTL_STRING_ACCESS
# include "unixstl_string_access.h"          // unixstl::c_str_ptr
#endif /* !UNIXSTL_INCL_H_UNIXSTL_STRING_ACCESS */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"           // stlsoft::auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef STLSOFT_INCL_H_STLSOFT_MALLOC_ALLOCATOR
# include "stlsoft_malloc_allocator.h"      // malloc_allocator
#endif /* STLSOFT_INCL_H_STLSOFT_MALLOC_ALLOCATOR */

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

#if !defined(__STLSOFT_COMPILER_IS_GCC) && \
    __GNUC__ < 3
stlsoft_ns_using(c_str_ptr)
#endif /* !gcc or gcc >= 3 */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_system System Library
/// \ingroup libraries
/// \brief This library provides facilities for accessing system attributes

/// \defgroup unixstl_system_library System Library (UNIXSTL)
/// \ingroup UNIXSTL libraries_system
/// \brief This library provides facilities for accessing UNIX system attributes
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * basic_environment_variable
 *
 * This class converts a relative path to an absolute one, and effectively acts
 * as a C-string of its value.
 */

/// Represents an environment variable
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
class basic_environment_variable
{
public:
    /// The char type
    typedef C                                   char_type;
    /// The traits type
    typedef T                                   traits_type;
    /// The current parameterisation of the type
    typedef basic_environment_variable<C, T>    class_type;
    /// The size type
    typedef us_size_t                           size_type;

// Construction
public:
    /// Create an instance representing the given environment variable
    ss_explicit_k basic_environment_variable(char_type const *name)
        : m_buffer(1 + traits_type::get_environment_variable(name, 0, 0))
    {
        if( 0 == traits_type::get_environment_variable(name, m_buffer, m_buffer.size()) &&
            0 != m_buffer.size())
        {
            m_buffer[0] = 0;
        }
    }
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    /// Create an instance representing the given environment variable
    template<ss_typename_param_k S>
    ss_explicit_k basic_environment_variable(S const &name)
        : m_buffer(1 + traits_type::get_environment_variable(c_str_ptr(name), 0, 0))
    {
        if( 0 == traits_type::get_environment_variable(c_str_ptr(name), m_buffer, m_buffer.size()) &&
            0 != m_buffer.size())
        {
            m_buffer[0] = 0;
        }
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */

// Conversions
public:
    /// Implicit conversion to a non-mutable (const) pointer to the variable
    operator char_type const *() const
    {
        return m_buffer.data();
    }

// Attributes
public:
    /// Returns the length of the variable
    size_type length() const
    {
        return m_buffer.size() - 1;
    }

// Members
private:
    typedef stlsoft_ns_qual(auto_buffer)<char_type, malloc_allocator<char_type> >  buffer_t;

    buffer_t    m_buffer;

// Not to be implemented
private:
    basic_environment_variable(basic_environment_variable const &);
    basic_environment_variable &operator =(basic_environment_variable const &);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs for commonly encountered types
 */

/// Instantiation of the basic_environment_variable template for the ANSI character type \c char
typedef basic_environment_variable<us_char_a_t, filesystem_traits<us_char_a_t> >     environment_variable_a;
/// Instantiation of the basic_environment_variable template for the Unicode character type \c wchar_t
typedef basic_environment_variable<us_char_w_t, filesystem_traits<us_char_w_t> >     environment_variable_w;

/* /////////////////////////////////////////////////////////////////////////////
 * Helper functions
 */

#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
    _MSC_VER >= 1100

/// This helper function makes an environment variable without needing to
/// qualify the template parameter.
template<ss_typename_param_k C>
inline basic_environment_variable<C> make_environment_variable(C const *path)
{
    return basic_environment_variable<C>(path);
}

#endif /* !(_MSC_VER < 1100) */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_unixstl_environment_variable(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "environment_variable", __FILE__);

        typedef basic_environment_variable<char>    env_var_t;

        env_var_t   path("PATH");

        if(0 != strcmp(getenv("PATH"), path))
        {
            r->report("basic_environment_variable<char> failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_unixstl_environment_variable(test_unixstl_environment_variable);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

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

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* UNIXSTL_INCL_H_UNIXSTL_ENVIRONMENT_VARIABLE */

/* ////////////////////////////////////////////////////////////////////////// */
