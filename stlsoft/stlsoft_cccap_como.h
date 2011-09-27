/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_cccap_como.h
 *
 * Purpose:     Compiler feature discrimination for Comeau C/C++.
 *
 * Created:     7th February 2003
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


#ifndef STLSOFT_INCL_H_STLSOFT
# error This file must not be included independently of stlsoft.h
#endif /* !STLSOFT_INCL_H_STLSOFT */

/// \file stlsoft_cccap_como.h
///
/// Compiler feature discrimination for Comeau C/C++.

#ifdef STLSOFT_INCL_H_STLSOFT_CCCAP_COMO
# error This file cannot be included more than once in any compilation unit
#endif /* STLSOFT_INCL_H_STLSOFT_CCCAP_COMO */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_CCCAP_COMO_MAJOR     2
# define STLSOFT_VER_H_STLSOFT_CCCAP_COMO_MINOR     0
# define STLSOFT_VER_H_STLSOFT_CCCAP_COMO_REVISION  1
# define STLSOFT_VER_H_STLSOFT_CCCAP_COMO_EDIT      31
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Auto-generation and compatibility
 */

/*
[<[STLSOFT-AUTO:NO-UNITTEST]>]
*/

/* /////////////////////////////////////////////////////////////////////////////
 * Compiler features
 */

/* Messaging
 */

//#define STLSOFT_CF_PRAGMA_MESSAGE_SUPPORT

/* Types:
 */

/* bool */
#ifdef _BOOL
# define __STLSOFT_CF_NATIVE_BOOL_SUPPORT
#else
 /* Not defined */
#endif /* _BOOL_DEFINED */

/* wchar_t */
#ifdef _WCHAR_T
# define __STLSOFT_CF_NATIVE_WCHAR_T_SUPPORT
#else
 /* Not defined */
#endif /* _WCHAR_T_DEFINED */

/* 8-bit integer */
#define STLSOFT_CF_8BIT_INT_SUPPORT
#define STLSOFT_CF_STANDART_INT8_IS_char
#define STLSOFT_CF_STANDARD_INT8_T               char
#define STLSOFT_CF_STANDARD_SINT8_T       signed char
#define STLSOFT_CF_STANDARD_UINT8_T     unsigned char

/* 16-bit integer */
#define STLSOFT_CF_16BIT_INT_SUPPORT
#define STLSOFT_CF_STANDARD_INT16_IS_short
#define STLSOFT_CF_STANDARD_INT16_T              short
#define STLSOFT_CF_STANDARD_SINT16_T      signed short
#define STLSOFT_CF_STANDARD_UINT16_T    unsigned short

/* 32-bit integer */
#define STLSOFT_CF_32BIT_INT_SUPPORT
#define STLSOFT_CF_STANDARD_INT32_IS_long
#define STLSOFT_CF_STANDARD_INT32_T              long
#define STLSOFT_CF_STANDARD_SINT32_T      signed long
#define STLSOFT_CF_STANDARD_UINT32_T    unsigned long

/* 64-bit integer */
#define STLSOFT_CF_64BIT_INT_SUPPORT
#define STLSOFT_CF_64BIT_INT_IS_long_long
#define STLSOFT_CF_64BIT_INT_IS_EXTENDED_TYPE
#define STLSOFT_CF_EXTENDED_INT64_T              long long
#define STLSOFT_CF_EXTENDED_SINT64_T      signed long long
#define STLSOFT_CF_EXTENDED_UINT64_T    unsigned long long

/* distinct int type */
#define __STLSOFT_CF_INT_DISTINCT_TYPE

/* Member constants */
#define STLSOFT_CF_MEMBER_CONSTANT_SUPPORT

/* Static assertions */
#define __STLSOFT_CF_STATIC_ASSERT_SUPPORT

/* Exception support */
#define __STLSOFT_CF_EXCEPTION_SUPPORT

/*  */
#define __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED

/* Namespace support */
//#define _STLSOFT_NO_NAMESPACES

#define __STLSOFT_CF_NAMESPACE_SUPPORT

#define STLSOFT_CF_ANONYMOUS_UNION_SUPPORT

/* Template support */
#define __STLSOFT_CF_TEMPLATE_SUPPORT

//#define STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS

#define __STLSOFT_CF_EXCEPTION_SIGNATURE_SUPPORT

#define STLSOFT_CF_EXCEPTION_SPEC_EXPENSIVE

#define __STLSOFT_CF_THROW_BAD_ALLOC

#define __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_FUNDAMENTAL_ARGUMENT_SUPPORT

#define __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT

#define STLSOFT_CF_MEM_FUNC_AS_TEMPLATE_PARAM_SUPPORT

#define __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT

#define __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT

#define __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT

#define __STLSOFT_CF_MEMBER_TEMPLATE_CLASS_SUPPORT

#define __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX

#define __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT

#define __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT

#define __STLSOFT_CF_std_NAMESPACE

#define __STLSOFT_CF_std_char_traits_AVAILABLE

#define __STLSOFT_CF_ALLOCATOR_ALLOCATE_HAS_HINT

#define __STLSOFT_CF_ALLOCATOR_DEALLOCATE_HAS_OBJECTCOUNT

#define STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT

#define __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT

#define __STLSOFT_CF_EXPLICIT_KEYWORD_SUPPORT

#define __STLSOFT_CF_MUTABLE_KEYWORD_SUPPORT

#define __STLSOFT_CF_TYPENAME_PARAM_KEYWORD_SUPPORT

#define __STLSOFT_CF_TYPENAME_TYPE_KEYWORD_SUPPORT

#define __STLSOFT_CF_TYPENAME_TYPE_DEF_KEYWORD_SUPPORT

#define __STLSOFT_CF_TYPENAME_TYPE_MIL_KEYWORD_SUPPORT

#define STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT

//#define __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT

#define __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT

#define __STLSOFT_CF_TEMPLATE_TEMPLATE_SUPPORT

#define __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT

#define __STLSOFT_CF_VENEER_SUPPORT

#define STLSOFT_CF_ALLOCATOR_BASE_EXPENSIVE

// Shims are supported
//# define __STLSOFT_CF_TEMPLATE_SHIMS_NOT_SUPPORTED

#define __STLSOFT_CF_NEGATIVE_MODULUS_POSITIVE_GIVES_NEGATIVE_RESULT

#define STLSOFT_CF_OPERATOR_BOOL_AS_OPERATOR_POINTER_TO_MEMBER_SUPPORT
#define STLSOFT_CF_OPERATOR_NOT_VIA_OPERATOR_POINTER_TO_MEMBER_SUPPORT

#if defined(_STLSOFT_CUSTOM_ASSERT)
 /* You have defined the pre-processor symbol _STLSOFT_CUSTOM_ASSERT,
  * which stipulates that you will be providing your own assert. This
  * requires that you have defined _STLSOFT_CUSTOM_ASSERT() as a macro
  * taking 1 parameter (the condition to assert).
  *
  * Suppose you have a function _DisplayAssert(), which has the
  * following signature:
  *
  *   void _DisplayAssert(char const *file, int line, char const *expression);
  *
  * Presumably you would also have your own assert macro, say MY_ASSERT(),
  * defined as:
  *
  *   #define MY_ASSERT(_x) ((void)((!(_x)) ? ((void)(_DisplayAssert(__FILE__, __LINE__, #_x))) : ((void)0)))
  *
  * so you would simply need to define _STLSOFT_CUSTOM_ASSERT() in terms of
  * MY_ASSERT(), as in:
  *
  *  #define _STLSOFT_CUSTOM_ASSERT(_x)    MY_ASSERT(_x)
  *
  * where
  */
# define __STLSOFT_CF_ASSERT_SUPPORT
# define stlsoft_assert(_x)                     _STLSOFT_CUSTOM_ASSERT(_x)
# if defined(_STLSOFT_CUSTOM_ASSERT_INCLUDE)
#  define   __STLSOFT_CF_ASSERT_INCLUDE_NAME    _STLSOFT_CUSTOM_ASSERT_INCLUDE
# else
#  error You must define _STLSOFT_CUSTOM_ASSERT_INCLUDE along with _STLSOFT_CUSTOM_ASSERT()
# endif /* !_STLSOFT_CUSTOM_ASSERT_INCLUDE */
#else
# if defined(__BORLANDC__)
#  define __STLSOFT_CF_ASSERT_INCLUDE_NAME      <assert.h>
#  define stlsoft_assert(_x)                    assert(_x)
# else /* compiler */
#  define __STLSOFT_CF_ASSERT_SUPPORT
#  define __STLSOFT_CF_USE_cassert
# endif /* compiler */
#endif /* _STLSOFT_CUSTOM_ASSERT */

/* /////////////////////////////////////////////////////////////////////////////
 * Calling convention
 */

#define STLSOFT_CF_FASTCALL_SUPPORTED
#define STLSOFT_CF_STDCALL_SUPPORTED

/* /////////////////////////////////////////////////////////////////////////////
 * Inline assembler
 */

#if !defined(_MSC_VER)
# define STSLSOFT_INLINE_ASM_SUPPORTED
# define STSLSOFT_ASM_IN_INLINE_SUPPORTED
#endif /* !_MSC_VER */

/* /////////////////////////////////////////////////////////////////////////////
 * Compiler warning suppression
 */

/* ////////////////////////////////////////////////////////////////////////// */
