/*_ fflush.c   Sat Sep	1 1990	 Modified by: Walter Bright */
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

FILE * _pascal __fopen(const char *name,const char *mode,FILE *fp);
FILE * _pascal __sopenf(const char *name,const char *mode,FILE *fp,int shflag);
void _pascal _freebuf(FILE *fp);

/***********************
 * Flush buffer.
 * Returns:
 *	0:	success
 *	EOF:	failure
 */

int fflush(FILE *fp)
{	int length;
	int result= 0;

	/* if fflush(NULL) flush all buffers */
	if (fp == NULL)
	{
		if (flushall() >= 0)
			result = 0; 
	}
	else
	{
	  /* don't flush buffer if we are not writing	*/
	__fp_lock(fp);
	if ((fp->_flag & (_IOWRT | _IONBF | _IOERR)) == _IOWRT &&
	    (fp->_base
#ifdef BIGBUF
		|| fp->_seg
#endif
			))
	{	length = fp->_ptr - fp->_base;	/* # of bytes in buffer	*/
#ifdef BIGBUF
		if (length && _writex(fp->_file,fp->_base,length,fp->_seg)
			!= length)
			fp->_flag |= _IOERR;
#else
		if (length)
		{   int nwritten = write(fp->_file,fp->_base,length);
		    /* The following check for isatty() is because:
		     *	#define WIN32_LEAN_AND_MEAN
		     *	#include <windows.h>
		     *	#include <stdio.h>
		     *	void main()
		     *	{
		     *	    // Set console output to UTF-8 (one can use 'chcp 65001' instead)
		     *	    SetConsoleOutputCP( 65001 );
		     *	    // Latin small letter e with acute
		     *	    fputs( "Output utf-8 accented char \xc3\xa9\n... and the rest is cut off!\n", stdout );
		     *	}
		     * fails because WriteFile() apparently treats UTF-8
		     * sequences as 1 byte when writing to the console.
		     */
		    if (!nwritten || (nwritten != length && !isatty(fp->_file)))
			fp->_flag |= _IOERR;
		}
#endif
		fp->_cnt = 0;
		fp->_ptr = fp->_base;
	}
	else
		fp->_cnt = 0;
	result = (ferror(fp)) ? EOF : 0;
	__fp_unlock(fp);
	}
	return result;
}
