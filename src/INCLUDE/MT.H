/*_ mt.h */
/* Copyright (C) 1991-1993 by Digital Mars		*/
/* All Rights Reserved				*/
/* Written by Walter Bright			*/
/* $Revision: 1.1.1.1 $      */

/* Multi-threaded library stuff	*/

#pragma once

#if __cplusplus
extern "C" {
#endif

#include <time.h>
#include <float.h>
#include <fltenv.h>
#include <stdio.h>
#include "ehsup.h"

int __cdecl _fputc_nlock(int,FILE *);	/* locked version of fputc	*/
int __cdecl _fgetc_nlock(FILE *);	/* locked version of fgetc	*/
#if __NT__
wint_t __cdecl _fputwc_nlock(wint_t,FILE *);	/* non-locked of fputwc */
wint_t __cdecl _fgetwc_nlock(FILE *);		/* non-locked of fgetwc */
#endif

#if _MT
void __cdecl __fp_lock(FILE *);
void __cdecl __fp_unlock(FILE *);
void _semerr();			/* fatal error with semaphores		*/
#else
#define __fp_lock(fp)
#define __fp_unlock(fp)
#endif

/* Data kept for each thread	*/

#pragma pack(1)

struct THREAD
{
	int t_stackoff;
#if __INTSIZE == 2
	short t_stackseg;	/* stack for this thread		*/
#else
	void * t_parameter;
	void (* t_func)(void *);
#endif
	int t_stacktop;		/* last valid offset for stack		*/
	int t_errno;		/* errno for this thread		*/
#if __INTSIZE == 4
	char *t_strtok;		/* static pointer for strtok()		*/
#else
	char __far *t_strtok;	/* static pointer for strtok()		*/
#endif	
#if __NT__
	unsigned t_id;		/* thread id 				*/
	int t_hndl;		/* thread handle 			*/
	char *t_envptr;		/* pointer to malloced memory for getenv return */
#endif
	struct tm t_tm;		/* static object for localtime()	*/
	fenv_t t_fenv;		/* floating point environment		*/
	char t_asctime[26];	/* static string for asctime()		*/
	char t_digstr[DBL_DIG*2+2];	/* for ecvt()/fcvt()		*/
	int t_doserrno;
#if __NT__
/* see to MT.H(This file), STDIO.ASM, CINIT.ASM */
	wchar_t *t_wcstok;	/* wcstok()		*/
	wchar_t *t_wenvptr;	/* wgetenv()		*/
	wchar_t t_wasctime[26];	/* wasctime()		*/
#endif
   struct Staticvar t_cppeh_sv;   /* Static information for Cplusplus exceptions for each thread */
};

#pragma pack()

struct THREAD *_getthreaddata();
struct THREAD *_addthreadtableentry(unsigned ,void *,void (*)(void *),int);
void _removethreadtableentry(unsigned);
void _thread_init(void);

#define	THREADMAX 	0x400 /* maximum number of threads */

extern	struct THREAD **__thdtbl;	/* RTL threadtable */

/******************************
 * Each OS has a very different API for semaphores, so we
 * isolate them in these macros (not functions because we
 * need speed).
 *	Mutex			Typedef of semaphore
 *	mutex_ctor(Mutex *m)	Semaphore constructor
 *	mutex_dtor(Mutex *m)	Semaphore destructor
 *	mutex_acquire(Mutex *m)	Lock
 *	mutex_release(Mutex *m)	Unlock
 */

#if _MT

#if __NT__

typedef void *Mutex;

extern int ThreadStarted;

#define mutex_ctor(m)	(((*(m) = CreateMutex(NULL,0,0)) != 0) || _semerr())
#define mutex_dtor(m)	(CloseHandle(*(m)) || 1)
//#define mutex_acquire(m) if (ThreadStarted) (WaitForSingleObject(*(m),-1L) && _semerr())
//#define mutex_release(m) if (ThreadStarted) (ReleaseMutex(*(m)) || _semerr())
#define mutex_acquire(m) (WaitForSingleObject(*(m),-1L) && _semerr())
#define mutex_release(m) (ReleaseMutex(*(m)) || _semerr())

#elif __OS2__
#if __INTSIZE == 4

typedef unsigned Mutex;

#define mutex_ctor(m)	(DosCreateMutexSem(NULL,&*(m),0,0) && _semerr())
#define mutex_dtor(m)	(DosCloseMutexSem(*(m)) || _semerr())
#define mutex_acquire(m) (DosRequestMutexSem(*(m),-1L) && _semerr())
#define mutex_release(m) (DosReleaseMutexSem(*(m)) && _semerr())

#else

typedef long Mutex;

#define mutex_ctor(m)	(*(m) = 0)
#define mutex_dtor(m)	((void) 0)
#define mutex_acquire(m) (DosSemRequest((m),-1L) && _semerr())
#define mutex_release(m) (DosSemClear(m) && _semerr())

#endif
#endif

#else

typedef long Mutex;

#define mutex_ctor(m)	((void) 0)
#define mutex_dtor(m)	((void) 0)
#define mutex_acquire(m) ((void) 0)
#define mutex_release(m) ((void) 0)

#endif

#if __cplusplus
}
#endif


