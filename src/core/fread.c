/*_ fread.c   Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/

#include	<stdio.h>
#include	<string.h>
#include	<io.h>
#if MSDOS || __OS2__ || _WINDOWS || __NT__
#include        <scrtl.h>
#include	<dos.h>
#endif
#include	"mt.h"

int __cdecl _fillbuf(FILE *fp);
int __cdecl _flushbu(int c,FILE *fp);
void _flushterm(void);

/***************************
 * Read data from stream.
 * Returns:
 *	# of complete items read
 */

size_t fread(void *pi,size_t bytesper,size_t numitems,FILE *fp)
{   char *p;
    unsigned bytesleft;
    unsigned u;

    p = (char *) pi;
    __fp_lock(fp);
    bytesleft = bytesper * numitems;
    if (bytesleft == 0)
	numitems = 0;
    else if (fp->_flag & _IOTRAN)
    {	char *pend = p + bytesleft;

	while (p < pend)
	{   int c;

	    c = _fgetc_nlock(fp);
	    if (c != EOF)
	    {   *p = c;
		p++;
		continue;
	    }
	    numitems = (p - (char *) pi) / bytesper;
	    break;
	}
    }
    /* Deal with non-buffered io */
    else if (fp->_flag & _IONBF)
    {
	int cnt;

	/* The logic here mirrors _fillbuf()	*/

	if (fp->_flag & _IORW)		/* if read/write	*/
	    fp->_flag = (fp->_flag & ~_IOWRT) |  _IOREAD; /*set read bit */

	if ((fp->_flag & (_IOREAD | _IOERR | _IOEOF)) != _IOREAD)
	{
	    if (!(fp->_flag & _IOREAD))
		fp->_flag |= _IOERR;
	    numitems = 0;		/* something is wrong here	*/
	    goto ret;
	}

	_flushterm();			/* flush terminal input output	*/
	cnt = read(fileno(fp),p,bytesleft);

	if (cnt <= 0)			/* if unsuccessful read	*/
	{
	    if (cnt == 0)		/* if end of file	*/
	    {
            L2: fp->_flag |= _IOEOF;
		if (fp->_flag & _IORW)
		    fp->_flag &= ~_IOERR;
	    }
	    else
            {
                    /* Note, this can occur for various reasons, */
                    /* one of which is a broken pipe.  This error*/
                    /* simply means EOF for a pipe               */
                    switch(GetLastError())
                    {
                    case ERROR_BROKEN_PIPE:
                        /* must clear errno */
                        __set_errno(0);
                        goto L2;

                    default:
			fp->_flag |= _IOERR;
                        break;
                    }
            }

	    numitems = 0;
	    goto ret;
	}
	numitems = cnt / bytesper;
    }
    else
    {
	while (bytesleft != 0)
	{
	   L1:
		u = fp->_cnt;		/* # of characters left in buffer */
		if (u == 0)
		{	if (_fillbuf(fp) == EOF)
			{   numitems = (p - (char *) pi) / bytesper;
			    break;
			}
			goto L1;
		}
		if (u > bytesleft)
			u = bytesleft;
#ifdef BIGBUF
		if (fp->_flag & _IOBIGBUF)
			peekbytes(fp->_seg,(unsigned) fp->_ptr,p,u);
		else
#endif
			memcpy(p,fp->_ptr,u);
		fp->_ptr += u;
		fp->_cnt -= u;
		p += u;
		bytesleft -= u;
	}
    }
ret:
    __fp_unlock(fp);
    return numitems;
}
