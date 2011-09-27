/*_ fpsig.c  */
/* Copyright (c) 1995 by Digital Mars		*/
/* All Rights Reserved				*/

/* Function Pointers for Signal handling */



#include <float.h>
#include <signal.h>

/*
   __fp_sigfunc contains a ptr to a routine to handle
   fp exceptions, by default this is SIG_DFL, changed by signal.
*/

void (*__fp_sigfunc)(int,int) = (void (*)(int,int))SIG_DFL;


#if defined(_WIN32)
/* Under WIN32 we can signal illegal instructions and seg violations */
void (*__sigill_sigfunc)(int) = SIG_DFL;
void (*__sigsegv_sigfunc)(int) = SIG_DFL;
#endif

