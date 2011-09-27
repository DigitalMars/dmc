/*_ xcfilter.c
 * Copyright (C) 1995 by Digital Mars Corporation
 * All Rights Reserved
 */

#include "ehsup.h"
#include <scrtl.h>
#include <float.h>
#include <signal.h>

/* Special function pointers to trap Illegal Instructions and Segment violations */
extern void (* __sigill_sigfunc)(int);
extern void (* __sigsegv_sigfunc)(int);

/* 
  Top level exception filter.
 */
long __cdecl __xcfilter (LPEXCEPTION_POINTERS eptr)
{
   
   int fpcode = 0; 
   void (*handler)(int) = SIG_DFL;
   int sig = 0;

   /* Check for possible signal handlers from user */
   switch (eptr->ExceptionRecord->ExceptionCode)
   {
        case STATUS_FLOAT_DENORMAL_OPERAND:
             fpcode = _FPE_DENORMAL;
             goto fp_handler;
        case STATUS_FLOAT_DIVIDE_BY_ZERO:
             fpcode = _FPE_ZERODIVIDE;
             goto fp_handler;
        case STATUS_FLOAT_INEXACT_RESULT:
             fpcode = _FPE_INEXACT;
             goto fp_handler;
        case STATUS_FLOAT_INVALID_OPERATION:
             fpcode = _FPE_INVALID;
             goto fp_handler;
        case STATUS_FLOAT_OVERFLOW:
             fpcode =_FPE_OVERFLOW;
             goto fp_handler;
        case STATUS_FLOAT_STACK_CHECK:         
             fpcode = _FPE_STACKOVERFLOW;
             goto fp_handler;
        case STATUS_FLOAT_UNDERFLOW:
             fpcode = _FPE_UNDERFLOW;
       fp_handler:
             sig = SIGFPE;
             handler = (void (*)(int))__fp_sigfunc;
             __fp_sigfunc = (void (*)(int,int))SIG_DFL;
             break;
        case STATUS_ACCESS_VIOLATION:
             sig = SIGSEGV;
             handler = __sigsegv_sigfunc;
             __sigsegv_sigfunc = SIG_DFL;
             break;
        case STATUS_ILLEGAL_INSTRUCTION:
        case STATUS_PRIVILEGED_INSTRUCTION:
             sig = SIGILL;
             handler = __sigill_sigfunc;
             __sigill_sigfunc = SIG_DFL;
             break;
        default:
             handler = SIG_DFL;
             break;
   }

   /* If there is a signal handler, call it and return to execution */
   if (handler != SIG_DFL)
   {

       if (sig == SIGFPE)
         (*((void (*)(int,int))handler))(sig,fpcode); /* signal user */
       else
         (*handler)(sig);
       
       /* exception handled, tell Win32 to continue executing */  
       return EXCEPTION_CONTINUE_EXECUTION;
   }
   /* Otherwise, signal an unhandled exception to the system */
   else 
   {
     return UnhandledExceptionFilter(eptr);
   }
   
}	                          
