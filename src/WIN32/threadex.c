/*_ threadex.c
 * Copyright (C) 1994 by Digital Mars
 * All rights reserved.
 */



#include <stdlib.h>
#include <errno.h>
#include <fltenv.h>
#include "mt.h"
#include <scrtl.h>

#ifdef _WIN32
#include <windows.h>

#include "dbgint.h"

DWORD WINAPI __threadstartex (void *param);
void _endthreadex(unsigned);
extern CRITICAL_SECTION __rtl_criticalsection;  /* Critical Section to be used by RTL */


/********************************;
* Start a new thread
* Returns:
* handle of new thread
* 0 if error,  errno and doserrno is set
*/
unsigned long _beginthreadex (void *security, unsigned stack_size, 
     unsigned (__stdcall *start_addr)(void *), void *arglist, unsigned initflag, unsigned *thrdaddr)
{
    unsigned long th_id;
    HANDLE th_hndl;
    int i;
    unsigned long retval;
    struct THREAD *thd;

#if defined(_MT)
    ThreadStarted = 1;
#endif

    /* Force this routine to be considered a critical section
       so that when reentered, threads are not stepping on the
       same thdtbl entry */
    EnterCriticalSection(&__rtl_criticalsection);

    for (i = 0; i < THREADMAX; i++) 
    {
        if (__thdtbl[i] == 0)
            break;
    }
    
    if (i == THREADMAX)
    {
        __set_errno (EAGAIN);
        retval = 0;
        goto end;
    }
    thd = _calloc_crt (sizeof (struct THREAD), 1);
    if (thd == 0)
    {
        __set_errno (ENOMEM);
        retval = 0;
        goto end;
    }
    th_hndl = CreateThread (security, stack_size, __threadstartex, arglist, 
    initflag | CREATE_SUSPENDED, &th_id);
    if (th_hndl == 0) 
    {
        _free_crt (thd);
        __set_nt_errno ();
        retval = 0;
        goto end;
    }

    __thdtbl[i] = thd;
    thd->t_id = *thrdaddr = th_id;
    thd->t_parameter = arglist;
    thd->t_func = (void (*)(void *))start_addr;
    thd->t_hndl = (int)th_hndl;

    if (!(initflag & CREATE_SUSPENDED))  
        ResumeThread (th_hndl);
    retval = (unsigned long)th_hndl;
end:
    LeaveCriticalSection(&__rtl_criticalsection);
    return retval;
}



/**************************************
* Execution of the thread always starts here.
*/

DWORD WINAPI __threadstartex (void *param)
{
    struct THREAD *thd;
    unsigned exit_code = 0;

    thd = _getthreaddata();
    thd->t_fenv = *FE_DFL_ENV;
    exit_code = ((unsigned (__stdcall *)(void *))thd->t_func)(thd->t_parameter);
    _endthreadex (exit_code);
}

/*****************************************
* Terminate a thread.
*/

void _endthreadex(unsigned retval)
{
    char * env;
    struct THREAD *thd;
    int i;

    thd = _getthreaddata();
    env = thd->t_envptr;
    if (env != 0)
        _free_crt (env);

    for (i = 0; i < THREADMAX; i++) 
    {
        if (__thdtbl[i] == thd)
        {
            __thdtbl[i] = 0;
            break;
        }
    }
    _free_crt (thd);
    ExitThread (retval);
}

#endif
