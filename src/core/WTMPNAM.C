/*_ wtmpnam.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#define UNICODE		1	/* Enable Windows Unicode API	*/
#define _UNICODE	1	/* Enable C Unicode Runtime	*/
#ifdef  _MBCS
#undef  _MBCS			/* Disable MBCS code		*/
#endif

#include "tmpnam.c"

#endif

/**/
