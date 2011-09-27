/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_literal_cast.h
 *
 * Purpose:     Compile-time checking for literal conversions.
 *
 * Created:     5th September 2003
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


/// \file stlsoft_literal_cast.h
///
/// Compile-time checking for literal conversions.

#ifndef STLSOFT_INCL_H_STLSOFT_LITERAL_CAST
#define STLSOFT_INCL_H_STLSOFT_LITERAL_CAST

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_LITERAL_CAST_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_LITERAL_CAST_MINOR       0
# define STLSOFT_VER_H_STLSOFT_LITERAL_CAST_REVISION    1
# define STLSOFT_VER_H_STLSOFT_LITERAL_CAST_EDIT        14
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_LIMIT_TRAITS
# include "stlsoft_limit_traits.h"  // limit_traits
#endif /* !STLSOFT_INCL_H_STLSOFT_LIMIT_TRAITS */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

/** \brief Provides compile-time validation for literal casts
 *
 *
 * \note Thanks to Kevlin Henney (http://www.curbralan.com/) for insights into
 * the solution.
 */

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
typedef ss_sint64_t         literal_cast_int_t;
typedef ss_uint64_t         invalid_int_t;
#else /* ? STLSOFT_CF_64BIT_INT_SUPPORT */
typedef ss_sint32_t         literal_cast_int_t;
typedef ss_uint32_t         invalid_int_t;
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */


#if 0
template<   ss_typename_param_k T
        ,   literal_cast_int_t  V
        >
inline T literal_cast()
{
#if defined(STLSOFT_CF_MEMBER_CONSTANT_SUPPORT)
    /* If member constants are supported, we can do compile-time assertions */
    const int __literal_cast_value_too_large__ = V <= limit_traits<T>::maximum_value;
    const int __literal_cast_value_too_small__ = V >= limit_traits<T>::minimum_value;

    stlsoft_static_assert(__literal_cast_value_too_large__);
    stlsoft_static_assert(__literal_cast_value_too_small__);
#else /* ? STLSOFT_CF_MEMBER_CONSTANT_SUPPORT */
    /* If not, then we are restricted to runtime assertions */
    const int __literal_cast_value_too_large__ = V <= limit_traits<T>::maximum();
    const int __literal_cast_value_too_small__ = V >= limit_traits<T>::minimum();

    stlsoft_message_assert("Literal cast value is too large", __literal_cast_value_too_large__);
    stlsoft_message_assert("Literal cast value is too small", __literal_cast_value_too_small__);
#endif /* STLSOFT_CF_MEMBER_CONSTANT_SUPPORT */

    return T(V);
}
#else /* ? 0 */

template<   ss_typename_param_k T
        ,   literal_cast_int_t  V
        >
class literal_cast
{
public:
    operator T () const
    {
#if defined(STLSOFT_CF_MEMBER_CONSTANT_SUPPORT)
        /* If member constants are supported, we can do compile-time assertions */
        const int __literal_cast_value_too_large__ = V <= limit_traits<T>::maximum_value;
        const int __literal_cast_value_too_small__ = V >= limit_traits<T>::minimum_value;

        stlsoft_static_assert(__literal_cast_value_too_large__);
        stlsoft_static_assert(__literal_cast_value_too_small__);
#else /* ? STLSOFT_CF_MEMBER_CONSTANT_SUPPORT */
        /* If not, then we are restricted to runtime assertions */
        const int __literal_cast_value_too_large__ = V <= limit_traits<T>::maximum();
        const int __literal_cast_value_too_small__ = V >= limit_traits<T>::minimum();

        stlsoft_message_assert("Literal cast value is too large", __literal_cast_value_too_large__);
        stlsoft_message_assert("Literal cast value is too small", __literal_cast_value_too_small__);
#endif /* STLSOFT_CF_MEMBER_CONSTANT_SUPPORT */

        return T(V);
    }
};


#ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
template<literal_cast_int_t V>
class literal_cast<invalid_int_t, V>
{
public:
#if 0
    literal_cast()
    {
        const int STLSOFT_LITERAL_CAST_INVALID_TYPE_WARNING =   0;

        stlsoft_static_assert(STLSOFT_LITERAL_CAST_INVALID_TYPE_WARNING);
    }
#endif /* 0 */

private:
    operator invalid_int_t () const
    {
        const int __literal_cast_cannot_cast_to_largest_unsigned_integer_type__ =   0;

        stlsoft_static_assert(__literal_cast_cannot_cast_to_largest_unsigned_integer_type__);

        return 0;
    }

#if defined(__STLSOFT_COMPILER_IS_GCC)
public: static void f() {}
#endif /* !__STLSOFT_COMPILER_IS_GCC */
};
#endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */

#endif /* 0 */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_literal_cast(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "literal_cast", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_literal_cast(test_stlsoft_literal_cast);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_LITERAL_CAST */

/* ////////////////////////////////////////////////////////////////////////// */
