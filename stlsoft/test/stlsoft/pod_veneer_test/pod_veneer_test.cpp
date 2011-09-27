/* /////////////////////////////////////////////////////////////////////////////
 * File:        pod_veneer_test.cpp
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
#include <string.h>

#include <stlsoft.h>

#include <stlsoft_pod_veneer.h>

stlsoft_ns_using(pod_veneer)

////////////////////////////////////////////////////////////////////////////////
// Typedefs

struct Simple
{
public:
	char	*buffer;
};

static void Init_Simple(Simple *simple);
static void Uninit_Simple(Simple *simple);
static void Set_Simple(Simple *simple, char const *contents);
static void Print_Simple(Simple *simple);

struct Simple_Init { void operator ()(Simple *simple) { Init_Simple(simple); } };
struct Simple_Uninit { void operator ()(Simple *simple) { Uninit_Simple(simple); } };

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char * /* argv */[])
{
	typedef	pod_veneer<Simple, Simple_Init, Simple_Uninit>	Simple_raii_t;

	Simple_raii_t	simple;

	Set_Simple(&simple, "We're applying the \"Resource Acquisition Is Initialisation\" mechanism to Plain Old Data!");

	Print_Simple(&simple);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Functions

void Init_Simple(Simple *simple)
{
	simple->buffer = 0;
}

void Uninit_Simple(Simple *simple)
{
	free(simple->buffer);
}

void Set_Simple(Simple *simple, char const *contents)
{
	free(simple->buffer);
	simple->buffer = strdup(contents);
}

void Print_Simple(Simple *simple)
{
	printf("Simple(%08x): \"%s\"\n", simple, simple->buffer);
}

////////////////////////////////////////////////////////////////////////////////
