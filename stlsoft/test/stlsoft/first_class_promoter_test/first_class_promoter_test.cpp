/* /////////////////////////////////////////////////////////////////////////////
 * File:        first_class_promoter_test.cpp
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

#include <stlsoft.h>

stlsoft_ns_using(ss_uint32_t)
stlsoft_ns_using(ss_sint32_t)
stlsoft_ns_using(ss_sint64_t)

#include <stlsoft_constraints.h>
#include <stlsoft_first_class_promoter.h>

stlsoft_ns_using(first_class_promoter)

#include <stdio.h>
#ifndef __STLSOFT_COMPILER_IS_WATCOM
#include <vector>
#endif /* __STLSOFT_COMPILER_IS_WATCOM */

// Enable/disable these to illustrate the use of the first_class_promoter
#define	_USE_PROMOTER
//#define	_SHOW_BAD

////////////////////////////////////////////////////////////////////////////////
// Forward declarations

static void test_Frequency();
static void test_Frequency2();
static void test_int();
static void test_FirstClass();

static void func(int &i);

////////////////////////////////////////////////////////////////////////////////
// Typedefs

typedef union _Win32_LARGE_INTEGER
{
    struct
	{
        ss_uint32_t LowPart;
        ss_sint32_t HighPart;
    };
    struct
	{
        ss_uint32_t LowPart;
        ss_sint32_t HighPart;
    } u;
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
    ss_sint64_t		QuadPart;
#endif // STLSOFT_CF_64BIT_INT_SUPPORT
} Win32_LARGE_INTEGER;

static int QueryPerformanceFrequency(Win32_LARGE_INTEGER * /* lpFrequency */)
{
	return 0;
}

class FirstClass
{
public:
#ifdef _SHOW_BAD
	FirstClass	&operator =(FirstClass const &rhs);
#endif /* _SHOW_BAD */

//	FirstClass(int i)
//	{}
};



class Int
#ifdef _USE_PROMOTER
	: public first_class_promoter<int>
#else
	: public int
#endif /* _USE_PROMOTER	*/
{
#ifdef _USE_PROMOTER
	typedef	first_class_promoter<int>	parent_class_type;
#else
	typedef	int							parent_class_type;
#endif /* _USE_PROMOTER	*/

public:

	Int()
	{}

	ss_explicit_k Int(int i)
		: parent_class_type(i)
	{}
};

class IntDerived
	: public Int
{
	IntDerived(int i)
		: Int(i)
	{}
};

class Frequency
#ifdef _USE_PROMOTER
	: public first_class_promoter<Win32_LARGE_INTEGER>
#else
	: public Win32_LARGE_INTEGER
#endif /* _USE_PROMOTER */
{
};

////////////////////////////////////////////////////////////////////////////////
// main

void test_Frequency()
{
	Frequency	frequency;

	::QueryPerformanceFrequency(&frequency);
}

void test_Frequency2()
{
#if 0
	typedef	Frequency								value_type;
#else
	typedef	Win32_LARGE_INTEGER						value_type;
#endif /* 0 */
#ifndef __STLSOFT_COMPILER_IS_WATCOM
	typedef	stlsoft_ns_qual_std(vector)<value_type>	frequency_vector_t;

	frequency_vector_t	frequencies;

	frequencies.push_back(value_type());
#endif /* __STLSOFT_COMPILER_IS_WATCOM */
}

void test_int()
{
	Int			i;

	func(i);
}

void test_FirstClass()
{
	first_class_promoter<FirstClass>	fc;
}

int main(int /* argc */, char* /*argv*/[])
{
#ifndef _STLSOFT_NO_NAMESPACE
	using namespace stlsoft;
#endif /* _STLSOFT_NO_NAMESPACE */

#if 0
	{
		typedef	unsigned __int8		uint8_t;

		unsigned uint8_t	u1;
		signed uint8_t		s1;

	}
#endif /* 0 */

	{
		ss_sint8_t	s8;
		ss_uint8_t	u8;
		ss_sint32_t	s32a;
		ss_sint32_t	s32b;
		ss_uint32_t	u32a;
		ss_uint32_t	u32b;

		s8 = -1;
		u8 = 0xff;

		s32a = s8;
		s32b = u8;

		u32a = s8;
		u32b = u8;

		STLSOFT_SUPPRESS_UNUSED(s32a);
		STLSOFT_SUPPRESS_UNUSED(s32b);
		STLSOFT_SUPPRESS_UNUSED(u32a);
		STLSOFT_SUPPRESS_UNUSED(u32b);
	}

	test_Frequency();
	test_Frequency2();
	test_int();
	test_FirstClass();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Functions

void func(int &i)
{
	i = 3;
}

////////////////////////////////////////////////////////////////////////////////
