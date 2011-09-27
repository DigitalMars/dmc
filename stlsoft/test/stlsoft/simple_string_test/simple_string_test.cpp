/* /////////////////////////////////////////////////////////////////////////////
 * File:        simple_string_test.cpp
 *
 * Purpose:     Implementation file for the simple_string_test project.
 *
 * Created:     30th January 2003
 * Updated:     22nd April 2004
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

//#define	_STLSOFT_SIMPLE_STRING_NO_USE_STLSOFT_CHAR_TRAITS

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>		// For printf
#include <stdlib.h>		// For rand()
#include <time.h>		// For time()

#include <stlsoft.h>
#include <stlsoft_simple_string.h>
#include <stlsoft_malloc_allocator.h>

#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
	_MSC_VER >= 1200
# pragma warning(disable : 4550)
#endif /* _MSC_VER < 1200 */


#include <algorithm>

stlsoft_ns_using_std(for_each)
stlsoft_ns_using_std(reverse)

#include <utility>

stlsoft_ns_using_std(distance)

#include <list>

stlsoft_ns_using_std(list)

/* /////////////////////////////////////////////////////////////////////////////
 * Warnings
 */

#ifdef __STLSOFT_COMPILER_IS_INTEL
# pragma warning(disable : 981)
#endif /* __STLSOFT_COMPILER_IS_INTEL */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
	_MSC_VER < 1200
# pragma warning(disable : 4550)
#endif /* _MSC_VER < 1200 */

////////////////////////////////////////////////////////////////////////////////
// Functions

static void dump_char(char c)
{
	printf("%c, ", c);
}

/* ////////////////////////////////////////////////////////////////////////// */

#ifdef __STLSOFT_COMPILER_IS_DMC
template <typename T1, typename T2>
inline ptrdiff_t distance(T1 t1, T2 t2)
{
	ptrdiff_t	d;

	distance(t1, t2, d);

	return d;
}
#endif // __STLSOFT_COMPILER_IS_DMC

// NOTE: This test will become part of the STLSoft STL concept validation
// test suite to be released in version 1.7.1.

/// This function tests that the string's state is consistent
template <ss_typename_param_k S>
inline void test_string_invariant(S &s)
{
	typedef ss_typename_type_k S::size_type	size_type;

	S const		&cs	=	s;
	size_type	i;

	// Basic attributes

	stlsoft_message_assert("size() != length()", s.size() == s.length());

	stlsoft_assert(s.empty() == (s.length() == 0));
	stlsoft_assert(cs.empty() == s.empty());
	stlsoft_assert(cs.length() == s.length());

	if(cs.empty())
	{
		stlsoft_message_assert("Empty string with non-zero length", s.length() == 0);
	}
	else
	{
		stlsoft_message_assert("Non-empty string with zero length", s.length() > 0);

	}

	stlsoft_message_assert("Capacity less than length", s.capacity() >= s.length());

	// Ranges

#ifdef __STLSOFT_COMPILER_IS_INTEL
# pragma warning(push)
// #pragma warning(disable : 383) // Suppresses "value copied to temporary, reference to temporary used"
#endif /* __STLSOFT_COMPILER_IS_INTEL */

	stlsoft_message_assert("mutating iterator range not valid", static_cast<size_type>(distance(s.begin(), s.end())) == s.length());
	stlsoft_message_assert("non-mutating iterator range not valid", static_cast<size_type>(distance(cs.begin(), cs.end())) == s.length());
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
	stlsoft_message_assert("mutating reverse iterator range not valid", static_cast<size_type>(distance(s.rbegin(), s.rend())) == s.length());
	stlsoft_message_assert("non-mutating reverse iterator range not valid", static_cast<size_type>(distance(cs.rbegin(), cs.rend())) == s.length());
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

#ifdef __STLSOFT_COMPILER_IS_INTEL
# pragma warning(pop)
#endif /* __STLSOFT_COMPILER_IS_INTEL */

	// Contents
	stlsoft_message_assert("c_str() returns null pointer", s.c_str() != NULL);
	stlsoft_message_assert("data() returns null pointer", s.data() != NULL);

	for(i = 0; i < s.length(); ++i)
	{
		stlsoft_message_assert("c_str() has returned a different buffer to data()", s.data()[i] == s.c_str()[i]);
	}

	for(i = 0; i < s.length(); ++i)
	{
		stlsoft_message_assert("c_str() has returned different buffer contents to the index operator", s.data()[i] == s[i]);
	}

	// Reverse
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
	{
		S	r(s);

		reverse(r.begin(), r.end());

		ss_typename_type_k S::iterator			fwd_begin	=	s.begin();
		ss_typename_type_k S::iterator			fwd_end		=	s.end();
		ss_typename_type_k S::reverse_iterator	rev_begin	=	r.rbegin();
//		ss_typename_type_k S::reverse_iterator	rev_end		=	r.rend();

		for(; fwd_begin != fwd_end; ++fwd_begin, ++rev_begin)
		{
			stlsoft_message_assert("Reserved sequence element != reverse iterated element", *fwd_begin == *rev_begin);
		}
	}
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

	// Clear
	if(!s.empty())
	{
		S	s1(s);

		s1.clear();
		stlsoft_message_assert("clear() has failed to render empty", s1.empty());
	}

	// Swap
	{
		S	s1(s);
		S	s2;

		s1.swap(s2);

		stlsoft_message_assert("Swapped string invalid", s1.empty());
		stlsoft_message_assert("Swapped string invalid", s2 == s);
	}

	// Copies
	{
		S	s1(s);

		stlsoft_message_assert("Copy-contructed copy not equal", s1 == s);
		stlsoft_message_assert("Copy-contructed copy not equal", !(s1 != s));

		S	s2(s.begin(), s.end());

		stlsoft_message_assert("Range contructed copy not equal", s2 == s);
		stlsoft_message_assert("Range contructed copy not equal", !(s2 != s));

		S	s3;

		s3.assign(s);

		stlsoft_message_assert("Assgined copy not equal", s3 == s);
		stlsoft_message_assert("Assgined copy not equal", !(s3 != s));

		S	s4;

		s4.assign(s.begin(), s.end());

		stlsoft_message_assert("Range-assgined copy not equal", s4 == s);
		stlsoft_message_assert("Range-assgined copy not equal", !(s4 != s));
	}

	// Capacity
	for(i = 0; i < 1000; ++i)
	{
		S			s1;
		size_type	size;

		size	=	static_cast<size_type>(rand());
		s1.reserve(size);
		stlsoft_message_assert("reserve() has failed to allocate sufficient capacity", s1.capacity() >= size);

		size	=	static_cast<size_type>(rand());
		s1.reserve(size);
		stlsoft_message_assert("reserve() has failed to allocate sufficient capacity", s1.capacity() >= size);
	}
}

int main()
{
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
	typedef	stlsoft_ns_qual(basic_simple_string)<char>		sstring_a_t;
	typedef	stlsoft_ns_qual(basic_simple_string)<wchar_t>	sstring_w_t;

	typedef	list<char>										char_list_a_t;
#else
	typedef	stlsoft_ns_qual(basic_simple_string)<char, stlsoft_ns_qual(char_traits)<char>, stlsoft_ns_qual_std(allocator)<char> >			sstring_a_t;
	typedef	stlsoft_ns_qual(basic_simple_string)<wchar_t, stlsoft_ns_qual(char_traits)<wchar_t>, stlsoft_ns_qual_std(allocator)<wchar_t> >	sstring_w_t;

	typedef	list<char, stlsoft_ns_qual_std(allocator)<char> >																				char_list_a_t;
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */

	puts("simple_string_test: " __STLSOFT_COMPILER_LABEL_STRING);

	// Initialise the randomiser
	srand((unsigned)time(NULL));

	char const	literal[]	=	"a literal string";

	char_list_a_t	cl;
	char const		chars[4] = { 'a', 'b', 'c', 'd' };

	cl.push_back(chars[0]);
	cl.push_back(chars[1]);
	cl.push_back(chars[2]);
	cl.push_back(chars[3]);

	printf("Testing stlsoft::simple_string<>\n");

	// (i) Validate that the member types exist (by taking sizeof())
	volatile size_t	x	=	0
						+	sizeof(sstring_a_t)
						+	sizeof(sstring_a_t::value_type)
						+	sizeof(sstring_a_t::traits_type)
						+	sizeof(sstring_a_t::allocator_type)
						+	sizeof(sstring_a_t::class_type)
						+	sizeof(sstring_a_t::char_type)
						+	sizeof(sstring_a_t::pointer)
						+	sizeof(sstring_a_t::const_pointer)
						+	sizeof(sstring_a_t::reference)
						+	sizeof(sstring_a_t::const_reference)
						+	sizeof(sstring_a_t::size_type)
						+	sizeof(sstring_a_t::difference_type)
						+	sizeof(sstring_a_t::iterator)
						+	sizeof(sstring_a_t::const_iterator)
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
						+	sizeof(sstring_a_t::reverse_iterator)
						+	sizeof(sstring_a_t::const_reverse_iterator)
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
						+	0;

	{ sstring_a_t	use_x("", x); }

	// (ii) Construction
	sstring_a_t	s0;

	stlsoft_assert(s0.empty());
	stlsoft_assert(s0.length() == 0);
	stlsoft_assert(s0.begin() == s0.end());
	test_string_invariant(s0);

	sstring_a_t	s0a(s0);

	test_string_invariant(s0a);

	sstring_a_t	s0b(static_cast<char*>(0));

	test_string_invariant(s0b);

	sstring_a_t	s1_("content");

	test_string_invariant(s1_);

	s1_	=	s1_.c_str() + 1;
	test_string_invariant(s1_);
	s1_.assign(s1_, 1, 100);
	test_string_invariant(s1_);
	s1_.assign(s1_, 1, s1_.length() > 0 ? s1_.length() - 1 : 0);
	test_string_invariant(s1_);

	s1_	+=	s1_;
	test_string_invariant(s1_);
	s1_.append(s1_, 1, 100);
	test_string_invariant(s1_);
	s1_.append(s1_, 1, s1_.length() > 0 ? s1_.length() - 1 : 0);
	test_string_invariant(s1_);

	sstring_a_t	another("... and now some more");

	s1_ = "Started again";
	s1_ += ' ';
	s1_ += s0;
	s1_ += " - ";
	s1_.append(another, 0, 100);
	s1_.append(another, 100, 9);

///

	sstring_a_t	s1a(s1_);
	sstring_a_t	s1b; s1b = s1_;

	test_string_invariant(s1a);
	test_string_invariant(s1b);

	s1b = "new content";
	test_string_invariant(s1b);
	s1b = char('a');
	test_string_invariant(s1b);

	s1b.assign("more new stuff");
	test_string_invariant(s1b);
	s1b.assign("more new stuff", 1);
	test_string_invariant(s1b);
	s1b.assign("more new stuff", 10);
	test_string_invariant(s1b);
	s1b.assign("more new stuff", 100);
	test_string_invariant(s1b);
	s1b.assign("more new stuff", size_t(0));
	test_string_invariant(s1b);
	s1b.assign(s1_, 4, 3);
	test_string_invariant(s1b);
	s1b.assign(s1_, 2, 10);
	test_string_invariant(s1b);
	s1b.assign(s1_);
	test_string_invariant(s1b);
	s1b.assign(s0);
	test_string_invariant(s1b);
	s1b.assign(100, 'a');
	test_string_invariant(s1b);
//	s1b.assign(&literal[0], &literal[stlsoft_num_elements(literal)]);
	{ size_t x__ = stlsoft_num_elements(literal); STLSOFT_SUPPRESS_UNUSED(x__); }
	s1b.assign((char const*)&literal[0], (char const*)&literal[stlsoft_num_elements(literal)]);
	test_string_invariant(s1b);
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT
	s1b.assign(cl.begin(), cl.end());
	test_string_invariant(s1b);
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

	s1b.append(" more new stuff ");
	test_string_invariant(s1b);
	s1b.append(" more new stuff ", 1);
	test_string_invariant(s1b);
	s1b.append(" more new stuff ", 10);
	test_string_invariant(s1b);
	s1b.append(" more new stuff ", 100);
	test_string_invariant(s1b);
	s1b.append(" more new stuff ", size_t(0));
	test_string_invariant(s1b);
	s1b.append(s1_, 4, 3);
	test_string_invariant(s1b);
	s1b.append(s1_, 2, 10);
	test_string_invariant(s1b);
	s1b.append(s1_);
	test_string_invariant(s1b);
	s1b.append(s0);
	test_string_invariant(s1b);
	s1b.append(100, 'a');
	test_string_invariant(s1b);
	s1b.append(&literal[0], &literal[stlsoft_num_elements(literal)]);
	test_string_invariant(s1b);
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT
	s1b.append(cl.begin(), cl.end());
	test_string_invariant(s1b);
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

	sstring_a_t	s1("Hello, Natty!");
	sstring_a_t	s2("Hello, Sarah!", 12);

	printf("s1: %s\n", s1.c_str());
	printf("s2: %s\n", s2.c_str());

	printf("s1 (forwards):  ");
	for_each(s1.begin(), s1.end(), dump_char);
	printf("\n");

	printf("s2 (forwards):  ");
	for_each(s2.begin(), s2.end(), dump_char);
	printf("\n");

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
	printf("s2 (backwards): ");
	for_each(s2.rbegin(), s2.rend(), dump_char);
	printf("\n");
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

	s1.swap(s2);

	{	sstring_a_t	s(s1);			}
	{	sstring_a_t	s(s1, 3);		}
	{	sstring_a_t	s(s1, 3, 6);	}

	printf("s1: %s\n", s1.c_str());
	printf("s2: %s\n", s2.c_str());

	stlsoft_assert(sstring_a_t("abc") < sstring_a_t("def"));
	stlsoft_assert(!(sstring_a_t("def") < sstring_a_t("abc")));
	stlsoft_assert(sstring_a_t("abc") < "def");
	stlsoft_assert(!(sstring_a_t("def") < "abc"));
	stlsoft_assert("abc" < sstring_a_t("def"));
	stlsoft_assert(!("def" < sstring_a_t("abc")));

	stlsoft_assert(sstring_a_t("abc") < sstring_a_t("abcd"));
	stlsoft_assert(!(sstring_a_t("abcd") < sstring_a_t("abc")));
	stlsoft_assert(sstring_a_t("abc") < "abcd");
	stlsoft_assert(!(sstring_a_t("abcd") < "abc"));
	stlsoft_assert("abc" < sstring_a_t("abcd"));
	stlsoft_assert(!("abcd" < sstring_a_t("abc")));

	stlsoft_assert(sstring_a_t("abc") <= sstring_a_t("def"));
	stlsoft_assert(sstring_a_t("abc") <= "def");
	stlsoft_assert("abc" <= sstring_a_t("def"));

	stlsoft_assert(sstring_a_t("abc") <= sstring_a_t("abc"));
	stlsoft_assert(sstring_a_t("abc") <= "abc");
	stlsoft_assert("abc" <= sstring_a_t("abc"));

	stlsoft_assert(sstring_a_t("abc") == sstring_a_t("abc"));
	stlsoft_assert(sstring_a_t("abc") == "abc");
	stlsoft_assert("abc" == sstring_a_t("abc"));

	stlsoft_assert(sstring_a_t("abc") != sstring_a_t("def"));
	stlsoft_assert(sstring_a_t("abc") != "def");
	stlsoft_assert("abc" != sstring_a_t("def"));

	stlsoft_assert(sstring_a_t("abc").compare("abc") == 0);
	stlsoft_assert(sstring_a_t("abc").compare("abcd") < 0);
	stlsoft_assert(sstring_a_t("abc").compare("abb") > 0);

	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), ("bc")) == 0);
	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), ("bcd")) < 0);
	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), ("bb")) > 0);

	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(1), ("bc"), 1) == 0);
	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), ("bcd"), 100) < 0);
	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), ("bb"), 2) > 0);

	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(1), ("bc"), 1) == 0);
	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), ("bcd"), 100) < 0);
	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), ("bb"), 2) > 0);

	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), sstring_a_t("bc")) == 0);
	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), sstring_a_t("bcd")) < 0);
	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), sstring_a_t("bb")) > 0);

	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), sstring_a_t("bc"), 0, 2) == 0);
	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), sstring_a_t("bcd"), 0, 100) < 0);
	stlsoft_assert(sstring_a_t("abc").compare(size_t(1), size_t(5), sstring_a_t("bb"), 0, 2) > 0);

	return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
