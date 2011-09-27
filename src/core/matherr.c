/*_ matherr.c   Sat Apr 15 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/

#include	<stdio.h>
#include	<errno.h>
#include	<math.h>
#include	<string.h>
#include        <fenv.h>
#include        <scrtl.h>

/*****************************
 * Default exception handling routine.
 * Returns:
 *	0	error
 *	1	error handled successfully
 */

#if _MT
extern void _pascal __FEEXCEPT(int e);
#define feexcept(e)	(__FEEXCEPT(e))
#else
extern fenv_t _fe_cur_env;
#define feexcept(e)	(_fe_cur_env.status |= (e))
#endif

//extern fenv_t _fe_cur_env;

int matherr(struct exception *pexc)
{
#if 0
     static char *typemsg[] =
	{	"argument domain",		/* DOMAIN		*/
		"argument singularity",		/* SING			*/
		"overflow",			/* OVERFLOW		*/
		"underflow",			/* UNDERFLOW		*/
		"total loss of significance",	/* TLOSS		*/
		"partial loss of significance"	/* PLOSS		*/
	};

#endif
   static int errorArray[] =
        {
            FE_INVALID,   /* DOMAIN          */
            FE_INVALID,   /* SING            */
            FE_OVERFLOW,  /* OVERFLOW        */
            FE_UNDERFLOW, /* UNDERFLOW       */
            0,            /* TLOSS           */
            0,            /* PLOSS           */
            FE_DIVBYZERO  /* DIVIDE_BY_ZERO  */
	};


        static int errnotab[] =
        {       EDOM,EDOM,ERANGE,ERANGE,ERANGE,ERANGE,EDOM   };

#if 0
#if sizeof(typemsg)/sizeof(char *) != sizeof(errnotab)/sizeof(int)
	#error "array size mismatch"
#endif
#endif
	/* ANSI C mandates that these messages be suppressed. *sigh* */
#if 0
        printf(
	    "Floating point exception: %s(%g",pexc->name,pexc->arg1);
	if (!strcmp(pexc->name,"pow") || !strcmp(pexc->name,"atan2"))
	    printf(
		",%g) %s\n",pexc->arg2,typemsg[pexc->type - 1]);
	else
	    printf(") %s\n",typemsg[pexc->type - 1]);
#endif

    feexcept(errorArray[pexc->type - 1]);
//  _fe_cur_env.status |= errorArray[pexc->type - 1];
	__set_errno (errnotab[pexc->type - 1]);
	return 0;
}
