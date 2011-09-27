/*_ sopenf.c   Sat Sep	1 1990	 Modified by: Walter Bright */
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

/*****************************
 * Open a file for input with sharing.
 * Input:
 *	name	pointer to drive, path and file name
 *	mode	one of the following strings:
 *		"r"	read
 *		"w"	write
 *		"a"	append to end of file
 *		"r+"	r/w, position at beginning of file
 *		"w+"	r/w, create or truncate file
 *		"a+"	r/w, position at end of file
 *		A 'b' may be appended to the string to indicate
 *		that the I/O is to be in binary (untranslated) mode.
 * Returns:
 *	NULL	if error
 *	fp	if successful
 */

FILE * _pascal ___tsopenf(const _TCHAR *name,const _TCHAR *mode,FILE *fp,int shflag)
{	int flag,fd;
	char append;
   int createmode = O_WRONLY|O_TRUNC;  /* Mode to or with if we create, default to WRONLY */

	if (!name || !fp || !*name || !shflag)
		goto err;

	flag = _IOTRAN;			/* default is translated mode	*/
	append = 0;
	switch (*mode++)
	{	case _T('r'):
			flag |= _IOREAD;
			break;
		case _T('a'):
			append++;	/* = 1				*/
			flag |= _IOAPP; /* file has been opened for append */
		case _T('w'):
			flag |= _IOWRT;
			break;
		default:
		err:
			return (FILE *) NULL;
	}

	/* Parse other flags, stopping on unrecognized ones	*/
	while (1)
	{   switch (*mode)
	    {
		case _T('+'):
		    flag |= _IORW;
		    goto L3;
		case _T('b'):		/* binary mode			*/
		    flag &= ~_IOTRAN;	/* turn off default translated mode */
		L3: mode++;
		    continue;
	    }
	    break;
	}

	if (flag & _IORW)
	{   if (flag & _IOWRT && !append)	/* if "w+"		*/
       {
      createmode = O_RDWR|O_TRUNC;  /* It may exist, so must truncate it */
		goto L2;			/* create file		*/
      }
	    /* "r+" or "a+"	*/
#if M_UNIX || M_XENIX
		if ((fd = _topen(name,O_RDWR,S_IREAD | S_IWRITE)) == -1)
#else
		if ((fd = _tsopen(name,O_RDWR|O_BINARY,shflag)) == -1)
#endif
	    {	if (flag & _IOWRT)		/* if "a+"		*/
          {
          createmode = O_RDWR;
		    goto L2;			/* create file		*/
          }
	    }
	    else if (append)
		goto L1;
	}
	else if (flag & _IOWRT)
	{   if (append)				/* if "a"		*/
	    {
#if M_UNIX || M_XENIX
		if ((fd = _topen(name,O_WRONLY,S_IWRITE)) == -1)
#else
		if ((fd = _tsopen(name,O_WRONLY|O_BINARY,shflag,S_IWRITE)) == -1)
#endif
         {
          createmode=O_WRONLY;
		    goto L2;
         }
		else
		L1: /* Position at end of file	*/
		    if (lseek(fd,0L,SEEK_END) == -1L)
		    {	close(fd);
			goto err;
		    }
	    }
	    else
	    {
#if M_UNIX | M_XENIX
		L2: fd = _topen(name,O_TRUNC|O_CREAT|O_RDWR,S_IWRITE | S_IREAD);
#else
	    L2:	fd = _tsopen(name,createmode|O_CREAT|O_BINARY,shflag,S_IWRITE | S_IREAD);
#endif
	    }
	}
	else /* if "r"	*/
#if M_UNIX || M_XENIX
		fd = _topen(name,O_RDONLY,S_IREAD);
#else
		fd = _tsopen(name,O_RDONLY|O_BINARY,shflag,S_IREAD);
#endif
	if ((fp->_file = fd) == -1)
		goto err;

	fp->_flag = (flag & _IORW) ? (flag & ~(_IOREAD | _IOWRT)) : flag;
#ifdef BIGBUF
	fp->_seg =
#endif
	fp->_cnt = 0;
	fp->_ptr = fp->_base = NULL;

	return fp;
}
