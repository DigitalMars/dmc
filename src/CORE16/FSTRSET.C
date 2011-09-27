/*_ fstrset.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#if __INTSIZE == 4 && (__OS2__ || __NT__)
#define __FAR
#else
#define __FAR __far
#endif

char __FAR * __FAR fstrset(char __FAR *p,int c)
{	char __FAR *pstart = p;

	while (*p)
		*p++ = c;
	return pstart;
}

