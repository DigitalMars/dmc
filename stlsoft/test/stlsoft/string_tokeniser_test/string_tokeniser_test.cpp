/* /////////////////////////////////////////////////////////////////////////////
 * File:        string_tokeniser_test.cpp
 *
 * Purpose:
 *
 * Created:
 * Updated:     19th June 2004
 *
 * Author:      Matthew Wilson
 *
 * Copyright:   Synesis Software Pty Ltd, (c) 2002. All rights reserved.
 *
 * Home:        www.stlsoft.org
 *
 * ////////////////////////////////////////////////////////////////////////// */


// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#ifdef __BORLANDC__
#pragma warn -8008
#pragma warn -8066
#endif /* __BORLANDC__ */

#include <stdio.h>
#include <wchar.h>

#include <stlsoft.h>

#if !defined(_STLSOFT_VER_1_7_1)
# define STLSOFT_TEMPLATE_SPECIALISATION	template <>
#endif

stlsoft_ns_using(ss_bool_t)

#include <stlsoft_simple_algorithms.h>

#include <stlsoft_string_access.h>

stlsoft_ns_using(c_str_ptr)

#include <stlsoft_string_tokeniser.h>

stlsoft_ns_using(string_tokeniser)
stlsoft_ns_using(string_tokeniser_ignore_blanks)
stlsoft_ns_using(string_tokeniser_type_traits)
stlsoft_ns_using(string_tokeniser_comparator)

#if _STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_MAJOR < 3 && \
    _STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_MINOR < 5
//# error This file now only works with string_tokeniser 2.5 and above
#endif /* 2.5 */

#include <string>
#include <functional>
#include <algorithm>

stlsoft_ns_using_std(string)

#include <vector>

stlsoft_ns_using_std(vector)
stlsoft_ns_using_std(wstring)

#ifndef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
// #error This can only work when member template functions are supported
#endif /* !__STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */

////////////////////////////////////////////////////////////////////////////////
// Typedefs

#if !defined(_STLSOFT_VER_1_6_3) || \
	_STLSOFT_VER < _STLSOFT_VER_1_6_3
typedef string_tokeniser_type_traits<string>			type_traits_a_t;
#else
typedef string_tokeniser_type_traits<string, string>	type_traits_a_t;
#endif /* _STLSOFT_VER_1_6_3 */

typedef string_tokeniser<   string
                        ,   char
                        ,   string_tokeniser_ignore_blanks<true>
#if defined(_STLSOFT_VER_1_6_3)
                        ,   string
#endif /* _STLSOFT_VER_1_6_3 */
                        ,   type_traits_a_t
                        ,   string_tokeniser_comparator<char, string, type_traits_a_t >
                        >       tokeniser_char_a_T;
typedef string_tokeniser<   string
                        ,   char
                        ,   string_tokeniser_ignore_blanks<false>
#if defined(_STLSOFT_VER_1_6_3)
                        ,   string
#endif /* _STLSOFT_VER_1_6_3 */
                        ,   type_traits_a_t
                        ,   string_tokeniser_comparator<char, string, type_traits_a_t >
                        >       tokeniser_char_a_F;

typedef string_tokeniser<   string
                        ,   string
                        ,   string_tokeniser_ignore_blanks<true>
#if defined(_STLSOFT_VER_1_6_3)
                        ,   string
#endif /* _STLSOFT_VER_1_6_3 */
                        ,   type_traits_a_t
                        ,   string_tokeniser_comparator<string, string, type_traits_a_t >
                        >       tokeniser_string_a_T;
typedef string_tokeniser<   string
                        ,   string
                        ,   string_tokeniser_ignore_blanks<false>
#if defined(_STLSOFT_VER_1_6_3)
                        ,   string
#endif /* _STLSOFT_VER_1_6_3 */
                        ,   type_traits_a_t
                        ,   string_tokeniser_comparator<string, string, type_traits_a_t >
                        >   tokeniser_string_a_F;

////////////////////////////////////////////////////////////////////////////////

template <ss_typename_param_k T>
struct dump_token
    : public stlsoft_ns_qual_std(unary_function)<const ss_typename_type_k T::value_type&, void>
{
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
	template <ss_typename_param_k V>
    void operator ()(V const &value)
    {
		printf_value(c_str_ptr(value));
    }
#else
    void operator ()(ss_typename_type_k T::value_type const &value)
    {
        fprintf(stdout, "[%s]", c_str_ptr(value));
    }
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
protected:
	void printf_value(char const *v)
	{
        fprintf(stdout, "[%s]", v);
	}
	void printf_value(wchar_t const *v)
	{
        fwprintf(stdout, L"[%s]", v);
	}
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
};

////////////////////////////////////////////////////////////////////////////////
// Functions

template <ss_typename_param_k C, ss_typename_param_k D, ss_bool_t B>
inline void test_tokeniser(const C **begin, const C **end, const D &delimiter)
{
	typedef	stlsoft_ns_qual_std(basic_string)<C>	string_t;
    typedef string_tokeniser<   string_t
                            ,   D
                            ,   string_tokeniser_ignore_blanks<B>
#if !defined(_STLSOFT_VER_1_6_3) || \
	_STLSOFT_VER < _STLSOFT_VER_1_6_3
                            ,   string_tokeniser_type_traits<string_t>
#else
                            ,   string_t
                            ,   string_tokeniser_type_traits<string_t, string_t>
#endif /* _STLSOFT_VER_1_6_3 */
                            ,   string_tokeniser_comparator<	D
															,	string_t
#if !defined(_STLSOFT_VER_1_6_3) || \
	_STLSOFT_VER < _STLSOFT_VER_1_6_3
															,   string_tokeniser_type_traits<string_t>
#else
								                            ,   string_tokeniser_type_traits<string_t, string_t>
#endif /* _STLSOFT_VER_1_6_3 */
															>
                            >						tokeniser_t;

    for(; begin != end; ++begin)
    {
        tokeniser_t     tokens(*begin, delimiter);
		size_t			size	=	tokens.size();

#if !defined(__DMC__) && \
	(	!defined(__STLSOFT_COMPILER_IS_MSVC) || \
		_MSC_VER > 1200)
		printf("Number of items: %d\n", stlsoft_ns_qual_std(distance)(tokens.begin(), tokens.end()));
#endif /* compiler */

		// This is a disgusting hack, Apologies all round!
		if(sizeof(C) == sizeof(wchar_t))
			wprintf(L"  \"%s\":\n    ", reinterpret_cast<wchar_t const*>(*begin));
		else
			printf("  \"%s\":\n    ", reinterpret_cast<char const*>(*begin));
#if	!defined(__STLSOFT_COMPILER_IS_MSVC) || \
	_MSC_VER > 1200
# if 1
        stlsoft_ns_qual_std(for_each)(tokens.begin(), tokens.end(), dump_token<tokeniser_t>());
# else
        stlsoft_ns_qual(for_each_postinc)(tokens.begin(), tokens.end(), dump_token<tokeniser_t>());
# endif /* 0 */
        printf("\n");

#ifndef __DMC__
		if(tokens.size() != static_cast<size_t>(stlsoft_ns_qual_std(distance)(tokens.begin(), tokens.end())))
		{
			fprintf(stderr, "implementation of size() is erroneous!!\n");
		}
#endif /* compiler */
#endif /* compiler */
    }
}

////////////////////////////////////////////////////////////////////////////////
// Source data

static char const   *strings[] =
{
        ";x;"
    ,   "<>x<>"
    ,   ""
    ,   ";"
    ,   ";;;;;;;;;;"
    ,   ";a;b"
    ,   "a;b;"
    ,   "a;b"
    ,   ";a;b;"
    ,   "x"
    ,   ";x"
    ,   "x;"
    ,   ";x;"
    ,   "x;;"
    ,   ";;x"
    ,   ";;x;;"
    ,   ";%BIN%;b2;;;WBEM;blah;;;bfg"
    ,   "<><>abc<sdfsdf<>sdfs><><><><>sdfsdf<>s<><><>"
    ,   "<>"
    ,   "<><>"
    ,   "x<><>"
    ,   "<>x<>"
    ,   "<><>x"
    ,   "x;;;;;"
    ,
};

static wchar_t const   *wstrings[] =
{
        L";x;"
    ,   L"<>x<>"
    ,   L""
    ,   L";"
    ,   L";;;;;;;;;;"
    ,   L";a;b"
    ,   L"a;b;"
    ,   L"a;b"
    ,   L";a;b;"
    ,   L"x"
    ,   L";x"
    ,   L"x;"
    ,   L";x;"
    ,   L"x;;"
    ,   L";;x"
    ,   L";;x;;"
    ,   L";%BIN%;b2;;;WBEM;blah;;;bfg"
    ,   L"<><>abc<sdfsdf<>sdfs><><><><>sdfsdf<>s<><><>"
    ,   L"<>"
    ,   L"<><>"
    ,   L"x<><>"
    ,   L"<>x<>"
    ,   L"<><>x"
    ,   L"x;;;;;"
    ,
};

////////////////////////////////////////////////////////////////////////////////
// main

//#include <windows.h>

int main(int /* argc */, char ** /* argv */)
{
//	::Sleep(100000);

	// ANSI

    {   // With a character
        const char		delimiter   =   ';';

        printf("\nIgnoring blanks (delimiter: '%c'):\n", delimiter);
        test_tokeniser<char, char, true>(strings, strings + stlsoft_num_elements(strings), delimiter);
        printf("\nNot ignoring blanks (delimiter: '%c'):\n", delimiter);
        test_tokeniser<char, char, false>(strings, strings + stlsoft_num_elements(strings), delimiter);
    }

#ifndef __STLSOFT_COMPILER_IS_DMC
    {   // With a string
        const string    delimiter   =   "<>";

        printf("\nIgnoring blanks (delimiter: \"%s\"):\n", c_str_ptr(delimiter));
        test_tokeniser<char, string, true>(strings, strings + stlsoft_num_elements(strings), delimiter);
        printf("\nNot ignoring blanks (delimiter: \"%s\"):\n", c_str_ptr(delimiter));
        test_tokeniser<char, string, false>(strings, strings + stlsoft_num_elements(strings), delimiter);
    }
#endif /* !__STLSOFT_COMPILER_IS_DMC */

	// Unicode

#if !defined(__STLSOFT_COMPILER_IS_MSVC) && \
    (   !defined(__STLSOFT_COMPILER_IS_GCC) || \
	    __GCC__ >= 3)
    {   // With a character
        const wchar_t	delimiter   =   L';';

        wprintf(L"\nIgnoring blanks (delimiter: '%c'):\n", delimiter);
        test_tokeniser<wchar_t, wchar_t, true>(wstrings, wstrings + stlsoft_num_elements(wstrings), delimiter);
        wprintf(L"\nNot ignoring blanks (delimiter: '%c'):\n", delimiter);
        test_tokeniser<wchar_t, wchar_t, false>(wstrings, wstrings + stlsoft_num_elements(wstrings), delimiter);
    }

#ifdef __STLSOFT_COMPILER_IS_DMC
    {   // With a string (unicode)
        const wstring    delimiter   =   L"<>";

        wprintf(L"\nIgnoring blanks (delimiter: \"%s\"):\n", c_str_ptr(delimiter));
        test_tokeniser<wchar_t, wstring, true>(wstrings, wstrings + stlsoft_num_elements(wstrings), delimiter);
        wprintf(L"\nNot ignoring blanks (delimiter: \"%s\"):\n", c_str_ptr(delimiter));
        test_tokeniser<wchar_t, wstring, false>(wstrings, wstrings + stlsoft_num_elements(wstrings), delimiter);
    }
#endif /* !__STLSOFT_COMPILER_IS_DMC */
#endif /* !__STLSOFT_COMPILER_IS_MSVC */

#ifndef _STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_MAJOR
# define _STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_MAJOR		STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_MAJOR
#endif
#ifndef _STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_MINOR
# define _STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_MINOR		STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_MINOR
#endif
#ifndef _STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_REVISION
# define _STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_REVISION	STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_REVISION
#endif
#ifndef _STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_EDIT
# define _STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_EDIT		STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_EDIT
#endif

	printf("\n_MSC_VER:  %d\n", _MSC_VER);
	printf("STLSoft:   %d.%d.%d\n", _STLSOFT_VER_MAJOR, _STLSOFT_VER_MINOR, _STLSOFT_VER_REVISION);
	printf("tokeniser: %d.%d.%d.%d\n"
		,	_STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_MAJOR
		,	_STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_MINOR
		,	_STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_REVISION
		,	_STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_EDIT);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
