/*
 * Updated: 22nd April 2004
 */

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stlsoft.h>

#if defined(__STLSOFT_COMPILER_IS_GCC) && \
	(__GNUC__ < 3)
# define SPST_NO_UNICODE
#endif /* compiler */


#include <wchar.h>

#include <stdio.h>

#include <winstl_searchpath_sequence.h>

winstl_ns_using(searchpath_sequence_a)
#ifndef SPST_NO_UNICODE
winstl_ns_using(searchpath_sequence_w)
#endif /* SPST_NO_UNICODE */

#include <stlsoft_simple_algorithms.h>

stlsoft_ns_using(remove_duplicates_from_unordered_sequence)

#include <winstl_findfile_sequence.h>
#include <winstl_functionals.h>

winstl_ns_using(compare_path)

#include <stlsoft_string_access.h>
#include <winstl_string_access.h>

stlsoft_ns_using(c_str_ptr)

#include <algorithm>

#include <string>

typedef stlsoft_ns_qual_std(string)		string_a;
#if !defined(__STLSOFT_COMPILER_IS_GCC) || \
	__GNUC__ >= 3
typedef stlsoft_ns_qual_std(wstring)	string_w;
#endif /* !__STLSOFT_COMPILER_IS_GCC || __GNUC__ >= 3 */

#include <vector>

#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
 typedef stlsoft_ns_qual_std(vector)<string_a>	string_vector_a_t;
#else
 typedef stlsoft_ns_qual_std(vector)<	string_a
									,	allocator<string_a>
									>			string_vector_a_t;
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */

#ifndef SPST_NO_UNICODE
# ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
  typedef stlsoft_ns_qual_std(vector)<string_w>	string_vector_w_t;
# else
 typedef stlsoft_ns_qual_std(vector)<	string_w
									,	allocator<string_w>
									>			string_vector_w_t;
# endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
#endif /* SPST_NO_UNICODE */

struct dump_item
{
public:
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
	template <typename T>
	void operator ()(T const &t)
	{
		stlsoft_ns_using(c_str_ptr)

		dump(c_str_ptr(t));
	}
#else
	void operator ()(char const *s)
	{
		dump(s);
	}
	void operator ()(wchar_t const *s)
	{
		dump(s);
	}
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */

// Implementation
protected:
	void dump(char const *s)
	{
		printf(" %s\n", s);
	}
	void dump(wchar_t const *s)
	{
		wprintf(L" %s\n", s);
	}
};

int main()
{
	{ // ANSI, indexed
		searchpath_sequence_a	searchpaths;

		for(size_t i = 0; i < searchpaths.size(); ++i)
		{
			printf("path %u: %s\n", i, searchpaths[i]);
		}
	}


	{ // ANSI, forward
		searchpath_sequence_a	searchpaths;

		printf("\nSearch paths (forward, ANSI):\n");
		stlsoft_ns_qual_std(for_each)(searchpaths.begin(), searchpaths.end(), dump_item());
	}


#ifndef SPST_NO_UNICODE
	{ // Unicode, reverse
		searchpath_sequence_w	searchpaths;

		printf("\nSearch paths (backward, Unicode):\n");
		stlsoft_ns_qual_std(for_each)(searchpaths.rbegin(), searchpaths.rend(), dump_item());

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
		searchpath_sequence_w::const_reverse_iterator	begin	= 	searchpaths.rbegin();
		searchpath_sequence_w::const_reverse_iterator	end		= 	searchpaths.rend();

#if !defined(__STLSOFT_COMPILER_IS_DMC) && \
	!defined(__STLSOFT_COMPILER_IS_INTEL) && \
	!defined(__STLSOFT_COMPILER_IS_MSVC)
		string_vector_w_t		sv(begin, end);
#else
		string_vector_w_t		sv;

		for(; begin != end; ++begin)
		{
#if defined(__STLSOFT_COMPILER_IS_DMC) || \
	defined(__STLSOFT_COMPILER_IS_INTEL) || \
	defined(__STLSOFT_COMPILER_IS_MSVC)
			sv.push_back(string_vector_w_t::value_type(*begin));
#endif /* __STLSOFT_COMPILER_IS_INTEL || __STLSOFT_COMPILER_IS_MSVC */
		}
#endif /* !__STLSOFT_COMPILER_IS_INTEL && !__STLSOFT_COMPILER_IS_MSVC */

		remove_duplicates_from_unordered_sequence(sv, compare_path<searchpath_sequence_w::char_type>());

		printf("\nSearch paths (backward, Unicode) (duplicates elided):\n");
		stlsoft_ns_qual_std(for_each)(sv.begin(), sv.end(), dump_item());
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */
	}
#endif /* SPST_NO_UNICODE */

	{ // ANSI, indexed
		searchpath_sequence_a	searchpaths(true, true);

		printf("\nSearch paths (indexed, ANSI, Current directory first):\n");
		stlsoft_ns_qual_std(for_each)(searchpaths.rbegin(), searchpaths.rend(), dump_item());

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
		string_vector_a_t		sv;

		for(size_t i = 0; i < searchpaths.size(); ++i)
		{
			sv.push_back(string_vector_a_t::value_type(searchpaths[i]));
		}

		remove_duplicates_from_unordered_sequence(sv, compare_path<searchpath_sequence_a::char_type>());

		printf("\nSearch paths (indexed, ANSI) (duplicates elided):\n");
		stlsoft_ns_qual_std(for_each)(sv.begin(), sv.end(), dump_item());
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */
	}

	return 0;
}
