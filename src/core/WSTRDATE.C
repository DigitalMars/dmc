/*_ wstrdate.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#define UNICODE		1	/* Enable Windows Unicode API	*/
#define _UNICODE	1	/* Enable C Unicode Runtime	*/

#define _NOSTRFTIME
/*#define _NOSTRDATE*/
#define _NOSTRTIME

#include "strf.c"

#endif

/**/
