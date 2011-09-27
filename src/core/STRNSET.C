/*_ strnset.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<stdlib.h>

char *strnset(char *p,int c,size_t n)
{	char *pstart = p;

	while (n-- && *p)
		*p++ = c;
	return pstart;
}
