/* /////////////////////////////////////////////////////////////////////////////
 * File:        delete_instance_test.cpp
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

#include <stlsoft.h>
#include <stlsoft_functionals.h>

stlsoft_ns_using(delete_instance)

#if !defined(__WATCOMC__)
#include <algorithm>
#include <vector>
#include <map>

stlsoft_ns_using_std(for_each)
stlsoft_ns_using_std(allocator)
stlsoft_ns_using_std(vector)
stlsoft_ns_using_std(map)
#endif /* !__WATCOMC__ */

////////////////////////////////////////////////////////////////////////////////
// Typedefs

class Simple
{
public:
	Simple(int i)
		: m_i(i)
	{
		printf(" Simple(0x%08p): [%d]\n", this, m_i);
	}
	~Simple()
	{
		printf("~Simple(0x%08p): [%d]\n", this, m_i);
	}

// Members
protected:
	int	const m_i;

// Not to be implemented
private:
	Simple &operator =(Simple const &);
};

#if !defined(__WATCOMC__)
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
typedef	vector<Simple *>					simple_vector_t;
#else
typedef allocator<Simple *>					int_allocator_t;
typedef	vector<Simple *, int_allocator_t>	simple_vector_t;
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
#endif /* !__WATCOMC__ */

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char* /* argv */[])
{
#if !defined(__WATCOMC__)
	// vector
	simple_vector_t			v;

	for(int i = 0; i < 10; ++i)
	{
		v.push_back(new Simple(i));
	}

// Enable/disable this to see the presence/absence of destruction of instances
#if 1
	for_each(v.begin(), v.end(), delete_instance<Simple>());
#endif /* 1 */
#else
	// For Watcom, just demonstrate the use of the function object on its own
	delete_instance<Simple>	fn;

	fn(new Simple(1));
#endif /* !__WATCOMC__ */

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
