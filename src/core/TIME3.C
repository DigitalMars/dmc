/*_ time3.c      Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1986-1994 by Digital Mars  */
/* All Rights Reserved                                  */
/* Written by Walter Bright                             */

#include        <stdlib.h>
#include        <stdio.h>
#include        <time.h>
#include        <errno.h>
#include        <string.h>

#if _MT
#include        "mt.h"
#endif

#undef difftime

double difftime(time_t t1,time_t t2)
{
	return t1 - t2;
}

