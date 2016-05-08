/*_ fplock.c   Sat Sep	1 1990	 Modified by: Walter Bright */
/* Copyright (C) 1985-1991 by Walter Bright	*/
/* All Rights Reserved				*/
/* $Revision: 1.1.1.1 $ */

#include	<stdio.h>
#include	<stdlib.h>
#include	<stddef.h>
#include	<io.h>
#include	<fcntl.h>
#include	<string.h>
#include	<process.h>
#include	<sys/stat.h>
#include	<errno.h>
#include	"mt.h"
#include	"semlock.h"

#if MSDOS || __OS2__ || _WINDOWS ||__NT__
#include	<dos.h>
#endif

#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <windows.h>
#endif

#if __INTSIZE == 4
#define MAX_EXIT_FUNC 64
#else
#define MAX_EXIT_FUNC 32
#endif

#if __NT__ || (__OS2__ && __INTSIZE == 4)
#define __FAR
#else
#define __FAR __far
#endif

#define FHND_TEXT   0x10

struct __FILE2			/* must match definition in tmpnam.c	*/
{	int tmpnum;
};

extern struct __FILE2 _iob2[];
extern unsigned char __fhnd_info[_NFILE];

#if (__OS2__ && __I86__ >= 3) || __NT__
#define _tmpnum(fp)	((fp)->__tmpnum)
#else
#define _tmpnum(fp)	(_iob2[(fp) - _iob].tmpnum)
#endif

FILE * _pascal __fopen(const char *name,const char *mode,FILE *fp);
FILE * _pascal __sopenf(const char *name,const char *mode,FILE *fp,int shflag);
void _pascal _freebuf(FILE *fp);

/************************************
 * Routines to lock/unlock exclusive access to a stream.
 * These routines nest.
 * Be careful to use them exactly in a nesting manner.
 */

#undef __fp_lock
#undef __fp_unlock

void __fp_lock(FILE *fp)
{
#if _MT
   int iIdx;
	iIdx = (int)(fp - _iob) + _semFpBase;

	LockSemaphoreNested(iIdx)
#endif
}

void __fp_unlock(FILE *fp)
{
#if _MT
   int iIdx;
//	int bOpen, bDelete;
	iIdx = (int)(fp - _iob) + _semFpBase;
//   bOpen = fp->_flag & (_IOREAD | _IOWRT | _IORW);  /* Is File Open */
//	bDelete = !bOpen;
//	UnlockSemaphoreNested(iIdx, bDelete)
	UnlockSemaphoreNested(iIdx, 0)
#endif
}

