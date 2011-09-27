/*_ fstrtok.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<string.h>

#if __INTSIZE == 4 && (M_UNIX || M_XENIX || __OS2__ || __NT__)
#define __FAR
#else
#define __FAR __far
#endif

#if _MT
#include	"mt.h"
#endif

char __FAR * __FAR fstrtok(p1,p2)
char __FAR *p1;
const char __FAR *p2;
{
#if _MT
	char __FAR **ps = &_getthreaddata()->t_strtok;
#define s (*ps)
#else
	static char __FAR *s = NULL;
#endif
	const char __FAR *p2start = p2;

	if (!p1)
	{	p1 = s;			/* use old value		*/
		if (!p1)
			return (char __FAR *) NULL;
	}
	p1 += fstrspn(p1,p2);		/* find first char of token	*/
	if (!*p1)
		return (char __FAR *) NULL;
	s = fstrpbrk(p1,p2);		/* find end of token		*/
	if (s)				/* if there was an end		*/
	{	*s = 0;			/* terminate the token		*/
		s++;			/* start of next token		*/
	}
	return p1;
}

