/*_ strtok.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<stdio.h>
#include	<ctype.h>
#include	<stddef.h>
#include	<string.h>
#include	<stdlib.h>

#if _MT
#include	"mt.h"
#endif

char* strtok(p1,p2)
char *p1;
const char *p2;
{
#if _MT
	char **ps = &_getthreaddata()->t_strtok;
#define s (*ps)
#else
	static char *s = NULL;
#endif
	const char *p2start = p2;

	if (!p1)
	{	p1 = s;			/* use old value		*/
		if (!p1)
			return (char *) NULL;
	}
	p1 += strspn(p1,p2);		/* find first char of token	*/
	if (!*p1)
		return (char *) NULL;
	s = strpbrk(p1,p2);		/* find end of token		*/
	if (s)				/* if there was an end		*/
	{	*s = 0;			/* terminate the token		*/
		s++;			/* start of next token		*/
	}
	return p1;
}
