/*_ strxfrm.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

size_t __CLIB strxfrm (char *s1, const char *s2, size_t size) {
  if (s1 == NULL) {
    size = 0;
  }
  if (IsCLocale (LC_COLLATE) && __locale_codepage == _LOCALE_C_CP) {
    strncpy (s1, s2, size);
    return strlen (s2);
  } else {
   char		apage[LC_CPSZ];
   int		cpage;
   size_t	sz;
    if (__aGetLocaleInfo (0, __locale_ident[LC_COLLATE].lgid,
	LOCALE_IDEFAULTCODEPAGE, apage, LC_CPSZ) == 0) {
      goto error;
    }
    cpage = (int) strtol (apage, NULL, 10);
    sz = __aLCMapString (cpage, __locale_ident[LC_COLLATE].lgid,
	LCMAP_SORTKEY, s2, -1, s1, size);
    if (sz > 0) {
    } else {
      sz = __aLCMapString (cpage, __locale_ident[LC_COLLATE].lgid,
	LCMAP_SORTKEY, s2, -1, NULL, 0);
      if (sz == 0) {
        goto error;
      }
    }
    sz--;
done:
    return sz;
error:
    sz = (size_t) -1;
    goto done;
  }
}

#else

/*_ strxfrm.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<string.h>
#include	<stdlib.h>

size_t strxfrm(char *s1, const char *s2, size_t n)
{
	strncpy(s1,s2,n);
	return strlen(s2);
}

#endif

/**/
