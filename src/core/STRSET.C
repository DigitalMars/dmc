/*_ strset.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

char *strset(char *p,int c)
{	char *pstart = p;

	while (*p)
		*p++ = c;
	return pstart;
}
