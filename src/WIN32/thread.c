/*_ thread.c
 * Copyright (C) 1991-1992 by Digital Mars
 * All rights reserved.
 * Written by Bill Andrews
 */



#include  <stdlib.h>
#include <errno.h>
#include <fltenv.h>
#include "mt.h"
#include <scrtl.h>

#ifdef _WIN32
#include <windows.h>

#include "dbgint.h"



DWORD WINAPI __threadstart (void *param);
void _endthread(void);
extern CRITICAL_SECTION __rtl_criticalsection;  /* Critical Section to be used by RTL */



/********************************;
* Start a new thread
* Returns:
* handle of new thread
* -1 if error, and errno is set
*/

int ThreadStarted = 0;

unsigned long _beginthread (void (*start_addr)(void *), unsigned stack_size, void *arglist)
{
    unsigned long th_id;
    HANDLE th_hndl;
    int i;
    struct THREAD *thd;
    int retval;

    ThreadStarted = 1;

    /* Force this routine to be considered a critical section
       so that when reentered, threads are not stepping on the
       same thdtbl entry */
    EnterCriticalSection(&__rtl_criticalsection);

    for (i = 0; i < THREADMAX; i++) 
    {
        if (__thdtbl[i] == 0)
        {
            __thdtbl[i] = 0;
            break;
        }
    }
    if (i == THREADMAX)
    {
        __set_errno (EAGAIN);
        retval = -1;
        goto end;
    }
    thd = _calloc_crt (sizeof (struct THREAD), 1);
    if (thd == 0)
    {
        __set_errno (ENOMEM);
        retval = -1;
        goto end;
    }
    th_hndl = CreateThread (NULL,stack_size,__threadstart,arglist,CREATE_SUSPENDED,&th_id);
    if (th_hndl == 0) 
    {
        _free_crt (thd);
        __set_nt_errno ();
        retval = -1;
        goto end;
    }
    
    __thdtbl[i] = thd;
    thd->t_id = th_id;
    thd->t_parameter = arglist;
    thd->t_func = start_addr;
    thd->t_hndl = (int)th_hndl;

    ResumeThread (th_hndl);
    retval = (unsigned long)th_hndl;
end:
    LeaveCriticalSection(&__rtl_criticalsection);
    return retval;
}



/**************************************
* Execution of the thread always starts here.
*/

DWORD WINAPI __threadstart (void *param)
{
    struct THREAD *thd;

    static unsigned short fpucw = 0x137F;
    __asm
    {
	asm	fldcw fpucw;
    }

    thd = _getthreaddata();
    thd->t_fenv = *FE_DFL_ENV;
    (*thd->t_func)(thd->t_parameter);
     _endthread ();
}

/*****************************************
* Terminate a thread.
*/

void _endthread()
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

    if (thd->t_hndl && thd->t_hndl != -1)
        CloseHandle ((HANDLE)thd->t_hndl);

    _free_crt (thd);
    ExitThread (0);
}

#endif
