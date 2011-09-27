/* /////////////////////////////////////////////////////////////////////////////
 * File:        string_access_test.cpp
 *
 * Purpose:     Implementation file for the string_access_test project.
 *
 * Created:     10th February 2003
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
#include <stlsoft_string_access.h>

#include <comstl.h>

#include <comstl_initialisers.h>

comstl_ns_using(com_initialiser)

#include <comstl_string_access.h>

stlsoft_ns_using(c_str_ptr)
stlsoft_ns_using(c_str_ptr_a)
stlsoft_ns_using(c_str_ptr_w)

#include <stdio.h>
#include <wchar.h>

#include <iostream>
//#include <iomanip>

/* ////////////////////////////////////////////////////////////////////////// */

int main(int /* argc */, char ** /*argv*/)
{
	/* . */
	VARIANT			var1;
	VARIANT			var2;
	VARIANT			var3;
	VARIANT			var4;
	com_initialiser	coinit;

	::VariantInit(&var1);
	::VariantInit(&var2);

	var1.vt			=	VT_I4;
	var1.lVal		=	12345;

	var2.vt			=	VT_BSTR;
	var2.bstrVal	=	::SysAllocString(L"<BSTR> value");

	var3.vt			=	VT_R8;
	var3.dblVal		=	12345.6789;

	var4.vt			=	VT_BOOL;
	var4.boolVal	=	VARIANT_TRUE;

	//
#if !defined(__STLSOFT_COMPILER_IS_MWERKS)
	_putws(c_str_ptr(var1));
	_putws(c_str_ptr(var2));
	_putws(c_str_ptr(var3));
	_putws(c_str_ptr(var4));
#endif /* !__MWERKS__ */
	puts(c_str_ptr_a(IID_IUnknown));
#if !defined(__STLSOFT_COMPILER_IS_MWERKS)
	_putws(c_str_ptr_w(IID_IUnknown));
#endif /* !__MWERKS__ */

	std::cout << "VARIANT #1: " << c_str_ptr_a(var1) << std::endl;
	std::cout << "VARIANT #2: " << c_str_ptr_a(var2) << std::endl;
	std::cout << "VARIANT #3: " << c_str_ptr_a(var3) << std::endl;
	std::cout << "VARIANT #4: " << c_str_ptr_a(var4) << std::endl;

	std::wcout << L"GUID_NULL: " << c_str_ptr(GUID_NULL) << std::endl;

	::VariantClear(&var1);
	::VariantClear(&var2);

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
