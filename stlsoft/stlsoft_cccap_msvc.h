/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_cccap_msvc.h
 *
 * Purpose:     Compiler feature discrimination for Visual C++.
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

/// \file stlsoft_cccap_msvc.h
///
/// Compiler feature discrimination for Visual C++.

#ifdef STLSOFT_INCL_H_STLSOFT_CCCAP_MSVC
# error This file cannot be included more than once in any compilation unit
#endif /* STLSOFT_INCL_H_STLSOFT_CCCAP_MSVC */

#ifndef __STLSOFT_COMPILER_IS_MSVC
# error This file has been erroneously included for a compiler other than Visual C++
#endif /* !__STLSOFT_COMPILER_IS_MSVC */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_CCCAP_MSVC_MAJOR     2
# define STLSOFT_VER_H_STLSOFT_CCCAP_MSVC_MINOR     0
# define STLSOFT_VER_H_STLSOFT_CCCAP_MSVC_REVISION  1
# define STLSOFT_VER_H_STLSOFT_CCCAP_MSVC_EDIT      50
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

#define STLSOFT_CF_PRAGMA_MESSAGE_SUPPORT

/* Types:
 */

/* bool */
#ifdef __BOOL_DEFINED
# define __STLSOFT_CF_NATIVE_BOOL_SUPPORT
#else
 /* Not defined */
#endif /* __BOOL_DEFINED */

/* wchar_t */
#if _MSC_VER >= 1300
 /* Even though VC 7.0 and 7.1 provide a native wchar_t type, that is __wchar_t,
  * it is not compatible with their libraries (which use the typedef wchar_t),
  * so we cannot use it.
  *
  * wchar_t itself may be used, when _NATIVE_WCHAR_T_DEFINED is defined
  */
# ifdef _NATIVE_WCHAR_T_DEFINED
#  define __STLSOFT_CF_NATIVE_WCHAR_T_SUPPORT
//#  define __STLSOFT_NATIVE_WCHAR_T            __wchar_t
# elif defined(_WCHAR_T_DEFINED)
#  define __STLSOFT_CF_TYPEDEF_WCHAR_T_SUPPORT
# else
  /* Not defined */
#  define __STLSOFT_CF_TYPEDEF_WCHAR_T_SUPPORT
# endif /* _WCHAR_T_DEFINED */
#else
 /* Previous versions do not have a native type, but do have the typedef wchar_t
  * when _WCHAR_T_DEFINED is defined
  */
# if defined(_WCHAR_T_DEFINED)
#  define __STLSOFT_CF_TYPEDEF_WCHAR_T_SUPPORT
# else
  /* Not defined */
# endif /* _WCHAR_T_DEFINED */
#endif /* _MSC_VER */

/* Integral types
 *
 * The integral types may be provided, for a given compiler, by standard types,
 * or by extended types, or both.
 */

/* 8-bit integer */
#define STLSOFT_CF_8BIT_INT_SUPPORT
#if _MSC_VER >= 1020
# define STLSOFT_CF_8BIT_INT_IS_EXTENDED_TYPE
# define STLSOFT_CF_EXTENDED_INT8_T              __int8
# define STLSOFT_CF_EXTENDED_SINT8_T      signed __int8
# define STLSOFT_CF_EXTENDED_UINT8_T    unsigned __int8
# if _MSC_VER == 1200
#  define STLSOFT_CF_8BIT_INT_EXTENDED_TYPE_IS_DISTINCT
# endif /* _MSC_VER */
#endif /* _MSC_VER >= 1020 */
#define STLSOFT_CF_STANDART_INT8_IS_char
#define STLSOFT_CF_STANDARD_INT8_T               char
#define STLSOFT_CF_STANDARD_SINT8_T       signed char
#define STLSOFT_CF_STANDARD_UINT8_T     unsigned char

/* 16-bit integer */
#define STLSOFT_CF_16BIT_INT_SUPPORT
#if _MSC_VER >= 1020
# define STLSOFT_CF_16BIT_INT_IS_EXTENDED_TYPE
# define STLSOFT_CF_EXTENDED_INT16_T            __int16
# define STLSOFT_CF_EXTENDED_SINT16_T    signed __int16
# define STLSOFT_CF_EXTENDED_UINT16_T  unsigned __int16
# if _MSC_VER == 1200
#  define STLSOFT_CF_16BIT_INT_EXTENDED_TYPE_IS_DISTINCT
# endif /* _MSC_VER */
#endif /* _MSC_VER >= 1020 */
#define STLSOFT_CF_STANDARD_INT16_IS_short
#define STLSOFT_CF_STANDARD_INT16_T             short
#define STLSOFT_CF_STANDARD_SINT16_T     signed short
#define STLSOFT_CF_STANDARD_UINT16_T   unsigned short

/* 32-bit integer */
#define STLSOFT_CF_32BIT_INT_SUPPORT
#if _MSC_VER >= 1020
# define STLSOFT_CF_32BIT_INT_IS_EXTENDED_TYPE
# define STLSOFT_CF_EXTENDED_INT32_T            __int32
# define STLSOFT_CF_EXTENDED_SINT32_T    signed __int32
# define STLSOFT_CF_EXTENDED_UINT32_T  unsigned __int32
# if _MSC_VER == 1200
#  define STLSOFT_CF_32BIT_INT_EXTENDED_TYPE_IS_DISTINCT
# endif /* _MSC_VER */
#endif /* _MSC_VER >= 1020 */
#define STLSOFT_CF_STANDARD_INT32_IS_long
#define STLSOFT_CF_STANDARD_INT32_T             long
#define STLSOFT_CF_STANDARD_SINT32_T     signed long
#define STLSOFT_CF_STANDARD_UINT32_T   unsigned long

/* 64-bit integer */
#if _MSC_VER >= 1020
# define STLSOFT_CF_64BIT_INT_SUPPORT
# define STLSOFT_CF_64BIT_INT_IS_EXTENDED_TYPE
# define STLSOFT_CF_64BIT_INT_IS___int64
# define STLSOFT_CF_EXTENDED_INT64_T            __int64
# define STLSOFT_CF_EXTENDED_SINT64_T    signed __int64
# define STLSOFT_CF_EXTENDED_UINT64_T  unsigned __int64
#endif /* _MSC_VER >= 1020 */

/* Are integers a unique type (i.e. not int8/16/32/64)? */
#if _MSC_VER == 1200
# define __STLSOFT_CF_INT_DISTINCT_TYPE
#endif /* _MSC_VER */


/* Member constants */
#if _MSC_VER >= 1300
# define STLSOFT_CF_MEMBER_CONSTANT_SUPPORT
#endif /* _MSC_VER > 1300 */

/* Static assertions */
#define __STLSOFT_CF_STATIC_ASSERT_SUPPORT

#define __STLSOFT_CF_STATIC_ASSERT_RELEASE_OPTIMISATION_SUPPORT

/* Exception support */
#ifdef _CPPUNWIND
# define __STLSOFT_CF_EXCEPTION_SUPPORT
#else
 /* Not defined */
#endif /* _CPPUNWIND */

/*  */
#if _MSC_VER >= 1100 && \
    _MSC_VER < 1310
# define __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
#endif /* _MSC_VER */

/* Namespace support */
#if _MSC_VER < 1100
 /* Since Visual C++ 4.2 and earlier do not correctly support using declarations
  * when applied to templates, it makes the use of namespaces with templates
  * too painful to use, so namespaces are suppressed.
  */
# define _STLSOFT_NO_NAMESPACES
#endif /* _MSC_VER < 1100 */

#if _MSC_VER >= 1020
# define __STLSOFT_CF_NAMESPACE_SUPPORT
#endif /* _MSC_VER >= 1020 */

#define STLSOFT_CF_ANONYMOUS_UNION_SUPPORT

/* Template support */
#define __STLSOFT_CF_TEMPLATE_SUPPORT

#if _MSC_VER < 1100
# define STLSOFT_CF_TEMPLATE_TYPE_REQUIRED_IN_ARGS
#endif /* _MSC_VER */

#if _MSC_VER >= 1100
# define __STLSOFT_CF_EXCEPTION_SIGNATURE_SUPPORT
#endif /* _MSC_VER >= 1100 */

//#define STLSOFT_CF_EXCEPTION_SPEC_EXPENSIVE

//#  define __STLSOFT_CF_THROW_BAD_ALLOC

#if _MSC_VER >= 1100
# define __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_FUNDAMENTAL_ARGUMENT_SUPPORT
#else
 /* Not defined */
#endif /* _MSC_VER */

/* Although VC++ 5.0 does work with this in isolated cases, in practice it experiences
 * too many internal compiler errors, or compiler lock-ups, to make it tolerable
 */
#if _MSC_VER >= 1100
# define __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
#else
 /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1310
# define STLSOFT_CF_MEM_FUNC_AS_TEMPLATE_PARAM_SUPPORT
#else
 /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1100
# define __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
#else
  /* Not defined */
#endif /* _MSC_VER */

/* Although VC++ 5.0 does work with this in isolated cases, in practice it experiences
 * too many internal compiler errors, or compiler lock-ups, to make it tolerable
 */
#if _MSC_VER > 1100
# define __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
#else
  /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1200
# define __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT
#else
  /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1100
# define __STLSOFT_CF_MEMBER_TEMPLATE_CLASS_SUPPORT
#else
  /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1100
# define __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX
#else
  /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1310
# define __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
#endif /* _MSC_VER */

#if _MSC_VER >= 1100
# define __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
#else
  /* Not supported */
# ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
#  undef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
# endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
#endif /* _MSC_VER */

#if _MSC_VER >= 1100
# define __STLSOFT_CF_std_NAMESPACE
#else
  /* Not defined */
#endif /* _MSC_VER */

#define __STLSOFT_CF_std_char_traits_AVAILABLE

#define __STLSOFT_CF_ALLOCATOR_ALLOCATE_HAS_HINT

#if _MSC_VER >= 1100
# define __STLSOFT_CF_ALLOCATOR_DEALLOCATE_HAS_OBJECTCOUNT
#else
 /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1100
# define STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT
#else
  /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1200
# define __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
#else
 /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1100
# define __STLSOFT_CF_EXPLICIT_KEYWORD_SUPPORT
#else
 /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1100
# define __STLSOFT_CF_MUTABLE_KEYWORD_SUPPORT
#else
 /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1100
# define __STLSOFT_CF_TYPENAME_PARAM_KEYWORD_SUPPORT
#else
 /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1200
# define __STLSOFT_CF_TYPENAME_TYPE_KEYWORD_SUPPORT
#else
 /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1300
# define __STLSOFT_CF_TYPENAME_TYPE_DEF_KEYWORD_SUPPORT
#else
 /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1310
//#  define __STLSOFT_CF_TYPENAME_TYPE_MIL_KEYWORD_SUPPORT
#else
 /* Not defined */
#endif /* _MSC_VER */

#if _MSC_VER >= 1310
//#  define STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT
#else
 /* Not defined */
#endif /* _MSC_VER */

#if defined(_MSC_EXTENSIONS) && \
    _MSC_VER < 1310
# define __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT
#endif /* _MSC_EXTENSIONS && _MSC_VER < 1310 */

#if _MSC_VER >= 1310
# define __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT
#endif /* _MSC_VER */

#if _MSC_VER >= 1300
# define __STLSOFT_CF_TEMPLATE_TEMPLATE_SUPPORT
#endif /* _MSC_VER */

#if _MSC_VER >= 1300
# define __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
#else
  /* Not defined */
#endif /* _MSC_VER */

#define __STLSOFT_CF_VENEER_SUPPORT

#if !defined(_ATL_MIN_CRT)
# define STLSOFT_CF_ALLOCATOR_BASE_EXPENSIVE
#endif /* !_ATL_MIN_CRT */

#if _MSC_VER < 1100
# define __STLSOFT_CF_TEMPLATE_SHIMS_NOT_SUPPORTED
#endif /* _MSC_VER */

#define __STLSOFT_CF_NEGATIVE_MODULUS_POSITIVE_GIVES_NEGATIVE_RESULT

#if _MSC_VER >= 1300
# define STLSOFT_CF_OPERATOR_BOOL_AS_OPERATOR_POINTER_TO_MEMBER_SUPPORT
# define STLSOFT_CF_OPERATOR_NOT_VIA_OPERATOR_POINTER_TO_MEMBER_SUPPORT
#endif /* _MSC_VER >= 1300 */


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
# define __STLSOFT_CF_ASSERT_SUPPORT
 //#define   __STLSOFT_CF_USE_cassert
# define __STLSOFT_CF_ASSERT_INCLUDE_NAME       <crtdbg.h>
# define stlsoft_assert(_x)                     _ASSERTE(_x)
#endif /* _STLSOFT_CUSTOM_ASSERT */

/* /////////////////////////////////////////////////////////////////////////////
 * Calling convention
 */

#define STLSOFT_CF_FASTCALL_SUPPORTED
#define STLSOFT_CF_STDCALL_SUPPORTED

/* /////////////////////////////////////////////////////////////////////////////
 * Inline assembler
 */

#define STSLSOFT_INLINE_ASM_SUPPORTED
#define STSLSOFT_ASM_IN_INLINE_SUPPORTED

/* /////////////////////////////////////////////////////////////////////////////
 * Compiler warning suppression
 */

/* Suppresses: "'identifier' : has bad storage class" */
#pragma warning(disable : 4042)

/* Suppresses: "typedef-name 'identifier1' used as synonym for class-name 'identifier2'" */
#pragma warning(disable : 4097)

/* Suppresses: "conditional expression is constant" */
#pragma warning(disable : 4127)

/* Suppresses: "qualifier applied to reference type ignored" */
#pragma warning(disable : 4181)

/* Suppresses: "'<function>' has C-linkage specified, but returns UDT '<udt>' which is incompatible with C" */
#if _MSC_VER < 1200
# pragma warning(disable : 4190)
#endif /* _MSC_VER < 1100 */

/* Suppresses: "nonstandard extension used : nameless struct/union" */
#pragma warning(disable : 4201)

/* Suppresses: "nonstandard extension used : 'xxxx' keyword is reserved for future use" */
#if _MSC_VER < 1100
# pragma warning(disable : 4237)
#endif /* _MSC_VER < 1100 */

/* Suppresses: "C++ Exception Specification ignored" */
#pragma warning(disable : 4290)

#if defined(_MSC_EXTENSIONS)
/* Suppresses: nonstandard extension used : 'argument' : conversion from 'X' to 'X&' */
# pragma warning(disable : 4239)
#endif /* _MSC_EXTENSIONS && _MSC_VER < 1310 */

/* Suppresses: "'' decorated name length exceeded, name was truncated" */
#pragma warning(disable : 4503)

/* Suppresses: "unreferenced inline function has been removed" */
#pragma warning(disable : 4514)

/* Suppresses: "C++ language change: to explicitly specialize class template 'X' use the following syntax: template<> struct X<Y>" */
#pragma warning(disable : 4663)

/* Suppresses: "'function' : resolved overload was found by argument-dependent lookup" */
#if _MSC_VER >= 1310
# pragma warning(disable : 4675)
#endif /* _MSC_VER >= 1310 */

/* Suppresses: "function not expanded" */
#pragma warning(disable : 4710)

/* Suppresses: "identifier was truncated to '255' characters in the browser information" */
#pragma warning(disable : 4786)

/* ////////////////////////////////////////////////////////////////////////// */
