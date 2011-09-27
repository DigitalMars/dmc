/*_ swab.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<stdlib.h>

void swab(char *src,char *dst,size_t n)
{	char c;

	n >>= 1;
	while (n--)
	{	c = src[0];	/* in case src and dst are the same	*/
		dst[0] = src[1];
		dst[1] = c;
		dst += 2;
		src += 2;
	}
}
