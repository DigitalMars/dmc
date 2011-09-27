/* /////////////////////////////////////////////////////////////////////////////
 * File:        functionals_test.cpp
 *
 * Purpose:     Implementation file for the functionals_test project.
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

#include <stlsoft.h>
#include <comstl.h>
#include <comstl_initialisers.h>
#include <comstl_functionals.h>

#include <algorithm>
#include <vector>

/* ////////////////////////////////////////////////////////////////////////// */

int main(int /* argc */, char ** /*argv*/)
{
	// This is defined and used simply to avoid link hassles with the various compilers.
	static const CLSID _CLSID_StdGlobalInterfaceTable = { 0x00000323, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 }};

	/* Interface functions. */
#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
	_MSC_VER < 1100
	typedef comstl_ns_qual_std(vector)<LPUNKNOWN, comstl_ns_qual(allocator)<LPUNKNOWN> >	unk_vector_t;
#else
	typedef comstl_ns_qual_std(vector)<LPUNKNOWN>											unk_vector_t;
#endif /* __STLSOFT_COMPILER_IS_MSVC && _MSC_VER < 1100 */

	comstl_ns_qual(com_initialiser)	coinit;
	unk_vector_t					items;
	size_t							i;

	for(i = 0; i < 10; ++i)
	{
		LPUNKNOWN	punk;
		HRESULT		hr	=	::CoCreateInstance(_CLSID_StdGlobalInterfaceTable, NULL, CLSCTX_ALL, IID_IUnknown, reinterpret_cast<void**>(&punk));

		if(SUCCEEDED(hr))
		{
			items.push_back(punk);
		}
	}

	// Add ref on all of the items
	comstl_ns_qual_std(for_each)(items.begin(), items.end(), comstl_ns_qual(interface_addref)<IUnknown>());

	// release on all of the items
#ifndef __DMC__ // Digital Mars does not yet support the way std_mem_fun_t works
	for(i = 0; i < items.size(); ++i)
	{
		comstl_ns_qual(std_mem_fun_ref_t)<ULONG, IUnknown>	addref(&IUnknown::AddRef);
		comstl_ns_qual(std_mem_fun_ref_t)<ULONG, IUnknown>	release(&IUnknown::Release);

		IUnknown	&unk	=	*items[i];

		addref(unk);
		release(unk);
	}

	comstl_ns_qual_std(for_each)(items.begin(), items.end(), comstl_ns_qual(std_mem_fun)(&IUnknown::Release));
#endif /* !__DMC__ */

	// release on all of the items
	comstl_ns_qual_std(for_each)(items.begin(), items.end(), comstl_ns_qual(interface_release)<IUnknown>());

	/* Interface functions. */
#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
	_MSC_VER < 1100
	typedef comstl_ns_qual_std(vector)<void*, comstl_ns_qual(allocator)<void*> >	ptr_vector_t;
#else
	typedef comstl_ns_qual_std(vector)<void*>										ptr_vector_t;
#endif /* __STLSOFT_COMPILER_IS_MSVC && _MSC_VER < 1100 */

	ptr_vector_t	pointers;

	pointers.push_back(::CoTaskMemAlloc(100));
	pointers.push_back(::CoTaskMemAlloc(100));
	pointers.push_back(::CoTaskMemAlloc(100));

	comstl_ns_qual_std(for_each)(pointers.begin(), pointers.end(), comstl_ns_qual(task_memory_close)());

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
