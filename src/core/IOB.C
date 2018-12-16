/*_ iob.c   Sat Sep	1 1990	 Modified by: Walter Bright */
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

#if M_UNIX || M_XENIX
char *_bufendtab[_NFILE];
FILE _iob[_NFILE] =
{
	{0,0,0,_IOREAD,0},		/* stdin	*/
	{0,0,0,_IOWRT ,1},		/* stdout	*/
	{0,0,0,_IOWRT | _IONBF,2},	/* stderr	*/

#define _NSYSFILE  3  /* This is the index into _iob where non-system files begin */

#elif __OS2__ && __INTSIZE == 4

FILE _iob[_NFILE] =
{
	{0,0,0,_IOREAD,0,1},		/* stdin	*/
	{0,0,0,_IOWRT ,1,1},		/* stdout	*/
	{0,0,0,_IOWRT | _IONBF ,2,1},	/* stderr	*/
	{0,0,0,_IOTRAN | _IORW	,3,1},	/* stdaux	*/
	{0,0,0,_IOTRAN | _IOWRT ,4,1},	/* stdprn	*/

#define _NSYSFILE  5  /* This is the index into _iob where non-system files begin */

#elif __NT__

FILE __cdecl _iob[_NFILE] =
{
	{0,0,0,_IOREAD,0},		/* stdin	*/
	{0,0,0,_IOWRT ,1},		/* stdout	*/
	{0,0,0,_IOWRT | _IONBF ,2},	/* stderr	*/

#define _NSYSFILE  3  /* This is the index into _iob where non-system files begin */

#else
FILE _iob[_NFILE] =
{
	{0,0,0,_IOREAD,0,1},		/* stdin	*/
	{0,0,0,_IOWRT ,1,1},		/* stdout	*/
	{0,0,0,_IOWRT | _IONBF ,2,1},	/* stderr	*/
	{0,0,0,_IOTRAN | _IORW	,3,1},	/* stdaux	*/
	{0,0,0,_IOTRAN | _IOWRT ,4,1},	/* stdprn	*/

#define _NSYSFILE  5  /* This is the index into _iob where non-system files begin */

#endif

	/* the rest have all 0 entries	*/
};

struct __FILE2 _iob2[_NFILE];

/****************************
 * Called by exit() to close all the open streams.
 */

#if !(__SMALL__ || __COMPACT__)	/* if large code model	*/
/* So that __fcloseall() is not at offset 0 in this segment	*/
static void __dummy() {}
#endif

void __fcloseall()
{   FILE _near *fp;
    for (fp = &_iob[0]; fp < &_iob[_NFILE]; fp++)
    {	__fp_lock(fp);
	if (fp->_flag & (_IOREAD | _IOWRT | _IORW))	/* if file is open */
	{
	    if (fp >= &_iob[_NSYSFILE])
		fclose(fp); // regular file
	    else if (!(fp->_flag & _IONBF))
		fflush(fp); // flush, but do not close system handle
	}
	__fp_unlock(fp);
    }
}

void (*_fcloseallp)(void) = __fcloseall;

/****************************
 * Increase max file handle count.
 */

#if !(M_UNIX || M_XENIX) && !defined(_WINDOWS)
void _STI_io_ctor()
{
#if __INTSIZE != 4 && (__SMALL__ || __MEDIUM__)
    /* This is necessary because _dos_sethandlecount() can alloc
	DOS memory, which could block us from expanding the heap.
     */
    extern int _okbigbuf;

    if (_okbigbuf)
#endif
	_dos_sethandlecount(_NFILE);
}
#endif
