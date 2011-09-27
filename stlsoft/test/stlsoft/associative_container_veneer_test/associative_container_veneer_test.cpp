/* /////////////////////////////////////////////////////////////////////////////
 * File:        associative_container_veneer_test.cpp
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
stlsoft_ns_using(noop_function)

#include <stlsoft_associative_container_veneer.h>

stlsoft_ns_using(associative_container_veneer)

#if defined(_MSC_VER) && \
	_MSC_VER >= 1200
# pragma warning(push, 3)
#endif /* _MSC_VER >= 1200 */
#include <map>
#if defined(_MSC_VER) && \
	_MSC_VER >= 1200
# pragma warning(pop)
#endif /* _MSC_VER >= 1200 */

stlsoft_ns_using_std(map)			// introduce std::map into global namespace
stlsoft_ns_using_std(multimap)		// introduce std::multimap into global namespace

stlsoft_ns_using_std(allocator)		// introduce std::allocator into global namespace
stlsoft_ns_using_std(less)			// introduce std::less into global namespace

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

// Accessors
public:
	int value() const
	{
		return m_i;
	}

// Members
protected:
	int	const m_i;

// Not to be implemented
private:
	Simple &operator =(Simple const &);
};

struct less_Simple
{
	bool operator ()(Simple const *s1, Simple const *s2) const
	{
		return s1->value() < s2->value();
	}
};

#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
typedef	map<int, Simple *>											_int_simple_map_t;
typedef	map<Simple *, int, less_Simple>								_simple_int_map_t;
typedef	map<Simple *, Simple *, less_Simple>						_simple_simple_map_t;
#else
typedef allocator<int *>											_simple_allocator_t;
typedef	map<int, Simple *, less<int>, _simple_allocator_t>			_int_simple_map_t;
typedef	map<Simple *, int, less_Simple, _simple_allocator_t>		_simple_int_map_t;
typedef	map<Simple *, Simple *, less_Simple, _simple_allocator_t>	_simple_simple_map_t;
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */



// Enable/disable the following in order to see whether the strings are deleted
// (obviously you'll need to use the debugger to verify this to be the case)
#if 0
typedef _int_simple_map_t											int_simple_map_t;
typedef _simple_int_map_t											simple_int_map_t;
typedef _simple_simple_map_t										simple_simple_map_t;
#else
typedef	associative_container_veneer<	_int_simple_map_t
									,	delete_instance<Simple>
//									,	noop_function<int>
									>								int_simple_map_t;
typedef associative_container_veneer<	_simple_int_map_t
									,	noop_function<int>
									,	delete_instance<Simple>
									>								simple_int_map_t;
typedef associative_container_veneer<	_simple_simple_map_t
									,	delete_instance<Simple>
									,	delete_instance<Simple>
									>								simple_simple_map_t;
#endif /* 0 */

////////////////////////////////////////////////////////////////////////////////
// main

static Simple *create_Simple(int i)
{
	return new Simple(i);
}

int main(int /* argc */, char* /* argv */[])
{
	{
		printf("\nint_simple_map_t:\n");

		int_simple_map_t	m;

		m[1] = new Simple(1);
		m[2] = new Simple(2);
		m[3] = new Simple(3);
		m[4] = new Simple(4);
		m[5] = new Simple(5);
	}

	{
		printf("\nsimple_int_map_t:\n");

		simple_int_map_t	m;

		Simple	*simple	=	new Simple(1);

		m[simple] = 1;
		m[create_Simple(2)] = 2;
		m[create_Simple(3)] = 3;
		m[create_Simple(4)] = 4;
		m[create_Simple(5)] = 5;
	}

	{
		printf("\nint_simple_map_t:\n");

		simple_simple_map_t	m;

		m[create_Simple(1)] = new Simple(1);
		m[create_Simple(2)] = new Simple(2);
		m[create_Simple(3)] = new Simple(3);
		m[create_Simple(4)] = new Simple(4);
		m[create_Simple(5)] = new Simple(5);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
