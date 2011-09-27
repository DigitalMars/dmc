/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_integer_to_string.h (originally MEL3Str.c, ::SynesisDev)
 *
 * Purpose:     Number to string conversion functions.
 *
 * Created:     7th April 2002
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


/// \file stlsoft_integer_to_string.h
///
/// Number to string conversion functions.

#ifndef STLSOFT_INCL_H_STLSOFT_INTEGER_TO_STRING
#define STLSOFT_INCL_H_STLSOFT_INTEGER_TO_STRING

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_INTEGER_TO_STRING_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_INTEGER_TO_STRING_MINOR       0
# define STLSOFT_VER_H_STLSOFT_INTEGER_TO_STRING_REVISION    1
# define STLSOFT_VER_H_STLSOFT_INTEGER_TO_STRING_EDIT        45
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifdef STLSOFT_UNITTEST
# ifndef STLSOFT_INCL_H_LIMIT_TRAITS
#  include "stlsoft_limit_traits.h"
# endif /* !STLSOFT_INCL_H_LIMIT_TRAITS */
# include <string.h>
#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Helper Functions
 */

/// Provides an indexable sequence of digit characters
template <ss_typename_param_k C>
#ifdef STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS
inline C const *get_digit_character(C * )
#else /* ? STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS */
inline C const *get_digit_character()
#endif /* STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS */
{
    static C const  s_characters[19] =
    {
            '9'
        ,   '8'
        ,   '7'
        ,   '6'
        ,   '5'
        ,   '4'
        ,   '3'
        ,   '2'
        ,   '1'
        ,   '0'
        ,   '1'
        ,   '2'
        ,   '3'
        ,   '4'
        ,   '5'
        ,   '6'
        ,   '7'
        ,   '8'
        ,   '9'
    };
#if 0
    static C const  *s_mid  =   s_characters + 9;

    return s_mid;
#else /* ? 0 */
    return s_characters + 9;
#endif /* 0 */
}

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

/// Highly efficient conversion of unsigned integers to string
///
/// \note The function does not check that cchBuf is sufficient for conversion
/// of the particular integer type, except for a debug version
/// <code>assert()</code>. It is the programmer's responsibility to ensure that
/// the supplied buffer is large enough for the conversion. This can be done
/// with the use of the printf_traits templates.
///
/// \param buf A buffer of sufficient space within which to convert the integer
/// \param cchBuf The number of characters available in \c buf
/// \param i The integer to convert
template<   ss_typename_param_k C
        ,   ss_typename_param_k I
        >
inline C const *unsigned_integer_to_string(C *buf, ss_size_t cchBuf, I i)
{
    C   *psz    =   buf + cchBuf - 1;  // Set pointer to last character.

    *psz = 0;   // Set the terminating null character.

    do
    {
#if defined(__STLSOFT_COMPILER_IS_MSVC)
        typedef I           rem_t;
#else
        typedef ss_uint_t   rem_t;
#endif /* __STLSOFT_COMPILER_IS_MSVC */

        rem_t   lsd = static_cast<rem_t>(i % 10);   // Determine the least significant digit.

        i = static_cast<I>(i / 10);                 // Deal with next most significant.

        --psz;                                      // Move back.

#ifdef STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS
        *psz = get_digit_character(static_cast<C*>(NULL))[lsd];
#else /* ? STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS */
        *psz = get_digit_character<C>()[lsd];
#endif /* STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS */

    } while(i != 0);

    stlsoft_assert(!(psz < buf));

    return psz;
}


/// Highly efficient conversion of unsigned integers to string
///
/// \note The function does not check that cchBuf is sufficient for conversion
/// of the particular integer type, except for a debug version
/// <code>assert()</code>. It is the programmer's responsibility to ensure that
/// the supplied buffer is large enough for the conversion. This can be done
/// with the use of the printf_traits templates.
///
/// \param buf A buffer of sufficient space within which to convert the integer
/// \param cchBuf The number of characters available in \c buf
/// \param i The integer to convert
/// \param cchRes Receives the number of characters written
template<   ss_typename_param_k C
        ,   ss_typename_param_k I
        >
inline C const *unsigned_integer_to_string(C *buf, ss_size_t cchBuf, I i, ss_size_t &cchRes)
{
    C const *psz    =   unsigned_integer_to_string<C, I>(buf, cchBuf, i);

    cchRes = cchBuf - (psz - (buf - 1));

    return psz;
}


/// Highly efficient conversion of signed integers to string
///
/// \note The function does not check that cchBuf is sufficient for conversion
/// of the particular integer type, except for a debug version
/// <code>assert()</code>. It is the programmer's responsibility to ensure that
/// the supplied buffer is large enough for the conversion. This can be done
/// with the use of the printf_traits templates.
///
/// \param buf A buffer of sufficient space within which to convert the integer
/// \param cchBuf The number of characters available in \c buf
/// \param i The integer to convert
template<   ss_typename_param_k C
        ,   ss_typename_param_k I
        >
inline C const *signed_integer_to_string(C *buf, ss_size_t cchBuf, I i)
{
#ifndef __STLSOFT_CF_NEGATIVE_MODULUS_POSITIVE_GIVES_NEGATIVE_RESULT
// If the compiler does not evaluate -9 % 10 to equal -9, then we need to work
// with it as if an unsigned, and prepend the -ve
    C const *psz = unsigned_integer_to_string(buf, cchBuf, i);

    if(i < 0)
    {
        *const_cast<C*>(--psz) = C('-');
    }

    return psz;
#else
// Compiler evaluates -9 % 10 to equal -9, so use the full -ve algorithm. This
// is chosen because it is more efficient on most compilers than calling the
// unsigned peer and converting.

#if defined(__STLSOFT_COMPILER_IS_MSVC)
    typedef I           rem_t;
#else
    typedef ss_sint_t   rem_t;
#endif /* __STLSOFT_COMPILER_IS_MSVC */

    C   *psz    =   buf + cchBuf - 1;  // Set pointer to last character.

    *psz = 0;   // Set the terminating null character.

    if(i < 0)
    {
        do
        {
            rem_t   lsd = static_cast<rem_t>(i % 10);   // Determine the least significant digit.

            i = static_cast<I>(i / 10);                 // Deal with next most significant.

            --psz;                                      // Move back.

#ifdef STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS
            *psz = get_digit_character(static_cast<C*>(NULL))[lsd];
#else /* ? STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS */
            *psz = get_digit_character<C>()[lsd];
#endif /* STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS */

        } while(i != 0);

        *(--psz) = C('-');              // Prepend the minus sign.
    }
    else
    {
        do
        {
            rem_t   lsd = static_cast<rem_t>(i % 10);   // Determine the least significant digit.

            i = static_cast<I>(i / 10);                 // Deal with next most significant.

            --psz;                                      // Move back.

#ifdef STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS
            *psz = get_digit_character(static_cast<C*>(NULL))[lsd];
#else /* ? STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS */
            *psz = get_digit_character<C>()[lsd];
#endif /* STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS */

        } while(i != 0);
    }

    stlsoft_assert(!(psz < buf));

    return psz;
#endif /* !__STLSOFT_CF_NEGATIVE_MODULUS_POSITIVE_GIVES_NEGATIVE_RESULT */
}

/// Highly efficient conversion of signed integers to string
///
/// \note The function does not check that cchBuf is sufficient for conversion
/// of the particular integer type, except for a debug version
/// <code>assert()</code>. It is the programmer's responsibility to ensure that
/// the supplied buffer is large enough for the conversion. This can be done
/// with the use of the printf_traits templates.
///
/// \param buf A buffer of sufficient space within which to convert the integer
/// \param cchBuf The number of characters available in \c buf
/// \param i The integer to convert
/// \param cchRes Receives the number of characters written
template<   ss_typename_param_k C
        ,   ss_typename_param_k I
        >
inline C const *signed_integer_to_string(C *buf, ss_size_t cchBuf, I i, ss_size_t &cchRes)
{
    C const *psz    =   signed_integer_to_string<C, I>(buf, cchBuf, i);

    cchRes = cchBuf - (psz - (buf - 1));

    return psz;
}

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_sint8_t i)
{
    return signed_integer_to_string(buf, cchBuf, i);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_uint8_t i)
{
    return unsigned_integer_to_string(buf, cchBuf, i);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_sint16_t i)
{
    return signed_integer_to_string(buf, cchBuf, i);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_uint16_t i)
{
    return unsigned_integer_to_string(buf, cchBuf, i);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_sint32_t i)
{
    return signed_integer_to_string(buf, cchBuf, i);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_uint32_t i)
{
    return unsigned_integer_to_string(buf, cchBuf, i);
}

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_sint64_t const &i)
{
#ifdef __STLSOFT_INTEGER_TO_STRING_OPTIMISE_64BIT
    if(i < 0x80000000)
    {
        return signed_integer_to_string(buf, cchBuf, static_cast<ss_sint32_t>(i));
    }
#endif // __STLSOFT_INTEGER_TO_STRING_OPTIMISE_64BIT

    return signed_integer_to_string(buf, cchBuf, i);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_uint64_t const &i)
{
#ifdef __STLSOFT_INTEGER_TO_STRING_OPTIMISE_64BIT
    if(i < 0x80000000)
    {
        return unsigned_integer_to_string(buf, cchBuf, static_cast<ss_uint32_t>(i));
    }
#endif // __STLSOFT_INTEGER_TO_STRING_OPTIMISE_64BIT

    return unsigned_integer_to_string(buf, cchBuf, i);
}
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */

#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, int i)
{
    return signed_integer_to_string(buf, cchBuf, i);
}
#endif /* !__STLSOFT_CF_INT_DISTINCT_TYPE */


#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT

template< ss_typename_param_k C
        , ss_size_t           N
        >
inline C const *integer_to_string(C (&buf)[N], ss_sint8_t i)
{
    stlsoft_static_assert(!(N < 5));

    return signed_integer_to_string(buf, N, i);
}

template< ss_typename_param_k C
        , ss_size_t           N
        >
inline C const *integer_to_string(C (&buf)[N], ss_uint8_t i)
{
    stlsoft_static_assert(!(N < 4));

    return unsigned_integer_to_string(buf, N, i);
}

template< ss_typename_param_k C
        , ss_size_t           N
        >
inline C const *integer_to_string(C (&buf)[N], ss_sint16_t i)
{
    stlsoft_static_assert(!(N < 7));

    return signed_integer_to_string(buf, N, i);
}

template< ss_typename_param_k C
        , ss_size_t           N
        >
inline C const *integer_to_string(C (&buf)[N], ss_uint16_t i)
{
    stlsoft_static_assert(!(N < 6));

    return unsigned_integer_to_string(buf, N, i);
}

template< ss_typename_param_k C
        , ss_size_t           N
        >
inline C const *integer_to_string(C (&buf)[N], ss_sint32_t i)
{
    stlsoft_static_assert(!(N < 12));

    return signed_integer_to_string(buf, N, i);
}

template< ss_typename_param_k C
        , ss_size_t           N
        >
inline C const *integer_to_string(C (&buf)[N], ss_uint32_t i)
{
    stlsoft_static_assert(!(N < 11));

    return unsigned_integer_to_string(buf, N, i);
}

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
template< ss_typename_param_k C
        , ss_size_t           N
        >
inline C const *integer_to_string(C (&buf)[N], ss_sint64_t const &i)
{
    stlsoft_static_assert(!(N < 21));

#ifdef __STLSOFT_INTEGER_TO_STRING_OPTIMISE_64BIT
    if(i < 0x80000000)
    {
        return signed_integer_to_string(buf, N, static_cast<ss_sint32_t>(i));
    }
#endif // __STLSOFT_INTEGER_TO_STRING_OPTIMISE_64BIT

    return signed_integer_to_string(buf, N, i);
}

template< ss_typename_param_k C
        , ss_size_t           N
        >
inline C const *integer_to_string(C (&buf)[N], ss_uint64_t const &i)
{
    stlsoft_static_assert(!(N < 21));

#ifdef __STLSOFT_INTEGER_TO_STRING_OPTIMISE_64BIT
    if(i < 0x80000000)
    {
        return unsigned_integer_to_string(buf, N, static_cast<ss_uint32_t>(i));
    }
#endif // __STLSOFT_INTEGER_TO_STRING_OPTIMISE_64BIT

    return unsigned_integer_to_string(buf, N, i);
}
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */

#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
template< ss_typename_param_k C
        , ss_size_t           N
        >
inline C const *integer_to_string(C (&buf)[N], int i)
{
    return signed_integer_to_string(buf, N, i);
}
#endif /* !__STLSOFT_CF_INT_DISTINCT_TYPE */

#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */

#if 0
template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_int_t i)
{
    return signed_integer_to_string(buf, cchBuf, i);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_uint_t i)
{
    return unsigned_integer_to_string(buf, cchBuf, i);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_bool_t i);

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_char_a_t i);

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_char_w_t i);
#endif /* 0 */



template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_sint8_t i, ss_size_t &cchRes)
{
    return signed_integer_to_string(buf, cchBuf, i, cchRes);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_uint8_t i, ss_size_t &cchRes)
{
    return unsigned_integer_to_string(buf, cchBuf, i, cchRes);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_sint16_t i, ss_size_t &cchRes)
{
    return signed_integer_to_string(buf, cchBuf, i, cchRes);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_uint16_t i, ss_size_t &cchRes)
{
    return unsigned_integer_to_string(buf, cchBuf, i, cchRes);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_sint32_t i, ss_size_t &cchRes)
{
    return signed_integer_to_string(buf, cchBuf, i, cchRes);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_uint32_t i, ss_size_t &cchRes)
{
    return unsigned_integer_to_string(buf, cchBuf, i, cchRes);
}

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_sint64_t const &i, ss_size_t &cchRes)
{
    return signed_integer_to_string(buf, cchBuf, i, cchRes);
}

template <ss_typename_param_k C>
inline C const *integer_to_string(C *buf, ss_size_t cchBuf, ss_uint64_t const &i, ss_size_t &cchRes)
{
    return unsigned_integer_to_string(buf, cchBuf, i, cchRes);
}
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    template <ss_typename_param_k T, ss_typename_param_k C>
    struct int2str_candidate
    {
        T       value;
        C const *string;
    };

#define STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(v)    \
                                                                \
    {   v,  #v  },

#define STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY_W(v)  \
                                                                \
    {   v,  #v  },

    ss_bool_t test_stlsoft_integer_to_string(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "integer_to_string", __FILE__);

        char                    buffer[21];
#if 0
        wchar_t                 bufferw[21];
#endif /* 0 */

        // 8-bit signed integer
        { for(ss_sint8_t i = limit_traits<ss_sint8_t>::minimum(); ; ++i)
        {
            char    sprintfForm[21];

            sprintf(sprintfForm, "%d", i);
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), i), sprintfForm))
            {
                r->report("conversion of 8-bit signed integer failed", __LINE__);
                bSuccess = false;
            }

            if(i == limit_traits<ss_sint8_t>::maximum())
            {
                break;
            }
        }}

        int2str_candidate<ss_sint8_t, char> si8s[] =
        {
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-128)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-127)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-12)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(0)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(9)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(10)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(45)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(127)
        };

        { for(ss_size_t i = 0; i < stlsoft_num_elements(si8s); ++i)
        {
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), si8s[i].value), si8s[i].string))
            {
                r->report("conversion of 8-bit signed integer failed", __LINE__);
                bSuccess = false;
            }
        }}

        // 8-bit unsigned integer
        { for(ss_uint8_t i = limit_traits<ss_uint8_t>::minimum(); ; ++i)
        {
            char    sprintfForm[21];

            sprintf(sprintfForm, "%u", i);
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), i), sprintfForm))
            {
                r->report("conversion of 8-bit unsigned integer failed", __LINE__);
                bSuccess = false;
            }

            if(i == limit_traits<ss_uint8_t>::maximum())
            {
                break;
            }
        }}

        int2str_candidate<ss_uint8_t, char> ui8s[] =
        {
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(0)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(1)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(9)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(10)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(45)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(127)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(128)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(200)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(254)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(255)
        };

        { for(ss_size_t i = 0; i < stlsoft_num_elements(ui8s); ++i)
        {
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), ui8s[i].value), ui8s[i].string))
            {
                r->report("conversion of 8-bit unsigned integer failed", __LINE__);
                bSuccess = false;
            }
        }}

        // 16-bit signed integer
        { for(ss_sint16_t i = limit_traits<ss_sint16_t>::minimum(); ; ++i)
        {
            char    sprintfForm[21];

            sprintf(sprintfForm, "%d", i);
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), i), sprintfForm))
            {
                r->report("conversion of 16-bit signed integer failed", __LINE__);
                bSuccess = false;
            }

            if(i == limit_traits<ss_sint16_t>::maximum())
            {
                break;
            }
        }}

        int2str_candidate<ss_sint16_t, char>    si16s[] =
        {
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-32768)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-32767)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-127)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-12)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(0)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(9)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(10)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(45)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(127)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(32766)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(32767)
        };

        { for(ss_size_t i = 0; i < stlsoft_num_elements(si16s); ++i)
        {
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), si16s[i].value), si16s[i].string))
            {
                r->report("conversion of 16-bit signed integer failed", __LINE__);
                bSuccess = false;
            }
        }}

        // 16-bit unsigned integer
        { for(ss_uint16_t i = limit_traits<ss_uint16_t>::minimum(); ; ++i)
        {
            char    sprintfForm[21];

            sprintf(sprintfForm, "%u", i);
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), i), sprintfForm))
            {
                r->report("conversion of 16-bit unsigned integer failed", __LINE__);
                bSuccess = false;
            }

            if(i == limit_traits<ss_uint16_t>::maximum())
            {
                break;
            }
        }}

        int2str_candidate<ss_uint16_t, char>    ui16s[] =
        {
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(0)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(1)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(9)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(10)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(45)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(127)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(32767)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(32768)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(32769)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(65534)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(65535)
        };

        { for(ss_size_t i = 0; i < stlsoft_num_elements(ui16s); ++i)
        {
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), ui16s[i].value), ui16s[i].string))
            {
                r->report("conversion of 16-bit unsigned integer failed", __LINE__);
                bSuccess = false;
            }
        }}

        // 32-bit signed integer
#if 0 // Too slow to use!!
        { for(ss_sint32_t i = limit_traits<ss_sint32_t>::minimum(); ; ++i)
        {
            char    sprintfForm[21];

            sprintf(sprintfForm, "%d", i);
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), i), sprintfForm))
            {
                r->report("conversion of 32-bit signed integer failed", __LINE__);
                bSuccess = false;
            }

            if(i == limit_traits<ss_sint32_t>::maximum())
            {
                break;
            }
        }}
#endif /* 0 */

        int2str_candidate<ss_sint32_t, char>    si32s[] =
        {
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-32727)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-14301)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-26434)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-6268)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-13599)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-17044)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-21290)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-3410)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-5806)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-8304)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-27063)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-4623)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(41)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(18467)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(6334)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(26500)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(19169)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(15724)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(11478)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(29358)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(26962)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(24464)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(5705)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(28145)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(23281)
        };

        { for(ss_size_t i = 0; i < stlsoft_num_elements(si32s); ++i)
        {
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), si32s[i].value), si32s[i].string))
            {
                r->report("conversion of 32-bit signed integer failed", __LINE__);
                bSuccess = false;
            }
        }}

        // 32-bit unsigned integer
#if 0 // Too slow to use!!
        { for(ss_uint32_t i = limit_traits<ss_uint32_t>::minimum(); ; ++i)
        {
            char    sprintfForm[21];

            sprintf(sprintfForm, "%d", i);
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), i), sprintfForm))
            {
                r->report("conversion of 32-bit unsigned integer failed", __LINE__);
                bSuccess = false;
            }

            if(i == limit_traits<ss_uint32_t>::maximum())
            {
                break;
            }
        }}
#endif /* 0 */

        int2str_candidate<ss_uint32_t, char>    ui32s[] =
        {
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(41)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(18467)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(6334)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(26500)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(19169)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(15724)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(11478)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(29358)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(26962)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(24464)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(5705)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(28145)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(23281)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(32809)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(51235)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(39102)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(59268)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(51937)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(48492)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(44246)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(62126)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(59730)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(57232)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(38473)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(60913)
        };

        { for(ss_size_t i = 0; i < stlsoft_num_elements(ui32s); ++i)
        {
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), ui32s[i].value), ui32s[i].string))
            {
                r->report("conversion of 32-bit unsigned integer failed", __LINE__);
                bSuccess = false;
            }
        }}

        // int
        int2str_candidate<int, char>    is[] =
        {
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-32727)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-14301)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-26434)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-6268)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-13599)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-17044)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-21290)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-3410)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-5806)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-8304)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-27063)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(-4623)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(41)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(18467)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(6334)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(26500)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(19169)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(15724)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(11478)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(29358)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(26962)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(24464)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(5705)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(28145)
            STLSOFT_INTEGER_TO_STRING_INT2STR_CANDIDATE_ENTRY(23281)
        };

        { for(ss_size_t i = 0; i < stlsoft_num_elements(is); ++i)
        {
            if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), is[i].value), is[i].string))
            {
                r->report("conversion of int failed", __LINE__);
                bSuccess = false;
            }
        }}

#if 0
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
        // 64-bit signed integer
        if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), si64), "-12012012012012"))
        {
            r->report("conversion of 64-bit signed integer failed", __LINE__);
            bSuccess = false;
        }

        // 64-bit unsigned integer
        if(0 != strcmp(integer_to_string(buffer, stlsoft_num_elements(buffer), ui64), "45045045045045"))
        {
            r->report("conversion of 64-bit unsigned integer failed", __LINE__);
            bSuccess = false;
        }
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_integer_to_string(test_stlsoft_integer_to_string);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_INTEGER_TO_STRING */

/* ////////////////////////////////////////////////////////////////////////// */
