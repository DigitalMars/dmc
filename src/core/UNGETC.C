/*_ ungetc.c   Sat Oct 17 1987   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/

#include	<stdio.h>
#include	<io.h>
#include	<dos.h>
#include	<errno.h>
#include	"mt.h"
#include        <scrtl.h>

/* Current position in file     */
#define CURPOS(fp)      lseek((fp)->_file,0L,1)

/**********************
 * Put char back in input stream.
 * Only 1 is allowed.
 * Returns:
 *      EOF     if failure
 *      c       if success
 */

int ungetc(c,fp)
int c;
FILE *fp;
{
	__fp_lock(fp);
	if (fp->_flag & (_IOWRT | _IOEOF | _IOERR) ||
	    c == EOF ||
#ifdef BIGBUF
	    !(fp->_base || fp->_seg)
#else
	    !fp->_base
#endif
	   )
		c = EOF;
	else
	{
		if (fp->_ptr != fp->_base)      /* if buffer isn't empty */
			--fp->_ptr;
#ifdef BIGBUF
		if (fp->_flag & _IOBIGBUF)
			*(char __far *)MK_FP(fp->_seg,fp->_ptr) = c;
		else
#endif
			*fp->_ptr = c;
		fp->_cnt++;
	}
	__fp_unlock(fp);
	return c;
}

/************************
 * Buffered version of lseek().
 *      origin  = 0     relative to beginning
 *	      = 1     relative to current position
 *	      = 2     relative to end
 * Returns:
 *      0       success
 *      -1      error
 */

int fseek(fp,offset,origin)
FILE *fp;
long offset;
int origin;
{       int cnt;
	int result;

	__fp_lock(fp);
	fp->_flag &= ~(_IOERR | _IOEOF);	/* switch off these bits */
	switch (fp->_flag & (_IORW | _IOREAD | _IOWRT))
	{
	    case _IOWRT:
		fflush(fp);
		break;
	    case _IORW | _IOWRT:
	    case _IORW:
		fflush(fp);
		fp->_flag &= ~_IOWRT;   /* no longer necessarilly writing */
		fp->_ptr = fp->_base;
		break;
	    case _IOREAD:
		/* see if we can just change the position in the buffer */
		if ((fp->_flag & _IONBF) == 0 &&	/* if buffered  */
#ifdef BIGBUF
		    (fp->_base || fp->_seg) &&  /* and there is a buffer */
#else
		    fp->_base &&		/* and there is a buffer */
#endif
		    origin < 2 &&	       /* and origin is 0 or 1 */
		    (cnt = fp->_cnt) != 0)	/* and something in read buf*/
		{       long rel;

			/* calc position relative to current spot in buffer */
			if (origin == 1)	/* if rel to current pos */
				rel = offset;
			else	    /* relative to beginning	*/
				rel = offset - (CURPOS(fp) - cnt);

			/* rel can be + or -. If +, see that it isn't   */
			/* the top of the buffer. If -, see that it     */
			/* isn't past the bottom.		       */
			if (rel <= cnt && -rel <= fp->_ptr - fp->_base)
			{       fp->_cnt -= (int) rel;
				fp->_ptr += (int) rel;
				result = 0;
				goto ret;
			}
		}
		goto L1;
	    case _IORW | _IOREAD:
		fp->_flag &= ~_IOREAD;
	    L1:
		if (origin == 1)	/* if relative to current position */
			offset -= fp->_cnt;     /* lseek pos is offset  */
					/* note that _cnt is 0 if unbuffered*/
		fp->_ptr = fp->_base;
		break;
	    default:
	    err:
		result = -1;		      /* error		*/
		goto ret;
	}
	fp->_cnt = 0;
	if (lseek(fp->_file,offset,origin) == -1)
		goto err;
	result = 0;

ret:
	__fp_unlock(fp);
	return result;
}

/**************************
 * Return current position in buffer.
 * Returns:
 *      -1 if error
 *      else file position
 */

long ftell(fp)
FILE *fp;
{	long curpos;
	int flag;

	__fp_lock(fp);
	flag = fp->_flag;
	if ((flag & (_IORW | _IOREAD | _IOWRT)) == 0)
	{
		curpos = -1;
		__set_errno (EBADF);
	}
	else if ((curpos = CURPOS(fp)) != -1)
	{   if (flag & _IOREAD)
		curpos -= fp->_cnt;
	    else if ((flag & (_IOWRT | _IONBF)) == _IOWRT &&
		     (fp->_base
#ifdef BIGBUF
			|| fp->_seg
#endif
			       ))
		curpos += fp->_ptr - fp->_base;
	}
	__fp_unlock(fp);
	return curpos;
}

/**********************************
 */

int fgetpos(FILE *fp,fpos_t *pos)
{
    *pos = ftell(fp);
    return (*pos == -1L) ? -1 : 0;
}

int fsetpos(FILE *fp,const fpos_t *pos)
{
    return fseek(fp,*pos,SEEK_SET);
}

