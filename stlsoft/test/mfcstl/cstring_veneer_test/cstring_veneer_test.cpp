/* /////////////////////////////////////////////////////////////////////////////
 * File:        cstring_veneer_test.cpp
 *
 * Purpose:     Implementation file for the cstring_veneer_test project.
 *
 * Created:     28th January 2003
 * Updated:     22nd April 2004
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

#include <mfcstl.h>
#include <mfcstl_cstring_veneer.h>

mfcstl_ns_using(cstring_veneer)

/* ////////////////////////////////////////////////////////////////////////// */

template <ss_typename_param_k T>
void use_string(T const &s)
{
	printf("%u %u %s %s\n", s.length(), s.size(), s.empty() ? "true" : "false", s.c_str());
}

int main(int /* argc */, char ** /*argv*/)
{
	puts("cstring_veneer_test: " __STLSOFT_COMPILER_LABEL_STRING);

	/* . */
	cstring_veneer	s1("String #1");
	cstring_veneer	s2("String #2", 9);
	cstring_veneer	s3("String #2", (int)0);
	cstring_veneer	s4(s3);
	cstring_veneer	s5;

	use_string(s1);
	use_string(s2);
	use_string(s3);
	use_string(s4);
	use_string(s5);

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
