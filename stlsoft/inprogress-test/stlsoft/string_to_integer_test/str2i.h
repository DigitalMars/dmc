/* /////////////////////////////////////////////////////////////////////////////
 * File:        MEStr2L.c
 *
 * Purpose:     Simple string to long function.
 *
 * Created:     11th October 2000
 * Updated:     30th May 2004
 * Author:      Synesis Software (Pty) Ltd (C) 1999. All rights reserved.
 *              (www.synesis-group.com/software).
 *
 * Status:      Internal.
 *
 * Copyright:   The source code contained herein is the confidential and
 *              proprietary property of Synesis Software (Pty) Ltd. Any use,
 *              replication or dissemination of its contents in any manner, or
 *              by any means, whatsoever is prohibited by law, and will lead to
 *              prosecution.
 *
 * ////////////////////////////////////////////////////////////////////////// */

#ifdef _MSC_VER
#pragma warning (disable : 4290)
#endif /* _MSC_VER */


/* /////////////////////////////////////////////////////////////////////////////
 * Auto-generation and compatibility
 */

/*
[<[STLSOFT-AUTO:NO-DOCFILELABEL]>]
[<[STLSOFT-AUTO:NO-UNITTEST]>]
*/

// comstl_interface_cast.h:

struct ignore_string_to_integer_bad_conversion_exception
{
public:
	struct thrown_type
	{
	};

public:
	void operator ()() stlsoft_throw_0()
	{
		// Do nothing
	}
};


struct ignore_string_to_integer_overflow_exception
{
public:
	struct thrown_type
	{
	};

public:
	void operator ()() stlsoft_throw_0()
	{
		// Do nothing
	}
};


template <typename C>
inline bool is_whitespace(const C &c);

inline bool is_whitespace(const char &c)
{
	return c == ' ' || c == '\t';
}

template <typename C>
inline bool is_digit(const C &c);

inline bool is_digit(const char &c)
{
	return c >= '0' && c <= '9';
}

template <typename C>
inline signed char get_digit_value(const C &c);

inline signed char get_digit_value(const char &c)
{
	return c - '0';
}


template <typename C, typename I, typename BadCharX>
inline long string_to_integer(const C * s, BadCharX bcx) throw(BadCharX::thrown_type)
{
	long	result		=	0;
	bool	bNegative	=	false;

	while(*s && is_whitespace(*s))
	{
		++s;
	}

	switch(*s)
	{
		case	'-':
			bNegative = true;
		case	'+':
			++s;
			break;
	}

	for(; *s; ++s)
	{
		if(is_digit(*s))
		{
			result *= 10;
			result += get_digit_value(*s);
		}
		else
		{
			bcx();

			break;
		}
	}

	if(bNegative)
	{
		result = -result;
	}

	return result;
}

#ifndef __DMC__
template <typename C>
inline long string_to_integer(const C * s) throw(ignore_string_to_integer_bad_conversion_exception::thrown_type)
{
	return string_to_integer(s, ignore_string_to_integer_bad_conversion_exception());
}
#else
inline long string_to_integer_a(const char * s) throw(ignore_string_to_integer_bad_conversion_exception::thrown_type)
{
	return string_to_integer(s, ignore_string_to_integer_bad_conversion_exception());
}
#endif /* 0 */
