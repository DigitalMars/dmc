/*_arcerr.c   Sat Apr 15 1989   Modified by: Phil Hinger */
/* Copyright (C) 1985-1994 by Digital Mars */
/* All Rights Reserved				*/

/* Algorithms from Cody and Waite, "Software Manual For The Elementary	*/
/* Functions".								*/

#include	<fltpnt.h>
#include	<math.h>

/*************************
 */

double _arcerr(int flag,double x)
{
	struct exception e;
	
	e.arg1 = x;
	e.name = (flag) ? "acos" : "asin";
	e.type = DOMAIN;
	e.retval = NAN;
	matherr(&e);
	return e.retval;
}

