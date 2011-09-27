/* /////////////////////////////////////////////////////////////////////////////
 * File:        printf_traits_test.cpp
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
#include <limits.h>

#include <stlsoft.h>

stlsoft_ns_using(ss_sint8_t)
stlsoft_ns_using(ss_uint8_t)
stlsoft_ns_using(ss_sint16_t)
stlsoft_ns_using(ss_uint16_t)
stlsoft_ns_using(ss_sint32_t)
stlsoft_ns_using(ss_uint32_t)
stlsoft_ns_using(ss_sint64_t)
stlsoft_ns_using(ss_uint64_t)

#include <stlsoft_limit_traits.h>
#include <stlsoft_printf_traits.h>

stlsoft_ns_using(printf_traits)

////////////////////////////////////////////////////////////////////////////////
// Typedefs

#if 0
template <class T>
inline void print_sizes()
{
	typedef	printf_traits<T>	printf_traits_type;

	int	size_min	=	printf_traits_type::size_min;
	int	size_max	=	printf_traits_type::size_max;
	int	size		=	printf_traits_type::size;

	printf("%d %d %d\n", size_min, size_max, size);
}
#endif /* 0 */

template <class T>
inline void print_sizes(const T &)
{
	typedef	printf_traits<T>	printf_traits_type;

	int	size_min	=	printf_traits_type::size_min;
	int	size_max	=	printf_traits_type::size_max;
	int	size		=	printf_traits_type::size;

	printf("%d %d %d\n", size_min, size_max, size);
}

#define __synsoft_stringize(x)          #x
#define __synsoft_stringize_symbol(x)   __synsoft_stringize(x) /* x is translated prior to stringizing */
#define __synsoft_quote(x)              x

#define _sscomp_fileline(x)            __FILE__ "(" __synsoft_stringize_symbol( __LINE__ ) "): "
#define _sscomp_fileline_message(x)    _sscomp_fileline(x) ## x

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char* /* argv */[])
{
	const char	sz_1[]	=	___stringise(__STLSOFT_PRINTF_TRAITS__UINT64_MAX);
	const char	sz_2[]	=	___stringise(__STLSOFT_PRINTF_TRAITS__SINT16_MIN);
	const char	sz_3[]	=	___stringise(__STLSOFT_PRINTF_TRAITS__UINT8_MAX);
	const char	sz_4[]	=	___stringise(__STLSOFT_PRINTF_TRAITS__SINT32_MIN);

	(void)sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__SINT8_MIN));
	(void)sizeof(___stringise(__STLSOFT_PRINTF_TRAITS__SINT8_MAX));

#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
	_MSC_VER > 1200

	(void)printf_traits<ss_sint8_t>::size_min;
	(void)printf_traits<ss_sint8_t>::size_max;
	(void)printf_traits<ss_sint8_t>::size;

	(void)printf_traits<ss_uint8_t>::size_min;
	(void)printf_traits<ss_uint8_t>::size_max;
	(void)printf_traits<ss_uint8_t>::size;

	(void)printf_traits<ss_sint16_t>::size_min;
	(void)printf_traits<ss_sint16_t>::size_max;
	(void)printf_traits<ss_sint16_t>::size;

	(void)printf_traits<ss_uint16_t>::size_min;
	(void)printf_traits<ss_uint16_t>::size_max;
	(void)printf_traits<ss_uint16_t>::size;

	(void)printf_traits<ss_sint32_t>::size_min;
	(void)printf_traits<ss_sint32_t>::size_max;
	(void)printf_traits<ss_sint32_t>::size;

	(void)printf_traits<ss_uint32_t>::size_min;
	(void)printf_traits<ss_uint32_t>::size_max;
	(void)printf_traits<ss_uint32_t>::size;

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
	(void)printf_traits<ss_sint64_t>::size_min;
	(void)printf_traits<ss_sint64_t>::size_max;
	(void)printf_traits<ss_sint64_t>::size;

	(void)printf_traits<ss_uint64_t>::size_min;
	(void)printf_traits<ss_uint64_t>::size_max;
	(void)printf_traits<ss_uint64_t>::size;
#endif /* !STLSOFT_CF_64BIT_INT_SUPPORT */

#endif /* !__STLSOFT_COMPILER_IS_MSVC || _MSC_VER > 1200 */

#if defined(__WATCOMC__) || \
	(	defined(_MSC_VER) && \
		_MSC_VER < 1100)
    print_sizes(ss_sint8_t());
	print_sizes(ss_uint8_t());
	print_sizes(ss_uint16_t());
	print_sizes(ss_sint16_t());
	print_sizes(ss_uint32_t());
	print_sizes(ss_sint32_t());
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
	print_sizes(ss_sint64_t());
	print_sizes(ss_uint64_t());
#endif /* !STLSOFT_CF_64BIT_INT_SUPPORT */
#else
	print_sizes<ss_sint8_t>(ss_sint8_t());
	print_sizes<ss_uint8_t>(ss_uint8_t());
	print_sizes<ss_uint16_t>(ss_uint16_t());
	print_sizes<ss_sint16_t>(ss_sint16_t());
	print_sizes<ss_uint32_t>(ss_uint32_t());
	print_sizes<ss_sint32_t>(ss_sint32_t());
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
	print_sizes<ss_sint64_t>(ss_sint64_t());
	print_sizes<ss_uint64_t>(ss_uint64_t());
#endif /* !STLSOFT_CF_64BIT_INT_SUPPORT */
#endif /* __WATCOMC__ */

	// Just suppress the compiler warnings.
	return sz_1[0] && sz_2[0] && sz_3[0] && sz_4[0];
}

////////////////////////////////////////////////////////////////////////////////
