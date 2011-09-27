/* /////////////////////////////////////////////////////////////////////////////
 * File:        fixed_substring_test.cpp
 *
 * Purpose:     Implementation file for the fixed_substring_test project.
 *
 * Created:     1st May 2003
 * Updated:     17th May 2004
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

#include <windows.h>
#include <stdio.h>

#include <stlsoft.h>
#include <stlsoft_null_allocator.h>
#include <stlsoft_fixed_substring.h>
#include <stlsoft_simple_string.h>

#include <winstl.h>
#include <winstl_string_access.h>

#include <winstl_performance_counter.h>

winstl_ns_using(performance_counter)


/* ////////////////////////////////////////////////////////////////////////// */

/* ////////////////////////////////////////////////////////////////////////// */

int main(int /* argc */, char ** /*argv*/)
{
//    performance_counter counter;

	puts("fixed_substring_test: " __STLSOFT_COMPILER_LABEL_STRING);

	/* . */
	typedef	stlsoft::basic_simple_string<char>		string_t;
	typedef	stlsoft::basic_fixed_substring<char>	seg_string_t;

	string_t		s("Hello Natty!");
	seg_string_t	ss("Hello Natty!");
	seg_string_t	ss1(&s[6], 6);
	seg_string_t	ss2(&s[0], 5);

	int				i = ss1 < ss2;

	stlsoft_assert(0 == seg_string_t("abc", 3).compare("abc"));
	stlsoft_assert(0 == seg_string_t("abcdef", 3).compare(0, 3, "abc"));

	stlsoft_assert(0 == seg_string_t("Well done Matty!", 16).compare(11, 5, ss, 7, 5));

	string_t		s2(ss1.begin(), ss1.end());




	puts(ss.c_str());
	puts(ss1.c_str());

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
