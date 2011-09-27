/*_ strnicmp.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<ctype.h>
#include	<stdlib.h>

int strnicmp(const char *s1, const char *s2, size_t n)
{
    unsigned char c1, c2;

    c1 = c2 = 0;
    while (n--)
    {
	c1 = tolower(*(const unsigned char *)s1);
	s1++;
	c2 = tolower(*(const unsigned char *)s2);
	s2++;
	if (!c1 || c1 != c2)
	    break;
    }
    return c1 - c2;
}
