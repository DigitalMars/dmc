/* ////////////////////////////////////////////////////////////////////////////
 * File:        unixstl_exceptions.h
 *
 * Purpose:     unix_exception class, and its policy class
 *
 * Date:        19th June 2004
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


/// \file unixstl_exceptions.h
///
/// unix_exception class, and its policy class

#ifndef UNIXSTL_INCL_H_UNIXSTL_EXCEPTIONS
#define UNIXSTL_INCL_H_UNIXSTL_EXCEPTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_EXCEPTIONS_MAJOR       2
# define UNIXSTL_VER_H_UNIXSTL_EXCEPTIONS_MINOR       0
# define UNIXSTL_VER_H_UNIXSTL_EXCEPTIONS_REVISION    1
# define UNIXSTL_VER_H_UNIXSTL_EXCEPTIONS_EDIT        4
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef UNIXSTL_INCL_H_UNIXSTL
# include "unixstl.h"                // Include the UNIXSTL root header
#endif /* !UNIXSTL_INCL_H_UNIXSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_EXCEPTIONS
# include "stlsoft_exceptions.h"    // os_exception
#endif /* !STLSOFT_INCL_H_STLSOFT_EXCEPTIONS */
#include <errno.h>

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

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// \brief Exception class for general Windows operating system failures
class unix_exception
    : public os_exception
{
/// \name Types
/// @{
public:
    typedef unix_exception   class_type;
    typedef os_exception        parent_class_type;
/// @}

/// \name Construction
/// @{
public:
    ss_explicit_k unix_exception(int err)
        : m_errno(err)
    {}
    unix_exception(char const * /* reason */, int err)
        : m_errno(err)
    {}
/// @}

/// \name Accessors
/// @{
public:
    virtual char const *what() const
    {
        return "Windows system exception";
    }

    int get_errno() const
    {
        return m_errno;
    }
/// @}

// Members
private:
    int m_errno;
};

/* ////////////////////////////////////////////////////////////////////////////
 * Policies
 */

/// The NULL exception type. It does not throw, and its throw type is empty.
struct unix_exception_policy
{
public:
    /// The thrown type
    typedef unix_exception   thrown_type;

public:
    /// Function call operator, taking no parameters
    void operator ()()
    {
        throw unix_exception(errno);
    }
    /// Function call operator, taking one parameter
    void operator ()(int err) const
    {
        throw unix_exception(err);
    }
    /// Function call operator, taking two parameters
    void operator ()(char const *reason, int err) const
    {
        throw unix_exception(reason, err);
    }
#if 0
    /// Function call operator, taking three parameters
    template<   ss_typename_param_k T1
            ,   ss_typename_param_k T2
            ,   ss_typename_param_k T3
            >
    void operator ()(T1 const &/* t1 */, T2 const &/* t2 */, T3 const &/* t3 */) const
    {
        // Do nothing
    }
    /// Function call operator, taking four parameters
    template<   ss_typename_param_k T1
            ,   ss_typename_param_k T2
            ,   ss_typename_param_k T3
            ,   ss_typename_param_k T4
            >
    void operator ()(T1 const &/* t1 */, T2 const &/* t2 */, T3 const &/* t3 */, T4 const &/* t4 */) const
    {
        // Do nothing
    }
#endif /* 0 */
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_unixstl_exceptions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "exceptions", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_unixstl_exceptions(test_unixstl_exceptions);

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

#endif /* !UNIXSTL_INCL_H_UNIXSTL_EXCEPTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
