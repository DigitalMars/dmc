/* /////////////////////////////////////////////////////////////////////////////
 * File:        algorithm_test.cpp
 *
 * Purpose:     Illustrates some of the simple alrogithms
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
#include <stlsoft_simple_algorithms.h>

stlsoft_ns_using(for_each_count_success)
stlsoft_ns_using(for_each_if)
stlsoft_ns_using(for_each_set_value)
stlsoft_ns_using(for_each_set_value_if)
stlsoft_ns_using(for_each_preinc)
stlsoft_ns_using(for_each_postinc)

#include <algorithm>

stlsoft_ns_using_std(for_each)

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
	_MSC_VER < 1200
# pragma warning(disable : 4550)
#endif /* _MSC_VER < 1200 */

////////////////////////////////////////////////////////////////////////////////
// Function declarations

static void print_int(int const &i);
static int is_odd(int const &i);

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char* /* argv */[])
{
	int		ints[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int		*begin	=	ints;
	int		*end	=	ints + stlsoft_num_elements(ints);
	size_t	cOdds	=	for_each_count_success(begin, end, is_odd);

	// stlsoft::for_each_count_success
	printf("Number of odds: %u\n", cOdds);

	printf("Odds:\n");
	// stlsoft::for_each_if
	for_each_if(begin, end, print_int, is_odd);

	printf("Setting all odd values to 99:\n");
	// stlsoft::for_each_set_value_if
	(void)for_each_set_value_if(begin, end, 99, is_odd);
	for_each(begin, end, print_int);

	printf("Setting all values to 3:\n");
	// stlsoft::for_each_set_value
	(void)for_each_set_value(begin, end, 3);
	for_each(begin, end, print_int);

	printf("Printing using for_each_preinc:\n");
	for_each_preinc(begin, end, print_int);

	printf("Printing using for_each_postinc:\n");
	for_each_postinc(begin, end, print_int);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Functions

void print_int(int const &i)
{
	printf("%d\n", i);
}

int is_odd(int const &i)
{
	return i & 1;
}

////////////////////////////////////////////////////////////////////////////////
