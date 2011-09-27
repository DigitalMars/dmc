/* /////////////////////////////////////////////////////////////////////////////
 * File:        Array_veneer_test.cpp
 *
 * Purpose:     Implementation file for the array_veneer_test project.
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
#include <mfcstl_array_veneer.h>

mfcstl_ns_using(array_veneer)

typedef array_veneer<CPtrArray>		ptr_array_t;
typedef array_veneer<CStringArray>	string_array_t;

/* ////////////////////////////////////////////////////////////////////////// */

static void dump_element(CString const &s)
{
	fprintf(stdout, "CString: \"%s\"\n", static_cast<char const *>(s));
}

static void dump_element(void const *p)
{
	fprintf(stdout, "pointer: 0x%08x\n", p);
}

#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
       _MSC_VER > 1300
template <ss_typename_param_k A>
void use_array(A const &a)
{
	ss_typename_type_k A::size_type	size	=	a.size();

	printf("size: %u\n", size);
	printf("empty: %s\n", a.empty() ? "true" : "false");

	ss_typename_type_k A::const_iterator	begin	=	a.begin();
	ss_typename_type_k A::const_iterator	end		=	a.end();

	for(; begin != end; ++begin)
	{
		ss_typename_type_k A::value_type const	&v	=	*begin;
		int										i	=	sizeof(v);

		dump_element(v);
	}
}
#endif /* _MSC_VER != 1200 */

template <ss_typename_param_k A>
void use_array(A &a)
{
	ss_typename_type_k A::size_type	size	=	a.size();

	printf("size: %u\n", size);
	printf("empty: %s\n", a.empty() ? "true" : "false");

	ss_typename_type_k A::const_iterator	begin	=	a.begin();
	ss_typename_type_k A::const_iterator	end		=	a.end();

	for(; begin != end; ++begin)
	{
		ss_typename_type_k A::value_type const	&v	=	*begin;
		int										i	=	sizeof(v);

		dump_element(v);
	}
}

int main(int /* argc */, char ** /*argv*/)
{
	puts("array_veneer_test: " __STLSOFT_COMPILER_LABEL_STRING);

	/* . */
	ptr_array_t		par;
	string_array_t	strar;

	par.Add(0);
	par.Add(&par);
	strar.Add("String 1");
	strar.push_back("String 2");
	strar[1] = "String 2 - again";

//	strar[4] = "String 5";

	use_array(par);
	use_array(const_cast<ptr_array_t const &>(par));

	use_array(strar);
	use_array(const_cast<string_array_t const &>(strar));

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
