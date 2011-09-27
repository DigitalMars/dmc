/*_ fputs.c   Sun Apr 16 1989   Modified by: Walter Bright */
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

/**************************
 * Write string to stream.
 * Returns:
 *	non-zero if error
 */

int fputs(const char *s,FILE *fp)
{	int result;

	__fp_lock(fp);
	while (*s)
	{	if (_fputc_nlock(*s,fp) == EOF)
		{   result = EOF;
		    goto ret;
		}
		s++;
	}
	result = 0;
ret:
	__fp_unlock(fp);
	return result;
}
