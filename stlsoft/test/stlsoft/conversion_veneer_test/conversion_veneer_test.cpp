/* /////////////////////////////////////////////////////////////////////////////
 * File:        conversion_veneer_test.cpp
 *
 * Purpose:
 *
 * Created:
 * Updated:     22nd April 2004
 *
 * Author:      Matthew Wilson
 *
 * Copyright:   Synesis Software Pty Ltd, (c) 2002. All rights reserved.
 *
 * Home:        www.stlsoft.org
 *
 * ////////////////////////////////////////////////////////////////////////// */


// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <stlsoft.h>

stlsoft_ns_using(ss_uint32_t)
stlsoft_ns_using(ss_uint64_t)

#include <stlsoft_conversion_veneer.h>

stlsoft_ns_using(conversion_veneer)
stlsoft_ns_using(reinterpret_conversion)
stlsoft_ns_using(invalid_conversion)

#include <stlsoft_first_class_promoter.h>

stlsoft_ns_using(first_class_promoter)

#include <stlsoft_limit_traits.h>

////////////////////////////////////////////////////////////////////////////////
// Forward declarations / inclusion of windows.h

#if defined(__BORLANDC__) || \
	defined(__DMC__) || \
	defined(_MSC_VER)
// Include windows.h for these exclusively Windows compilers

# include <windows.h>
#else
// Emulate types and functions used for the other compilers

typedef struct _FILETIME {
    ss_uint32_t dwLowDateTime;
    ss_uint32_t dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

typedef union _LARGE_INTEGER {
    struct {
        ss_uint32_t LowPart;
        ss_sint32_t HighPart;
    };
    struct {
        ss_uint32_t LowPart;
        ss_sint32_t HighPart;
    } u;
    ss_sint64_t QuadPart;
} LARGE_INTEGER;

int GetThreadTimes(void *, LPFILETIME , LPFILETIME , LPFILETIME , LPFILETIME )
{
	return 0;
}
void *GetCurrentThread()
{
	return 0;
}

int QueryPerformanceCounter(LARGE_INTEGER *)
{
	return 0;
}

int QueryPerformanceFrequency(LARGE_INTEGER *)
{
	return 0;
}

void Sleep(unsigned int)
{
}

#endif /* 0 */

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char* /* argv */[])
{
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
	// This block demonstrates the conversion of s 64-bit integral type to the
	// Windows FILETIME
	{
		// Define a type that will convert from ss_uint64_t as pointer
		// (to FILETIME *) only, and that will NOT convert as pointer-to-const,
		// or as reference, or as reference-to-const
		//
		// The type will be used in call to GetThreadTimes(), which requires
		// FILETIME*, so it is sensible caution to proscribe all other
		// conversions
		typedef	conversion_veneer	<
										first_class_promoter<ss_uint64_t>
									,	FILETIME
									,	ss_uint64_t
									,	reinterpret_conversion<ss_uint64_t, FILETIME>
									,	invalid_conversion<ss_uint64_t, FILETIME>
									,	invalid_conversion<ss_uint64_t, FILETIME>
//									,	invalid_conversion<ss_uint64_t, FILETIME> // Don't need to specify this, as will default from RC param
									>		uint64_filetime_t;

		uint64_filetime_t	creation;
		uint64_filetime_t	exit;
		uint64_filetime_t	kernel1;
		uint64_filetime_t	kernel2;
		uint64_filetime_t	user1;
		uint64_filetime_t	user2;

		::GetThreadTimes(GetCurrentThread(), &creation, &exit, &kernel1, &user1);
		::Sleep(static_cast<ss_uint32_t>(static_cast<ss_uint64_t&>(creation)) % 10000);
		::GetThreadTimes(GetCurrentThread(), &creation, &exit, &kernel2, &user2);
	}
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */

	//
	{
		typedef	conversion_veneer	<
										FILETIME
									,	LARGE_INTEGER
									,	FILETIME
									,	reinterpret_conversion<FILETIME, LARGE_INTEGER>
									,	reinterpret_conversion<FILETIME, LARGE_INTEGER>
									,	reinterpret_conversion<FILETIME, LARGE_INTEGER>
// Enable/disable this to show how the reference-to-const conversion is proscribed
//									,	invalid_conversion<FILETIME, LARGE_INTEGER>
									>		filetime_largeint_t;

		filetime_largeint_t	frequency;
		filetime_largeint_t	pre;
		filetime_largeint_t	post;

		::QueryPerformanceFrequency(&frequency);
		::QueryPerformanceCounter(&pre);
		::Sleep(1000);
		::QueryPerformanceCounter(&post);


// Enable/disable this to show how the reference-to-const conversion, if proscribed above, is inaccessible
#if 1
		filetime_largeint_t const				&cft2li	=	pre;
		LARGE_INTEGER const 	&cli	=	cft2li;
#endif /* 0 */

		LARGE_INTEGER			&liPre	=	pre;
		LARGE_INTEGER			&liPost	=	post;

		(void)liPre;
		(void)liPost;
		STLSOFT_SUPPRESS_UNUSED(cft2li);
		STLSOFT_SUPPRESS_UNUSED(cli);
	}

	{
		typedef	conversion_veneer	<
										first_class_promoter<ss_uint64_t>
									,	LARGE_INTEGER
									,	ss_uint64_t
									,	reinterpret_conversion<ss_uint64_t, LARGE_INTEGER>
									,	reinterpret_conversion<ss_uint64_t, LARGE_INTEGER>
									,	reinterpret_conversion<ss_uint64_t, LARGE_INTEGER>
// Enable/disable this to show how the reference-to-const conversion is proscribed
//									,	invalid_conversion<ss_uint64_t, LARGE_INTEGER>
									>		filetime_largeint_t;

		filetime_largeint_t	frequency;
		filetime_largeint_t	pre;
		filetime_largeint_t	post;

		::QueryPerformanceFrequency(&frequency);
		::QueryPerformanceCounter(&pre);
		::Sleep(1000);
		::QueryPerformanceCounter(&post);

// Enable/disable this to show how the reference-to-const conversion, if proscribed above, is inaccessible
#if 0
		filetime_largeint_t const	&cft2li	=	pre;
		LARGE_INTEGER const 		&cli	=	cft2li;
#endif /* 0 */

		LARGE_INTEGER				&liPre	=	pre;
		LARGE_INTEGER				&liPost	=	post;

		(void)liPre;
		(void)liPost;
	}

	{
		typedef conversion_veneer < first_class_promoter<ss_uint64_t>
								  , LARGE_INTEGER
								  , ss_uint64_t
								  , reinterpret_conversion<ss_uint64_t, LARGE_INTEGER>
								  , invalid_conversion<ss_uint64_t, LARGE_INTEGER>
								  >   large_integer_64_t;

		ss_uint64_t			i64	=	__STLSOFT_GEN_UINT64_SUFFIX(0x1234567012345670);
		large_integer_64_t	should_not_compile	=	i64;
		ss_uint64_t			&i1	=	should_not_compile;

		LARGE_INTEGER		*p	=	&should_not_compile;
#if 0
		LARGE_INTEGER		&r	=	should_not_compile;
#endif /* 0 */

		i1 = __STLSOFT_GEN_UINT64_SUFFIX(0x1234567012345670);

#if 1
		should_not_compile = i1;
		should_not_compile = ~i1;
#endif /* 0 */

		STLSOFT_SUPPRESS_UNUSED(p);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
