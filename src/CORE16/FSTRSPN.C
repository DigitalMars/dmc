/*_ fstrspn.c	 Sun Apr 16 1989   Modified by: Walter Bright */
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

size_t __FAR fstrspn(p1,p2)
const char __FAR *p1, __FAR *p2;
{	const char __FAR *p1start = p1;
	const char __FAR *p2start = p2;

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

