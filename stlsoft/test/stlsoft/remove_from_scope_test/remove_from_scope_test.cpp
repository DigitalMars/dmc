/* /////////////////////////////////////////////////////////////////////////////
 * File:        remove_from_scope_test.cpp
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
#include <stlsoft_remove_from_scope.h>

stlsoft_ns_using(remove_from_scope)

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char* /* argv */[])
{
	remove_from_scope	v1;
	remove_from_scope	v2;

// Enable the following to demonstrate the inaccessibility of the various
// operations

//	remove_from_scope	v3(v1);

//	v1 = v2;

//	&v1;

//	v1.operator ->();

//	!v1;

//	if(v2)	{}

//	reinterpret_cast<void*>(&v1);

//	int &i	=	v1;

//	reinterpret_cast<int&>(v1);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
