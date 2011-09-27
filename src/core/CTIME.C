/*_ ctime.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1986-1994 by Digital Mars	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/

#include	<tchar.h>
#include	<sctchar.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<time.h>
#include	<errno.h>
#include	<string.h>

#if _MT
#include	"mt.h"
#endif

/***************************
 * Convert time from time_t to string returned by asctime().
 */

_TCHAR *_tctime(const time_t *pt)
{
	return _tasctime(localtime(pt));
}

