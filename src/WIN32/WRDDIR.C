/*_ wrddir.c						*/
/* Copyright (C) 1995 by Digital Mars Corporation		*/
/* All Rights Reserved					*/

#ifdef __NT__

#define UNICODE		1	/* Enable Windows Unicode API	*/
#define _UNICODE	1	/* Enable C Unicode Runtime	*/

#define _NOCHDIR
#define _NOMKDIR
/*#define _NORDDIR*/

#include "getdos.c"

#endif

/**/
