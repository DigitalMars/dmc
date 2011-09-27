/*_ fgets.c   Sun Apr 16 1989   Modified by: Walter Bright */
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
 * Read string from stream.
 * Returns:
 *	NULL no chars read or read error
 *	else s
 */

char *fgets(char *s,int n,FILE *fp)
{	unsigned c;
	char *sstart = s;

	__fp_lock(fp);
	if (n == 0)			/* if n is zero then return NULL */
		goto err;
	while (--n > 0)
	{	c = _fgetc_nlock(fp);
		if (c != EOF)
		{
		    if ((*s = c) != '\n')
		    {	s++;
			continue;
		    }
		    s++;
		    break;
		}
		else
		{   if (s == sstart)	/* if no chars read		*/
			goto err;
		    else
			break;
		}
	}
	*s = 0;				/* terminate the string		*/
	if (ferror(fp))
err:
	    sstart = NULL;
ret:
	__fp_unlock(fp);
	return sstart;
}
