/*_ fdopen.c   Sat Sep	1 1990	 Modified by: Walter Bright */
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

/****************************************************************/
/*  Function to buffer an already open'ed file.			*/
/*  Buffers an already open'ed file.				*/
/*								*/
/*  Parameters: fd   - Handle of open file to buffer		*/
/*		mode - Access mode of buffered I/O		*/
/*								*/
/*  Returns: FILE pointer to opened buffer if successful, else	*/
/*	     NULL.						*/
/*								*/
/*  Side effects: Temporarily opens then closes an additional	*/
/*		  buffered stream.				*/
/*								*/
/*  NOTE: No checking is done to assure that the stream mode	*/
/*	  is compatible with the rwmode used to open the	*/
/*	  original file.					*/
/* Written by Robert B. Stout					*/

FILE *_tfdopen(int fd, const _TCHAR *mode)
{
	FILE *fp;

    if (fd == -1)
       return NULL;

#if M_UNIX || M_XENIX
	if (NULL == (fp = _tfopen(_T("/dev/null"), mode)))
#else
	if (NULL == (fp = _tfopen(_T("NUL"), mode)))
#endif
	{
		return NULL;
	}
	__fp_lock(fp);
	close(fp->_file);
	fp->_file = fd;
	
   /* turn off translation, fd is setup for it!!*/
	/* This fixes a problem where the write will also */
	/* do translation causing problems with extra lines in output code */
	/* if the user has \n in the buffer */
	fp->_flag &= ~_IOTRAN;

	__fp_unlock(fp);

	switch (*mode)
	{
	    case _T('a'):
		if (-1L == lseek(fd, 0L, SEEK_END))
		    goto error;
		break;
	    case _T('w'):
		if (chsize(fd, 0L))
		    goto error;
		break;
	    case _T('r'):
		if (-1L == lseek(fd, 0L, SEEK_SET))
		    goto error;
		break;
	    default:
		goto error;
	}
	return fp;

error:
	fclose(fp);
	return NULL;
}
