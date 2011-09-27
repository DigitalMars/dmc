/*_ strcspn.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<stdio.h>
#include	<ctype.h>
#include	<stddef.h>
#include	<string.h>
#include	<stdlib.h>

/**************************
 * Search for char c in first n bytes pointed to by p.
 * Returns:
 *	pointer to char found, else NULL.
 */

size_t strcspn(p1,p2)
const char *p1,*p2;
{	const char *p1start = p1;
	const char *p2start = p2;

	while (*p1)
	{	while (*p2)
		{	if (*p2 == *p1)
				goto L1;
			p2++;
		}
		p2 = p2start;
		p1++;
	}
    L1:
	return p1 - p1start;
}
