/*_ fwrite.c   Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/

#include	<stdio.h>
#include	<string.h>
#include	<io.h>
#if MSDOS || __OS2__ || _WINDOWS || __NT__
#include	<dos.h>
#endif
#include	"mt.h"

int __cdecl _fillbuf(FILE *fp);
int __cdecl _flushbu(int c,FILE *fp);
void _flushterm(void);

/*************************
 * Write data to stream.
 * Returns:
 *	# of complete items written
 */

size_t fwrite(const void *pi,size_t bytesper,size_t numitems,FILE *fp)
{   unsigned bytesleft;
    unsigned u;
    const char *p = pi;

    bytesleft = bytesper * numitems;
    __fp_lock(fp);
    if (fp->_flag & _IOTRAN)
    {	const char *pend = p + bytesleft;

	while (p < pend)
	{
	    if (_fputc_nlock(*p,fp) != EOF)
	    {	p++;
		continue;
	    }
	    numitems = (p - (const char *) pi) / bytesper;
	    break;
	}
    }
    /* Deal with non-buffered io */
    else if (fp->_flag & _IONBF)
    {	/* The logic here mirrors _flushbu()	*/

	if (fp->_flag & _IORW)
	    fp->_flag = (fp->_flag & ~_IOREAD) | _IOWRT;

	if ((fp->_flag & (_IOWRT | _IOERR | _IOEOF)) != _IOWRT)
	{   numitems = 0;
	    goto ret;
	}

	if (bytesleft &&
	  (bytesleft != (u=write(fileno(fp),(void *) pi,bytesleft))))
	{
	    fp->_flag |= _IOERR;
	    numitems = u / bytesper;
	}
    }
    else
    {
	while (bytesleft != 0)
	{
		u = fp->_cnt;
		if (u == 0)
		{
			if (_flushbu(*p,fp) == EOF)
			{   numitems = (p - (const char *) pi) / bytesper;
			    break;
			}
			bytesleft--;
			p++;
			continue;
		}
		if (u > bytesleft)
			u = bytesleft;
#ifdef BIGBUF
		if (fp->_flag & _IOBIGBUF)
			pokebytes(fp->_seg,(unsigned) fp->_ptr,p,u);
		else
#endif
			memcpy(fp->_ptr,p,u);
		fp->_ptr += u;
		fp->_cnt -= u;
		bytesleft -= u;
		p += u;
	}
    }
ret:
    __fp_unlock(fp);
    return numitems;
}
