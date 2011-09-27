/*_ feexcept.c
 * Copyright (C) 1991-2002 by Digital Mars
 * All Rights Reserved
 * www.digitalmars.com
 * Written by Walter Bright
 */

/* C99 floating point environment support functions	*/

#include <float.h>
#include <fenv.h>
#if M_UNIX || M_XENIX
#include <sys/types.h>
#endif
#include <signal.h>
#include "mt.h"

#define FE_ALL_ROUND	0xC00	/* mask for rounding mode		*/
#define FE_ALL_PREC	0x300	/* mask for precision mode		*/

#if !_MT && !(M_UNIX || M_XENIX)
extern fenv_t _fe_cur_env;	/* current floating environment		*/
#endif

/*************************************
 */

int feraiseexcept(int excepts)
{
    if (excepts & FE_ALL_EXCEPT)
    {
#if _MT
	fenv_t fe;

	fegetenv(&fe);
	fe.status |= excepts & FE_ALL_EXCEPT;
	fesetenv(&fe);
#else
	_fe_cur_env.status |= excepts & FE_ALL_EXCEPT;
#endif
	raise(SIGFPE);		/* raise floating point exception	*/
    }
    return (excepts & ~FE_ALL_EXCEPT) == 0;
}

/*************************************
 */

int feupdateenv(const fenv_t *envp)
{   fenv_t fe;
    int result;

    result = fegetenv(&fe);
    if (!result)
    {	result = fesetenv(envp);
	if (!result)
	    result = feraiseexcept(fe.status & FE_ALL_EXCEPT);
    }
    return result;
}

