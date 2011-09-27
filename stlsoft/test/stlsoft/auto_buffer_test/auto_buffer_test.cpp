/* /////////////////////////////////////////////////////////////////////////////
 * File:        auto_buffer_test.cpp
 *
 * Purpose:     Demonstrates the auto_buffer class
 *
 * Created:
 * Updated:     1st July 2004
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
#include <string.h>

#define	_STLSOFT_AUTO_BUFFER_ALLOW_NON_POD

#include <stlsoft_constraints.h>			// Include the STLSoft constraints header

#include <stlsoft.h>
#include <stlsoft_malloc_allocator.h>		/* stlsoft::malloc_allocator */

stlsoft_ns_using(malloc_allocator)

#include <stlsoft_new_allocator.h>			/* stlsoft::new_allocator */

stlsoft_ns_using(new_allocator)

#include <stlsoft_null_allocator.h>			/* stlsoft::null_allocator */

stlsoft_ns_using(null_allocator)

#ifdef _AUTO_BUFFER_TEST_USE_STD_ALLOCATOR
# undef _AUTO_BUFFER_TEST_USE_STD_ALLOCATOR
#endif /* _AUTO_BUFFER_TEST_USE_STD_ALLOCATOR */

#if !defined(__GNUC__) && \
    !defined(__WATCOMC__)
# define _AUTO_BUFFER_TEST_USE_STD_ALLOCATOR
#endif /* !__GNUC__ && !__WATCOMC__ */


#ifdef _AUTO_BUFFER_TEST_USE_STD_ALLOCATOR
#ifndef __DMC__								/* std::allocator */
# include <memory>
#else /* ? __DMC__ */
# include <stl_config.h>						/* alloc */
# include <stl_alloc.h>
#endif /* __DMC__ */

stlsoft_ns_using_std(allocator)

#endif /* _AUTO_BUFFER_TEST_USE_STD_ALLOCATOR */
#include <stlsoft_auto_buffer.h>			/* stlsoft::auto_buffer */

stlsoft_ns_using(auto_buffer)

#ifdef _MSC_VER
# include <crtdbg.h>
#endif /* _MSC_VER */

#ifndef __WATCOMC__
# include <algorithm>
#endif /* !__WATCOMC__ */

////////////////////////////////////////////////////////////////////////////////
// Typedefs

// Enable/disable the following members of MyClass to see how this affects
// whether the type can be used to paramterise auto_buffer
struct MyClass
{
public:
#if 1
	MyClass()
	{
	}
#endif /* 1 */
#if 0
	MyClass(int x)
	{
	}
#endif /* 0 */
#if 0
	~MyClass()
	{
	}
#endif /* 0 */

#if 0
	operator =(const MyClass &x);
#endif /* 0 */
};

////////////////////////////////////////////////////////////////////////////////
// main

static void print_char(char const &ch)
{
	printf("[%c] ", ch);
}

//extern "C" void __stdcall Sleep(unsigned);

int main(int /* argc */, char ** /*argv*/)
{
//	::Sleep(100000);

	{
		auto_buffer<char, malloc_allocator<char>, 8>		buffer1(16);
		auto_buffer<char, new_allocator<char>, 64>			buffer2(32);
#ifdef _AUTO_BUFFER_TEST_USE_STD_ALLOCATOR
# ifndef __DMC__
		auto_buffer<char, allocator<char>, 8192 >			buffer3(9000);
# else /* ? __DMC__ */
		auto_buffer<char, alloc>							buffer3(9000);
# endif /* __DMC__ */
#endif /* _AUTO_BUFFER_TEST_USE_STD_ALLOCATOR */
		auto_buffer<char, new_allocator<char>, 64>			buffer4(0);

		auto_buffer<MyClass, malloc_allocator<MyClass>, 8>	buffer_MyClass(16);

		// There are four changes possible:
		//
		// 1. Expansion within the internal buffer
		// 2. Contraction within the internal buffer
		// 3. Expansion from the internal buffer to an allocated buffer
		// 4. Contraction from an allocated buffer to the internal buffer
		// 5. Expansion from the allocated buffer to another allocated buffer
		// 6. Contraction from the allocated buffer to another allocated buffer

		// 1. Expansion within the internal buffer
		buffer2.resize(buffer2.size() + 1);		memset(buffer2, 1, buffer2.size());
		buffer2.resize(64);						memset(buffer2, 2, buffer2.size());
		buffer2.resize(0);						memset(buffer2, 3, buffer2.size());

		// 3. Expansion from the internal buffer to an allocated buffer
		buffer2.resize(65);						memset(buffer2, 4, buffer2.size());

		// 5. Expansion from the allocated buffer to another allocated buffer
		buffer2.resize(128);					memset(buffer2, 5, buffer2.size());

		// 6. Contraction from the allocated buffer to another allocated buffer
		buffer2.resize(65);						memset(buffer2, 6, buffer2.size());

		// 4. Contraction from an allocated buffer to the internal buffer
		buffer2.resize(64);						memset(buffer2, 7, buffer2.size());

		// 2. Contraction within the internal buffer
		buffer2.resize(32);						memset(buffer2, 8, buffer2.size());

		buffer1.resize(buffer1.size() + 1);
		buffer1.resize(buffer1.size() - 1);

		strcpy(buffer1, "A bit of string");
		strcpy(buffer2, "A bigger bit of string");
#ifdef _AUTO_BUFFER_TEST_USE_STD_ALLOCATOR
		strcpy(buffer3, "An absolutely huge amount of string");
		buffer3.resize(1 + strlen(buffer3));
#endif /* _AUTO_BUFFER_TEST_USE_STD_ALLOCATOR */

		printf("Buffer 1: %s\n", (const char*)buffer1);
		printf("Buffer 2: %s\n", (const char*)buffer2);
#ifdef _AUTO_BUFFER_TEST_USE_STD_ALLOCATOR
		printf("Buffer 3: %s\n", (const char*)buffer3);
#endif /* _AUTO_BUFFER_TEST_USE_STD_ALLOCATOR */

#ifndef __WATCOMC__
		stlsoft_ns_qual_std(for_each)(buffer3.begin(), buffer3.end(), print_char);		puts("");
		stlsoft_ns_qual_std(for_each)(buffer3.rbegin(), buffer3.rend(), print_char);	puts("");
#endif /* !__WATCOMC__ */
	}

#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif /* _MSC_VER */

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
