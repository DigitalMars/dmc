/* trigerr.c   Sat Apr 15 1989   Modified by: Phil Hinger */
/* Copyright (C) 1985-1994 by Digital Mars */
/* All Rights Reserved				*/

/* Algorithms from Cody and Waite, "Software Manual For The Elementary	*/
/* Functions".								*/

#include	<fltpnt.h>
#include	<math.h>

/* Return pointer to the 4 unsigned shorts in a double	*/
#define P(d)	((unsigned short *)(&d))

/* Turn a signalling NaN into a quiet NaN in place	*/
#define toqnan(x)	(P(x)[3] |= 8)

double __trigerr(int type,double retval,double x,int flag)
{ struct exception e;
  static char _near *name[] = {"sin","cos","tan","sqrt","exp","tanh"};

  e.retval = retval;
  e.type = type;
  e.arg1 = x;
  e.name = name[flag];
  matherr(&e);
  return e.retval;
}

double __matherr(int type,double retval,double arg1,double arg2,const char *name)
{
  struct exception e;

  if (isnan(retval))
	toqnan(retval);			/* convert to quiet NaN		*/
  e.retval = retval;
  e.type = type;
  e.arg1 = arg1;
  e.arg2 = arg2;
  e.name = (char *)name;
  matherr(&e);
  return e.retval;
}
