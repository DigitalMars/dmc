/* _ thsup.c
 * Copyright (C) 1995-2003 by Digital Mars
 * All rights reserved.
 * www.digitalmars.com
 */
 
#include <windows.h> 
#include <stdlib.h>
#include <errno.h>
#include <scrtl.h>
#include "mt.h"

#undef AX

#include "dbgint.h"

extern struct THREAD _thread1;

struct THREAD **__thdtbl;

static struct THREAD *threadtable[THREADMAX + 1];

#if !_MT
extern fenv_t _fe_cur_env;
#endif

/******************************************
 * Initialize for multithreaded operation.
 */

void _thread_init()
{
    __thdtbl = &threadtable[0];
    __thdtbl[1] = &_thread1;
}


/************************************************
 *  Using the parameters, insert a new record into the thread table.
 *  Returns:
 *     Success : pointer to record added, 0:failure
 */
 
 
struct THREAD *_addthreadtableentry(unsigned id,void *arglist,void (* startaddr)(void *),int thhndl)
{
    int i;
    struct THREAD *thd=NULL;
    
    /* Loop through table, stopping on first empty slot, or if id is found */
    for (i = 0; i < THREADMAX; i++) 
    {
        /* if found an empty slot, then break */
	    if (__thdtbl[i] == 0)
	    {
	        break;
	    }
	    /* id already in table, do nothing and return success */
	    else if (__thdtbl[i]->t_id == id)
	    {
	       thd = __thdtbl[i];
	       goto end;
	    }
    }
    
    if (i == THREADMAX)
    {
        __set_errno (EAGAIN);
        goto end;
    }
    
    thd = _calloc_crt(sizeof (struct THREAD), 1);
    if (!thd)
    {
        __set_errno (ENOMEM);
        goto end;
    }
    
    __thdtbl[i] = thd;
    thd->t_id = id;
    thd->t_parameter = arglist;
    thd->t_func = startaddr;
    thd->t_hndl = (int)thhndl;
end:
    return thd;
    
}

/*******************************************************
 * Remove the record represented by id from thread table,
 * and free associated memory... 
 */
void _removethreadtableentry(unsigned id)
{
    int i;
    struct THREAD *thd = NULL;
    
    for (i=0; 1; i++)
    {
	if (i == THREADMAX)
	    return;    // not found, simply return
        
        if (__thdtbl[i] && __thdtbl[i]->t_id == id)
        {
            thd = __thdtbl[i];
            __thdtbl[i] = NULL;
            break;
        }
    }
    
    _free_crt(thd->t_envptr);   
    if (thd->t_hndl)
        CloseHandle((HANDLE)thd->t_hndl);
    _free_crt(thd);
}


/*********************************
 * Get pointer to thread data for current thread.
 *	struct THREAD *_getthreaddata();
 * Returns:
 *	NULL	can't get thread data
 *	EAX is only register affected
 */

#ifdef _MT
__declspec(naked) struct THREAD *_getthreaddata()
{
    asm
    {
	push	EBX			// save registers
	push	ECX
	push	EDX

	call	GetCurrentThreadId
	mov	ECX,THREADMAX+1
	mov	EBX,__thdtbl

gidlp:
	dec	ECX
	je	gadd

	mov	EDX,[EBX]		// search thread table for this ID
	add	EBX,4

	test	EDX,EDX
	je	gidlp

	cmp	EAX,t_id[EDX]
	jne	gidlp

	mov	EAX, EDX		// found - return pointer to it
gdun:
	pop	EDX
	pop	ECX
	pop	EBX
	ret

gadd:
	push	ECX			// ECX is 0
	push	ECX
	push	ECX
	push	EAX
	call	_addthreadtableentry
	add	ESP,16

	jmp	gdun
    }
}
#else
struct THREAD *_getthreaddata()
{
    return &_thread1;			// single threaded
}
#endif


/*********************************
 * Get pointer to current thread number.
 * Returns:
 *	EAX is only register modified
 */

#ifdef _MT
__declspec(naked) int __threadid()
{
    asm
    {
	push	EBX			// save registers
	push	ECX
	push	EDX
	push	EDI

	call	GetCurrentThreadId
	mov	ECX, THREADMAX
	mov	EBX, __thdtbl

idlp:
	mov	EDI,[EBX]		// search thread table for this ID
	add	EBX, 4
	test	EDI,EDI
	je	next_id

	cmp	EAX, t_id[EDI]
	jne	next_id

	lea	EAX, t_id[EDI]		// found - return pointer to it
dun:
	pop	EDI
	pop	EDX
	pop	ECX
	pop	EBX
	ret

next_id:
	loop	idlp

	xor	EAX,EAX
	jmp	dun
    }
}
#endif


/*********************************
 * Set errno based on value in EAX.
 * Preserve contents of all registers and flags.
 */

__declspec(naked) void _errno_set()
{
    asm
    {
#ifdef _MT
	pushf
	push	EAX
	push	EAX
	call	_getthreaddata
	pop	dword ptr t_errno[EAX]
	pop	EAX
	popf
#else
#undef errno
	mov	errno,EAX	// for link compatibility with non-_MT code
#endif
	ret
    }
}


/*********************************
 * Retrieve pointer to where errno is stored.
 */

int *_errno()
{
#ifdef _MT
    return &_getthreaddata()->t_errno;
#else
#undef errno
    return &errno;
#endif
}

/*********************************
 * Retrieve pointer to where _doserrno is stored.
 */

int *__doserrno()
{
#ifdef _MT
    return &_getthreaddata()->t_doserrno;
#else
#undef _doserrno
    return &_doserrno;
#endif
}


/**********************************
 * Set exception bits.
 * No registers modified.
 */

__declspec(naked) void _FEEXCEPT(unsigned bits)
{
    asm
    {
	push	EAX
#ifdef _MT
	push	EBX
	call	_getthreaddata
	mov	EBX,bits+4[ESP]		// get exception bits
	or	t_fenv+status[EAX],BX
	pop	EBX
#else
	mov	EAX,bits[ESP]
	or	_fe_cur_env.status,AX
#endif
	pop	EAX
	ret	4
    }
}

/**********************************
 * Get rounding mode in EAX.
 * No registers modified.
 */

__declspec(naked) unsigned _FEROUND()
{
    asm
    {
#ifdef _MT
	call	_getthreaddata
	movzx	EAX,word ptr t_fenv+round[EAX]
#else
	movzx	EAX,_fe_cur_env.round
#endif
	ret
    }
}

