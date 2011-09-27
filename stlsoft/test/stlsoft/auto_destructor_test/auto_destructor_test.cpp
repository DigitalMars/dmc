/* /////////////////////////////////////////////////////////////////////////////
 * File:        auto_destructor_test.cpp
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
#include <stlsoft_auto_destructor.h>

stlsoft_ns_using(auto_array_destructor)
stlsoft_ns_using(auto_destructor)

////////////////////////////////////////////////////////////////////////////////
// Typedefs

class Class
{
public:
	Class()
	{
		printf("Class(0x%08p)\n", this);
	}
	~Class()
	{
		printf("~Class(0x%08p)\n", this);
	}
};

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char ** /*argv*/)
{
	auto_destructor<Class>			adc(new Class);
	auto_destructor<char>			ad(new char);
	auto_array_destructor<char>		aad(new char[10]);
	auto_destructor<char>			ad1(new char);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
