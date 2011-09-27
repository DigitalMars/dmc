/*_ fstrrchr.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<string.h>

#if __INTSIZE == 4 && (__OS2__ || __NT__)
#define __FAR
#else
#define __FAR __far
#endif

char __FAR * __FAR fstrrchr(p,c)
register const char __FAR *p;
int c;
{	size_t u;

	u = fstrlen(p);
	do
	{   if (p[u] == (char)c)
		return (char __FAR *) (p + u);
	} while (u--);
	return NULL;
}

