
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stlsoft.h>

#if 0 // Enable this to test the static behaviour of processheap_allocator
#include <stlsoft_allocator_base.h>
#define	__STLSOFT_CF_ALLOCATOR_STATIC_ALLOCATE_METHODS
#endif /* 0 */

#include <winstl.h>
#include <winstl_processheap_allocator.h>	/* winstl::processheap_allocator */
#include <winstl_global_allocator.h>		/* winstl::global_allocator */
#include <winstl_shell_allocator.h>			/* winstl::shell_allocator */
#ifndef __GNUC__
//#ifndef __DMC__								/* std::allocator */
#include <memory>
//#else
//#include <stl_config.h>						/* alloc */
//#include <stl_alloc.h>
//#endif /* __DMC__ */
#endif /* __GNUC__ */

#include <stdio.h>

const size_t	CB_SMALL			=	100;
const size_t	CB_MEDIUM			=	1000000;
const size_t	CB_LARGE			=	1000000000;

typedef struct _ator_test_struct
{
    unsigned long	len;
    char			*buf;

	~_ator_test_struct()
	{
		printf("~ator_test_struct()\n");
	}

} ator_test_struct;

template <class A>
inline void test_allocator(A &a, char const *allocator_name, size_t cUnits)
{
#ifdef __STLSOFT_CF_THROW_BAD_ALLOC
	try
	{
#endif /* __STLSOFT_CF_THROW_BAD_ALLOC */
#ifdef __STLSOFT_CF_ALLOCATOR_ALLOCATE_HAS_HINT
		if(0 != a.allocate(cUnits, 0))
#else
		if(0 != a.allocate(cUnits))
#endif /* __STLSOFT_CF_ALLOCATOR_ALLOCATE_HAS_HINT */
		{
			printf("%s: %u units allocated successfully\n", allocator_name, cUnits);
		}
		else
		{
			printf("%s: %u units could not be allocated. allocate() returned NULL\n", allocator_name, cUnits);
		}
#ifdef __STLSOFT_CF_THROW_BAD_ALLOC
	}
	catch(::std::bad_alloc &x)
	{
		printf("%s: %d units could not be allocated. allocate() threw bad_alloc: %s\n", allocator_name, cUnits, x.what());
	}
#endif /* __STLSOFT_CF_THROW_BAD_ALLOC */
}

int main(int /* argc */, char ** /*argv*/)
{
	winstl_ns_qual(processheap_allocator)<char>	phac;
	winstl_ns_qual(global_allocator)<char>		gac;
	winstl_ns_qual(shell_allocator)<char>		shac;
#ifndef __GNUC__
//#ifndef __DMC__
	winstl_ns_qual_std(allocator)<char>			sac;
//#else
//	alloc										sac;
//#endif /* __DMC__ */
#endif /* __GNUC__ */

	winstl_ns_qual(global_allocator)<ator_test_struct>		atsac;

	test_allocator(phac, (char const *)"processheap_allocator<char>", CB_SMALL);
	test_allocator(phac, (char const *)"processheap_allocator<char>", CB_MEDIUM);
	test_allocator(phac, (char const *)"processheap_allocator<char>", CB_LARGE);

	test_allocator(gac, (char const *)"global_allocator<char>", CB_SMALL);
	test_allocator(gac, (char const *)"global_allocator<char>", CB_MEDIUM);
	test_allocator(gac, (char const *)"global_allocator<char>", CB_LARGE);

	test_allocator(shac, (char const *)"shell_allocator<char>", CB_SMALL);
	test_allocator(shac, (char const *)"shell_allocator<char>", CB_MEDIUM);
	test_allocator(shac, (char const *)"shell_allocator<char>", CB_LARGE);

#ifndef __GNUC__
	test_allocator(sac, (char const *)"allocator<char>", CB_SMALL);
	test_allocator(sac, (char const *)"allocator<char>", CB_MEDIUM);
	test_allocator(sac, (char const *)"allocator<char>", CB_LARGE);
#endif /* __GNUC__ */

	test_allocator(atsac, (char const *)"global_allocator<ator_test_struct>", CB_SMALL);
	test_allocator(atsac, (char const *)"global_allocator<ator_test_struct>", CB_MEDIUM);
	test_allocator(atsac, (char const *)"global_allocator<ator_test_struct>", CB_LARGE);

    return 0;
}
