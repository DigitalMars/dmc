/*_ signal.c   Modified by: Walter Bright */
/* Copyright (c) 1987-1994 by Digital Mars		*/
/* All Rights Reserved				*/
/* Written by Walter Bright			*/

/* Signal handling routines			*/

#include	<signal.h>
#include	<errno.h>
#if M_UNIX || M_XENIX
#include <sys/types.h>
#else
#include <dos.h>
#endif
#ifndef _WIN32
#include	<int.h>
#else
#include <windows.h>
#endif
#include	<process.h>
#include	<stdlib.h>
#include <float.h>  /* for floating point error codes */
#include        <scrtl.h>


static void (*sighandler[])(int) =
{	SIG_DFL,SIG_DFL,SIG_DFL,
	SIG_DFL,SIG_DFL,SIG_DFL,
	SIG_DFL,SIG_DFL,
};

#define MAX_SIG  (sizeof(sighandler)/sizeof(sighandler[0]))

static int _checktable [MAX_SIG] = {
SIGABRT,SIGFPE,SIGILL,SIGINT,
SIGSEGV,SIGTERM,SIGTERM,SIGBREAK };

#if _WIN32
static void (*ctrl_b_handler)(int) = 0; /* CTRL-BREAK */
static void (*ctrl_c_handler)(int) = 0; /* CTRL-C */
static int _ctrl_handler_initialized = 0; /* have we registered with WIN32 or not */

/* Special function pointers to trap Illegal Instructions and Segment violations */
extern void (* __sigill_sigfunc)(int);
extern void (* __sigsegv_sigfunc)(int);



static BOOL WINAPI console_ctrl_handler(DWORD type)
{
    void (*handler)(int) = ctrl_b_handler;
    BOOL retval = TRUE;
    int sig = SIGBREAK;
        
    if (type == CTRL_C_EVENT)
    {
        handler = ctrl_c_handler;
        sig = SIGINT;
    }
    
    if (!handler)
    {
        retval = FALSE;
    }
    else if (handler != SIG_IGN)
    {
        signal(sig,SIG_DFL);   /* reset to default */
        (*handler)(sig);
    }
        
    return retval; 
}
	
#elif __OS2__ && __INTSIZE == 4
	/* not implemented	*/
#elif __OS2__

/* Type of OS/2 signal handler	*/
typedef void (__pascal __far __loadds *sh)(unsigned short,unsigned short);

extern int __far __pascal DosSetSigHandler(sh,sh,unsigned short __far *,unsigned short,unsigned short);

/***************************
 * Convert from OS/2 signal number to our signal number.
 */

static int os2_sig[] = {
	-1,			/* not used			*/
	SIGINT,			/* corresponding to ^C		*/
	-1,			/* broken pipe			*/
	SIGTERM,		/* program terminated		*/
	SIGBREAK,		/* control-break		*/
	-1,			/* event flag A			*/
	-1,			/* event flag B			*/
	-1,			/* event flag C			*/
};



/***************************
 * Signal handler for OS/2
 */

void __pascal __far __loadds os2sighandler(unsigned short SigArg, unsigned short SigNum)
{   void (*handler)(int);
    int signum;

    DosSetSigHandler(os2sighandler,0L,NULL,4,SigNum);	/* acknowledge	*/
    signum = os2_sig[SigNum];			/* translate		*/
    if (signum != -1)
    {
	handler = sighandler[signum];
	signal(signum,SIG_IGN);			/* reset to default	*/
	if (handler != SIG_DFL && handler != SIG_IGN)
	    (*handler)(signum);
    }
}

#else

static void (*cb_handler)(int) = 0;

static int ctrl_break_handler(struct INT_DATA *id)
{	void (*handler)(int);

	handler = cb_handler;
	signal(SIGINT,SIG_IGN);		/* reset to default		*/
	if (handler && handler != SIG_IGN)
	    (*handler)(SIGINT);
	return 1;			/* return from interrupt	*/
}

#endif

/*************************
 * Convert from signal number to signal index.
 */

static int __near check_sig(int sig)
{	int i;

	for (i=0;i<MAX_SIG;i++)
	{
		if (sig == _checktable[i])
			break;
	}
	return i;
}

/****************************
 * Set handler for a signal.
 */

void (*signal(sig,func))(int)
int sig;
void (*func)(int);
{	void (*previous)(int);
	int signum;

	signum = check_sig(sig);
	if ((unsigned) signum >= MAX_SIG)
	{	__set_errno (EINVAL);		
		return SIG_ERR;		/* unsuccessful			*/
	}
	previous = sighandler[signum];

#if _WIN32
   if (sig == SIGINT || sig == SIGBREAK)		/* if control-c	or conrol-break	*/
	{
	    if (_ctrl_handler_initialized)		
           SetConsoleCtrlHandler(console_ctrl_handler,FALSE);

       _ctrl_handler_initialized = 0;
       
       if (func != SIG_DFL)
       {
		     if (SetConsoleCtrlHandler(console_ctrl_handler,TRUE) == FALSE)
		     {
		        __set_nt_errno();
		        return SIG_ERR;
		     }
		     _ctrl_handler_initialized = 1;
      }
		
		if (sig == SIGBREAK)
		   ctrl_b_handler = (func == SIG_DFL) ? 0 : func;
		else
		   ctrl_c_handler = (func == SIG_DFL) ? 0 : func;
	}
#elif __OS2__ && __INTSIZE == 4
	/* not implemented	*/
#elif __OS2__
	{
	    int result;
	    int action;
	    int os2sig;

	    /* Determine OS/2 equivalents	*/
	    switch (sig)
	    {   case SIGINT:	os2sig = 1;	break;
		case SIGBREAK:	os2sig = 4;	break;
		case SIGTERM:	os2sig = 3;	break;
		default:
		    goto L1;
	    }
	    if (func == SIG_DFL)
		action = 0;
	    else if (func == SIG_IGN)
		action = 1;
	    else
		action = 2;

	    result = DosSetSigHandler(os2sighandler,0,NULL,action,1);
	    if (result)
	    {	__set_errno (result);
		return SIG_ERR;
	    }
	}
    L1: ;
#else
	if (sig == SIGINT)		/* if control-break		*/
	{
	    if (cb_handler)		/* if already a handler		*/
		int_restore(0x23);	/* restore previous handler	*/
	    cb_handler = 0;
	    if (func != SIG_DFL)
	    {
		if (int_intercept(0x23,ctrl_break_handler,1000) != 0)
		{   /* failed to intercept the interrupt	*/
		    __set_errno (ENOMEM);	/* probably out of memory	*/
		    return SIG_ERR;
		}
		cb_handler = func;
	    }
	}
#endif
   if (sig == SIGFPE)
   {
      __fp_sigfunc = (void (*)(int,int))func;  /* setup up global FPE handler */
   }

#if defined(_WIN32)
   if (sig == SIGSEGV)
   {
       __sigsegv_sigfunc = func;
   }
   
   if (sig == SIGILL)
   {
       __sigill_sigfunc = func;
   }
#endif
	sighandler[signum] = func;
	return previous;
}

/***********************
 * Raise signal sig.
 * Input:
 *	sig	SIGXXXX
 * Returns:
 *	0	successful
 *	!= 0	unsuccessful
 */

int raise(sig)
int sig;
{	void (*func)(int);
	int signum;

	signum = check_sig(sig);
	if ((unsigned) signum >= MAX_SIG)
		return 1;		/* unsuccessful			*/
	func = sighandler[signum];
	sighandler[signum] = SIG_DFL;	/* reset to default		*/
	if (func == SIG_DFL)		/* if default			*/
	{   switch (sig)
	    {
		case SIGFPE:
		    break;		/* ignore			*/
		case SIGABRT:
				/* NOTE: faterr will call exit, but if it returns, call exit */
				__faterr("\nabnormal program termination\n");
				_Exit(EXIT_FAILURE);
		case SIGILL:
		case SIGINT:
		case SIGSEGV:
		case SIGTERM:
#if defined(_WIN32)
		case SIGBREAK:
#endif
		default:
		    _Exit(EXIT_FAILURE); /* abort with error		*/
	    }
	}
   else if (func != SIG_IGN)	/* else if not ignored		*/
   {
       if (sig == SIGFPE)
       { 
         __fp_sigfunc = (void (*)(int,int))SIG_DFL;
         (*(void (*)(int,int))func)(sig,_FPE_EXPLICITGEN);
       }
       else
       {
#if defined(_WIN32)
         if (sig == SIGSEGV)
           __sigsegv_sigfunc = SIG_DFL;

         if (sig == SIGILL)
           __sigill_sigfunc = SIG_DFL;
#endif
	     (*func)(sig);
       }
   }
  return 0;
}

#ifdef _WIN32
void _STI_signal()
{
}

void _STD_signal()
{
   signal(SIGINT,SIG_DFL);
   signal(SIGBREAK,SIG_DFL);
}
#endif

/**********************
 * Exit with an error level of 1.
 */

void abort()
{
	raise(SIGABRT);
}




