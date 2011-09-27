/* /////////////////////////////////////////////////////////////////////////////
 * File:		listbox_sequence_test.cpp
 *
 * Purpose:
 *
 * Created:		11th December 2002
 * Updated:		22nd April 2004
 *
 * Copyright:	Synesis Software Pty Ltd, (c) 2002. All rights reserved.
 *
 * Home:		www.winstl.org
 *
 * ////////////////////////////////////////////////////////////////////////// */


// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <winstl.h>

#include <stdio.h>

#include <winstl_listbox_sequence.h>

#include <winstl_windows_type_conversions.h>

winstl_ns_using(StrT2LPARAM)

#include <stlsoft_string_access.h>

stlsoft_ns_using (c_str_ptr)

#include <algorithm>

winstl_ns_using_std(for_each)

#include <string>

winstl_ns_using_std(string)

////////////////////////////////////////////////////////////////////////////////

struct dump_string
{
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
	template <ss_typename_param_k T>
	void operator ()(T const &s)
#else
	void operator ()(string const &s)
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */
	{
		printf("  %s\n", static_cast<const char*>(c_str_ptr(s)));
	}
};

int main(int /* argc */, char ** /*argv*/)
{
	typedef winstl_ns_qual(listbox_sequence)<string>	listbox_sequence;

	HWND	hwndListBox	=	::CreateWindowExA(0, "LISTBOX", "", LBS_SORT, 0, 0, 0, 0, 0, 0, 0, 0);

	int		iItem;

	iItem = SendMessage(hwndListBox, LB_ADDSTRING, 0, StrT2LPARAM("String 3"));
	SendMessage(hwndListBox, LB_SETITEMDATA, iItem, 3);
	iItem = SendMessage(hwndListBox, LB_ADDSTRING, 0, StrT2LPARAM("String 2"));
	SendMessage(hwndListBox, LB_SETITEMDATA, iItem, 2);
	iItem = SendMessage(hwndListBox, LB_ADDSTRING, 0, StrT2LPARAM("String 45"));
	SendMessage(hwndListBox, LB_SETITEMDATA, iItem, 45);
	iItem = SendMessage(hwndListBox, LB_ADDSTRING, 0, StrT2LPARAM("String 1"));
	SendMessage(hwndListBox, LB_SETITEMDATA, iItem, 1);

	listbox_sequence					lbs(hwndListBox);

	printf("Forward:\n");
	for_each(lbs.begin(), lbs.end(), dump_string());

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
	printf("Backward:\n");
	for_each(lbs.rbegin(), lbs.rend(), dump_string());
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

	printf("Indexed:\n");
	for(size_t i = 0; i < lbs.size(); ++i)
	{
		dump_string()(lbs[i]);
	}

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
