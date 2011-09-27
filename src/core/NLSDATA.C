/*_ nlsdata.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>

/* Multibyte size, define MB_CUR_MAX, see CTYPE.H, STDLIB.H, and SETLOCAL.H
*/
int	__locale_mbsize = 1;

/* Locale code page.
*/
UINT	__locale_codepage = _LOCALE_C_CP;

/* Locale ident's.
*/
LCIDENT	__locale_ident[] = {		// LC_MAX - LC_MIN + 1
  {_LOCALE_C, 0, 0, _LOCALE_C, 0, 0, 0},	/* LC_ALL	*/
  {_LOCALE_C, 0, 0, _LOCALE_C, 0, 0, 0},	/* LC_COLLATE	*/
  {_LOCALE_C, 0, 0, _LOCALE_C, 0, 0, 0},	/* LC_CTYPE	*/
  {_LOCALE_C, 0, 0, _LOCALE_C, 0, 0, 0},	/* LC_MONETARY	*/
  {_LOCALE_C, 0, 0, _LOCALE_C, 0, 0, 0},	/* LC_NUMERIC	*/
  {_LOCALE_C, 0, 0, _LOCALE_C, 0, 0, 0}		/* LC_TIME	*/
};

#endif

/**/
