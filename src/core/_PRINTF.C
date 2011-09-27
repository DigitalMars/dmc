/*_ _printf.c   Mon Jun 6, 1994 */
/* Copyright (C) 1994 by Digital Mars		*/
/* All Rights Reserved				*/

#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<scio.h>


/***********************
 */

int printf(const char *format,...)
{
	return vprintf(format,(va_list)&(format) + sizeof(format));
}

