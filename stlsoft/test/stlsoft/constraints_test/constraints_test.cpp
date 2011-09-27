/* /////////////////////////////////////////////////////////////////////////////
 * File:        constraints_test.cpp
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

//#define _STLSOFT_NO_NAMESPACE

#include <stlsoft.h>
#include <stlsoft_constraints.h>

#include <string>

// Enable the following to show how Complex and std::string violate the
// type_is_non_class_or_trivial_class constraint
//#define _CT_SHOW_BAD

////////////////////////////////////////////////////////////////////////////////
// Typedefs

struct Simple
{
};

struct Intermediate
{
    Intermediate()
    {}
};

struct Complex
{
    Complex()
    {}

    Complex(Complex const & /* rhs */)
    {}

    Complex &operator =(Complex const & /* rhs */)
    {
        return *this;
    }
};

////////////////////////////////////////////////////////////////////////////////

class base
{};

class derived
	: public base
{};

class not_derived
{};

////////////////////////////////////////////////////////////////////////////////

struct subs
{
public:
	char operator [](size_t ) const
	{
		return 0;
	}
};

struct not_subs
{};

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char* /* argv */[])
{
#if 0
	stlsoft_constraint_must_have_base(derived, base);
	stlsoft_constraint_must_have_base(not_derived, base);
#endif /* 0 */

#if 0
	stlsoft_constraint_must_be_subscriptable(subs);
	stlsoft_constraint_must_be_subscriptable(char*);
	stlsoft_constraint_must_be_subscriptable(not_subs);
#endif /* 0 */

#if 0
    stlsoft_constraint_must_be_pod(int);
    stlsoft_constraint_must_be_pod(Simple);
    stlsoft_constraint_must_be_pod(Intermediate);
#ifdef _CT_SHOW_BAD
    stlsoft_constraint_must_be_pod(Complex);
    stlsoft_constraint_must_be_pod(string);
#endif /* _CT_SHOW_BAD */

	stlsoft_constraint_must_be_pod(char);
	stlsoft_constraint_must_be_pod(short);
	stlsoft_constraint_must_be_pod(long);
#endif /* 0 */

	stlsoft_constraint_must_be_same_size(signed char, unsigned char);
	stlsoft_constraint_must_be_same_size(signed short, unsigned short);
	stlsoft_constraint_must_be_same_size(signed int, unsigned int);
	stlsoft_constraint_must_be_same_size(signed long, unsigned long);

//	stlsoft_constraint_must_be_same_size(signed char, unsigned long);
	stlsoft_constraint_must_be_same_size(void, char);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
