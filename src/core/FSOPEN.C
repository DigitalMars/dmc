/*_ fsopen.c   Sat Sep	1 1990	 Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars */
/* All Rights Reserved				*/
/* $Revision: 1.1.1.1 $ */

#include	<tchar.h>
#include	<sctchar.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<stddef.h>
#include	<io.h>
#include	<fcntl.h>
#include	<string.h>
#include	<process.h>
#include	<errno.h>
#include	"mt.h"
#include	"semlock.h"
#if M_UNIX || M_XENIX
#include	<unistd.h>
#include	<sys/stat.h>
#else
#include	<sys\stat.h>
#endif

#if MSDOS || __OS2__ || _WINDOWS ||__NT__
#include	<dos.h>
#endif

#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <windows.h>
#endif

#include <scrtl.h>

#if __INTSIZE == 4
#define MAX_EXIT_FUNC 64
#else
#define MAX_EXIT_FUNC 32
#endif

#if M_UNIX || M_XENIX || __NT__ || (__OS2__ && __INTSIZE == 4)
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

#if (__OS2__ && __INTSIZE == 4) || __NT__
#define _tmpnum(fp)	((fp)->__tmpnum)
#else
#define _tmpnum(fp)	(_iob2[(fp) - _iob].tmpnum)
#endif

FILE * _pascal ___tfopen(const _TCHAR *name,const _TCHAR *mode,FILE *fp);
FILE * _pascal ___tsopenf(const _TCHAR *name,const _TCHAR *mode,FILE *fp,int shflag);
void _pascal _freebuf(FILE *fp);

/*************
 * Open file with sharing.
 */

FILE *_tfsopen(const _TCHAR *name,const _TCHAR *mode,int shflag)
{	FILE near *fp;
	FILE *fpr;

	/* Search for a free slot	*/
	for (fp = &_iob[0]; fp < &_iob[_NFILE]; fp++)
	{
		__fp_lock(fp);
		if (!(fp->_flag & (_IOREAD | _IOWRT | _IORW)))
		{
			fpr = (FILE *) ___tsopenf(name,mode,fp,shflag);
			__fp_unlock(fp);
			return fpr;
		}
		__fp_unlock(fp);
	}

	/* failed to find one	*/

	__set_errno (EMFILE);			/* too many files	*/
	return NULL;
}
