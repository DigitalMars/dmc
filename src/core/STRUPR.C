/*_ strupr.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

char * __CLIB strupr (char * s) {
 char *		stmp = NULL;
  if (IsCLocale (LC_CTYPE)) {
   char *	p;
    for (p = s; *p != '\0'; p++) {
      if (islower ((BYTE) *p)) {
        *p -= ('a' - 'A');
      }
    }
  } else {
   int		sz;
    sz = __aLCMapString (0, __locale_ident[LC_CTYPE].lgid,
	LCMAP_UPPERCASE, s, -1, NULL, 0);
    if (sz == 0) {
      goto error;
    }
    stmp = GETCMAIN (sz);
    if (stmp == NULL) {
      goto error;
    }
    if (__aLCMapString (0, __locale_ident[LC_CTYPE].lgid,
	LCMAP_UPPERCASE, s, -1, stmp, sz) == 0) {
      goto error;
    }
    memcpy (s, stmp, sz);
  }
done:
  free (stmp);
  return s;
error:
  s = NULL;
  goto done;
}

#else

/*_ strupr.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<ctype.h>

char *strupr(char *p)
{	char *pold = p;

	while (*p)
	{	*p = toupper(*p);
		p++;
	}
	return pold;
}

#endif

/**/
