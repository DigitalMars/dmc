/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_null.h
 *
 * Purpose:     NULL_v template class.
 *
 * Created:     8th September 2002
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


/// \file stlsoft_null.h
///
/// NULL_v template class.

#ifndef STLSOFT_INCL_H_STLSOFT_NULL
#define STLSOFT_INCL_H_STLSOFT_NULL

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_NULL_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_NULL_MINOR       0
# define STLSOFT_VER_H_STLSOFT_NULL_REVISION    1
# define STLSOFT_VER_H_STLSOFT_NULL_EDIT        29
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

/* _STLSOFT_NULL_v_DEFINED */

#ifdef _STLSOFT_NULL_v_DEFINED
# undef _STLSOFT_NULL_v_DEFINED
#endif /* _STLSOFT_NULL_v_DEFINED */

#define _STLSOFT_NULL_v_DEFINED

#if defined(__STLSOFT_COMPILER_IS_DMC)
// TODO: Review this for a future release of DMC++
# undef _STLSOFT_NULL_v_DEFINED
#elif defined(__STLSOFT_COMPILER_IS_MSVC) && \
      _MSC_VER < 1310
# undef _STLSOFT_NULL_v_DEFINED
#elif defined(__STLSOFT_COMPILER_IS_WATCOM)
# undef _STLSOFT_NULL_v_DEFINED
#endif /* compiler */

/* _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT */

#ifdef _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT
# undef _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT
#endif /* _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT */

#define _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT

#if defined(__STLSOFT_COMPILER_IS_GCC)
# undef _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT
#elif defined(__STLSOFT_COMPILER_IS_MWERKS)
# undef _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifdef _STLSOFT_NULL_v_DEFINED

/// \brief Represents a type that can be an active replacement for NULL
///
/// This class can act as a replacement for the NULL macro, by being validly
/// assigned to or equated with pointer types only, as in
///
///   int   i = NULL; // error
///   int   *p = NULL; // OK
///
///   if(i == NULL) {} // error
///   if(NULL == i) {} // error
///
///   if(p == NULL) {} // OK
///   if(NULL == p) {} // OK
///
///
/// When used via inclusion of the file stlsoft_nulldef.h, the macro NULL is
/// redefined as NULL_v(), such that expressions containing NULL will be valid
/// against pointers only.
struct NULL_v
{
// Construction
public:
    /// Default constructor
    NULL_v()
    {}

/// Static creation
public:
    static NULL_v create()
    {
        return NULL_v();
    }

// Conversion
public:
    /// Implicit conversion operator (convertible to any pointer type)
    template <ss_typename_param_k T>
    operator T *() const
    {
        return 0;
    }

#ifdef _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT
    /// Implicit conversion operator (convertible to any pointer type)
    template <ss_typename_param_k T2, ss_typename_param_k C>
    operator T2 C::*() const
    {
        return 0;
    }
#endif /* _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT */

    /// Evaluates whether an instance of a type is null
    ///
    /// \param rhs A reference arbitrary type which will be compared to null
    template <ss_typename_param_k T>
    ss_bool_t equals(T const &rhs) const
    {
        return rhs == 0;
    }

// Not to be implemented
private:
    void operator &() const;

#if !defined(__STLSOFT_COMPILER_IS_GCC) || \
    __GNUC__ < 3 || \
    (   __GNUC__ == 3 && \
        __GNUC_MINOR__ < 4)
    NULL_v(NULL_v const &);
    NULL_v const &operator =(NULL_v const &);
#endif /* compiler */
};

#if 1
/// operator == for NULL_v and an arbitrary type
template <ss_typename_param_k T>
inline ss_bool_t operator ==(NULL_v const &lhs, T const &rhs)
{
    return lhs.equals(rhs);
}

/// operator == for an arbitrary type and NULL_v
template <ss_typename_param_k T>
inline ss_bool_t operator ==(T const &lhs, NULL_v const &rhs)
{
    return rhs.equals(lhs);
}

/// operator != for NULL_v and an arbitrary type
template <ss_typename_param_k T>
inline ss_bool_t operator !=(NULL_v const &lhs, T const &rhs)
{
    return !lhs.equals(rhs);
}

/// operator != for an arbitrary type and NULL_v
template <ss_typename_param_k T>
inline ss_bool_t operator !=(T const &lhs, NULL_v const &rhs)
{
    return !rhs.equals(lhs);
}
#endif /* 0 */

#endif /* _STLSOFT_NULL_v_DEFINED */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_null(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "null", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_null(test_stlsoft_null);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_NULL */

/* ////////////////////////////////////////////////////////////////////////// */
