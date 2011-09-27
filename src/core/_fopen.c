/*_ _fopen.c   Sat Sep	1 1990	 Modified by: Walter Bright */
/* Copyright (C) 1985-2003 by Digital Mars */
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
static void __pascal _saveerrnostatus(int *, int *);
static void __pascal _restoreerrnostatus(int, int);

/*****************************
 * save status of errno and doserrno for restoring if necessary
 */
 static void __pascal _saveerrnostatus(int *_olderrno, int *_olddoserrno)
 {
    *_olderrno = errno;
    *_olddoserrno = _doserrno;
 }
 
 /**************************
  * restore status of errno and doserrno from values passed in.
  */
 static void __pascal _restoreerrnostatus(int _olderrno, int _olddoserrno)
 {
 # ifdef _MT
    struct THREAD *my_thread = _getthreaddata ();
    
    my_thread->t_errno = _olderrno;
    my_thread->t_doserrno = _olddoserrno;
# else
    errno = _olderrno;
    _doserrno = _olddoserrno;
# endif
 }
 

/*****************************
 * Open a file for input.
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

FILE * _pascal ___tfopen(const _TCHAR *name,const _TCHAR *mode,FILE *fp)
{   int flag,fd;
    char append;
    int oldfmode;
    int creat_mode = O_WRONLY;  /* Mode used at L2 when creating file */
   
    /* Used at L2 where the open sets errno yet we may still successfully
       create file */                                                    
    int _old_doserrno;
    int _old_errno;  
                       

    if (!name || !fp || !*name)
	goto err;
	    
    /* save current doserrno and errno. This is useful because, dependant
       upon the flags, if an open fails, then we do a create, but the failed
       open will have set errno, we must the restore errno and doserrno because
       the create may succeed and thus errno would be improperly set.
     */
    _saveerrnostatus(&_old_errno, &_old_doserrno);

    flag = _IOTRAN;			/* default is translated mode	*/
    append = 0;
    switch (*mode++)
    {   case _T('r'):
		    flag |= _IOREAD;
		    break;
	    case _T('a'):
		    append++;		// = 1
		    flag |= _IOAPP;	// file has been opened for append
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
	    creat_mode = O_RDWR;  // Setup for both read and write permision
	    goto L2;		  // create file
	}
	/* "r+" or "a+"	*/
#if M_UNIX || M_XENIX
	if ((fd = _topen(name,O_RDWR)) == -1)
#else
	if ((fd = _topen(name,O_RDWR|O_BINARY)) == -1)
#endif
	{
	    if (flag & _IOWRT)		/* if "a+"		*/
	    {
		creat_mode = O_RDWR;  // Setup for both read and write access
		_restoreerrnostatus(_old_errno,_old_doserrno);
		goto L2;			// create file
	    }
	}
	else if (append)
	    goto L1;
    }
    else if (flag & _IOWRT)
    {	if (append)				/* if "a"		*/
	{
#if M_UNIX || M_XENIX
	    if ((fd = _topen(name,O_WRONLY)) == -1)
#else
	    if ((fd = _topen(name,O_WRONLY|O_BINARY)) == -1)
#endif	 
	    {
		creat_mode = O_WRONLY;
		_restoreerrnostatus(_old_errno,_old_doserrno);
		goto L2;
	    }
	    else
	      L1: /* Position at end of file	*/
		if (lseek(fd,0L,SEEK_END) == -1L)
		{   close(fd);
		    goto err;
		}
	}
	else
	{
#if M_UNIX | M_XENIX
	  L2:
	    fd = creat(name,0666);
#else
	  L2: 
	    fd = _topen (name, O_BINARY|O_CREAT|O_TRUNC|creat_mode, S_IWRITE|S_IREAD);
#endif
	}
    }
    else /* if "r"	*/
#if M_UNIX || M_XENIX
	fd = _topen(name,O_RDONLY);
#else
	fd = _topen(name,O_RDONLY|O_BINARY);
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
