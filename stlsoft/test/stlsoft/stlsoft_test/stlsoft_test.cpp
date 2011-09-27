/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_test.cpp
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


#ifndef _DEBUG
//# error veneer
#endif // _DEBUG

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this# define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <stlsoft.h>

#ifdef __STLSOFT_COMPILER_IS_INTEL
# pragma warning(disable : 1418)
#endif /* __STLSOFT_COMPILER_IS_INTEL */

stlsoft_ns_using(ss_sint8_t)
stlsoft_ns_using(ss_uint8_t)
stlsoft_ns_using(ss_sint16_t)
stlsoft_ns_using(ss_uint16_t)
stlsoft_ns_using(ss_sint32_t)
stlsoft_ns_using(ss_uint32_t)
stlsoft_ns_using(ss_int32_t)
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
stlsoft_ns_using(ss_sint64_t)
stlsoft_ns_using(ss_uint64_t)
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */
stlsoft_ns_using(ss_sint_t)
stlsoft_ns_using(ss_uint_t)
stlsoft_ns_using(ss_int_t)
stlsoft_ns_using(ss_long_t)
stlsoft_ns_using(ss_bool_t)
stlsoft_ns_using(ss_size_t)
stlsoft_ns_using(ss_ptrdiff_t)
stlsoft_ns_using(ss_char_a_t)
stlsoft_ns_using(ss_char_w_t)

#define	_type_sizeof(t)	fprintf(stdout, "  size of "# t " is %u\n", sizeof(t))

#ifdef __STLSOFT_CF_std_NAMESPACE
/* Because most implementations of STL use the bool type, we typedef it here
 * in order to allow them to compile
 */
#ifndef __STLSOFT_CF_NATIVE_BOOL_SUPPORT
typedef ss_bool_t	bool;
#endif /* !__STLSOFT_CF_NATIVE_BOOL_SUPPORT */

#include <algorithm>

#include <memory>

stlsoft_ns_using_std(bad_alloc)
stlsoft_ns_using_std(allocator)

#endif /* __STLSOFT_CF_std_NAMESPACE */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_COMPILER_IS_MSVC
# pragma warning(disable : 4505)
#endif /* __STLSOFT_COMPILER_IS_MSVC */

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char ** /* argv */)
{
	puts("stlsoft_test: " __STLSOFT_COMPILER_VERSION_STRING);

	/* . */
	int	iRet	=	0;

	stlsoft_static_assert(sizeof(ss_sint8_t) == 1);
	stlsoft_static_assert(sizeof(ss_uint8_t) == 1);
	stlsoft_static_assert(sizeof(ss_sint16_t) == 2);
	stlsoft_static_assert(sizeof(ss_uint16_t) == 2);
	stlsoft_static_assert(sizeof(ss_sint32_t) == 4);
	stlsoft_static_assert(sizeof(ss_uint32_t) == 4);
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
	stlsoft_static_assert(sizeof(ss_sint64_t) == 8);
	stlsoft_static_assert(sizeof(ss_uint64_t) == 8);
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */

	fprintf(stdout, "\nKeywords:\n");

#ifdef __STLSOFT_CF_EXPLICIT_KEYWORD_SUPPORT
	fprintf(stdout, "  explicit keyword supported\n");
#else
	fprintf(stdout, "  explicit keyword not supported\n");
#endif /* __STLSOFT_CF_EXPLICIT_KEYWORD_SUPPORT */

#ifdef __STLSOFT_CF_MUTABLE_KEYWORD_SUPPORT
	fprintf(stdout, "  mutable keyword supported\n");
#else
	fprintf(stdout, "  mutable keyword not supported\n");
#endif /* __STLSOFT_CF_MUTABLE_KEYWORD_SUPPORT */

#ifdef __STLSOFT_CF_TYPENAME_PARAM_KEYWORD_SUPPORT
	fprintf(stdout, "  typename keyword as param supported\n");
#else
	fprintf(stdout, "  typename keyword as param not supported\n");
#endif /* __STLSOFT_CF_TYPENAME_PARAM_KEYWORD_SUPPORT */

#ifdef __STLSOFT_CF_TYPENAME_TYPE_KEYWORD_SUPPORT
	fprintf(stdout, "  typename keyword as type supported\n");
#else
	fprintf(stdout, "  typename keyword as type not supported\n");
#endif /* __STLSOFT_CF_TYPENAME_TYPE_KEYWORD_SUPPORT */


	fprintf(stdout, "\nTypes:\n");

	fprintf(stdout, "\n Integral types:\n");
	fprintf(stdout, "  8-bit integers supported, as ");
#ifdef STLSOFT_CF_8BIT_INT_IS_EXTENDED_TYPE
		fprintf(stdout, "extended type\n");
#else /* ? STLSOFT_CF_8BIT_INT_IS_EXTENDED_TYPE */
		fprintf(stdout, "standard type\n");
#endif /* STLSOFT_CF_8BIT_INT_IS_EXTENDED_TYPE */
	fprintf(stdout, "  16-bit integers supported, as ");
#ifdef STLSOFT_CF_16BIT_INT_IS_EXTENDED_TYPE
		fprintf(stdout, "extended type\n");
#else /* ? STLSOFT_CF_16BIT_INT_IS_EXTENDED_TYPE */
		fprintf(stdout, "standard type\n");
#endif /* STLSOFT_CF_16BIT_INT_IS_EXTENDED_TYPE */
	fprintf(stdout, "  32-bit integers supported, as ");
#ifdef STLSOFT_CF_32BIT_INT_IS_EXTENDED_TYPE
		fprintf(stdout, "extended type\n");
#else /* ? STLSOFT_CF_32BIT_INT_IS_EXTENDED_TYPE */
		fprintf(stdout, "standard type\n");
#endif /* STLSOFT_CF_32BIT_INT_IS_EXTENDED_TYPE */
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
	fprintf(stdout, "  64-bit integers supported, as ");
# ifdef STLSOFT_CF_64BIT_INT_IS_EXTENDED_TYPE
		fprintf(stdout, "extended type\n");
# else /* ? STLSOFT_CF_64BIT_INT_IS_EXTENDED_TYPE */
		fprintf(stdout, "standard type\n");
# endif /* STLSOFT_CF_64BIT_INT_IS_EXTENDED_TYPE */
#else
	fprintf(stdout, "  64-bit integers not supported\n");
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */
	_type_sizeof(ss_sint8_t);
	_type_sizeof(ss_uint8_t);
	_type_sizeof(ss_sint16_t);
	_type_sizeof(ss_uint16_t);
	_type_sizeof(ss_sint32_t);
	_type_sizeof(ss_uint32_t);
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
	_type_sizeof(ss_sint64_t);
	_type_sizeof(ss_uint64_t);
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */
	_type_sizeof(ss_int_t);
	_type_sizeof(ss_sint_t);
	_type_sizeof(ss_uint_t);
	_type_sizeof(ss_long_t);

	fprintf(stdout, "\n Additional types:\n");
#ifdef __STLSOFT_CF_NATIVE_BOOL_SUPPORT
	fprintf(stdout, "  Native bool supported\n");
#else
	fprintf(stdout, "  Native bool not supported\n");
#endif /* __STLSOFT_CF_NATIVE_BOOL_SUPPORT */
	struct bool_test
	{
#if !defined(_MSC_VER) || \
    _MSC_VER > 1020
		ss_bool_t operator()(ss_bool_t b)
		{
			return b ? ss_false_v : ss_true_v;
		}
#else
		enum { b = (int)(ss_bool_t)(ss_false_v ? ss_false_v : ss_true_v) };
#endif /* _MSC_VER, 1020 */
	};
	_type_sizeof(ss_bool_t);
	_type_sizeof(ss_size_t);
	_type_sizeof(ss_ptrdiff_t);

	fprintf(stdout, "\n Character types:\n");
#ifdef __STLSOFT_CF_NATIVE_WCHAR_T_SUPPORT
	fprintf(stdout, "  Native wchar_t supported\n");
#else
	fprintf(stdout, "  Native wchar_t not supported\n");
#endif /* __STLSOFT_CF_NATIVE_WCHAR_T_SUPPORT */
	_type_sizeof(ss_char_a_t);
	_type_sizeof(ss_char_w_t);
	_type_sizeof(char);
	_type_sizeof(wchar_t);

	fprintf(stdout, "\nTemplates:\n");
#ifdef __STLSOFT_CF_TEMPLATE_SUPPORT
	fprintf(stdout, "  Templates supported\n");
#else
	fprintf(stdout, "  Templates not supported\n");
#endif /* __STLSOFT_CF_TEMPLATE_SUPPORT */

#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_FUNDAMENTAL_ARGUMENT_SUPPORT
	fprintf(stdout, "  Template class default fundamental type argument supported\n");
#else
	fprintf(stdout, "  Template class default argument not supported\n");
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
	fprintf(stdout, "  Member template function supported\n");
#else
	fprintf(stdout, "  Member template function not supported\n");
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT

#ifdef __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX
	fprintf(stdout, "  Template specialisation syntax supported\n");
#else
	fprintf(stdout, "  Template specialisation syntax not supported\n");
#endif /* __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX */


	fprintf(stdout, "\nExceptions:\n");

#ifdef __STLSOFT_CF_EXCEPTION_SIGNATURE_SUPPORT
	fprintf(stdout, "  Exception signature supported\n");
#else
	fprintf(stdout, "  Exception signature not supported\n");
#endif /* __STLSOFT_CF_EXCEPTION_SIGNATURE_SUPPORT */

#ifdef __STLSOFT_CF_THROW_BAD_ALLOC
	fprintf(stdout, "  bad_alloc exception supported\n");
#else
	fprintf(stdout, "  bad_alloc exception not supported\n");
#endif /* __STLSOFT_CF_THROW_BAD_ALLOC */


	fprintf(stdout, "\nNamespaces:\n");

#ifndef _STLSOFT_NO_NAMESPACES
	fprintf(stdout, "  Namespaces supported\n");
#else
	fprintf(stdout, "  Namespaces not supported\n");
#endif /* !_STLSOFT_NO_NAMESPACES */

#ifdef __STLSOFT_CF_std_NAMESPACE
	fprintf(stdout, "  std namespace supported\n");
#else
	fprintf(stdout, "  std namespace not supported\n");
#endif /* __STLSOFT_CF_std_NAMESPACE */


	fprintf(stdout, "\nMiscellaneous:\n");

#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
	fprintf(stdout, "  Function signature full argument qualification required\n");
#else
	fprintf(stdout, "  Function signature full argument qualification not required\n");
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */

#ifdef __STLSOFT_CF_ALLOCATOR_ALLOCATE_HAS_HINT
	fprintf(stdout, "  allocator::allocate has hint\n");
#else
	fprintf(stdout, "  allocator::allocate does not have hint\n");
#endif /* __STLSOFT_CF_ALLOCATOR_ALLOCATE_HAS_HINT */

#ifdef __STLSOFT_CF_ALLOCATOR_DEALLOCATE_HAS_OBJECTCOUNT
	fprintf(stdout, "  allocator::deallocate has object count\n");
#else
	fprintf(stdout, "  allocator::deallocate does not have object count\n");
#endif /* __STLSOFT_CF_ALLOCATOR_DEALLOCATE_HAS_OBJECTCOUNT */

#ifdef __STLSOFT_CF_NEGATIVE_MODULUS_POSITIVE_GIVES_NEGATIVE_RESULT
	volatile int	numerator;
	volatile int	denominator;
	volatile int	modulus;

	numerator	=	-9;
	denominator	=	10;
	modulus		=	numerator % denominator;

	stlsoft_assert(modulus == -9);

	STLSOFT_SUPPRESS_UNUSED(modulus);
#endif // __STLSOFT_CF_NEGATIVE_MODULUS_POSITIVE_GIVES_NEGATIVE_RESULT

    return iRet;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_EXCEPTION_SIGNATURE_SUPPORT
inline void exception_signature_support_func() throw()
{}
#endif /* __STLSOFT_CF_EXCEPTION_SIGNATURE_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_NATIVE_BOOL_SUPPORT
struct native_bool_support
{
	native_bool_support(ss_bool_t /* b */)
	{}
	native_bool_support(ss_int_t /* i */)
	{}
};
#endif /* __STLSOFT_CF_NATIVE_BOOL_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_NATIVE_WCHAR_T_SUPPORT
struct native_wchar_t_support
{
	native_wchar_t_support(ss_char_a_t /* ui16 */)
	{}
	native_wchar_t_support(ss_char_w_t /* c */)
	{}
	native_wchar_t_support(ss_sint16_t /* ui16 */)
	{}
	native_wchar_t_support(ss_uint16_t /* ui16 */)
	{}
	native_wchar_t_support(ss_sint32_t /* ui32 */)
	{}
	native_wchar_t_support(ss_uint32_t /* ui32 */)
	{}
};
#elif defined(__STLSOFT_CF_TYPEDEF_WCHAR_T_SUPPORT)
struct typedef_wchar_t_support
{
	typedef_wchar_t_support(ss_char_a_t /* ui16 */)
	{}
	typedef_wchar_t_support(ss_char_w_t /* c */)
	{}
	typedef_wchar_t_support(ss_sint16_t /* ui16 */)
	{}
	typedef_wchar_t_support(ss_sint32_t /* ui32 */)
	{}
	typedef_wchar_t_support(ss_uint32_t /* ui32 */)
	{}
};
#endif /* __STLSOFT_CF_NATIVE_WCHAR_T_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_NATIVE_8BIT_INT_SUPPORT
struct native_8bit_integer_support
{
	native_8bit_integer_support(char /* i */)
	{}
//	native_8bit_integer_support(ss_int8_pr_t_ /* i */)
//	{}
	native_8bit_integer_support(stlsoft_ns_qual(ss_sint8_pr_t_) /* i */)
	{}
	native_8bit_integer_support(stlsoft_ns_qual(ss_uint8_pr_t_) /* i */)
	{}
};
#endif /* __STLSOFT_CF_NATIVE_8BIT_INT_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
struct native_64bit_integer_support
{
	native_64bit_integer_support(ss_sint8_t /* i8 */)
	{}
	native_64bit_integer_support(ss_sint16_t /* i16 */)
	{}
	native_64bit_integer_support(ss_sint32_t /* i32 */)
	{}
	native_64bit_integer_support(ss_sint64_t /* i64 */)
	{}

	static native_64bit_integer_support fn()
	{
# ifdef STLSOFT_CF_64BIT_INT_IS_long_long
		native_64bit_integer_support	o((signed long long)0);
# elif defined(STLSOFT_CF_64BIT_INT_IS___int64)
		native_64bit_integer_support	o((signed __int64)0);
# else
		fprintf(stdout, "<representation unknown>\n");
# endif /* __STLSOFT_CF_NATIVE_BOOL_SUPPORT */

		return o;
	}
};
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#if 0
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
	fprintf(stdout, "  Exception signature full argument qualification required\n");
#else
	fprintf(stdout, "  Exception signature full argument qualification not required\n");
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
#endif /* 0 */

////////////////////////////////////////////////////////////////////////////////

#ifdef STLSOFT_CF_ANONYMOUS_UNION_SUPPORT
class anon_union_class
{
	union
	{
		int	i;
		long l;
	};

	anon_union_class()
	{
		i = 0;
		l = 1;
	}
};
#endif /* STLSOFT_CF_ANONYMOUS_UNION_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_THROW_BAD_ALLOC
struct throw_bad_alloc
{
	throw_bad_alloc()
	{
		size_t const 	siAlloc	=	0x7fffffff;

		try
		{
			char	*pc	=	new char[siAlloc];

			fprintf(stderr, "operator new[%d] returned: 0x%p\n", siAlloc, pc);
		}
		catch(bad_alloc &x)
		{
			fprintf(stderr, "operator new[%d] throw bad_alloc: %s\n", siAlloc, x.what());
		}
	}

} s_initialiser_tester;
#endif /* __STLSOFT_CF_THROW_BAD_ALLOC */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_FUNDAMENTAL_ARGUMENT_SUPPORT
template<	ss_typename_param_k T1
		,	int I2 = 10
		,	long I3 = 20
		>
struct template_class_default_fundamental_argument_support
{
};
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_FUNDAMENTAL_ARGUMENT_SUPPORT */

#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
template<	ss_typename_param_k T1
		, 	ss_typename_param_k T2 = char const *
        , 	ss_typename_param_k T3 = size_t>
struct template_class_default_class_argument_support
{
};
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
struct member_template_function
{
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
	template <ss_typename_param_k C>
	member_template_function(C const &c)
	{
		int	i	=	c;

		assign(i);
	}
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */

#if 0
	template <ss_typename_param_k T>
	void assign(T const &/* t */)
	{}
#else
	void assign(int const &/* i */)
	{}
#endif /* 0 */

	member_template_function()
	{
		int	i;

		assign(i);
	}
};

template <ss_typename_param_k O>
struct template_member_template_function
{
	template_member_template_function()
	{
		O	o;

		assign(o);
	}

	template <ss_typename_param_k T>
	void assign(T const &/* t */)
	{}
};

static void mtfs_f()
{
	member_template_function				x1;
	template_member_template_function<int>	x2;
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
	char									c = 0;
	member_template_function				x3(c);
	STLSOFT_SUPPRESS_UNUSED(x3);
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */
	STLSOFT_SUPPRESS_UNUSED(x2);
	STLSOFT_SUPPRESS_UNUSED(x1);
}
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CLASS_SUPPORT
struct member_template_class
{
	template <ss_typename_param_k T>
	class member_class
	{
	public:
		member_class(T const &/* t */)
		{}
	};
};

template <ss_typename_param_k O>
struct template_member_template_class
{
	template <ss_typename_param_k T>
	class member_class
	{
	public:
		member_class(T const &/* t */)
		{}
	};
};

static int mtcs_f()
{
	int		i	=	1;

	member_template_class::member_class<int>				x1(i);
	template_member_template_class<int>::member_class<int>	x2(i);

	STLSOFT_SUPPRESS_UNUSED(x2);
	STLSOFT_SUPPRESS_UNUSED(x1);

	return i;
}
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CLASS_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX
template <ss_typename_param_k T>
struct template_specialisation_syntax;

template <>
struct template_specialisation_syntax<int>
{
public:
	template_specialisation_syntax(int )
	{}
};

static template_specialisation_syntax<int> tss_f()
{
	template_specialisation_syntax<int>	x(2);

	return x;
}

#endif /* __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
template<	ss_typename_param_k T1
		,	ss_typename_param_k T2
		>
struct template_partial_specialisation_support
{
	enum { value = sizeof(T1) + sizeof(T2) };
};

template<	ss_typename_param_k T1
		>
struct template_partial_specialisation_support<T1, int>
{
	enum { value = sizeof(T1) * sizeof(int) };
};

static int tpss_f()
{
	size_t	s1	=	template_partial_specialisation_support<long, long>::value;
	size_t	s2	=	template_partial_specialisation_support<long, int>::value;

	return s1 == s2;
}
#endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#ifndef _STLSOFT_NO_NAMESPACES

namespace
{}

#endif /* !_STLSOFT_NO_NAMESPACES */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_std_NAMESPACE
struct std_namespace_test
{
	static void func()
	{
		using namespace std;
	}
};

struct allocator_allocate_has_hint
{
	static void func()
	{
		allocator<char>	char_allocator;

#ifdef __STLSOFT_CF_ALLOCATOR_ALLOCATE_HAS_HINT
		char_allocator.allocate(1, NULL);
#else
		char_allocator.allocate(1);
#endif /* __STLSOFT_CF_ALLOCATOR_ALLOCATE_HAS_HINT */
	}
};

struct allocator_allocate_has_objectcount
{
	static void func()
	{
		allocator<char>	char_allocator;

#ifdef __STLSOFT_CF_ALLOCATOR_DEALLOCATE_HAS_OBJECTCOUNT
		char_allocator.deallocate(NULL, 1);
#else
		char_allocator.deallocate(NULL);
#endif /* __STLSOFT_CF_ALLOCATOR_DEALLOCATE_HAS_OBJECTCOUNT */
	}
};

#endif /* __STLSOFT_CF_std_NAMESPACE */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_EXPLICIT_KEYWORD_SUPPORT
struct explicit_keyword_support
{
	explicit explicit_keyword_support(int /* i */)
	{}
};
#endif /* __STLSOFT_CF_EXPLICIT_KEYWORD_SUPPORT */

#ifdef __STLSOFT_CF_MUTABLE_KEYWORD_SUPPORT
struct mutable_keyword_support
{
	explicit mutable_keyword_support(int i)
		: m_i(i)
	{}

	void func() const
	{
		++m_i;
	}

	mutable int m_i;
};
#endif /* __STLSOFT_CF_MUTABLE_KEYWORD_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_TYPENAME_PARAM_KEYWORD_SUPPORT
template<typename T>
struct typename_param_keyword_support
{
};
#endif /* __STLSOFT_CF_TYPENAME_PARAM_KEYWORD_SUPPORT */


#if defined(__STLSOFT_CF_TYPENAME_TYPE_KEYWORD_SUPPORT) || \
    defined(__STLSOFT_CF_TYPENAME_TYPE_DEF_KEYWORD_SUPPORT)
template<ss_typename_param_k T>
struct typename_type_keyword_support_base
{
	typedef T	type;
};
#endif /* __STLSOFT_CF_TYPENAME_TYPE_KEYWORD_SUPPORT || __STLSOFT_CF_TYPENAME_TYPE_DEF_KEYWORD_SUPPORT */

#ifdef __STLSOFT_CF_TYPENAME_TYPE_KEYWORD_SUPPORT
template<ss_typename_param_k T>
struct typename_type_keyword_support
	: public typename_type_keyword_support_base<T>
{
	typedef typename typename_type_keyword_support_base<T>::type	type;

public:
	typename_type_keyword_support(int )
	{}
};

static typename_type_keyword_support<int> ttks_f()
{
	typename_type_keyword_support<int>	x(2);

	return x;
}

#endif /* __STLSOFT_CF_TYPENAME_TYPE_KEYWORD_SUPPORT */

#ifdef __STLSOFT_CF_TYPENAME_TYPE_DEF_KEYWORD_SUPPORT

template<	ss_typename_param_k T1
		,	ss_typename_param_k T2 = typename T1::type
		>
struct typename_type_def_keyword_support
{
	enum { size = sizeof(T2) };

	typename_type_def_keyword_support()
	{
		size_t s	=	size;

		s = s;
	}
};

static void ttdks_f()
{
	typename_type_def_keyword_support<typename_type_keyword_support_base<int> >	x;

	STLSOFT_SUPPRESS_UNUSED(x);
}

#endif /* __STLSOFT_CF_TYPENAME_TYPE_DEF_KEYWORD_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT
#endif /* __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT

static void koenig_lookup_fn();

namespace koenig_lookup_ns
{
	struct S
	{
	};

	static void f(S &);

	void f(S &)
	{}
}

void koenig_lookup_fn()
{
	koenig_lookup_ns::S s;

	f(s);
}

#endif /* __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#ifdef __STLSOFT_CF_TEMPLATE_TEMPLATE_SUPPORT

static int template_template_fn();


template<class T>
struct template_template_class_1
{
};

template< template< class U> class T>
struct template_template_class_2
{
};

int template_template_fn()
{
	template_template_class_2<template_template_class_1>	x;

	return sizeof(x);
}

#endif /* __STLSOFT_CF_TEMPLATE_TEMPLATE_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#if defined(STLSOFT_CF_MEMBER_CONSTANT_SUPPORT)
struct member_constant_support
{
	static const ss_sint8_t			c1	=	10;
	static const ss_uint32_t		c2	=	1000000;

	member_constant_support()
	{
		printf("Member constants supported\n");
	}

} member_constant_support_instance;
#endif /* STLSOFT_CF_MEMBER_CONSTANT_SUPPORT */

////////////////////////////////////////////////////////////////////////////////

#if defined(x)
struct _8bit_extended_type_support
{
	void f(STLSOFT_CF_STANDARD_INT8_T )
	{}
	void f(STLSOFT_CF_EXTENDED_INT8_T )
	{}

	_8bit_extended_type_support()
	{
		STLSOFT_CF_STANDARD_INT8_T	s = 0;
		STLSOFT_CF_EXTENDED_INT8_T	x = 0;

		f(s);
		f(x);

		printf("Standard and Extended 8-bit integer types are distinct\n");
	}
} _8bit_extended_type_support_instance;

#endif /* STLSOFT_CF_8BIT_INT_EXTENDED_TYPE_IS_DISTINCT */

#if defined(STLSOFT_CF_16BIT_INT_EXTENDED_TYPE_IS_DISTINCT)
struct _16bit_extended_type_support
{
	void f(STLSOFT_CF_STANDARD_INT16_T )
	{}
	void f(STLSOFT_CF_EXTENDED_INT16_T )
	{}

	_16bit_extended_type_support()
	{
		STLSOFT_CF_STANDARD_INT16_T	s = 0;
		STLSOFT_CF_EXTENDED_INT16_T	x = 0;

		f(s);
		f(x);

		printf("Standard and Extended 16-bit integer types are distinct\n");
	}
} _16bit_extended_type_support_instance;

#endif /* STLSOFT_CF_32BIT_INT_EXTENDED_TYPE_IS_DISTINCT */

#if defined(STLSOFT_CF_32BIT_INT_EXTENDED_TYPE_IS_DISTINCT)
struct _32bit_extended_type_support
{
	void f(STLSOFT_CF_STANDARD_INT32_T )
	{}
	void f(STLSOFT_CF_EXTENDED_INT32_T )
	{}

	_32bit_extended_type_support()
	{
		STLSOFT_CF_STANDARD_INT32_T	s = 0;
		STLSOFT_CF_EXTENDED_INT32_T	x = 0;

		f(s);
		f(x);

		printf("Standard and Extended 32-bit integer types are distinct\n");
	}
} _32bit_extended_type_support_instance;

#endif /* STLSOFT_CF_32BIT_INT_EXTENDED_TYPE_IS_DISTINCT */

#if defined(STLSOFT_CF_64BIT_INT_EXTENDED_TYPE_IS_DISTINCT)
struct _64bit_extended_type_support
{
	void f(STLSOFT_CF_STANDARD_INT64_T )
	{}
	void f(STLSOFT_CF_EXTENDED_INT64_T )
	{}

	_64bit_extended_type_support()
	{
		STLSOFT_CF_STANDARD_INT64_T	s = 0;
		STLSOFT_CF_EXTENDED_INT64_T	x = 0;

		f(s);
		f(x);

		printf("Standard and Extended 64-bit integer types are distinct\n");
	}
} _64bit_extended_type_support_instance;

#endif /* STLSOFT_CF_64BIT_INT_EXTENDED_TYPE_IS_DISTINCT */

#if defined(__STLSOFT_CF_INT_DISTINCT_TYPE)
struct _int_is_distinct_from_int32
{
	void f(ss_int32_t )
	{}
	void f(int )
	{}

	_int_is_distinct_from_int32()
	{
		ss_sint32_t	s = 0;
		int			i = 0;

		f(s);
		f(i);

		printf("int and ss_int32_t integer types are distinct\n");
	}
} _int_is_distinct_from_int32_instance;

#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */


////////////////////////////////////////////////////////////////////////////////
