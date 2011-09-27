/*_ strrchr.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<string.h>
#include	<stdlib.h>

/***********************
 * Find last occurrence of char c in string p.
 * Returns:
 *	pointer to last occurrence
 *	NULL if not found
 */

char *strrchr(p,c)
register const char *p;
int c;
{	size_t u;

	u = strlen(p);
	do
	{   if (p[u] == (char)c)
		return (char *) (p + u);
	} while (u--);
	return NULL;
}
