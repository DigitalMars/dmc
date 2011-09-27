/* /////////////////////////////////////////////////////////////////////////////
 * File:        true_typedef_test.cpp
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

#include <stlsoft.h>
#include <stlsoft_true_typedef.h>

stlsoft_ns_using(true_typedef)

////////////////////////////////////////////////////////////////////////////////
// Typedefs

// Enable/disable this to demonstrate the ability of true-typedefs to
// disambiguate type definitions (by allowing the two versions of func() to be
// proper overloads, rather than multiple-definitions)
#define DEMONSTRATE_TT

#ifdef DEMONSTRATE_TT
stlsoft_gen_opaque(channel_id_t_h)
typedef	true_typedef<long, channel_id_t_h>		channel_id_t;

stlsoft_gen_opaque(key_id_t_h)
typedef	true_typedef<long, key_id_t_h>			key_id_t;
#else
typedef	long									channel_id_t;

typedef	long									key_id_t;
#endif /* DEMONSTRATE_TT */

////////////////////////////////////////////////////////////////////////////////
// Functions

struct overload_context
{
	static void func(channel_id_t /* chid */);
	static void func(key_id_t /* chid */);
};

////////////////////////////////////////////////////////////////////////////////
// main

#pragma pack(push, 16)

struct S1_	{	char	s[1];	};
typedef true_typedef<S1_, S1_>	S1;

struct S2_	{	char	s[2];	};
typedef true_typedef<S2_, S2_>	S2;

struct S3_	{	char	s[3];	};
typedef true_typedef<S3_, S3_>	S3;

struct S4_	{	char	s[4];	};
typedef true_typedef<S4_, S4_>	S4;

struct S5_	{	char	s[5];	};
typedef true_typedef<S5_, S5_>	S5;

struct S6_	{	char	s[6];	};
typedef true_typedef<S6_, S6_>	S6;

#pragma pack(pop)

int main(int /* argc */, char ** /*argv*/)
{
	channel_id_t	ch1;	// Default construction is ok
	channel_id_t	ch2(1);	// Explicit construction from base type is ok
//	channel_id_t	ch3 = 0;// Implicit construction from base type is NOT
	key_id_t		k1(2);

	stlsoft_static_assert(sizeof(key_id_t) == sizeof(key_id_t::value_type));

	stlsoft_static_assert(sizeof(S1) == sizeof(S1::value_type));
	stlsoft_static_assert(sizeof(S2) == sizeof(S2::value_type));
	stlsoft_static_assert(sizeof(S3) == sizeof(S3::value_type));
	stlsoft_static_assert(sizeof(S4) == sizeof(S4::value_type));
	stlsoft_static_assert(sizeof(S5) == sizeof(S5::value_type));
	stlsoft_static_assert(sizeof(S6) == sizeof(S6::value_type));

//	key_id_t		k2(ch1);// Different true-types are not implicitly inter-convertible
#ifdef DEMONSTRATE_TT
	key_id_t		k3(ch1.base_type_value());// Different true-types can be explicitly inter-convertible
#endif /* DEMONSTRATE_TT */

//	long			l1	=	ch1;	// True-types are not implicitly convertible to base type
#ifdef DEMONSTRATE_TT
	long			l2	=	ch1.base_type_value();	// True-types are explicitly convertible to base type

	STLSOFT_SUPPRESS_UNUSED(l2);
#endif /* DEMONSTRATE_TT */

	++ch1;
	ch1++;

	// True-types support any arithmetic operations that their base type
	// supports, involving other true-types (of the same type) or the
	// base type

	channel_id_t	ch_res1 = ch1 - ch2;
	channel_id_t	ch_res2 = ch1 - 12l;
	channel_id_t	ch_res3 = 102l - ch1;
	channel_id_t	ch_res4 = ch1 + ch2;
	channel_id_t	ch_res5 = ch1 / ch2;
	channel_id_t	ch_res6 = ch1 * ch2;
	channel_id_t	ch_res7 = ch1 * 17l;

	ch_res1 /= ch1;
	ch_res2 *= 13l;

	STLSOFT_SUPPRESS_UNUSED(l2);
	STLSOFT_SUPPRESS_UNUSED(ch_res3);
	STLSOFT_SUPPRESS_UNUSED(ch_res4);
	STLSOFT_SUPPRESS_UNUSED(ch_res5);
	STLSOFT_SUPPRESS_UNUSED(ch_res6);
	STLSOFT_SUPPRESS_UNUSED(ch_res7);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
