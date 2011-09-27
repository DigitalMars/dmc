/*_ fstrnset.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<stdlib.h>

#if __INTSIZE == 4 && (M_UNIX || M_XENIX || __OS2__ || __NT__)
#define __FAR
#else
#define __FAR __far
#endif

char __FAR * __FAR fstrnset(char __FAR *p,int c,size_t n)
{	char __FAR *pstart = p;

	while (n-- && *p)
		*p++ = c;
	return pstart;
}

