/*_ gets.c   Sun Apr 16 1989   Modified by: Walter Bright */
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

/****************************
 * Read string from stdin.
 */

char *gets(char *s)
{	unsigned c;
	char *sstart = s;

	__fp_lock(stdin);
	c = getchar();
	if (c == EOF)
	    goto err;
	while (c != EOF && c != '\n')
	{	*s++ = c;
		c = getchar();
	}
	*s++ = 0;			/* terminate the string		*/
	if (ferror(stdin))
err:
	    sstart = NULL;
	__fp_unlock(stdin);
	return sstart;
}
