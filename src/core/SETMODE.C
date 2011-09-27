/*_ setmode.c   Sat Sep	1 1990	 Modified by: Walter Bright */
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
#include        <scrtl.h>
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

/***********************************
 * Set file translation mode to binary or translated mode.
 * (Note: This routine only works if the file handle has a corresponding
 *	  FILE* stream pointer)
 *	int setmode(int fd, int mode);
 * Input:
 *	fd	File handle (value returned by fileno())
 *	mode	Either O_TEXT for translated mode, O_BINARY for binary mode
 * Returns:
 *	-1 on error, errno is set to EINVAL or EBADF.
 *	otherwise, the previous translation mode is returned on success.
 * Graciously provided by E. McCreight.
 */

int setmode(int fd, int mode)
{   FILE *fp;
    int result;
    int flag;
    int nmatch;

    /* Search for the stream corresponding to fd	*/
    result = -1;
    nmatch = 0;
    for (fp = &_iob[0]; fp < &_iob[_NFILE]; fp++)
    {
	__fp_lock(fp);
	if (fileno(fp) == fd && fp->_flag & (_IOREAD | _IOWRT | _IORW))
	{
	    nmatch ++;
	    switch (mode)
	    {	case O_TEXT:	mode = _IOTRAN;	break;
		case O_BINARY:	mode = 0;	break;
		default:
		    __set_errno (EINVAL);	/* invalid mode argument	*/
		    goto unlock;
	    }

	    flag = fp->_flag & _IOTRAN;
	    fp->_flag = (fp->_flag & ~_IOTRAN) | mode;
	    result = flag ? O_TEXT : O_BINARY;
unlock:	    __fp_unlock(fp);
	    goto ret;
	}
	__fp_unlock(fp);
    }
    __set_errno (EBADF);			/* invalid file handle		*/
ret:
    if (nmatch == 0)
    {
       if (fd >= _NFILE)
       {
          __set_errno (EBADF);
          result = -1;
       }
       else
       {
          if (__fhnd_info[fd] & FHND_TEXT)
             result = _O_TEXT;
          else
             result = _O_BINARY;
      
          switch (mode)
          {
          case _O_TEXT:
             __fhnd_info[fd] |= FHND_TEXT;
             break;
          case _O_BINARY:
             __fhnd_info[fd] &= ~FHND_TEXT;
             break;
          default:
             __set_errno (EINVAL);
             result = -1;
          }
       }      
    }
    return result;
}
