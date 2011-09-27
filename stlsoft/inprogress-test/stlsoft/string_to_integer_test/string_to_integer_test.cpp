// string_to_integer_test.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <stdio.h>

#include <stlsoft.h>
#include <winstl.h>

#include "str2i.h"

#include <winstl_performance_counter.h>

using ::winstl::performance_counter;

int main(int /* argc */, char* /* argv */[])
{
	printf("Timing ...\n");

	performance_counter::interval_type	str2i;
	performance_counter::interval_type	a2l;
	performance_counter::interval_type	str2l;
	performance_counter					counter;
	int									i;
	int									iRet;

	const int							ITERATIONS	=	1000000;

	for(counter.start(), i = 0; i < ITERATIONS; ++i)
	{
		long	l1	=	string_to_integer("1234567");
		long	l2	=	string_to_integer("-121");
		long	l3	=	string_to_integer("+-121");

		iRet = l1 + l2 + l3;
	}
	counter.stop();
	str2i = counter.get_milliseconds();

	for(counter.start(), i = 0; i < ITERATIONS; ++i)
	{
		long	l1	=	atol("1234567");
		long	l2	=	atol("-121");
		long	l3	=	atol("+-121");

		iRet = l1 + l2 + l3;
	}
	counter.stop();
	a2l = counter.get_milliseconds();

	for(counter.start(), i = 0; i < ITERATIONS; ++i)
	{
		long	l1	=	strtol("1234567", NULL, 0);
		long	l2	=	strtol("-121", NULL, 0);
		long	l3	=	strtol("+-121", NULL, 0);

		iRet = l1 + l2 + l3;
	}
	counter.stop();
	str2l = counter.get_milliseconds();

	fprintf(stderr, "str2i: %I64d; a2l: %I64d; str2l: %I64d\n", str2i, a2l, str2l);

	return iRet;
}
