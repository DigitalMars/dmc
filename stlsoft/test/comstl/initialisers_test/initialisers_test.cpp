/* /////////////////////////////////////////////////////////////////////////////
 * file:        initialisers_test.cpp
 *
 * Purpose:     Implementation file for the initialisers_test project.
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

#define	_WIN32_DCOM

#include <stdio.h>

#include <stlsoft.h>

#include <comstl.h>
#include <comstl_initialisers.h>

comstl_ns_using(com_initialiser)
comstl_ns_using(ole_initialiser)

/* ////////////////////////////////////////////////////////////////////////// */

int main(int /* argc */, char ** /*argv*/)
{
	// This is defined and used simply to avoid link hassles with the various compilers.
	static const CLSID _CLSID_StdGlobalInterfaceTable = { 0x00000323, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 }};

	{
		LPUNKNOWN	punk;
		HRESULT		hr	=	::CoCreateInstance(_CLSID_StdGlobalInterfaceTable, NULL, CLSCTX_ALL, IID_IUnknown, reinterpret_cast<void**>(&punk));

		if(FAILED(hr))
		{
			if(hr == CO_E_NOTINITIALIZED)
			{
				printf("Com libraries not initialised\n");
			}
			else
			{
				printf("Failed to create object\n");
			}
		}
		else
		{
			printf("Object allocated successfully\n");

			punk->Release();
		}
	}

	{
#ifdef __COMSTL_CF_DCOM_SUPPORT
		com_initialiser	coinit(COINIT_APARTMENTTHREADED);
#else // __COMSTL_CF_DCOM_SUPPORT
		com_initialiser	coinit;
#endif // __COMSTL_CF_DCOM_SUPPORT
		LPUNKNOWN		punk;
		HRESULT			hr	=	::CoCreateInstance(_CLSID_StdGlobalInterfaceTable, NULL, CLSCTX_ALL, IID_IUnknown, reinterpret_cast<void**>(&punk));

		if(FAILED(hr))
		{
			if(hr == CO_E_NOTINITIALIZED)
			{
				printf("Com libraries not initialised\n");
			}
			else
			{
				printf("Failed to create object\n");
			}
		}
		else
		{
			printf("Object allocated successfully\n");

			punk->Release();
		}
	}

	{
		ole_initialiser	coinit;
		LPUNKNOWN		punk;
		HRESULT			hr	=	::CoCreateInstance(_CLSID_StdGlobalInterfaceTable, NULL, CLSCTX_ALL, IID_IUnknown, reinterpret_cast<void**>(&punk));

		if(FAILED(hr))
		{
			if(hr == CO_E_NOTINITIALIZED)
			{
				printf("Com libraries not initialised\n");
			}
			else
			{
				printf("Failed to create object\n");
			}
		}
		else
		{
			printf("Object allocated successfully\n");

			punk->Release();
		}
	}

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
