/* /////////////////////////////////////////////////////////////////////////////
 * File:        inetstl_session.h
 *
 * Purpose:     Contains the internet_exception class.
 *
 * Created:     25th April 2004
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


/// \file inetstl_exceptions.h
///
/// Contains the internet_exception class.

#ifndef INETSTL_INCL_H_INETSTL_EXCEPTIONS
#define INETSTL_INCL_H_INETSTL_EXCEPTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define INETSTL_VER_H_INETSTL_EXCEPTIONS_MAJOR     2
# define INETSTL_VER_H_INETSTL_EXCEPTIONS_MINOR     0
# define INETSTL_VER_H_INETSTL_EXCEPTIONS_REVISION  1
# define INETSTL_VER_H_INETSTL_EXCEPTIONS_EDIT      11
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef INETSTL_INCL_H_INETSTL
# include "inetstl.h"                        // Include the InetSTL root header
#endif /* !INETSTL_INCL_H_INETSTL */

#ifndef __STLSOFT_CF_EXCEPTION_SUPPORT
# error This file cannot be included when exception-handling is not supported
#endif /* !__STLSOFT_CF_EXCEPTION_SUPPORT */

#include <exception>

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _INETSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::inetstl */
namespace inetstl
{
# else
/* Define stlsoft::inetstl_project */

namespace stlsoft
{

namespace inetstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_INETSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_filesystem File-System Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating file-system objects

/// \weakgroup inetstl_filesystem_library File-System Library (InetSTL)
/// \ingroup InetSTL libraries_filesystem
/// \brief This library provides facilities for defining and manipulating file-system objects for the WinInet API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// The exception type thrown by throw_internet_exception_policy
class internet_exception
#if defined(__STLSOFT_COMPILER_IS_DMC)
    : public std::exception
#else /* ? compiler */
    : public inetstl_ns_qual_std(exception)
#endif /* compiler */
{
public:
#if defined(__STLSOFT_COMPILER_IS_DMC)
    typedef std::exception                  parent_class_type;
#else /* ? compiler */
    typedef inetstl_ns_qual_std(exception)  parent_class_type;
#endif /* compiler */
    typedef internet_exception              class_type;

public:
    ss_explicit_k internet_exception(is_dword_t errorCode) inetstl_throw_0()
        : m_errorCode(errorCode)
    {}

#if defined(__STLSOFT_COMPILER_IS_DMC)
    char const  *what() const throw()
#else /* ? compiler */
    char const  *what() const inetstl_throw_0()
#endif /* compiler */
    {
        return "Internet failure";
    }

    is_dword_t error() const inetstl_throw_0()
    {
        return m_errorCode;
    }

private:
    is_dword_t const    m_errorCode;

// Not to be implemented
private:
    class_type &operator =(class_type const &);
};

/// This type throws internet_exception
struct throw_internet_exception_policy
{
public:
    /// The exception type
    typedef internet_exception  thrown_type;

public:
    /// The function call operator, which does not throw an exception
    ///
    /// \param error The Win32/WinInet error value associated with the exceptional condition
    void operator ()(is_dword_t error) inetstl_throw_1(internet_exception)
    {
        throw internet_exception(error);
    }
};

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group inetstl_filesystem_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _INETSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace inetstl
# else
} // namespace inetstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_INETSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* INETSTL_INCL_H_INETSTL_EXCEPTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
