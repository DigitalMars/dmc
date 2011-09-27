// string_access_test.cpp : Defines the entry point for the console application.
//
// Updated: 11th March 2004

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <iostream>
#include <iomanip>

#include <winstl.h>

#include <stlsoft_string_access.h>
#include <winstl_string_access.h>

stlsoft_ns_using(c_str_len)
stlsoft_ns_using(c_str_len_a)
stlsoft_ns_using(c_str_ptr)
stlsoft_ns_using(c_str_ptr_a)

#include <winstl_windows_type_conversions.h>

winstl_ns_using(StrT2LPARAM)
winstl_ns_using(HWND2UINT)

#include <string>

winstl_ns_using_std(string)

static BOOL CALLBACK EnumChildProc(HWND hwnd, char const *text)
{
	string	s		=	text;
	size_t	size	=	c_str_len(text) + c_str_len(hwnd) + c_str_len(s);
	size_t	size_a	=	c_str_len_a(text) + c_str_len_a(hwnd) + c_str_len_a(s);
//	size_t	size	=	c_str_len(text) + c_str_len(hwnd) + c_str_len(s);

//	fprintf(stdout, "Window: 0x%08x;\"%s\"\n", (UINT)HWND2UINT(hwnd), static_cast<char const *>(c_str_ptr_a(hwnd)));

	std::cout << "Windows: 0x" << std::hex << hwnd << ";\"" << /* (char const *) */c_str_ptr_a(hwnd) << "\"" << std::endl;

	STLSOFT_SUPPRESS_UNUSED(size_a);
	STLSOFT_SUPPRESS_UNUSED(size);

	return size != 0 || size_a != 0 || NULL != hwnd; // Stops compiler whinging about size not being used
}


int main(int /* argc */, char* /* argv */[])
{
	HWND	hwndDesktop	= ::GetDesktopWindow();
	HWND	hwndListBox	=	::CreateWindowExA(0, "LISTBOX", "", LBS_SORT, 0, 0, 0, 0, 0, 0, 0, 0);

	::EnumChildWindows(hwndDesktop, (WNDENUMPROC)EnumChildProc, StrT2LPARAM("Some text"));

	SendMessage(hwndListBox, LB_ADDSTRING, 0, StrT2LPARAM("String 3"));
	SendMessage(hwndListBox, LB_ADDSTRING, 0, StrT2LPARAM("String 2"));
	SendMessage(hwndListBox, LB_ADDSTRING, 0, StrT2LPARAM("String 45"));
	SendMessage(hwndListBox, LB_ADDSTRING, 0, StrT2LPARAM("String 1"));

	std::cout << "Listbox (pre-select): " << c_str_ptr_a(hwndListBox) << std::endl;

	SendMessage(hwndListBox, LB_SETCURSEL, 2, 0L);

	std::cout << "Listbox (pre-select): " << c_str_ptr_a(hwndListBox) << std::endl;

	return 0;
}
