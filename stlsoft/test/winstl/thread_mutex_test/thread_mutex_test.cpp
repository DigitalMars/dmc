
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <winstl.h>

#include <winstl_thread_mutex.h>
#include <stlsoft_null_mutex.h>
#include <stlsoft_lock_scope.h>
//#include <stlsoft_synchronisation_functions.h>

int main(int argc, char *argv[])
{
	int					iRet	=	0;
	int					i;

	for(i = 1; i < argc; ++i)
	{
		const char	*arg	=	argv[i];

		if(arg[0] == '-')
		{
			if(arg[1] == '-')
			{
				/* -- arguments */
			}
			else
			{
				/* - arguments */
			}
		}
		else
		{
			/* other arguments */
		}
	}

	/* . */
	winstl_ns_qual(thread_mutex)	mx_w;
    stlsoft_ns_qual(null_mutex)	    mx_s;

	{
		stlsoft_ns_qual(lock_scope)	<	winstl_ns_qual(thread_mutex)
#ifndef __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT
									,	winstl_ns_qual(thread_mutex_lock_traits)
#endif /* !__STLSOFT_CF_KOENIG_LOOKUP_SUPPORT */
									>						scope(mx_w);

		{
			stlsoft_ns_qual(lock_scope)	<	winstl_ns_qual(thread_mutex)
#ifndef __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT
										,	stlsoft_ns_qual(lock_traits_inverter)<winstl_ns_qual(thread_mutex_lock_traits)>
#else
										,	stlsoft_ns_qual(lock_invert_traits)<winstl_ns_qual(thread_mutex)>
#endif /* !__STLSOFT_CF_KOENIG_LOOKUP_SUPPORT */
										>					scope(mx_w);
		}
	}

	{
		stlsoft_ns_qual(lock_scope)<	stlsoft_ns_qual(null_mutex)
#ifndef __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT
									,	stlsoft_ns_qual(lock_traits)<stlsoft_ns_qual(null_mutex)>
#endif /* !__STLSOFT_CF_KOENIG_LOOKUP_SUPPORT */
										>	scope(mx_s);

		{
			stlsoft_ns_qual(lock_scope)	<	stlsoft_ns_qual(null_mutex)
										,	stlsoft_ns_qual(lock_invert_traits)<stlsoft_ns_qual(null_mutex)>
										>	scope(mx_s);
		}
	}

    return iRet;
}
