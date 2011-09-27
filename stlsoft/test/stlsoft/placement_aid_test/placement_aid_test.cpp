/* /////////////////////////////////////////////////////////////////////////////
 * File:        placement_aid_test.cpp
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
#include <stlsoft_placement_aid.h>

stlsoft_ns_using(placement_aid)

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Typedefs

class X
{
public:
	X()
		: m_i0(99)
		, m_i1(77)
		, m_i2(55)
	{
		printf("X()\n");
	}
	~X()
	{
		printf("~X()\n");
	}

public:
	int	m_i0;
	int	m_i1;
	int	m_i2;
};

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char* /* argv */[])
{
	struct { char _[sizeof(X)]; }	_;

	placement_aid<X>	_x(&_);
	X					&x	=	_x;

	(void)x.m_i0;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
