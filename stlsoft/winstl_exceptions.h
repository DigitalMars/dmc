/* ////////////////////////////////////////////////////////////////////////////
 * File:        winstl_exceptions.h
 *
 * Purpose:     windows_exception class, and its policy class
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


/// \file winstl_exceptions.h
///
/// windows_exception class, and its policy class

#ifndef WINSTL_INCL_H_WINSTL_EXCEPTIONS
#define WINSTL_INCL_H_WINSTL_EXCEPTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_EXCEPTIONS_MAJOR       2
# define WINSTL_VER_H_WINSTL_EXCEPTIONS_MINOR       0
# define WINSTL_VER_H_WINSTL_EXCEPTIONS_REVISION    1
# define WINSTL_VER_H_WINSTL_EXCEPTIONS_EDIT        5
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_EXCEPTIONS
# include "stlsoft_exceptions.h"    // os_exception
#endif /* !STLSOFT_INCL_H_STLSOFT_EXCEPTIONS */

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

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// \brief Exception class for general Windows operating system failures
class windows_exception
    : public os_exception
{
/// \name Types
/// @{
public:
    typedef windows_exception   class_type;
    typedef os_exception        parent_class_type;
/// @}

/// \name Construction
/// @{
public:
    ss_explicit_k windows_exception(ws_dword_t lastError)
        : m_lastError(lastError)
    {}
    windows_exception(char const * /* reason */, ws_dword_t lastError)
        : m_lastError(lastError)
    {}
/// @}

/// \name Accessors
/// @{
public:
    virtual char const *what() const
    {
        return "Windows system exception";
    }

    ws_dword_t last_error() const
    {
        return m_lastError;
    }
/// @}

// Members
private:
    ws_dword_t m_lastError;
};

/* ////////////////////////////////////////////////////////////////////////////
 * Policies
 */

/// The NULL exception type. It does not throw, and its throw type is empty.
struct windows_exception_policy
{
public:
    /// The thrown type
    typedef windows_exception   thrown_type;

public:
    /// Function call operator, taking no parameters
    void operator ()()
    {
        throw windows_exception(::GetLastError());
    }
    /// Function call operator, taking one parameter
    void operator ()(ws_dword_t lastError) const
    {
        throw windows_exception(lastError);
    }
    /// Function call operator, taking two parameters
    void operator ()(char const *reason, ws_dword_t lastError) const
    {
        throw windows_exception(reason, lastError);
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
    ss_bool_t test_winstl_exceptions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "exceptions", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_exceptions(test_winstl_exceptions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

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

#endif /* !WINSTL_INCL_H_WINSTL_EXCEPTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
