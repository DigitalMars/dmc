/*_ gcvt.c				*/
/* Copyright (C) 1986-1994 by Digital Mars	*/
/* All Rights Reserved			*/
/* $Revision: 1.1.1.1 $ 			*/

#include	<stdlib.h>
#include	<string.h>
#include	<float.h>
#include	<stdio.h>
#include	<fltpnt.h>
#include	<ctype.h>

#if _MT
#include	"mt.h"
#endif

#define DIGMAX	(DBL_DIG*2)	/* max # of digits in string		*/
				/* (*2 is a good fudge factor)		*/
#if !_MT
static char digstr[DIGMAX + 1 + 1];	/* +1 for end of string		*/
					/* +1 in case rounding adds	*/
					/* another digit		*/
#endif

/*******************************
 */

static void killzeroes(char *p)
{
    char *q;

    // Knock out the trailing zeroes
    for (q = p+strlen(p); q>p && q[-1]=='0'; q--)
	;

    *q = 0;
}

/*************************************************
 */

char *gcvt(double v, int precision, char *buffer)
{
    char *b = buffer;
    int dec;
    char *p;
    int sign;

    p = _ecvt(v, precision, &dec, &sign);

    if (sign)
	*b++ = '-';
	    
    if (dec > precision)
	{
	killzeroes(p+1);
	sprintf(b, "%c.%se+%03d", *p, p+1, dec-1);
	}
    else if (dec == precision)
	strcpy(b, p);
    else if (dec > 0)
	{
	killzeroes(p+dec);
	strncpy(b, p, dec);
	strcpy(b+dec, ".");
	strcpy(b+dec+1, p+dec);
	}
    else if (dec == 0)
	{
	killzeroes(p);
	sprintf(b, "0.%s", p);
	}
    else
	{
	killzeroes(p+1);
	sprintf(b, "%c.%se%04d", *p, p+1, dec-1);
	}

    return buffer;
}

