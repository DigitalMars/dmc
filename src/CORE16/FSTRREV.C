/*_ fstrrev.c	 Sun Apr 16 1989   Modified by: Walter Bright */
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

char __FAR * __FAR fstrrev(char __FAR *p)
{	char __FAR *pend, __FAR *pstart = p;

	pend = p + fstrlen(p) - 1;
	while (p < pend)
	{	*p ^= *pend;
		*pend ^= *p;
		*p++ ^= *pend--;	/* swap *p and *pend		*/
	}
	return pstart;
}

