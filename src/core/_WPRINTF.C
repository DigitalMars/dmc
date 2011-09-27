/*_ _wprintf.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#define UNICODE		1	/* Enable Windows Unicode API	*/
#define _UNICODE	1	/* Enable C Unicode Runtime	*/

#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <scio.h>
#include <sctchar.h>

int wprintf (const wchar_t *format,...)
{
	return vwprintf (format, (va_list)&(format) + sizeof (format));
}

#endif

/**/
