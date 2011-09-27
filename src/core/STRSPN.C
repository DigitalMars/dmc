/*_ strspn.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<stdlib.h>

size_t strspn(p1,p2)
const char *p1,*p2;
{	const char *p1start = p1;
	const char *p2start = p2;

	while (*p1)
	{	while (*p2)
		{	if (*p2 == *p1)
				goto L1;;
			p2++;
		}
		break;			/* it's not in p2               */
	    L1:
		p2 = p2start;
		p1++;
	}
	return p1 - p1start;
}
