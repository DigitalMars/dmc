/*_ fclose.c   Sat Sep	1 1990	 Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars		*/
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
#include        <unistd.h>
#include        <sys/stat.h>
#else
#include        <sys\stat.h>
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

/*********************
 * Close file stream.
 * Returns:
 *	0 if successful
 *	EOF if error
 */

int fclose(FILE *fp)
{   int result,flag;

    if (!fp)
	result = EOF;
    else
    {
	result = 0;
	__fp_lock(fp);
	flag = fp->_flag;

	/* if stream is open	*/
	if (flag & (_IOREAD | _IOWRT | _IORW))
	{	/* flush buffer	*/
		if (!(flag & _IONBF))
			result = fflush(fp);
		result |= close(fp->_file);
	}
	else
	{
	    result = EOF;  /* file was not open, return failure */
	}

#if 1
	{   char string[L_tmpnam];
	    int tmpnum = _tmpnum(fp);

	    if (tmpnum)
		{
		sprintf(string, "%s%d", P_tmpdir, tmpnum);
		remove(string);
		}
	    _tmpnum(fp) = 0;
	}
#endif
	_freebuf(fp);			/* dump buffer if there is one	*/
	memset(fp,0,sizeof(FILE));	/* set all fields to 0		*/
	__fp_unlock(fp);
    }
    return result;
}
