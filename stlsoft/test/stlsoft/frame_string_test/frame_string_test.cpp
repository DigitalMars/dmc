/* /////////////////////////////////////////////////////////////////////////////
 * File:        static_string_test.cpp
 *
 * Purpose:
 *
 * Created:
 * Updated:     7th June 2004
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

#include <stlsoft_static_string.h>

#include <algorithm>
#include <string>

////////////////////////////////////////////////////////////////////////////////

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
	_MSC_VER < 1200
# pragma warning(disable : 4550)
#endif /* _MSC_VER < 1200 */

////////////////////////////////////////////////////////////////////////////////
// Functions

static void dump_char(char c)
{
	printf("%c, ", c);
}

////////////////////////////////////////////////////////////////////////////////
// main

int main()
{
#if defined(__STLSOFT_COMPILER_IS_GCC)
	typedef	stlsoft_ns_qual(basic_static_string)<char, 13>											string_a_10_t;
#else
	typedef	stlsoft_ns_qual(basic_static_string)<char, 13, stlsoft_ns_qual_std(char_traits)<char> >	string_a_10_t;
#endif /* __STLSOFT_COMPILER_IS_GCC */

#ifndef __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT
	stlsoft_ns_using(c_str_ptr)
#endif /* __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT */

	char const	hello1[]	=	"Hello, Natty!";
	char const	hello2[]	=	"Hello, Sarah!";

	string_a_10_t	s0;
	string_a_10_t	s1(hello1);
	string_a_10_t	s2(hello2, 6);
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT
	string_a_10_t	s3((char const*)hello2, hello2 + (stlsoft_num_elements(hello2) - 1));

#ifndef __STLSOFT_CF_TEMPLATE_SHIMS_NOT_SUPPORTED
	puts(c_str_ptr(s3));
#endif /* !__STLSOFT_CF_TEMPLATE_SHIMS_NOT_SUPPORTED */
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

	s2.assign(hello2, 9);

	s2.assign((char const*)hello1, hello1 + (stlsoft_num_elements(hello1) - 1));

	s2 = hello2;

	printf("s1: %s\n", s1.c_str());
	printf("s2: %s\n", s2.c_str());

	printf("s1 (forwards):  ");
	stlsoft_ns_qual_std(for_each)(s1.begin(), s1.end(), dump_char);
	printf("\n");

	printf("s2 (forwards):  ");
	stlsoft_ns_qual_std(for_each)(s2.begin(), s2.end(), dump_char);
	printf("\n");

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
	printf("s2 (backwards): ");
	stlsoft_ns_qual_std(for_each)(s2.rbegin(), s2.rend(), dump_char);
	printf("\n");
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

	s1.swap(s2);

	{	string_a_10_t	s(s1);			}
	{	string_a_10_t	s(s1, 3);		}
	{	string_a_10_t	s(s1, 3, 6);	}

	printf("s1: %s\n", s1.c_str());
	printf("s2: %s\n", s2.c_str());

	s2.resize(6);
	s1.assign(s1, 6, 7);
	s2.append(s1);
	s1.assign(s2, 0, 7);
//		= string_a_10_t(s2, 0, 7);
	s1.append(s2, 7, 6);

	printf("s1: %s\n", s1.c_str());
	printf("s2: %s\n", s2.c_str());

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
