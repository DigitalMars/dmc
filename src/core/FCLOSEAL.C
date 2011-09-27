/*_ fcloseal.c   Sat Sep	1 1990	 Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars */
/* All Rights Reserved				*/
/* $Revision: 1.1.1.1 $ */

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

#if  defined(__NT__)
#define _NSYSFILE  3  /* This is the index into _iob where non-system files begin */
#else
#define _NSYSFILE 5
#endif

FILE * _pascal __fopen(const char *name,const char *mode,FILE *fp);
FILE * _pascal __sopenf(const char *name,const char *mode,FILE *fp,int shflag);
void _pascal _freebuf(FILE *fp);

/**************************
 * Close all open files.
 * Returns:
 *	# of files closed
 */

int fcloseall()
{	FILE near *fp;
	int nclosed = 0;

	for (fp = &_iob[_NSYSFILE]; fp < &_iob[_NFILE]; fp++)
	{	__fp_lock(fp);
		if (fp->_flag & (_IOREAD | _IOWRT | _IORW))
		{	fclose(fp);
			nclosed++;
		}
		__fp_unlock(fp);
	}
	return nclosed;
}
