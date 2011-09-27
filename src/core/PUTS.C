/*_ puts.c   Sun Apr 16 1989   Modified by: Walter Bright */
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

/*****************************
 * Write string to stdout, followed by a newline.
 * Returns:
 *	0 if successful
 *	non-zero if not
 */

int puts(const char *p)
{	int result;

	__fp_lock(stdout);
	while (*p)
	{	if (putchar(*p) == EOF)
		{	result = 1;
			goto L1;
		}
		p++;
	}
	result = putchar('\n') == EOF;
    L1:
	__fp_unlock(stdout);
	return result;
}
