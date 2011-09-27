////////////////////////////////////////////////////////////////////////////////
// File:	string_access_test.cpp
//
// Purpose:	Test program for ATLSTL string access
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

#include <atlbase.h>

extern CComModule	_Module;

#include <atlwin.h>

#include <atlstl.h>
#include <atlstl_string_access.h>

stlsoft_ns_using(c_str_ptr)
stlsoft_ns_using(c_str_len)

#include <string>

atlstl_ns_using_std(wstring)

////////////////////////////////////////////////////////////////////////////////
// Forward declarations
//
// To appease the mightly CodeWarrior

void dump_string_(wchar_t const *s, int cch);

////////////////////////////////////////////////////////////////////////////////
// Functions

void dump_string_(char const *s, int /* cch */)
{
	puts(s);
}

void dump_string_(wchar_t const *s, int cch)
{
	// This function prints the string to stdout. It also converts it to ANSI for
	// no better reason than that I didn't want to mess around in CodeWarrior
	// searching for the implementation to _putws(). Nevertheless, it illustrates
	// that the strings were correctly converted via the generalising shims and
	// the template function dump_string.

	char	sz[2001];
	int		cchConv	=	::WideCharToMultiByte(0, 0, s, cch, sz, atlstl_num_elements(sz) - 1, NULL, NULL);

	sz[cchConv] = '\0';

	puts(sz);
}

template <typename S>
void dump_string(S const &s)
{
	// This function demonstrates the generalised string access facilitated
	// by use of the Access Shim c_str_ptr() and Attribute Shims. For a
	// definition of Shims consult
	//
	//		http:stlsoft.org/white_papers.html#white_papers.html
	//

	dump_string_(c_str_ptr(s), c_str_len(s));
}

////////////////////////////////////////////////////////////////////////////////
// main()

int main(int /* argc */, char* /* argv */[])
{
	CComBSTR		bstr(L"[CComBSTR]");
	wstring	        wstr(L"[std::wstring]");
	wchar_t const	_w_str[] = L"[C-style Unicode string]";
	CWindow			wnd(::CreateWindow("BUTTON", "The button text", 0, 0, 0, 10, 10, NULL, NULL, NULL, NULL));

	dump_string(bstr);
	dump_string(wstr);
	dump_string(_w_str);
	dump_string(wnd);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
