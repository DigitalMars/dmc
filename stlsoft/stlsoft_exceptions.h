/* ////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_exceptions.h
 *
 * Purpose:     Basic exception classes.
 *
 * Created:     19th January 2002
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


/// \file stlsoft_exceptions.h
///
/// Basic exception classes and policies.

#ifndef STLSOFT_INCL_H_STLSOFT_EXCEPTIONS
#define STLSOFT_INCL_H_STLSOFT_EXCEPTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_EXCEPTIONS_MAJOR     2
# define STLSOFT_VER_H_STLSOFT_EXCEPTIONS_MINOR     0
# define STLSOFT_VER_H_STLSOFT_EXCEPTIONS_REVISION  1
# define STLSOFT_VER_H_STLSOFT_EXCEPTIONS_EDIT      27
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#include <exception>

/* ////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// \brief Root exception class for sub-project platform-specific exceptions
class os_exception
    : public std::exception
{
/// \name Types
/// @{
public:
    /// The type of the current instantiation
    typedef os_exception        class_type;
    /// The parent type
    typedef std::exception      parent_class_type;
/// @}

/// \name Construction
/// @{
public:
    /// Default constructor
    os_exception()
    {}
/// @}

/// \name Accessors
/// @{
public:
    /// Returns a human-readable string describing the exception condition
    virtual char const *what() const stlsoft_throw_0()
    {
        return "operating system exception";
    }
/// @}
};

/* ////////////////////////////////////////////////////////////////////////////
 * Policies
 */

/// The NULL exception type. It does not throw, and its throw type is empty.
struct null_exception_policy
{
public:
    /// The thrown type
    struct thrown_type
    {
    };

public:
    /// Function call operator, taking no parameters
    void operator ()()
    {}
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator, taking one parameter
    template <ss_typename_param_k T>
    void operator ()(T const &/* t */) const stlsoft_throw_0()
    {
        // Do nothing
    }
    /// Function call operator, taking two parameters
    template<   ss_typename_param_k T1
            ,   ss_typename_param_k T2
            >
    void operator ()(T1 const &/* t1 */, T2 const &/* t2 */) const stlsoft_throw_0()
    {
        // Do nothing
    }
    /// Function call operator, taking three parameters
    template<   ss_typename_param_k T1
            ,   ss_typename_param_k T2
            ,   ss_typename_param_k T3
            >
    void operator ()(T1 const &/* t1 */, T2 const &/* t2 */, T3 const &/* t3 */) const stlsoft_throw_0()
    {
        // Do nothing
    }
    /// Function call operator, taking four parameters
    template<   ss_typename_param_k T1
            ,   ss_typename_param_k T2
            ,   ss_typename_param_k T3
            ,   ss_typename_param_k T4
            >
    void operator ()(T1 const &/* t1 */, T2 const &/* t2 */, T3 const &/* t3 */, T4 const &/* t4 */) const stlsoft_throw_0()
    {
        // Do nothing
    }
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

typedef null_exception_policy   null_exception;
typedef null_exception_policy   nothrow_exception;

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_EXCEPTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
