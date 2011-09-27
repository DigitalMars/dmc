/* /////////////////////////////////////////////////////////////////////////////
 * File:        return_value_dtor_test.cpp
 *
 * Purpose:     Illustrates return_value_destructor and
 *              return_value_array_destructor classes
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

// Enable this to show how the return_value_destructor class
// works with raw pointers
//#define	_STLSOFT_RETURN_VALUE_DESTRUCTOR_ENABLE_DIRECT_CTOR

// Enable this to hide the assert that is 'thrown' in the
// destructor of the unused value in case (i) (see main(), below)
// #define _STLSOFT_RETURN_VALUE_DESTRUCTOR_DISABLE_UNUSED_ASSERT

#include <stlsoft_auto_destructor.h>

stlsoft_ns_using(auto_destructor)
stlsoft_ns_using(return_value_destructor)
stlsoft_ns_using(auto_array_destructor)
stlsoft_ns_using(return_value_array_destructor)


// Enable this to use the std::string, rather than the simple
// null string class defined below
//#define _RVDT_USE_STD_STRING

#if defined(_RVDT_USE_STD_STRING) && \
    defined(__WATCOMC__)
# undef _RVDT_USE_STD_STRING
#endif /* _RVDT_USE_STD_STRING && __WATCOMC__ */

#ifdef _RVDT_USE_STD_STRING
# include <string>
 stlsoft_ns_using_std(string)

#else
# include <string.h>

class string
{
public:
	string(char const *s)
		: m_s(strdup(s))
	{}
	~string()
	{
		free(m_s);
	}

// Accessors
public:
	char const *c_str() const
	{
		return m_s;
	}

// Members
protected:
	char *m_s;

// Not to be implemented
private:
	string(string const *&);
	string const &operator =(string const *&);
};
#endif /* _RVDT_USE_STD_STRING */

////////////////////////////////////////////////////////////////////////////////
// Typedefs

typedef auto_destructor<string>				auto_string_t;
typedef return_value_destructor<string>		return_value_string_t;

typedef auto_array_destructor<int>			auto_int_array_t;
typedef return_value_array_destructor<int>	return_value_int_array_t;

////////////////////////////////////////////////////////////////////////////////
// Function declarations

static return_value_string_t	get_string_instance(const char *pcsz);
static return_value_int_array_t	get_int_array();
static void						dump_string_ptr(char const *message
											,	string const *string_ptr);

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char* /* argv */[])
{
	// (i) This demonstrates how an unused returned resource is
	// correctly disposed of
	get_string_instance("Hello 1");

	// (ii) This demonstrates returning safely and then
	// transferring ownership into the auto_destructor, from which
	// it can be accessed
	{
		return_value_string_t	rvds(get_string_instance("Hello 2"));

		// This would be a compile error, since return_value_destructors do
		// not allow access to their contained resource.
//		dump_string_ptr("return_value_destructor", rvds);

		auto_string_t	ads(rvds);

		dump_string_ptr("return_value_destructor", ads);
	}

	// (iii) This demonstrates returning safely directly into the
	// auto_destructor, from which it can be accessed
	{
		auto_string_t	ads(get_string_instance("Hello 3"));

		dump_string_ptr("return_value_destructor", ads);
	}

	// (iv) This section demonstrates the use of the array variants
	{
		return_value_int_array_t	aadi(get_int_array());
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Functions

return_value_string_t get_string_instance(const char *pcsz)
{
#ifndef _STLSOFT_RETURN_VALUE_DESTRUCTOR_ENABLE_DIRECT_CTOR
	auto_string_t	ads(new string(pcsz));

	return return_value_string_t(ads);
#else
	return return_value_string_t(new string(pcsz));
#endif /* !_STLSOFT_RETURN_VALUE_DESTRUCTOR_ENABLE_DIRECT_CTOR */
}

return_value_int_array_t get_int_array()
{
#ifndef _STLSOFT_RETURN_VALUE_DESTRUCTOR_ENABLE_DIRECT_CTOR
	auto_int_array_t	ads(new int[10]);

	return return_value_int_array_t(ads);
#else
	return return_value_int_array_t(new string(pcsz));
#endif /* !_STLSOFT_RETURN_VALUE_DESTRUCTOR_ENABLE_DIRECT_CTOR */
}

void dump_string_ptr(char const *message, string const *string_ptr)
{
	if(string_ptr == 0)
	{
		printf("%s: string(%08p)\n", message, (void*)string_ptr);
	}
	else
	{
		printf("%s: string(%08p): \"%s\"\n", message, string_ptr, string_ptr->c_str());
	}
}

////////////////////////////////////////////////////////////////////////////////
