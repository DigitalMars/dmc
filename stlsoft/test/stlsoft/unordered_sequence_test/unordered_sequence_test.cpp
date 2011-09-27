/* /////////////////////////////////////////////////////////////////////////////
 * File:        unordered_sequence_test.cpp
 *
 * Purpose:
 *
 * Created:
 * Updated:     22nd April 2004
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

#include <stdio.h>

#include <stlsoft_simple_algorithms.h>

stlsoft_ns_using(remove_duplicates_from_unordered_sequence)

#include <algorithm>
#include <vector>
#include <list>

stlsoft_ns_using_std(allocator)
stlsoft_ns_using_std(list)
stlsoft_ns_using_std(vector)

////////////////////////////////////////////////////////////////////////////////
// Typedefs

#ifdef __DMC__
typedef	list<int>						int_list_t;
typedef	vector<int>						int_vector_t;
#else
typedef	list<int, allocator<int> >		int_list_t;
typedef	vector<int, allocator<int> >	int_vector_t;
#endif /* __DMC__ */

template <ss_typename_param_k T>
struct is_equal
{
	bool operator ()(const T &t1, const T &t2)
	{
		return t1 == t2;
	}
};

template <ss_typename_param_k T>
struct print_item
{
	void operator ()(const T &t)
	{
		fprintf(stdout, "%d ", t);
	}
};

template <ss_typename_param_k L, ss_typename_param_k T>
inline void init_container_from_sequence(L &l, T *begin, T *end)
{
#if defined(__DMC__) || \
	(	defined(_MSC_VER) && \
		_MSC_VER < 1300)
	for(; begin != end; ++begin)
	{
		l.push_back(*begin);
	}
#else
	l.assign(begin, end);
#endif /* compiler */
}

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char* /* argv */[])
{
	int				ints[] =
	{
			12
		,	-3
		,	-3
		,	-3
		,	0
		,	12
		,	11
		,	-21
		,	-3
		,	-3
		,	-3
		,	4
		,	4
	};
	int_list_t		il;//(ints, ints + stlsoft_num_elements(ints));
	int_vector_t	iv;//(ints, ints + stlsoft_num_elements(ints));

	init_container_from_sequence(il, ints, ints + stlsoft_num_elements(ints));
	init_container_from_sequence(iv, ints, ints + stlsoft_num_elements(ints));

	il.insert(il.begin(), ints, ints + stlsoft_num_elements(ints));
	iv.insert(iv.begin(), ints, ints + stlsoft_num_elements(ints));

	fprintf(stdout, "\nlist:   ");
	stlsoft_ns_qual_std(for_each)(il.begin(), il.end(), print_item<int>());
	fprintf(stdout, "\nvector: ");
	stlsoft_ns_qual_std(for_each)(iv.begin(), iv.end(), print_item<int>());

	remove_duplicates_from_unordered_sequence(il, is_equal<int>());
	remove_duplicates_from_unordered_sequence(iv, is_equal<int>());

	fprintf(stdout, "\nlist:   ");
	stlsoft_ns_qual_std(for_each)(il.begin(), il.end(), print_item<int>());
	fprintf(stdout, "\nvector: ");
	stlsoft_ns_qual_std(for_each)(iv.begin(), iv.end(), print_item<int>());

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
