/*_ fltenv.c
 * Copyright (C) 1991-2004 by Digital Mars
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

#if M_UNIX || M_XENIX
fenv_t _fe_cur_env = {	0,
			(0x1000 | FE_TONEAREST | FE_LDBLPREC | FE_ALL_EXCEPT),
			FE_TONEAREST,
			0,
			0 };

fenv_t _FE_DFL_ENV = {	0,
			(0x1000 | FE_TONEAREST | FE_LDBLPREC | FE_ALL_EXCEPT),
			FE_TONEAREST,
			0,
			0 };
#endif

/*************************************
 */

int fetestexcept(int excepts)
{   fenv_t fe;

    fegetenv(&fe);
    return fe.status & excepts;
}

/*************************************
 */

int feclearexcept(int excepts)
{   fenv_t fe;

    fegetenv(&fe);
    fe.status &= ~(excepts & FE_ALL_EXCEPT);
    return fesetenv(&fe);
}

/*************************************
 */

#if 0
int fegetexcept(fexcept_t *flagp,int excepts)
{
    *flagp = excepts;
    return (excepts & ~FE_ALL_EXCEPT) == 0;
}
#endif

/*************************************
 */

int fegetexceptflag(fexcept_t *flagp, int excepts)
{
    *flagp = excepts;
    return 0;
}

/*************************************
 */

#if 0
int fesetexcept(const fexcept_t *flagp,int excepts)
{
#if _MT
    fenv_t fe;

    fegetenv(&fe);
    fe.status |= *flagp & FE_ALL_EXCEPT;
    fesetenv(&fe);
#else
    _fe_cur_env.status |= *flagp & FE_ALL_EXCEPT;
#endif
    return (excepts & ~FE_ALL_EXCEPT) == 0;
}
#endif

/*************************************
 */

int fesetexceptflag(const fexcept_t *flagp,int excepts)
{
#if _MT
    fenv_t fe;

    fegetenv(&fe);
    fe.status |= *flagp & FE_ALL_EXCEPT;
    fesetenv(&fe);
#else
    _fe_cur_env.status |= *flagp & FE_ALL_EXCEPT;
#endif
    return 0;
}

/*************************************
 */

int fegetround()
{   fenv_t fe;

    fegetenv(&fe);
    return fe.control & FE_ALL_ROUND;
}

/*************************************
 */

int fesetround(int round)
{
    if (!(round & ~FE_ALL_ROUND))	/* if valid rounding mode	*/
    {   fenv_t fe;

	fegetenv(&fe);
	fe.control = (fe.control & ~FE_ALL_ROUND) | round;
	fe.round = round;
	fesetenv(&fe);
	return 0;			/* valid rounding mode		*/
    }
    else
	return 1;			/* invalid rounding mode	*/
}

/*************************************
 */

int fegetprec()
{   fenv_t fe;

    fegetenv(&fe);
    return fe.control & FE_ALL_PREC;
}

/*************************************
 */

int fesetprec(int prec)
{
    /* Note that the emulator does not support different precisions	*/
    if (prec & ~FE_ALL_PREC || _8087 == 0)
	return 0;			/* invalid precision		*/
    else
    {	fenv_t fe;

	fegetenv(&fe);
	fe.control = (fe.control & ~FE_ALL_PREC) | prec;
	fesetenv(&fe);
	return 1;
    }
}

/*************************************
 */

int fegetenv(fenv_t *envp)
{
    /* Update the environment with anything from the 8087	*/
#if _MT
    fenv_t *pfe;

    pfe = &_getthreaddata()->t_fenv;
#define _fe_cur_env (*pfe)
#endif
    if (_8087)
    {
	/* Read 8087 status, clear on-chip exceptions	*/
	_fe_cur_env.status = (_fe_cur_env.status & FE_ALL_EXCEPT) | _clear87();

	/* Read 8087 control word without changing it	*/
	_fe_cur_env.control = _control87(0,0);

	/* Having a separate copy of the rounding mode is convenient	*/
	_fe_cur_env.round = _fe_cur_env.control & FE_ALL_ROUND;
    }
    *envp = _fe_cur_env;
    return 0;
}

/*************************************
 */

int fesetenv(const fenv_t *envp)
{
#if _MT
    fenv_t *pfe;

    pfe = &_getthreaddata()->t_fenv;
#define _fe_cur_env (*pfe)
#endif
    _fe_cur_env = *envp;
    /* Update the 8087 with the new environment	*/
    if (_8087)
    {
	/* Only support changing precision and rounding controls	*/
	_control87(_fe_cur_env.control,FE_ALL_PREC | FE_ALL_ROUND);
    }
    return 0;
}

/*************************************
 */

int feholdexcept(fenv_t *envp)
{
    fegetenv(envp);		/* save current environment	*/
    fesetenv(FE_DFL_ENV);	/* install default environment	*/
    return 0;
}

