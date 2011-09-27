
/* Copyright (C) 2001-2003 by Digital Mars */
/* www.digitalmars.com */

#if __DMC__ || __RCC__
#pragma once
#endif

#ifndef __STDINT_H
#define __STDINT_H 1

#define __EXPAND(x) x

#define __LONGLONG (__INTSIZE == 4)

/* Exact sizes */
typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef long int32_t;
typedef unsigned long uint32_t;

/* At least sizes */

typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;

typedef short int_least16_t;
typedef unsigned short uint_least16_t;

typedef long int_least32_t;
typedef unsigned long uint_least32_t;

/* Fastest minimum width sizes */

typedef signed char int_fast8_t;
typedef unsigned char uint_fast8_t;

typedef int int_fast16_t;
typedef unsigned uint_fast16_t;

typedef long int_fast32_t;
typedef unsigned long uint_fast32_t;

/* Integer pointer holders */

#if defined(__COMPACT__) || defined(__LARGE__) || defined(__VCM__)
typedef long intptr_t;
typedef unsigned long uintptr_t;
#else
typedef int intptr_t;
typedef unsigned uintptr_t;
#endif

/* Greatest width integer types */

#if __LONGLONG
typedef long long intmax_t;
typedef unsigned long long uintmax_t;
#else
typedef long intmax_t;
typedef unsigned long uintmax_t;
#endif

/* long long typedefs */

#if __LONGLONG

typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef long long int_least64_t;
typedef unsigned long long uint_least64_t;

typedef long long int_fast64_t;
typedef unsigned long long uint_fast64_t;

#endif

#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

#define INT8_MIN	(-128)
#define INT8_MAX	127
#define UINT8_MAX	255

#define INT16_MIN	(-32768)
#define INT16_MAX	32767
#define UINT16_MAX	65535

#define INT32_MIN	(-2147483647L - 1)
#define INT32_MAX	2147483647
#define UINT32_MAX	4294967295

#define INT_LEAST8_MIN	(-128)
#define INT_LEAST8_MAX	127
#define UINT_LEAST8_MAX	255

#define INT_LEAST16_MIN	(-32768)
#define INT_LEAST16_MAX	32767
#define UINT_LEAST16_MAX 65535

#define INT_LEAST32_MIN	(-2147483647L - 1)
#define INT_LEAST32_MAX	2147483647
#define UINT_LEAST32_MAX 4294967295

#define INT_FAST8_MIN	(-128)
#define INT_FAST8_MAX	127
#define UINT_FAST8_MAX	255

#if __INTSIZE == 4
#define INT_FAST16_MIN	(-2147483647L - 1)
#define INT_FAST16_MAX	2147483647
#define UINT_FAST16_MAX	4294967295
#else
#define INT_FAST16_MIN	(-32768)
#define INT_FAST16_MAX	32767
#define UINT_FAST16_MAX	65535
#endif

#define INT_FAST32_MIN	(-2147483647L - 1)
#define INT_FAST32_MAX	2147483647
#define UINT_FAST32_MAX	4294967295

#if __LONGLONG

#define INT64_MIN	(-9223372036854775807LL-1LL)
#define INT64_MAX	9223372036854775807
#define UINT64_MAX	18446744073709551615

#define INT_LEAST64_MIN	(-9223372036854775807LL-1LL)
#define INT_LEAST64_MAX	9223372036854775807
#define UINT_LEAST64_MAX 18446744073709551615

#define INT_FAST64_MIN	(-9223372036854775807LL-1LL)
#define INT_FAST64_MAX	9223372036854775807
#define UINT_FAST64_MAX	18446744073709551615

#endif

#if defined(__COMPACT__) || defined(__LARGE__) || defined(__VCM__) || __INTSIZE == 4
#define INTPTR_MIN	(-2147483647L - 1)
#define INTPTR_MAX	2147483647
#define UINTPTR_MAX	4294967295
#else
#define INTPTR_MIN	(-32768)
#define INTPTR_MAX	32767
#define UINTPTR_MAX	65535
#endif

#if __LONGLONG
#define INTMAX_MIN	(-9223372036854775807LL-1LL)
#define INTMAX_MAX	9223372036854775807
#define UINTMAX_MAX	18446744073709551615
#define INTMAX_C(v)	__EXPAND(__EXPAND(v)##LL)
#define UINTMAX_C(v)	__EXPAND(__EXPAND(v)##ULL)
#else
#define INTMAX_MIN	(-2147483647L - 1)
#define INTMAX_MAX	2147483647
#define UINTMAX_MAX	4294967295
#define INTMAX_C(v)	__EXPAND(__EXPAND(v)##L)
#define UINTMAX_C(v)	__EXPAND(__EXPAND(v)##UL)
#endif

#if __INTSIZE == 4
#define PTRDIFF_MIN	(-2147483647L - 1)
#define PTRDIFF_MAX	2147483647
#define SIG_ATOMIC_MIN	(-2147483647L - 1)
#define SIG_ATOMIC_MAX	2147483647
#define SIZE_MAX	4294967295
#else
#define PTRDIFF_MIN	(-32768)
#define PTRDIFF_MAX	32767
#define SIG_ATOMIC_MIN	(-32768)
#define SIG_ATOMIC_MAX	32767
#define SIZE_MAX	65535
#endif

#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif

#define WCHAR_MIN	(wchar_t)0
#define WCHAR_MAX	(wchar_t)0xFFFF

#define WINT_MIN	0
#define WINT_MAX	65535

#define INT8_C(v)	v
#define UINT8_C(v)	v
#define INT16_C(v)	v
#define UINT16_C(v)	v
#define INT32_C(v)	__EXPAND(__EXPAND(v)##L)
#define UINT32_C(v)	__EXPAND(__EXPAND(v)##UL)

#if __LONGLONG
#define INT64_C(v)	__EXPAND(__EXPAND(v)##LL)
#define UINT64_C(v)	__EXPAND(__EXPAND(v)##ULL)
#endif

#endif

#endif
