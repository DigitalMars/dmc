/*_ decdata.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <locale.h>
#include <setlocal.h>

/* Decimal point constants data.
*/
char	__locale_decimal_const[] = ".";

/* Localized decimal point string.
*/
char *	__locale_decpoint = __locale_decimal_const;

#endif

/**/
