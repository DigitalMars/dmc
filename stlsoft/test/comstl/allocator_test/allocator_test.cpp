/* /////////////////////////////////////////////////////////////////////////////
 * File:        allocator_test.cpp
 *
 * Purpose:     Implementation file for the allocator_test project.
 *
 * Created:     11th February 2003
 * Updated:     30th May 2004
 *
 * Status:      Wizard-generated
 *
 * License:     (Licensed under the Synesis Software Open License)
 *
 *              Copyright (C) 1999-2003, Synesis Software Pty Ltd.
 *              All rights reserved.
 *
 *              www:        http://www.synesis.com.au/software
 *
 *              email:      software@synesis.com.au
 *
 *              This source code is placed into the public domain 2003
 *              by Synesis Software Pty Ltd. There are no restrictions
 *              whatsoever to your use of the software.
 *
 *              This source code is provided by Synesis Software Pty Ltd "as is"
 *              and any warranties, whether expressed or implied, including, but
 *              not limited to, the implied warranties of merchantability and
 *              fitness for a particular purpose are disclaimed. In no event
 *              shall the Synesis Software Pty Ltd be liable for any direct,
 *              indirect, incidental, special, exemplary, or consequential
 *              damages (including, but not limited to, procurement of
 *              substitute goods or services; loss of use, data, or profits; or
 *              business interruption) however caused and on any theory of
 *              liability, whether in contract, strict liability, or tort
 *              (including negligence or otherwise) arising in any way out of
 *              the use of this software, even if advised of the possibility of
 *              such damage.
 *
 *              Neither the name of Synesis Software Pty Ltd nor the names of
 *              any subdivisions, employees or agents of Synesis Software Pty
 *              Ltd, nor the names of any other contributors to this software
 *              may be used to endorse or promote products derived from this
 *              software without specific prior written permission.
 *
 * ////////////////////////////////////////////////////////////////////////// */


// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <stlsoft.h>
#include <comstl.h>
#include <comstl_task_allocator.h>

comstl_ns_using(task_allocator)

////////////////////////////////////////////////////////////////////////////////
// Functions

template <class A, class C, class SZ>
void test_allocator(A &a, const C *allocator_name, SZ cb)
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
	/* . */
	typedef	task_allocator<char>	char_allocator_t;

	char_allocator_t						ator;

	test_allocator(ator, "comstl::task_allocator<char>", 100);
	test_allocator(ator, "comstl::task_allocator<char>", 10000);
	test_allocator(ator, "comstl::task_allocator<char>", 1000000);
	test_allocator(ator, "comstl::task_allocator<char>", 100000000);
	test_allocator(ator, "comstl::task_allocator<char>", 1000000000);
	test_allocator(ator, "comstl::task_allocator<char>", 2000000000);
	test_allocator(ator, "comstl::task_allocator<char>", 4000000000);

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
