/*_ strrev.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<string.h>

char *strrev(char *p)
{	char *pend,*pstart = p;

	pend = p + strlen(p) - 1;
	while (p < pend)
	{	*p ^= *pend;
		*pend ^= *p;
		*p++ ^= *pend--;	/* swap *p and *pend		*/
	}
	return pstart;
}
