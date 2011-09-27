/* ////////////////////////////////////////////////////////////////////////////
 * File:        comstl_interface_bad_cast.h
 *
 * Purpose:     Exception thrown when interface casts fail.
 *
 * Created:     22nd December 2003
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


/// \file comstl_bad_interface_cast.h
///
/// Exception thrown when interface casts fail.

#ifndef COMSTL_INCL_H_COMSTL_BAD_INTERFACE_CAST
#define COMSTL_INCL_H_COMSTL_BAD_INTERFACE_CAST

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_BAD_INTERFACE_CAST_MAJOR       2
# define COMSTL_VER_H_COMSTL_BAD_INTERFACE_CAST_MINOR       0
# define COMSTL_VER_H_COMSTL_BAD_INTERFACE_CAST_REVISION    1
# define COMSTL_VER_H_COMSTL_BAD_INTERFACE_CAST_EDIT        11
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"    // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */
#include <typeinfo>

/* ////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The COMSTL components are contained within the comstl namespace. This is
 * actually an alias for stlsoft::comstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _COMSTL_NO_NAMESPACE    comstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::comstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             comstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::comstl */
namespace comstl
{
# else
/* Define stlsoft::comstl_project */

namespace stlsoft
{

namespace comstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup comstl_itf_library Interface Library
/// \ingroup COMSTL libraries
/// \brief This library provides facilities for manipulating COM interfaces
/// @{

/* ////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Exception class thrown when interface queries fail
class bad_interface_cast
#if defined(__STLSOFT_COMPILER_IS_DMC)
    : public std::bad_cast
#else /* ? compiler */
    : public comstl_ns_qual_std(bad_cast)
#endif /* compiler */
{
public:
#if defined(__STLSOFT_COMPILER_IS_DMC)
    typedef std::bad_cast                   parent_class_type;
#else /* ? compiler */
    typedef comstl_ns_qual_std(bad_cast)    parent_class_type;
#endif /* compiler */
    typedef bad_interface_cast              class_type;

public:
    bad_interface_cast(REFIID riid, HRESULT hr) comstl_throw_0()
        : m_riid(riid)
        , m_hr(hr)
    {}

#if defined(__STLSOFT_COMPILER_IS_DMC)
    char const  *what() const throw()
#else /* ? compiler */
    char const  *what() const comstl_throw_0()
#endif /* compiler */
    {
        return "Failed to acquire requested interface";
    }

    REFIID      iid() const comstl_throw_0()
    {
        return m_riid;
    }

    HRESULT     hresult() const comstl_throw_0()
    {
        return m_hr;
    }

private:
    REFIID          m_riid;
    HRESULT const   m_hr;

// Not to be implemented
private:
    class_type &operator =(class_type const &);
};

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_itf_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace comstl
# else
} // namespace comstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !COMSTL_INCL_H_COMSTL_BAD_INTERFACE_CAST */

/* ////////////////////////////////////////////////////////////////////////// */
