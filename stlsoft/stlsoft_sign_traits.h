/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_sign_traits.h
 *
 * Purpose:     sign_traits classes.
 *
 * Created:     16th January 2002
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


/// \file stlsoft_sign_traits.h
///
/// sign_traits classes.

#ifndef STLSOFT_INCL_H_STLSOFT_SIGN_TRAITS
#define STLSOFT_INCL_H_STLSOFT_SIGN_TRAITS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_SIGN_TRAITS_MAJOR        2
# define STLSOFT_VER_H_STLSOFT_SIGN_TRAITS_MINOR        0
# define STLSOFT_VER_H_STLSOFT_SIGN_TRAITS_REVISION     1
# define STLSOFT_VER_H_STLSOFT_SIGN_TRAITS_EDIT         25
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_SIZE_TRAITS
# include "stlsoft_size_traits.h"   // int_size_traits
#endif /* !STLSOFT_INCL_H_STLSOFT_SIZE_TRAITS */

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

// struct sign_traits

#ifdef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/// Traits for determining the signed, unsigned and alternate-signed type
///
/// sign_traits is a traits class for acquiring the corresponding signed,
/// unsigned, and alternate-signed type eg.
///
/// &nbsp;&nbsp;<code>sign_traits<ss_sint16_t>::signed_type == ss_sint16_t</code><br>
/// &nbsp;&nbsp;<code>sign_traits<ss_sint16_t>::unsigned_type == ss_uint16_t</code><br>
/// &nbsp;&nbsp;<code>sign_traits<ss_sint16_t>::alt_sign_type == ss_uint16_t</code><br>
///
/// \param T The char type
template <ss_typename_param_k T>
struct sign_traits
{
    enum
    {
            bytes   =   1
        ,   bits    =   8                        //!< The type size, in bits
    };

    typedef signed T    signed_type;    //!< The signed type
    typedef unsigned T  unsigned_type;  //!< The unsigned type
    typedef unsigned T  alt_sign_type;  //!< The alternate-signed type
};

#else

template <ss_typename_param_k T>
struct sign_traits;

/* char */
STLSOFT_TEMPLATE_SPECIALISATION
struct sign_traits<ss_char_a_t>
{
    enum { bytes = 1, bits = 8 * bytes };

    typedef ss_uint8_t  signed_type;
    typedef ss_uint8_t  unsigned_type;
//    typedef ss_uint8_t  alt_sign_type;
};

#ifdef __STLSOFT_CF_NATIVE_WCHAR_T_SUPPORT
STLSOFT_TEMPLATE_SPECIALISATION
struct sign_traits<ss_char_w_t>
{
    enum { bytes = sizeof(ss_char_w_t), bits = 8 * bytes };

    typedef int_size_traits<bytes>::signed_type     signed_type;
    typedef int_size_traits<bytes>::unsigned_type   unsigned_type;
};
#endif /* __STLSOFT_CF_NATIVE_WCHAR_T_SUPPORT */

/* s/uint8 */
STLSOFT_TEMPLATE_SPECIALISATION
struct sign_traits<ss_sint8_t>
{
    enum { bytes = 1, bits = 8 * bytes };

    typedef ss_sint8_t  signed_type;
    typedef ss_uint8_t  unsigned_type;
    typedef ss_uint8_t  alt_sign_type;
};

STLSOFT_TEMPLATE_SPECIALISATION
struct sign_traits<ss_uint8_t>
{
    enum { bytes = 1, bits = 8 * bytes };

    typedef ss_sint8_t  signed_type;
    typedef ss_uint8_t  unsigned_type;
    typedef ss_sint8_t  alt_sign_type;
};

/* s/uint16 */
STLSOFT_TEMPLATE_SPECIALISATION
struct sign_traits<ss_sint16_t>
{
    enum { bytes = 2, bits = 8 * bytes };

    typedef ss_sint16_t signed_type;
    typedef ss_uint16_t unsigned_type;
    typedef ss_uint16_t alt_sign_type;
};

STLSOFT_TEMPLATE_SPECIALISATION
struct sign_traits<ss_uint16_t>
{
    enum { bytes = 2, bits = 8 * bytes };

    typedef ss_sint16_t signed_type;
    typedef ss_uint16_t unsigned_type;
    typedef ss_sint16_t alt_sign_type;
};

/* s/uint32 */
STLSOFT_TEMPLATE_SPECIALISATION
struct sign_traits<ss_sint32_t>
{
    enum { bytes = 4, bits = 8 * bytes };

    typedef ss_sint32_t signed_type;
    typedef ss_uint32_t unsigned_type;
    typedef ss_uint32_t alt_sign_type;
};

STLSOFT_TEMPLATE_SPECIALISATION
struct sign_traits<ss_uint32_t>
{
    enum { bytes = 4, bits = 8 * bytes };

    typedef ss_sint32_t signed_type;
    typedef ss_uint32_t unsigned_type;
    typedef ss_sint32_t alt_sign_type;
};

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
/* s/uint64 */
STLSOFT_TEMPLATE_SPECIALISATION
struct sign_traits<ss_sint64_t>
{
    enum { bytes = 8, bits = 8 * bytes };

    typedef ss_sint64_t signed_type;
    typedef ss_uint64_t unsigned_type;
    typedef ss_uint64_t alt_sign_type;
};

STLSOFT_TEMPLATE_SPECIALISATION
struct sign_traits<ss_uint64_t>
{
    enum { bytes = 8, bits = 8 * bytes };

    typedef ss_sint64_t signed_type;
    typedef ss_uint64_t unsigned_type;
    typedef ss_sint64_t alt_sign_type;
};
#endif /* !STLSOFT_CF_64BIT_INT_SUPPORT */

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_SIGN_TRAITS */

/* ////////////////////////////////////////////////////////////////////////// */
