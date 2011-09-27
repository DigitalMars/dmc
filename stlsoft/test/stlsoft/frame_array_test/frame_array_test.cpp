/* /////////////////////////////////////////////////////////////////////////////
 * File:        static_array_test.cpp
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

#ifdef __BORLANDC__
#pragma warn -8008
#pragma warn -8066
#endif /* __BORLANDC__ */

#include <stdio.h>

#include <stlsoft.h>

stlsoft_ns_using(ss_sint8_t)
stlsoft_ns_using(ss_sint16_t)
stlsoft_ns_using(ss_sint32_t)
stlsoft_ns_using(ss_uint32_t)
stlsoft_ns_using(ss_size_t)

#include <stlsoft_frame_array.h>

stlsoft_ns_using(static_array_1d)
stlsoft_ns_using(static_array_2d)
stlsoft_ns_using(static_array_3d)
stlsoft_ns_using(static_array_4d)
stlsoft_ns_using(do_construction)

#include <algorithm>

stlsoft_ns_using_std(for_each)

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_COMPILER_IS_INTEL
# pragma warning(disable : 810)
#endif /* __STLSOFT_COMPILER_IS_INTEL */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
	_MSC_VER < 1200
# pragma warning(disable : 4550)
#endif /* _MSC_VER < 1200 */

////////////////////////////////////////////////////////////////////////////////
// Function declarations

static void func1D_fixed();
static void func2D_fixed();
static void func3D_fixed();
static void func4D_fixed();
static void func1D_frame();
static void func2D_frame();
static void func3D_frame();
static void func4D_frame();

static void print_sint8(ss_sint8_t i);
static void print_sint16(ss_sint16_t i);
static void print_sint24(ss_sint32_t i);

////////////////////////////////////////////////////////////////////////////////
// main

extern "C" void __stdcall Sleep(unsigned long);

int main(int /* argc */, char* /* argv */[])
{
//	for(stlsoft::ss_uint64_t i = 1; i ; ++i)	{}

	func1D_fixed();
	func2D_fixed();
	func3D_fixed();
	func4D_fixed();

	func1D_frame();
	func2D_frame();
	func3D_frame();
	func4D_frame();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Function declarations

void print_sint8(ss_sint8_t i)
{
	printf("[%d] ", i & 0xff);
}

void print_sint16(ss_sint16_t i)
{
	printf("[%d, %d] ", (i >> 8) & 0xff, i & 0xff);
}

void print_sint24(ss_sint32_t i)
{
	printf("[%d, %d, %d] ", (i >> 16) & 0xff, (i >> 8) & 0xff, i & 0xff);
}

void func1D_fixed()
{
#ifdef _STLSOFT_INCL_H_STLSOFT_FIXED_ARRAY
	typedef fixed_array_1d	<	ss_sint8_t
							,	new_allocator<ss_sint8_t>
							,	do_construction<ss_sint8_t>
							>	int_fixed_array_1D;

	int_fixed_array_1D			ix1a(5);
	int_fixed_array_1D const	&ix1ac	=	ix1a;

    (void)ix1a.at(4);
    (void)ix1ac.at(4);

    (void)ix1a[4];
    (void)ix1ac[4];

    (void)ix1a.front();
    (void)ix1a.back();
    (void)ix1ac.front();
    (void)ix1ac.back();

    (void)ix1a.dimension0();
    (void)ix1a.size();
    (void)ix1a.empty();
    (void)ix1a.max_size();

    (void)ix1a.begin();
    (void)ix1a.end();
    (void)ix1ac.begin();
    (void)ix1ac.end();

    (void)ix1a.rbegin();
    (void)ix1a.rend();
    (void)ix1ac.rbegin();
    (void)ix1ac.rend();

	ix1a.data();

	for(ss_size_t i = 0; i < ix1a.dimension0(); ++i)
	{
		ss_sint8_t	x	=	static_cast<ss_sint8_t>(i);

		ix1a[i] = x;
	}

	printf("1d:\n");
	for_each(ix1ac.begin(), ix1ac.end(), print_sint8);
	printf("\n1d:\n");
	for_each(ix1ac.rbegin(), ix1ac.rend(), print_sint8);
	printf("\n");
#endif /* _STLSOFT_INCL_H_STLSOFT_FIXED_ARRAY */
}

void func2D_fixed()
{
#ifdef _STLSOFT_INCL_H_STLSOFT_FIXED_ARRAY
	typedef fixed_array_2d	<	ss_sint16_t
							,	new_allocator<ss_sint16_t>
							,	do_construction<ss_sint16_t>
							>	int_fixed_array_2D;

	int_fixed_array_2D			ix2a(5, 5);
	int_fixed_array_2D const	&ix2ac	=	ix2a;

    (void)ix2a.at(4, 2);
    (void)ix2ac.at(4, 2);

    (void)ix2a.at(3);
    (void)ix2ac.at(3);

    (void)ix2a[4][4];
    (void)ix2ac[4][4];

    (void)ix2a.front();
    (void)ix2a.back();
    (void)ix2ac.front();
    (void)ix2ac.back();

    (void)ix2a.dimension0();
    (void)ix2a.dimension1();
    (void)ix2a.size();
    (void)ix2a.empty();
    (void)ix2a.max_size();

    (void)ix2a.begin();
    (void)ix2a.end();
    (void)ix2ac.begin();
    (void)ix2ac.end();

    (void)ix2a.rbegin();
    (void)ix2a.rend();
    (void)ix2ac.rbegin();
    (void)ix2ac.rend();

	(void)ix2a.data();


	for(ss_size_t i = 0; i < ix2a.dimension0(); ++i)
	{
		for(ss_size_t j = 0; j < ix2a.dimension1(); ++j)
		{
			ss_sint16_t	x	=	static_cast<ss_sint16_t>((i << 8) | j);

			ix2a[i][j] = x;
		}
	}

	printf("2d:\n");
	for_each(ix2ac.begin(), ix2ac.end(), print_sint16);
	printf("\n2d:\n");
	for_each(ix2ac.rbegin(), ix2ac.rend(), print_sint16);
	printf("\n");
#endif /* _STLSOFT_INCL_H_STLSOFT_FIXED_ARRAY */
}

void func3D_fixed()
{
#ifdef _STLSOFT_INCL_H_STLSOFT_FIXED_ARRAY
	typedef fixed_array_3d	<	ss_sint32_t
							,	new_allocator<ss_sint32_t>
							,	do_construction<ss_sint32_t>
							>	int_fixed_array_3D;

	int_fixed_array_3D			ix3a(5, 5, 5);
	int_fixed_array_3D const	&ix3ac	=	ix3a;

    (void)ix3a.at(4, 2, 1);
    (void)ix3ac.at(4, 2, 1);

    (void)ix3a.at(3);
    (void)ix3ac.at(3);

    (void)ix3a[4][4][4];
    (void)ix3ac[4][4][4];

    (void)ix3a.front();
    (void)ix3a.back();
    (void)ix3ac.front();
    (void)ix3ac.back();

    (void)ix3a.dimension0();
    (void)ix3a.dimension1();
    (void)ix3a.dimension2();
    (void)ix3a.size();
    (void)ix3a.empty();
    (void)ix3a.max_size();

    (void)ix3a.begin();
    (void)ix3a.end();
    (void)ix3ac.begin();
    (void)ix3ac.end();

    (void)ix3a.rbegin();
    (void)ix3a.rend();
    (void)ix3ac.rbegin();
    (void)ix3ac.rend();

	(void)ix3a.data();


	for(ss_size_t i = 0; i < ix3a.dimension0(); ++i)
	{
		for(ss_size_t j = 0; j < ix3a.dimension1(); ++j)
		{
			for(ss_size_t k = 0; k < ix3a.dimension2(); ++k)
			{
				ss_sint32_t	x	=	static_cast<ss_sint32_t>((i << 16) | (j << 8) | k);

				ix3a[i][j][k] = x;
			}
		}
	}

	printf("3d:\n");
	for_each(ix3ac.begin(), ix3ac.end(), print_sint24);
	printf("\n3d:\n");
	for_each(ix3ac.rbegin(), ix3ac.rend(), print_sint24);
	printf("\n");
#endif /* _STLSOFT_INCL_H_STLSOFT_FIXED_ARRAY */
}

void func4D_fixed()
{
#ifdef _STLSOFT_INCL_H_STLSOFT_FIXED_ARRAY
#endif /* _STLSOFT_INCL_H_STLSOFT_FIXED_ARRAY */
}

void func1D_frame()
{
#ifdef _STLSOFT_INCL_H_STLSOFT_FRAME_ARRAY
	typedef static_array_1d	<	ss_sint8_t
							,	5
							,	do_construction<ss_sint32_t>
							>	int_static_array_1d;

	int_static_array_1d			ix1a;
	int_static_array_1d const	&ix1ac	=	ix1a;

    (void)ix1a.at(4);
    (void)ix1ac.at(4);

    (void)ix1a[4];
    (void)ix1ac[4];

    (void)ix1a.front();
    (void)ix1a.back();
    (void)ix1ac.front();
    (void)ix1ac.back();

    (void)ix1a.dimension0();
    (void)ix1a.size();
    (void)ix1a.empty();
    (void)ix1a.max_size();

    (void)ix1a.begin();
    (void)ix1a.end();
    (void)ix1ac.begin();
    (void)ix1ac.end();

    (void)ix1a.rbegin();
    (void)ix1a.rend();
    (void)ix1ac.rbegin();
    (void)ix1ac.rend();

	(void)ix1a.data();

	for(ss_size_t i = 0; i < ix1a.dimension0(); ++i)
	{
		ss_sint8_t	x	=	static_cast<ss_sint8_t>(i);

		ix1a[i] = x;
	}

	printf("1d:\n");
	for_each(ix1ac.begin(), ix1ac.end(), print_sint8);
	printf("\n1d:\n");
	for_each(ix1ac.rbegin(), ix1ac.rend(), print_sint8);
	printf("\n");
#endif /* _STLSOFT_INCL_H_STLSOFT_FRAME_ARRAY */
}

void func2D_frame()
{
#ifdef _STLSOFT_INCL_H_STLSOFT_FRAME_ARRAY
	typedef static_array_2d	<	ss_sint16_t
							,	5
							,	5
							,	do_construction<ss_sint32_t>
							>	int_fixed_array_2D;

	int_fixed_array_2D			ix2a;
	int_fixed_array_2D const	&ix2ac	=	ix2a;

    (void)ix2a.at(4, 2);
    (void)ix2ac.at(4, 2);

    (void)ix2a.at(3);
    (void)ix2ac.at(3);

    (void)ix2a[4][4];
    (void)ix2ac[4][4];

    (void)ix2a.front();
    (void)ix2a.back();
    (void)ix2ac.front();
    (void)ix2ac.back();

    (void)ix2a.dimension0();
    (void)ix2a.dimension1();
    (void)ix2a.size();
    (void)ix2a.empty();
    (void)ix2a.max_size();

    (void)ix2a.begin();
    (void)ix2a.end();
    (void)ix2ac.begin();
    (void)ix2ac.end();

    (void)ix2a.rbegin();
    (void)ix2a.rend();
    (void)ix2ac.rbegin();
    (void)ix2ac.rend();

	(void)ix2a.data();


	for(ss_size_t i = 0; i < ix2a.dimension0(); ++i)
	{
		for(ss_size_t j = 0; j < ix2a.dimension1(); ++j)
		{
			ss_sint16_t	x	=	static_cast<ss_sint16_t>((i << 8) | j);

			ix2a[i][j] = x;
		}
	}

	printf("2d:\n");
	for_each(ix2ac.begin(), ix2ac.end(), print_sint16);
	printf("\n2d:\n");
	for_each(ix2ac.rbegin(), ix2ac.rend(), print_sint16);
	printf("\n");
#endif /* _STLSOFT_INCL_H_STLSOFT_FRAME_ARRAY */
}

void func3D_frame()
{
#ifdef _STLSOFT_INCL_H_STLSOFT_FRAME_ARRAY
	typedef static_array_3d	<	ss_sint32_t
							,	2
							,	3
							,	5
							,	do_construction<ss_sint32_t>
							>	int_fixed_array_3D;

	int_fixed_array_3D			ix3a;
	int_fixed_array_3D const	&ix3ac	=	ix3a;

    (void)ix3a.at(1, 2, 4);
    (void)ix3ac.at(1, 2, 4);

    (void)ix3a.at(1);
    (void)ix3ac.at(1);

    (void)ix3a[1][2][4];
    (void)ix3ac[1][2][4];

    (void)ix3a.front();
    (void)ix3a.back();
    (void)ix3ac.front();
    (void)ix3ac.back();

    (void)ix3a.dimension0();
    (void)ix3a.dimension1();
    (void)ix3a.size();
    (void)ix3a.empty();
    (void)ix3a.max_size();

    (void)ix3a.begin();
    (void)ix3a.end();
    (void)ix3ac.begin();
    (void)ix3ac.end();

    (void)ix3a.rbegin();
    (void)ix3a.rend();
    (void)ix3ac.rbegin();
    (void)ix3ac.rend();

	(void)ix3a.data();


	for(ss_size_t i = 0; i < ix3a.dimension0(); ++i)
	{
		for(ss_size_t j = 0; j < ix3a.dimension1(); ++j)
		{
			for(ss_size_t k = 0; k < ix3a.dimension2(); ++k)
			{
				ss_sint32_t	x	=	static_cast<ss_sint32_t>((i << 16) | (j << 8) | k);

				ix3a[i][j][k] = x;
			}
		}
	}

	printf("3d:\n");
	for_each(ix3ac.begin(), ix3ac.end(), print_sint24);
	printf("\n3d:\n");
	for_each(ix3ac.rbegin(), ix3ac.rend(), print_sint24);
	printf("\n");
#endif /* _STLSOFT_INCL_H_STLSOFT_FRAME_ARRAY */
}

void func4D_frame()
{
#ifdef _STLSOFT_INCL_H_STLSOFT_FRAME_ARRAY
	typedef static_array_4d	<	ss_uint32_t
#if 0
							,	2
							,	3
							,	5
							,	7
#else
							,	2
							,	4
							,	6
							,	8
#endif /* 0 */
							,	do_construction<ss_uint32_t>
							>		array_t;

	ss_uint32_t		i;
	array_t			array1;
	array_t const	&array2	=	array1;

	array_t::iterator	begin	=	array1.begin();
	array_t::iterator	end		=	array1.end();

	for(i = 0; begin != end; ++begin, ++i)
	{
		*begin = i;
	}

	printf("4d:\n[");
	for(ss_size_t i0 = 0; i0 < array2.dimension0(); ++i0)
	{
		for(ss_size_t i1 = 0; i1 < array2.dimension1(); ++i1)
		{
			for(ss_size_t i2 = 0; i2 < array2.dimension2(); ++i2)
			{
				for(ss_size_t i3 = 0; i3 < array2.dimension3(); ++i3)
				{
					ss_uint32_t	value	=	array2[i0][i1][i2][i3];

					printf("%u ", value);
				}
			}
		}
	}
	printf("]\n\n");

#endif /* _STLSOFT_INCL_H_STLSOFT_FRAME_ARRAY */
}

////////////////////////////////////////////////////////////////////////////////
