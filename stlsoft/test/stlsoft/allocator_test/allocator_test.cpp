/* /////////////////////////////////////////////////////////////////////////////
 * File:        allocator_test.cpp
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

#include <stlsoft_malloc_allocator.h>       /* stlsoft::malloc_allocator */

stlsoft_ns_using(malloc_allocator)

#include <stlsoft_new_allocator.h>          /* stlsoft::new_allocator */

stlsoft_ns_using(new_allocator)

#ifdef _ALLOCATOR_TEST_USE_STD_ALLOCATOR
# undef _ALLOCATOR_TEST_USE_STD_ALLOCATOR
#endif /* _ALLOCATOR_TEST_USE_STD_ALLOCATOR */

#if !defined(__GNUC__) && \
    !defined(__WATCOMC__)
# define _ALLOCATOR_TEST_USE_STD_ALLOCATOR
#endif /* !__GNUC__ && !__WATCOMC__ */

#ifdef _ALLOCATOR_TEST_USE_STD_ALLOCATOR
//#ifndef __DMC__                             /* std::allocator */
#include <memory>
//#else
//#include <stl_config.h>                     /* alloc */
//#include <stl_alloc.h>
//#endif /* __DMC__ */

stlsoft_ns_using_std(allocator)

#endif /* _ALLOCATOR_TEST_USE_STD_ALLOCATOR */

////////////////////////////////////////////////////////////////////////////////
// Constants

const size_t    CB_SMALL            =   100;
const size_t    CB_MEDIUM           =   1000000;
const size_t    CB_LARGE            =   1000000000;

////////////////////////////////////////////////////////////////////////////////
// Typedefs

typedef struct _xyz
{
    unsigned long   len;
    char            *buf;

    ~_xyz()
    {
        printf("~xyz()\n");
    }

} xyz;

////////////////////////////////////////////////////////////////////////////////
// Function declarations

static void test_xyz();  // Don't elicit warning from Metrowerks

////////////////////////////////////////////////////////////////////////////////
// Functions

template <class A>
static void test_allocator(A &a, const char *allocator_name, size_t cb)
{
#ifdef __STLSOFT_CF_THROW_BAD_ALLOC
    try
    {
#endif /* __STLSOFT_CF_THROW_BAD_ALLOC */
#ifndef __DMC__
        char    *buf    =   a.allocate(cb, NULL);
#else
        char    *buf    =   a.allocate(cb);
#endif /* __DMC__ */

        if(buf != NULL)
        {
            printf("%s: %u bytes allocated successfully\n", allocator_name, cb);
        }
        else
        {
            printf("%s: %u bytes could not be allocated. allocate() returned NULL\n", allocator_name, cb);
        }
#ifdef __STLSOFT_CF_THROW_BAD_ALLOC
    }
    catch(::std::bad_alloc &x)
    {
        printf("%s: %d bytes could not be allocated. allocate() threw bad_alloc: %s\n", allocator_name, cb, x.what());
    }
#endif /* __STLSOFT_CF_THROW_BAD_ALLOC */
}

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char ** /*argv*/)
{
    malloc_allocator<char>					phac;
    new_allocator<char>						gac;
#ifdef _ALLOCATOR_TEST_USE_STD_ALLOCATOR
//#ifdef __DMC__
//    alloc                                   sac;
//#else
    allocator<char>							sac;
//#endif /* __DMC__ */
#endif /* _ALLOCATOR_TEST_USE_STD_ALLOCATOR */

    test_xyz();

    test_allocator(phac, (const char *)"malloc_allocator<char>", CB_SMALL);
    test_allocator(phac, (const char *)"malloc_allocator<char>", CB_MEDIUM);
    test_allocator(phac, (const char *)"malloc_allocator<char>", CB_LARGE);

    test_allocator(gac, (const char *)"new_allocator<char>", CB_SMALL);
    test_allocator(gac, (const char *)"new_allocator<char>", CB_MEDIUM);
    test_allocator(gac, (const char *)"new_allocator<char>", CB_LARGE);

#ifdef _ALLOCATOR_TEST_USE_STD_ALLOCATOR
    test_allocator(sac, (const char *)"allocator<char>", CB_SMALL);
    test_allocator(sac, (const char *)"allocator<char>", CB_MEDIUM);
    test_allocator(sac, (const char *)"allocator<char>", CB_LARGE);
#endif /* _ALLOCATOR_TEST_USE_STD_ALLOCATOR */

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Functions

void test_xyz()
{
    typedef new_allocator<xyz>	xyz_ator;

    xyz_ator    ator;
    xyz			*p  =   ator.allocate(3);

    ator.destroy(p);

    ator.deallocate(p);
}

////////////////////////////////////////////////////////////////////////////////
