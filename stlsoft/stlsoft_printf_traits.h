/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_printf_traits.h
 *
 * Purpose:     printf_traits classes.
 *
 * Created:     16th January 2002
 * Updated:     12th September 2004
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


/// \file stlsoft_printf_traits.h
///
/// printf_traits classes.

#ifndef STLSOFT_INCL_H_STLSOFT_PRINTF_TRAITS
#define STLSOFT_INCL_H_STLSOFT_PRINTF_TRAITS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_PRINTF_TRAITS_MAJOR        2
# define STLSOFT_VER_H_STLSOFT_PRINTF_TRAITS_MINOR        0
# define STLSOFT_VER_H_STLSOFT_PRINTF_TRAITS_REVISION     1
# define STLSOFT_VER_H_STLSOFT_PRINTF_TRAITS_EDIT         25
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

#ifdef STLSOFT_UNITTEST
# ifndef STLSOFT_INCL_H_STLSOFT_LIMIT_TRAITS
#  include "stlsoft_limit_traits.h" // limit_traits
# endif /* !STLSOFT_INCL_H_STLSOFT_LIMIT_TRAITS */
# include <stdarg.h>
# include <stdio.h>
# include <string.h>
# include <wchar.h>
#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Constants & definitions
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

#define __STLSOFT_PRINTF_TRAITS__SINT8_MIN      -128
#define __STLSOFT_PRINTF_TRAITS__SINT8_MAX      127

#define __STLSOFT_PRINTF_TRAITS__UINT8_MIN      0
#define __STLSOFT_PRINTF_TRAITS__UINT8_MAX      255

#define __STLSOFT_PRINTF_TRAITS__SINT16_MIN     -32768
#define __STLSOFT_PRINTF_TRAITS__SINT16_MAX     32767

#define __STLSOFT_PRINTF_TRAITS__UINT16_MIN     0
#define __STLSOFT_PRINTF_TRAITS__UINT16_MAX     65535

#define __STLSOFT_PRINTF_TRAITS__SINT32_MIN     -2147483648
#define __STLSOFT_PRINTF_TRAITS__SINT32_MAX     2147483647

#define __STLSOFT_PRINTF_TRAITS__UINT32_MIN     0
#define __STLSOFT_PRINTF_TRAITS__UINT32_MAX     4294967295

#define __STLSOFT_PRINTF_TRAITS__SINT64_MIN     -9223372036854775808
#define __STLSOFT_PRINTF_TRAITS__SINT64_MAX     9223372036854775807

#define __STLSOFT_PRINTF_TRAITS__UINT64_MIN     0
#define __STLSOFT_PRINTF_TRAITS__UINT64_MAX     18446744073709551615

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Macros
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

# define ___stringise_(x)           # x
# define ___stringise(x)            ___stringise_(x)

# if defined(__STLSOFT_COMPILER_IS_BORLAND)
#  define ___stringise_w_(x)        L"" ## #x
# else /* ? compiler */
#  define ___stringise_w_(x)        L ## #x
# endif /* compiler */
# define ___stringise_w(x)          ___stringise_w_(x)

#ifdef STLSOFT_CF_64_BIT_PRINTF_USES_I64
# undef STLSOFT_CF_64_BIT_PRINTF_USES_I64
#endif /* STLSOFT_CF_64_BIT_PRINTF_USES_I64 */

#ifdef STLSOFT_CF_64_BIT_PRINTF_USES_LL
# undef STLSOFT_CF_64_BIT_PRINTF_USES_LL
#endif /* STLSOFT_CF_64_BIT_PRINTF_USES_LL */

# if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
     (  defined(__STLSOFT_COMPILER_IS_COMO) && \
        defined(_MSC_VER)) || \
     (  defined(WIN32) && \
        (   defined(__STLSOFT_COMPILER_IS_GCC)) || \
            defined(__STLSOFT_COMPILER_IS_INTEL)) || \
     defined(__STLSOFT_COMPILER_IS_MSVC) || \
     defined(__STLSOFT_COMPILER_IS_VECTORC) || \
     defined(__STLSOFT_COMPILER_IS_WATCOM)
#  define STLSOFT_CF_64_BIT_PRINTF_USES_I64
# elif defined(__STLSOFT_COMPILER_IS_COMO) || \
       defined(__STLSOFT_COMPILER_IS_DMC) || \
       defined(__STLSOFT_COMPILER_IS_INTEL) || \
       defined(__STLSOFT_COMPILER_IS_MSVC) || \
       defined(__STLSOFT_COMPILER_IS_GCC) || \
       defined(__STLSOFT_COMPILER_IS_MWERKS)
#  define STLSOFT_CF_64_BIT_PRINTF_USES_LL
# else
#  error Further compiler discrimination is required
# endif /* printf-64 */

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// struct printf_traits

#ifdef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/// Traits for determining the size, in printf-ed form, of the minimum and
/// maximum values of types
///
/// printf_traits is a traits class for acquiring enum values representing the
/// lengths, when expressed in string form, of the minimum and maximum values of
/// the type, and the maximum of the two. The lengths are inclusive of the
/// null terminator.
///
/// &nbsp;&nbsp;<code>printf_traits<ss_sint16_t>::size_min == 6</code><br>
/// &nbsp;&nbsp;<code>printf_traits<ss_sint16_t>::size_max == 7</code><br>
/// &nbsp;&nbsp;<code>printf_traits<ss_sint16_t>::size == 7</code><br>
///
/// \param T The type
template <ss_typename_param_k T>
struct printf_traits
{
    enum
    {
            size_min    //!< The number of characters (& null) in the minimum value
        ,   size_max    //!< The number of characters (& null) in the maximum value
        ,   size        //!< The maximum of \c size_min and \c size_max
    };

    /// Returns the appropriate printf format for the type
    static ss_char_a_t const *format_a();
    /// Returns the appropriate wprintf format for the type
    static ss_char_w_t const *format_w();
};

#else

template <ss_typename_param_k T>
struct printf_traits;

/* s/uint8 */
STLSOFT_TEMPLATE_SPECIALISATION
struct printf_traits<ss_sint8_t>
{
    enum
    {
            size_min = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__SINT8_MIN))
        ,   size_max = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__SINT8_MAX))
        ,   size = (size_min < size_max) ? size_max : size_min
    };

    static ss_char_a_t const *format_a()
    {
        return "%d";
    }
    static ss_char_w_t const *format_w()
    {
        return L"%d";
    }
};

STLSOFT_TEMPLATE_SPECIALISATION
struct printf_traits<ss_uint8_t>
{
    enum
    {
            size_min = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__UINT8_MIN))
        ,   size_max = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__UINT8_MAX))
        ,   size = (size_min < size_max) ? size_max : size_min
    };

    static ss_char_a_t const *format_a()
    {
        return "%u";
    }
    static ss_char_w_t const *format_w()
    {
        return L"%u";
    }
};

/* s/uint16 */
STLSOFT_TEMPLATE_SPECIALISATION
struct printf_traits<ss_sint16_t>
{
    enum
    {
            size_min = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__SINT16_MIN))
        ,   size_max = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__SINT16_MAX))
        ,   size = (size_min < size_max) ? size_max : size_min
    };

    static ss_char_a_t const *format_a()
    {
        return "%d";
    }
    static ss_char_w_t const *format_w()
    {
        return L"%d";
    }
};

STLSOFT_TEMPLATE_SPECIALISATION
struct printf_traits<ss_uint16_t>
{
    enum
    {
            size_min = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__UINT16_MIN))
        ,   size_max = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__UINT16_MAX))
        ,   size = (size_min < size_max) ? size_max : size_min
    };

    static ss_char_a_t const *format_a()
    {
        return "%u";
    }
    static ss_char_w_t const *format_w()
    {
        return L"%u";
    }
};

/* s/uint32 */
STLSOFT_TEMPLATE_SPECIALISATION
struct printf_traits<ss_sint32_t>
{
    enum
    {
            size_min = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__SINT32_MIN))
        ,   size_max = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__SINT32_MAX))
        ,   size = (size_min < size_max) ? size_max : size_min
    };

    static ss_char_a_t const *format_a()
    {
        return "%ld";
    }
    static ss_char_w_t const *format_w()
    {
        return L"%ld";
    }
};

STLSOFT_TEMPLATE_SPECIALISATION
struct printf_traits<ss_uint32_t>
{
    enum
    {
            size_min = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__UINT32_MIN))
        ,   size_max = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__UINT32_MAX))
        ,   size = (size_min < size_max) ? size_max : size_min
    };

    static ss_char_a_t const *format_a()
    {
        return "%lu";
    }
    static ss_char_w_t const *format_w()
    {
        return L"%lu";
    }
};

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT

/* s/uint64 */
STLSOFT_TEMPLATE_SPECIALISATION
struct printf_traits<ss_sint64_t>
{
    enum
    {
            size_min = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__SINT64_MIN))
        ,   size_max = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__SINT64_MAX))
        ,   size = (size_min < size_max) ? size_max : size_min
    };

    static ss_char_a_t const *format_a()
    {
#if defined(STLSOFT_CF_64_BIT_PRINTF_USES_I64)
        return "%I64d";
#elif defined(STLSOFT_CF_64_BIT_PRINTF_USES_LL)
        return "%lld";
#else
# error Further compiler discrimination is required
#endif /* printf-64 */
    }
    static ss_char_w_t const *format_w()
    {
#if defined(STLSOFT_CF_64_BIT_PRINTF_USES_I64)
        return L"%I64d";
#elif defined(STLSOFT_CF_64_BIT_PRINTF_USES_LL)
        return L"%lld";
#else
# error Further compiler discrimination is required
#endif /* printf-64 */
    }
};

STLSOFT_TEMPLATE_SPECIALISATION
struct printf_traits<ss_uint64_t>
{
    enum
    {
            size_min = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__UINT64_MIN))
        ,   size_max = sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__UINT64_MAX))
        ,   size = (size_min < size_max) ? size_max : size_min
    };

    static ss_char_a_t const *format_a()
    {
#if defined(STLSOFT_CF_64_BIT_PRINTF_USES_I64)
        return "%I64u";
#elif defined(STLSOFT_CF_64_BIT_PRINTF_USES_LL)
        return "%llu";
#else
# error Further compiler discrimination is required
#endif /* printf-64 */
    }
    static ss_char_w_t const *format_w()
    {
#if defined(STLSOFT_CF_64_BIT_PRINTF_USES_I64)
        return L"%I64u";
#elif defined(STLSOFT_CF_64_BIT_PRINTF_USES_LL)
        return L"%llu";
#else
# error Further compiler discrimination is required
#endif /* printf-64 */
    }
};

#endif /* !STLSOFT_CF_64BIT_INT_SUPPORT */

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    namespace
    {
# if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
     (  defined(__STLSOFT_COMPILER_IS_COMO) && \
        defined(_MSC_VER)) || \
     defined(__STLSOFT_COMPILER_IS_GCC) || \
     defined(__STLSOFT_COMPILER_IS_INTEL) || \
     defined(__STLSOFT_COMPILER_IS_MSVC)
        int swprintf(wchar_t *sz, size_t cch, wchar_t const *fmt, ...)
        {
            va_list args;
            int     iRet;
            va_start(args, fmt);

#  if defined(__STLSOFT_COMPILER_IS_MSVC) && \
      _MSC_VER >= 1400
            iRet = _vswprintf_c(sz, cch, fmt, args);
            sz[iRet] = '\0';
#else /* ? compiler */
            STLSOFT_SUPPRESS_UNUSED(cch);
            iRet = vswprintf(sz, fmt, args);
#endif /* compiler */

            va_end(args);

            return iRet;
        }
# endif /* compiler */
    } // anonymous namespace

    ss_bool_t test_stlsoft_printf_traits(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "printf_traits", __FILE__);

        char    sz[21];
        wchar_t szw[21];
        int     cch;

        // (i) min (with char)

        cch = sprintf(sz, printf_traits<ss_sint8_t>::format_a(), limit_traits<ss_sint8_t>::minimum());
        if(printf_traits<ss_sint8_t>::size_min != 1 + cch)
        {
            r->report("traits (8-bit, signed) min invalid", __LINE__);
            bSuccess = false;
        }
        if(0 != strcmp(sz, ___stringise(__STLSOFT_PRINTF_TRAITS__SINT8_MIN)))
        {
            r->report("traits (8-bit, signed) format invalid", __LINE__);
            bSuccess = false;
        }

        cch = sprintf(sz, printf_traits<ss_uint8_t>::format_a(), limit_traits<ss_uint8_t>::minimum());
        if(printf_traits<ss_uint8_t>::size_min != 1 + cch)
        {
            r->report("traits (8-bit, unsigned) min invalid", __LINE__);
            bSuccess = false;
        }
        if(0 != strcmp(sz, ___stringise(__STLSOFT_PRINTF_TRAITS__UINT8_MIN)))
        {
            r->report("traits (8-bit, unsigned) format invalid", __LINE__);
            bSuccess = false;
        }

        cch = sprintf(sz, printf_traits<ss_sint16_t>::format_a(), limit_traits<ss_sint16_t>::minimum());
        if(printf_traits<ss_sint16_t>::size_min != 1 + cch)
        {
            r->report("traits (16-bit, signed) min invalid", __LINE__);
            bSuccess = false;
        }
        if(0 != strcmp(sz, ___stringise(__STLSOFT_PRINTF_TRAITS__SINT16_MIN)))
        {
            r->report("traits (16-bit, signed) format invalid", __LINE__);
            bSuccess = false;
        }

        cch = sprintf(sz, printf_traits<ss_uint16_t>::format_a(), limit_traits<ss_uint16_t>::minimum());
        if(printf_traits<ss_uint16_t>::size_min != 1 + cch)
        {
            r->report("traits (16-bit, unsigned) min invalid", __LINE__);
            bSuccess = false;
        }
        if(0 != strcmp(sz, ___stringise(__STLSOFT_PRINTF_TRAITS__UINT16_MIN)))
        {
            r->report("traits (16-bit, unsigned) format invalid", __LINE__);
            bSuccess = false;
        }

        cch = sprintf(sz, printf_traits<ss_sint32_t>::format_a(), limit_traits<ss_sint32_t>::minimum());
        if(printf_traits<ss_sint32_t>::size_min != 1 + cch)
        {
            r->report("traits (32-bit, signed) min invalid", __LINE__);
            bSuccess = false;
        }
        if(0 != strcmp(sz, ___stringise(__STLSOFT_PRINTF_TRAITS__SINT32_MIN)))
        {
            r->report("traits (32-bit, signed) format invalid", __LINE__);
            bSuccess = false;
        }

        cch = sprintf(sz, printf_traits<ss_uint32_t>::format_a(), limit_traits<ss_uint32_t>::minimum());
        if(printf_traits<ss_uint32_t>::size_min != 1 + cch)
        {
            r->report("traits (32-bit, unsigned) min invalid", __LINE__);
            bSuccess = false;
        }
        if(0 != strcmp(sz, ___stringise(__STLSOFT_PRINTF_TRAITS__UINT32_MIN)))
        {
            r->report("traits (32-bit, unsigned) format invalid", __LINE__);
            bSuccess = false;
        }

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
        cch = sprintf(sz, printf_traits<ss_sint64_t>::format_a(), limit_traits<ss_sint64_t>::minimum());
        if(printf_traits<ss_sint64_t>::size_min != 1 + cch)
        {
            r->report("traits (64-bit, signed) min invalid", __LINE__);
            bSuccess = false;
        }
        if(0 != strcmp(sz, ___stringise(__STLSOFT_PRINTF_TRAITS__SINT64_MIN)))
        {
            r->report("traits (64-bit, signed) format invalid", __LINE__);
            bSuccess = false;
        }

        cch = sprintf(sz, printf_traits<ss_uint64_t>::format_a(), limit_traits<ss_uint64_t>::minimum());
        if(printf_traits<ss_uint64_t>::size_min != 1 + cch)
        {
            r->report("traits (64-bit, unsigned) min invalid", __LINE__);
            bSuccess = false;
        }
        if(0 != strcmp(sz, ___stringise(__STLSOFT_PRINTF_TRAITS__UINT64_MIN)))
        {
            r->report("traits (64-bit, unsigned) format invalid", __LINE__);
            bSuccess = false;
        }
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */

        // (ii) max (with char)

        // 8-bit signed
        cch = swprintf(szw, stlsoft_num_elements(szw), printf_traits<ss_sint8_t>::format_w(), limit_traits<ss_sint8_t>::maximum());
        if(printf_traits<ss_sint8_t>::size_max != 1 + cch)
        {
            r->report("traits (8-bit, signed) max invalid", __LINE__);
            bSuccess = false;
        }
        wchar_t sint8_t_max[] = ___stringise_w(__STLSOFT_PRINTF_TRAITS__SINT8_MAX);
        if(0 != wcscmp(szw, sint8_t_max))
        {
            r->report("traits (8-bit, signed) format invalid", __LINE__);
            bSuccess = false;
        }

        // 8-bit unsigned
        cch = swprintf(szw, stlsoft_num_elements(szw), printf_traits<ss_uint8_t>::format_w(), limit_traits<ss_uint8_t>::maximum());
        if(printf_traits<ss_uint8_t>::size_max != 1 + cch)
        {
            r->report("traits (8-bit, unsigned) max invalid", __LINE__);
            bSuccess = false;
        }
        wchar_t uint8_t_max[] = ___stringise_w(__STLSOFT_PRINTF_TRAITS__UINT8_MAX);
        if(0 != wcscmp(szw, uint8_t_max))
        {
            r->report("traits (8-bit, unsigned) format invalid", __LINE__);
            bSuccess = false;
        }

        // 16-bit signed
        cch = swprintf(szw, stlsoft_num_elements(szw), printf_traits<ss_sint16_t>::format_w(), limit_traits<ss_sint16_t>::maximum());
        if(printf_traits<ss_sint16_t>::size_max != 1 + cch)
        {
            r->report("traits (16-bit, signed) max invalid", __LINE__);
            bSuccess = false;
        }
        wchar_t sint16_t_max[] = ___stringise_w(__STLSOFT_PRINTF_TRAITS__SINT16_MAX);
        if(0 != wcscmp(szw, sint16_t_max))
        {
            r->report("traits (16-bit, signed) format invalid", __LINE__);
            bSuccess = false;
        }

        // 16-bit unsigned
        cch = swprintf(szw, stlsoft_num_elements(szw), printf_traits<ss_uint16_t>::format_w(), limit_traits<ss_uint16_t>::maximum());
        if(printf_traits<ss_uint16_t>::size_max != 1 + cch)
        {
            r->report("traits (16-bit, unsigned) max invalid", __LINE__);
            bSuccess = false;
        }
        wchar_t uint16_t_max[] = ___stringise_w(__STLSOFT_PRINTF_TRAITS__UINT16_MAX);
        if(0 != wcscmp(szw, uint16_t_max))
        {
            r->report("traits (16-bit, unsigned) format invalid", __LINE__);
            bSuccess = false;
        }

        // 32-bit signed
        cch = swprintf(szw, stlsoft_num_elements(szw), printf_traits<ss_sint32_t>::format_w(), limit_traits<ss_sint32_t>::maximum());
        if(printf_traits<ss_sint32_t>::size_max != 1 + cch)
        {
            r->report("traits (32-bit, signed) max invalid", __LINE__);
            bSuccess = false;
        }
        wchar_t sint32_t_max[] = ___stringise_w(__STLSOFT_PRINTF_TRAITS__SINT32_MAX);
        if(0 != wcscmp(szw, sint32_t_max))
        {
            r->report("traits (32-bit, signed) format invalid", __LINE__);
            bSuccess = false;
        }

        // 32-bit unsigned
        cch = swprintf(szw, stlsoft_num_elements(szw), printf_traits<ss_uint32_t>::format_w(), limit_traits<ss_uint32_t>::maximum());
        if(printf_traits<ss_uint32_t>::size_max != 1 + cch)
        {
            r->report("traits (32-bit, unsigned) max invalid", __LINE__);
            bSuccess = false;
        }
        wchar_t uint32_t_max[] = ___stringise_w(__STLSOFT_PRINTF_TRAITS__UINT32_MAX);
        if(0 != wcscmp(szw, uint32_t_max))
        {
            r->report("traits (32-bit, unsigned) format invalid", __LINE__);
            bSuccess = false;
        }

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
        // 64-bit signed
        cch = swprintf(szw, stlsoft_num_elements(szw), printf_traits<ss_sint64_t>::format_w(), limit_traits<ss_sint64_t>::maximum());
        if(printf_traits<ss_sint64_t>::size_max != 1 + cch)
        {
            r->report("traits (64-bit, signed) max invalid", __LINE__);
            bSuccess = false;
        }
        wchar_t sint64_t_max[] = ___stringise_w(__STLSOFT_PRINTF_TRAITS__SINT64_MAX);
        if(0 != wcscmp(szw, sint64_t_max))
        {
            r->report("traits (64-bit, signed) format invalid", __LINE__);
            bSuccess = false;
        }

        // 64-bit unsigned
        cch = swprintf(szw, stlsoft_num_elements(szw), printf_traits<ss_uint64_t>::format_w(), limit_traits<ss_uint64_t>::maximum());
        stlsoft_assert('\0' == szw[cch]);
        if(printf_traits<ss_uint64_t>::size_max != 1 + cch)
        {
            r->report("traits (64-bit, unsigned) max invalid", __LINE__);
            bSuccess = false;
        }
        wchar_t uint64_t_max[] = ___stringise_w(__STLSOFT_PRINTF_TRAITS__UINT64_MAX);
        if(0 != wcscmp(szw, uint64_t_max))
        {
            r->report("traits (64-bit, unsigned) format invalid", __LINE__);
            bSuccess = false;
        }
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_printf_traits(test_stlsoft_printf_traits);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_PRINTF_TRAITS */

/* ////////////////////////////////////////////////////////////////////////// */
