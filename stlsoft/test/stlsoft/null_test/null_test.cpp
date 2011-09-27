/*
 * Updated: 22nd April 2004
 */


#if 0
TABLE HERE OF WHAT WORKS WITH WHICH COMPILER
VC pre 7.1 cannot have the member template operators
GCC 3.2 does not like the ptr-to-member
GCC needs the free operators
Borland needs the free operators, otherwise ambiguities
Digital Mars needs the free operators, otherwise

        if(NULL == pfn1)
                       ^
..\null_test.cpp(84) : Error: need explicit cast to convert
from: void ( D::*member func)(int )
to  : void *

GCC 3.2 and CodeWarrior cannot handle the pointer to member stuff.
#endif /* 0 */




#define DO_OVERLOAD
#define DO_OVERLOAD_2

//#define USE_NULL_C
#define USE_NULL_V

#include <stdio.h>

#include <stlsoft_nulldef.h>
#include <stlsoft_zerodef.h>

static void fn(int /* i */);
#ifdef DO_OVERLOAD
static void fn(char const * /* p */);
#endif // DO_OVERLOAD
#ifdef DO_OVERLOAD_2
static void fn(double /* d */);
#endif // DO_OVERLOAD_2

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER == 1310
//# pragma warning(disable : 4930)
#endif /* _MSC_VER == 1300 */


// Things to object to:
//
// When there are overloads of free functions, the literal 0 becomes static_cast<int>(0)
// Thus if there are overloads and none is of the int type (even if it is short, or something else)
// then we will fail by getting an ambiguous symbol. However, as soon as there is one with an int argument
// it will get precedence over the others, by virtue of the fact that 0 converts to an int literal. This is
// so even when (all but one of the) other function argument types are not of integral type
//
// The second problem is where a

#if 0
# define	TOKEN_FIRST		0
# define	TOKEN_SECOND	1
#else
# define	TOKEN_FIRST		"0"
# define	TOKEN_SECOND	"1"
#endif /* 0 */

class C
{
public:
	explicit C(int)
	{
		printf("C::C(int)\n");
	}
	explicit C(char const *)
	{
		printf("C::C(char const *)\n");
	}
};

static void fn2(void *);
void fn2(void *)
{}

class D
{
public:
	void df0()
	{}
	void df1(int )
	{}
};

template <typename T>
static bool is_zero(T const &t)
{
  return stlsoft_ns_qual(ZERO_v)() == t; // Does not work for pointer types
}


int main()
{
	bool	b					=	0;
	double	*dp					=	NULL;
	int		i					=	ZERO;

#ifdef _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT
	void (D::*pfn0)()			=	&D::df0;
	void (D::*pfn1)(int)		=	&D::df1;

	pfn1	=	NULL;

//	is_zero(dp);

	if(NULL == pfn1)
	{
		b = true;
	}

	if(pfn0 != NULL)
	{
		b = true;
	}
#endif /* _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT */

	if(i == ZERO)
	{
	}

	if(ZERO == i)
	{
	}

	if(i == ZERO)
	{
	}

#if 0
	dp = ZERO;

	if(ZERO == dp)
	{
	}
#endif /* 0 */


	fn2(NULL);

	if(dp == NULL)
	{
		b = true;
	}

	if(NULL == dp)
	{
		b = true;
	}


	fn(0);
	fn(NULL);
//	fn(static_cast<void*>(NULL));

#if 1
	float	*pf	=	NULL;

	if(pf == NULL)
	{
		b = true;
	}

	if(NULL == pf)
	{
		b = true;
	}
#endif /* 0 */

	C	c1(NULL);
	C	c2(ZERO);

	STLSOFT_SUPPRESS_UNUSED(b);
	STLSOFT_SUPPRESS_UNUSED(pf);
#ifdef _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT
	STLSOFT_SUPPRESS_UNUSED(pfn0);
	STLSOFT_SUPPRESS_UNUSED(pfn1);
#endif /* _STLSOFT_NULL_v_DEFINED_PTR_TO_MEMBER_SUPPORT */
	STLSOFT_SUPPRESS_UNUSED(dp);
	STLSOFT_SUPPRESS_UNUSED(i);

	return 0;
}

void fn(int /* i */)
{
	printf("fn(int)\n");
}

#ifdef DO_OVERLOAD
void fn(char const * /* p */)
{
	printf("fn(char const*)\n");
}
#endif // DO_OVERLOAD

#ifdef DO_OVERLOAD_2
void fn(double /* d */)
{
	printf("fn(double)\n");
}
#endif // DO_OVERLOAD_2
