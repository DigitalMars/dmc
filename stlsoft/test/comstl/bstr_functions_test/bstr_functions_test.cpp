/* /////////////////////////////////////////////////////////////////////////////
 * File:        bstr_functions_test.cpp
 *
 * Purpose:     Implementation file for the bstr_functions_test project.
 *
 * Created:     17th February 2003
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

#include <stlsoft.h>

#include <comstl.h>
#include <comstl_bstr_functions.h>

comstl_ns_using(bstr_create_a)
comstl_ns_using(bstr_create_w)
comstl_ns_using(bstr_create)
comstl_ns_using(bstr_dup)
comstl_ns_using(bstr_destroy)

#include <stdio.h>
#include <wchar.h>

/* ////////////////////////////////////////////////////////////////////////// */

int main(int /* argc */, char ** /*argv*/)
{
	puts("bstr_functions_test: " __STLSOFT_COMPILER_LABEL_STRING);

	BSTR s1	=	bstr_create_a("String #1");
	BSTR s2	=	bstr_create_w(L"String #2");
	BSTR s3	=	bstr_create("String #3");
	BSTR s4	=	bstr_create(L"String #4");
	BSTR s5	=	bstr_dup(s2);
	BSTR s6	=	bstr_dup(s3);

	fputws(s1, stdout);
	fputws(s2, stdout);
	fputws(s3, stdout);
	fputws(s4, stdout);
	fputws(s5, stdout);
	fputws(s6, stdout);

	bstr_destroy(s1);
	bstr_destroy(s2);
	bstr_destroy(s3);
	bstr_destroy(s4);
	bstr_destroy(s5);
	bstr_destroy(s6);

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
