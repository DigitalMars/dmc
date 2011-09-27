/* /////////////////////////////////////////////////////////////////////////////
 * File:        sequence_container_veneer_test.cpp
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

#include <stlsoft_sequence_container_veneer.h>

stlsoft_ns_using(sequence_container_veneer)

#include <deque>
#include <list>
#include <string>
#include <vector>

stlsoft_ns_using_std(deque)			// introduce std::deque into global namespace
stlsoft_ns_using_std(list)			// introduce std::list into global namespace
stlsoft_ns_using_std(vector)		// introduce std::vector into global namespace
stlsoft_ns_using_std(basic_string)	// introduce std::basic_string into global namespace

stlsoft_ns_using_std(allocator)		// introduce std::allocator into global namespace
stlsoft_ns_using_std(char_traits)	// introduce std::char_traits into global namespace

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

#if	(	!defined(_MSC_VER) || \
		_MSC_VER >= 1100) && \
	(	!defined(__GNUC__) || \
		__GNUC__ > 2)
# define _SCVT_USE_BASIC_STRING
#endif /* !(_MSC_VER < 1100) && !(__GNUC__ < 2) */



#ifdef __DMC__
typedef deque<Simple*>								_simple_deque_t;
typedef list<Simple*>								_simple_list_t;
typedef basic_string<	Simple*
					,	char_traits<Simple*> >		_simple_string_t;
typedef vector<Simple*>								_simple_vector_t;
#else
typedef deque<Simple*, allocator<Simple*> >			_simple_deque_t;
typedef list<Simple*, allocator<Simple*> >			_simple_list_t;
#ifdef _SCVT_USE_BASIC_STRING
typedef basic_string<	Simple*
					,	char_traits<Simple*>
					,	allocator<Simple*> >		_simple_string_t;
#endif /* _SCVT_USE_BASIC_STRING */
typedef vector<Simple*, allocator<Simple*> >		_simple_vector_t;
#endif /* __DMC__ */

// Enable/disable the following in order to see whether the strings are deleted
#if 0
typedef _simple_deque_t															simple_deque_t;
typedef _simple_list_t															simple_list_t;
#ifdef _SCVT_USE_BASIC_STRING
typedef _simple_string_t														simple_string_t;
#endif /* _SCVT_USE_BASIC_STRING */
typedef _simple_vector_t														simple_vector_t;
#else
typedef	sequence_container_veneer<_simple_deque_t, delete_instance<Simple> >	simple_deque_t;
typedef	sequence_container_veneer<_simple_list_t, delete_instance<Simple> >		simple_list_t;
#ifdef _SCVT_USE_BASIC_STRING
typedef	sequence_container_veneer<_simple_string_t, delete_instance<Simple> >	simple_string_t;
#endif /* _SCVT_USE_BASIC_STRING */
typedef	sequence_container_veneer<_simple_vector_t, delete_instance<Simple> >	simple_vector_t;
#endif /* 0 */

////////////////////////////////////////////////////////////////////////////////
// Functions

template <ss_typename_param_k C>
static void test_container(C &container, char const *typeName)
{
	printf("\n%s:\n", typeName);

	Simple	*array[] =
	{
			new Simple(1)
		,	new Simple(2)
		,	new Simple(3)
		,
	};

#if !defined(__DMC__) && \
	(	!defined(_MSC_VER) || \
		_MSC_VER > 1200)
	container.assign(array, array + stlsoft_num_elements(array));
#else
	container.push_back(array[0]);
	container.push_back(array[1]);
	container.push_back(array[2]);
#endif /* __SYNSOFT_DBS_DEBUG */

	container.push_back(new Simple(4));
	container.push_back(new Simple(5));
}

////////////////////////////////////////////////////////////////////////////////
// main



int main(int /* argc */, char* /* argv */[])
{
	{
		simple_deque_t	container;
		test_container(container, static_cast<char const *>("deque"));
	}

	{
		simple_list_t	container;
		test_container(container, static_cast<char const *>("list"));
	}

#ifdef _SCVT_USE_BASIC_STRING
	{
		simple_string_t	container;

		printf("\nbasic_string:\n");

		container.append(1, new Simple(0));
		container.append(1, new Simple(1));
		container.append(1, new Simple(2));
		container.append(1, new Simple(4));
		container.append(1, new Simple(5));
	}
#endif /* _SCVT_USE_BASIC_STRING */

	{
		simple_vector_t	container;
		test_container(container, static_cast<char const *>("vector"));
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
