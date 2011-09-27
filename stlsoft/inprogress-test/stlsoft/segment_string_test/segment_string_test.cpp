/* /////////////////////////////////////////////////////////////////////////////
 * File:        fixed_substring_test.cpp
 *
 * Purpose:     Implementation file for the fixed_substring_test project.
 *
 * Created:     1st May 2003
 * Updated:     1st May 2003
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
#include <stlsoft_simple_string.h>

#include <winstl.h>

#include <winstl_performance_counter.h>

winstl_ns_using(performance_counter)


/* ////////////////////////////////////////////////////////////////////////// */

using namespace stlsoft;

template<	ss_typename_param_k	C
		,	ss_typename_param_k	T = char_traits<C>
		>
class fixed_substring
{
public:
    /// The value type
    typedef C                           value_type;
	/// The traits type
	typedef	T							traits_type;
    /// The allocator type
    typedef null_allocator<C>			allocator_type;
	/// The current parameterisation of the type
	typedef fixed_substring<C, T>		class_type;
	/// The character type
    typedef value_type                  char_type;
    /// The pointer type
    typedef value_type                  *pointer;
    /// The non-mutable (const) pointer type
    typedef value_type const            *const_pointer;
    /// The reference type
    typedef value_type                  &reference;
    /// The non-mutable (const) reference type
    typedef value_type const            &const_reference;
    /// The size type
    typedef ss_size_t                   size_type;
    /// The difference type
    typedef ss_ptrdiff_t                difference_type;

/// Construction
/// @{
public:
	fixed_substring(char_type const *begin, char_type const *end);
//	fixed_substring(char_type const *p, char_type const *end);

};


/* ////////////////////////////////////////////////////////////////////////// */

int main(int /* argc */, char ** /*argv*/)
{
    performance_counter counter;

	puts("fixed_substring_test: " __STLSOFT_COMPILER_LABEL_STRING);

	/* . */
	typedef	stlsoft::basic_simple_string<char>	string_t;
	typedef	fixed_substring<char>				seg_string_t;


    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
