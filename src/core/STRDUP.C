/*_ strdup.c   Tue Jan 31 1995   Modified by: Walter Bright */
/* Copyright (C) 1985-1995 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.2 $ */

#include	<string.h>
#include	<stdlib.h>


/**************************
 * Search for char c in first n bytes pointed to by p.
 * Returns:
 *	pointer to char found, else NULL.
 */

char *strdup(const char *p)
{	char *pnew;
	size_t len;

	len = strlen(p) + 1;
	pnew = malloc(len);
	if (pnew)
		memcpy(pnew,p,len);
	return pnew;
}
