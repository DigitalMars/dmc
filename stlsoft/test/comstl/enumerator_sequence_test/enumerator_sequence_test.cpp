/* /////////////////////////////////////////////////////////////////////////////
 * File:        enumerator_sequence_test.cpp
 *
 * Purpose:     Implementation file for the enumerator_sequence_test project.
 *
 * Created:     2nd February 2003
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
#include <comstl_refcount_functions.h>
#include <comstl_enum_sequence.h>
#include <comstl_value_policies.h>

comstl_ns_using(cs_bool_t)
comstl_ns_using(cs_size_t)
comstl_ns_using(degenerate_cloning_policy)
comstl_ns_using(forward_cloning_policy)
comstl_ns_using(input_cloning_policy)
comstl_ns_using(enum_simple_sequence)
comstl_ns_using(GUID_policy)
comstl_ns_using(BSTR_policy)
comstl_ns_using(VARIANT_policy)
comstl_ns_using(interface_policy)
comstl_ns_using(LPUNKNOWN_policy)
comstl_ns_using(LPOLESTR_policy)

#ifdef USE_SYNESIS_COM_BASE_HEADER
 #include <MMComBsc.h>
#else

#ifndef SYFNCOMDECL
 #define SYFNCOMDECL    STDAPI
#endif /* !SYFNCOMDECL */

SYFNCOMDECL CreateEnumUnknownFromArray(LPUNKNOWN __RPC_FAR *items, size_t cItems, IEnumUnknown __RPC_FAR * __RPC_FAR *ppvEnum);
SYFNCOMDECL CreateEnumStringFromArray(OLECHAR const __RPC_FAR * const __RPC_FAR *items, size_t cItems, IEnumString __RPC_FAR * __RPC_FAR *ppvEnum);
SYFNCOMDECL CreateEnumVARIANTFromArray(VARIANT const __RPC_FAR *items, size_t cItems, IEnumVARIANT __RPC_FAR * __RPC_FAR *ppvEnum);

#endif /* USE_SYNESIS_COM_BASE_HEADER */

#include <stdio.h>

#include <algorithm>

/* ////////////////////////////////////////////////////////////////////////// */

void Trace(wchar_t const *fmt, ...)
{
	va_list	args;
	wchar_t	sz[2048];

	va_start(args, fmt);

	vwprintf(fmt, args);
	wvsprintfW(sz, fmt, args);
	OutputDebugStringW(sz);

	va_end(args);
}

/* ////////////////////////////////////////////////////////////////////////// */

class Unknown
	: public IUnknown
{
	STDMETHOD_(ULONG, AddRef)()
	{
		fprintf(stdout, "%p, AddRef()\n", this);

		return 0;
	}
	STDMETHOD_(ULONG, Release)()
	{
		fprintf(stdout, "%p, Release()\n", this);

		return 0;
	}
	STDMETHOD(QueryInterface)(REFIID , void **)
	{
		fprintf(stdout, "%p, QueryInterface()\n", this);

		return E_NOINTERFACE;
	}
};

/* ////////////////////////////////////////////////////////////////////////// */

void test_guid()
{
#ifdef USE_SYNESIS_COM_BASE_HEADER
	GUID items[5];

	items[0] = IID_IUnknown;
	items[1] = IID_IEnumVARIANT;
	items[2] = IID_IEnumUnknown;
	items[3] = IID_IStorage;
	items[4] = IID_IStream;

	IEnumGUID				*pen;
	HRESULT					hr	=	CreateEnumGUIDFromArray(items, stlsoft_num_elements(items), &pen);

	if(SUCCEEDED(hr))
	{
		typedef enum_simple_sequence<IEnumGUID, GUID, GUID_policy, GUID const &, forward_cloning_policy<IEnumGUID>, 2>	enum_sequence_t;

		enum_sequence_t				egs(pen, true);
		enum_sequence_t::iterator	begin	=	egs.begin();
		enum_sequence_t::iterator	end		=	egs.end();

		for(; begin != end; ++begin)
		{
			enum_sequence_t::reference_type value	=	*begin;
			LPOLESTR						str;

			hr = ::StringFromCLSID(value, &str);
			if(SUCCEEDED(hr))
			{
				Trace(L" O: %s\n", str);

				::CoTaskMemFree(str);
			}

			for(enum_sequence_t::iterator it = begin; it != end; ++it)
			{
				enum_sequence_t::reference_type value	=	*it;
				LPOLESTR						str;

				hr = ::StringFromCLSID(value, &str);
				if(SUCCEEDED(hr))
				{
					Trace(L" O: %s\n", str);

					::CoTaskMemFree(str);
				}
			}
		}

		pen->Release();
	}
#endif /* USE_SYNESIS_COM_BASE_HEADER */
}

void test_bstr()
{
#ifdef USE_SYNESIS_COM_BASE_HEADER
	LPCOLESTR	items[5] =
	{
			L"string 1"
		,	L"string 2"
		,	L"string 3"
		,	L"string 4"
		,	L"string 5"
	};

	IEnumBSTR	*pen;
	HRESULT		hr	=	CreateEnumBSTRFromArray(items, stlsoft_num_elements(items), &pen);

	if(SUCCEEDED(hr))
	{
		typedef enum_simple_sequence<IEnumBSTR, BSTR, BSTR_policy, BSTR, input_cloning_policy<IEnumBSTR>, 2>	enum_sequence_t;

		enum_sequence_t				egs(pen, true);
		enum_sequence_t::iterator	begin	=	egs.begin();
		enum_sequence_t::iterator	end		=	egs.end();

		for(; begin != end; ++begin)
		{
			enum_sequence_t::reference_type value	=	*begin;

			Trace(L" O: %s\n", value);

			for(enum_sequence_t::iterator it = begin; it != end; ++it)
			{
				enum_sequence_t::reference_type value	=	*it;

				Trace(L"  I: %s\n", value);
			}
		}

		pen->Release();
	}
#endif /* USE_SYNESIS_COM_BASE_HEADER */
}

void test_string()
{
	LPCOLESTR	items[5] =
	{
			L"string 1"
		,	L"string 2"
		,	L"string 3"
		,	L"string 4"
		,	L"string 5"
	};

	IEnumString	*pen;
	HRESULT		hr	=	CreateEnumStringFromArray(items, stlsoft_num_elements(items), &pen);

	if(SUCCEEDED(hr))
	{
		typedef enum_simple_sequence<IEnumString, LPOLESTR, LPOLESTR_policy, LPCOLESTR, degenerate_cloning_policy<IEnumString>, 2>	enum_sequence_t;

		enum_sequence_t				egs(pen, true);
		enum_sequence_t::iterator	begin	=	egs.begin();
		enum_sequence_t::iterator	end		=	egs.end();

		for(; begin != end; ++begin)
		{
			enum_sequence_t::reference_type value	=	*begin;

			Trace(L" O: %s\n", value);

			for(enum_sequence_t::iterator it = begin; it != end; ++it)
			{
				enum_sequence_t::reference_type value	=	*it;

				Trace(L"  I: %s\n", value);
			}
		}

		pen->Release();
	}
}

void test_variant()
{
	VARIANT	items[5];

	for(int i = 0; i < stlsoft_num_elements(items); ++i)
	{
		VariantInit(&items[i]);
	}

	items[0].vt			=	VT_BOOL;
	items[0].boolVal	=	VARIANT_FALSE;

	items[1].vt			=	VT_I4;
	items[1].lVal		=	12345;

	items[2].vt			=	VT_R8;
	items[2].dblVal		=	12345.6789;

	items[3].vt			=	VT_I2;
	items[3].iVal		=	32767;

	items[4].vt			=	VT_BOOL;
	items[4].boolVal	=	VARIANT_TRUE;

	IEnumVARIANT	*pen;
	HRESULT			hr	=	CreateEnumVARIANTFromArray(items, stlsoft_num_elements(items), &pen);

	if(SUCCEEDED(hr))
	{
		typedef enum_simple_sequence<IEnumVARIANT, VARIANT, VARIANT_policy, VARIANT const &, forward_cloning_policy<IEnumVARIANT>, 2>	enum_sequence_t;

		enum_sequence_t				egs(pen, true);
		enum_sequence_t::iterator	begin	=	egs.begin();
		enum_sequence_t::iterator	end		=	egs.end();

		for(; begin != end; ++begin)
		{
			enum_sequence_t::reference_type value	=	*begin;
			VARIANT							var;

			::VariantInit(&var);
			::VariantChangeType(&var, const_cast<VARIANT*>(&value), VARIANT_ALPHABOOL, VT_BSTR);

			Trace(L" O: [%d, %s]\n", value.vt, var.bstrVal);

			for(enum_sequence_t::iterator it = begin; it != end; ++it)
			{
				enum_sequence_t::reference_type value	=	*it;
				VARIANT							var;

				::VariantInit(&var);
				::VariantChangeType(&var, const_cast<VARIANT*>(&value), VARIANT_ALPHABOOL, VT_BSTR);

				Trace(L"  I: [%d, %s]\n", value.vt, var.bstrVal);

				::VariantClear(&var);
			}

			::VariantClear(&var);
		}

		pen->Release();
	}

}

void test_unknown()
{
	class Unknown	_items[5];
	LPUNKNOWN		items[5] =
	{
			&_items[0]
		,	&_items[1]
		,	&_items[2]
		,	&_items[3]
		,	&_items[4]
	};

	IEnumUnknown	*pen;
	HRESULT			hr	=	CreateEnumUnknownFromArray(items, stlsoft_num_elements(items), &pen);

	if(SUCCEEDED(hr))
	{
		typedef enum_simple_sequence<IEnumUnknown, LPUNKNOWN, interface_policy<IUnknown>, LPUNKNOWN, degenerate_cloning_policy<IEnumUnknown>, 1>	enum_sequence_t;

		enum_sequence_t				egs(pen, true);
		enum_sequence_t::iterator	begin	=	egs.begin();
		enum_sequence_t::iterator	end		=	egs.end();

		for(; begin != end; ++begin)
		{
			enum_sequence_t::reference_type value	=	*begin;

			Trace(L" O: %p\n", value);

			for(enum_sequence_t::iterator it = begin; it != end; ++it)
			{
				enum_sequence_t::reference_type value	=	*it;

				Trace(L"  I: %p\n", value);
			}
		}

		pen->Release();
	}
}

int main(int /* argc */, char ** /*argv*/)
{
	comstl_ns_qual(com_initialiser)	coinit;

	long			*pl		=	0;
	long volatile	*pvl		=	0;
	double			*pd		=	stlsoft::sap_cast<double*>(pl);
	double const	*pcd	=	stlsoft::sap_cast<double const*>(pl);

//	pd		=	stlsoft::sap_cast<double*>(pvl);
	pcd		=	stlsoft::sap_cast<double const*>(pvl);

	test_guid();
	test_bstr();
	test_string();
	test_variant();
	test_unknown();

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
