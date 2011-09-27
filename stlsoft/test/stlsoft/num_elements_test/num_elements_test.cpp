/* /////////////////////////////////////////////////////////////////////////////
 * File:        num_elements_test.cpp
 *
 * Purpose:     Testing the stlsoft_num_elements() smart-macro.
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

// Enable the following testing symbols to see how the compilers catch the errors

//#define   TEST_ERROR_0    // All compilers should catch this one
//#define   TEST_ERROR_1    // Only Borland, Comeau, Intel, Metrowerks and VC.NET catch this one
#define   TEST_ERROR_2    // Only Borland, Comeau, Intel, Metrowerks and VC.NET catch this one

#include <stlsoft.h>

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Typedefs

template <ss_typename_param_k T>
struct vect
{
    T &operator [](size_t index)
    {
        return *static_cast<T*>(0);
    }

    T *m_values;
};

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char* /* argv */[])
{
    int                 ai[20];
#ifdef TEST_ERROR_0
    int                 i;
#endif // TEST_ERROR_0
#ifdef TEST_ERROR_1
    int                 val =   32;
    int                 *pi =   &val;
#endif // TEST_ERROR_1
#ifdef TEST_ERROR_2
    vect<int>           vi;
	int					_ai[stlsoft_num_elements(vi)]; STLSOFT_SUPPRESS_UNUSED(_ai); STLSOFT_SUPPRESS_UNUSED(vi);
#endif // TEST_ERROR_2

    fprintf(stdout, "Number of elements in ai: %u\n", stlsoft_num_elements(ai));
#ifdef TEST_ERROR_0
    fprintf(stdout, "Number of elements in i: %u\n", stlsoft_num_elements(i));
#endif // TEST_ERROR_0
#ifdef TEST_ERROR_1
    fprintf(stdout, "Number of elements in pi: %u\n", stlsoft_num_elements(pi));
#endif // TEST_ERROR_1
#ifdef TEST_ERROR_2
    fprintf(stdout, "Number of elements in vi: %u\n", stlsoft_num_elements(vi));
#endif // TEST_ERROR_2

	STLSOFT_SUPPRESS_UNUSED(ai);

	int	a1[1];
	int	a2[2];
	int	a3[3];
	int	a4[4];
	int	a5[5];
	int	a6[6];
	int	a7[7];
	int	a8[8];
	int	a9[9];
	int	a10[10];
	int	a11[11];
	int	a12[12];
	int	a13[13];
	int	a14[14];
	int	a15[15];
	int	a16[16];
	int	a17[17];

	stlsoft_static_assert(stlsoft_num_elements(a1) == 1);
	stlsoft_static_assert(stlsoft_num_elements(a2) == 2);
	stlsoft_static_assert(stlsoft_num_elements(a3) == 3);
	stlsoft_static_assert(stlsoft_num_elements(a4) == 4);
	stlsoft_static_assert(stlsoft_num_elements(a5) == 5);
	stlsoft_static_assert(stlsoft_num_elements(a6) == 6);
	stlsoft_static_assert(stlsoft_num_elements(a7) == 7);
	stlsoft_static_assert(stlsoft_num_elements(a8) == 8);
	stlsoft_static_assert(stlsoft_num_elements(a9) == 9);
	stlsoft_static_assert(stlsoft_num_elements(a10) == 10);
	stlsoft_static_assert(stlsoft_num_elements(a11) == 11);
	stlsoft_static_assert(stlsoft_num_elements(a12) == 12);
	stlsoft_static_assert(stlsoft_num_elements(a13) == 13);
	stlsoft_static_assert(stlsoft_num_elements(a14) == 14);
	stlsoft_static_assert(stlsoft_num_elements(a15) == 15);
	stlsoft_static_assert(stlsoft_num_elements(a16) == 16);
	stlsoft_static_assert(stlsoft_num_elements(a17) == 17);

	STLSOFT_SUPPRESS_UNUSED(a1);
    STLSOFT_SUPPRESS_UNUSED(a2);
    STLSOFT_SUPPRESS_UNUSED(a3);
    STLSOFT_SUPPRESS_UNUSED(a4);
    STLSOFT_SUPPRESS_UNUSED(a5);
    STLSOFT_SUPPRESS_UNUSED(a6);
    STLSOFT_SUPPRESS_UNUSED(a7);
    STLSOFT_SUPPRESS_UNUSED(a8);
    STLSOFT_SUPPRESS_UNUSED(a9);
    STLSOFT_SUPPRESS_UNUSED(a10);
    STLSOFT_SUPPRESS_UNUSED(a11);
    STLSOFT_SUPPRESS_UNUSED(a12);
    STLSOFT_SUPPRESS_UNUSED(a13);
    STLSOFT_SUPPRESS_UNUSED(a14);
    STLSOFT_SUPPRESS_UNUSED(a15);
    STLSOFT_SUPPRESS_UNUSED(a16);
    STLSOFT_SUPPRESS_UNUSED(a17);

#ifdef TEST_ERROR_1
    return pi != NULL;
#else
    return 0;
#endif // TEST_ERROR_1
}

////////////////////////////////////////////////////////////////////////////////
