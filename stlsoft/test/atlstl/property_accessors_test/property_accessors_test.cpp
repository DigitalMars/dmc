////////////////////////////////////////////////////////////////////////////////
// File:	property_accessors_test.cpp
//
// Purpose:	Test program for ATLSTL string access property accessors.
//
// Created:	20th November 2002
// Updated:	30th May 2004
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <atlstl.h>
#include <atlstl_property_method_helpers.h>

atlstl_ns_using(get_MemberValue)
atlstl_ns_using(put_MemberValue)

#include <string>

atlstl_ns_using_std(wstring)

#if _ATL_VER < 0x0700
#include <atlimpl.cpp>
#endif /* _ATL_VER < 0x0700 */

////////////////////////////////////////////////////////////////////////////////
// Classes

class SimpleServer
{
public:
	SimpleServer()
		: m_bstrUserName("[No username]")
		, m_logonKey(-1)
	{}

// Property accessors
public:
	STDMETHODIMP SimpleServer::get_UserName(/* [retval][out] */ BSTR *pVal)
	{
		return get_MemberValue(this, pVal, &SimpleServer::m_bstrUserName);
	}

	STDMETHODIMP SimpleServer::put_UserName(/* [in] */ BSTR newVal)
	{
		return put_MemberValue(this, newVal, &SimpleServer::m_bstrUserName);
	}

	STDMETHODIMP SimpleServer::get_LogonKey(/* [retval][out] */ short *pVal)
	{
		return get_MemberValue(this, pVal, &SimpleServer::m_logonKey);
	}

	STDMETHODIMP SimpleServer::put_LogonKey(/* [in] */ short newVal)
	{
		return put_MemberValue(this, newVal, &SimpleServer::m_logonKey);
	}

// Members
protected:
	CComBSTR	m_bstrUserName;
	short		m_logonKey;
};

////////////////////////////////////////////////////////////////////////////////

// Non-threadsafe Unicode-to-ANSI just for this test program
char const *U2A(wchar_t const *s)
{
	static char	s_sz[201];
	int			cch;

	cch = ::WideCharToMultiByte(0, 0, s, -1, s_sz, atlstl_num_elements(s_sz) - 1, NULL, NULL);

	s_sz[cch + 1] = '\0';

	return s_sz;
}

////////////////////////////////////////////////////////////////////////////////
// main()

int main(int /* argc */, char* /* argv */[])
{
	SimpleServer	simpleserver;
	HRESULT			hr;

	// LogonKey
	short	logonKey;

	hr = simpleserver.get_LogonKey(&logonKey);
	if(SUCCEEDED(hr))
	{
		printf("Original LogonKey: %d\n", logonKey);
	}

	printf("Updating LogonKey: 12\n");
	if(SUCCEEDED(hr))
	{
		simpleserver.put_LogonKey(12);
	}

	hr = simpleserver.get_LogonKey(&logonKey);
	if(SUCCEEDED(hr))
	{
		printf("Updated LogonKey:  %d\n", logonKey);
	}

	// UserName
	BSTR	bstrUserName;

	simpleserver.get_UserName(NULL);

	hr = simpleserver.get_UserName(&bstrUserName);
	if(SUCCEEDED(hr))
	{
		printf("Original UserName: %s\n", U2A(bstrUserName));
		::SysFreeString(bstrUserName);
	}

	CComBSTR	bstrNewUserName("Fatboy Slim");

	printf("Updating UserName: %s\n", U2A(bstrNewUserName));
	if(SUCCEEDED(hr))
	{
		simpleserver.put_UserName(CComBSTR("Fatboy Slim"));
	}

	hr = simpleserver.get_UserName(&bstrUserName);
	if(SUCCEEDED(hr))
	{
		printf("Updated UserName:  %s\n", U2A(bstrUserName));
		::SysFreeString(bstrUserName);
	}

	return !SUCCEEDED(hr);
}

////////////////////////////////////////////////////////////////////////////////
