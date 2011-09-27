/*_ wsetloca.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <stdlib.h>
#include <limits.h>
#include <locale.h>
#include <setlocal.h>

static wchar_t *	__wCurrentLocale = NULL;

wchar_t * __cdecl _wsetlocale (int catg, const wchar_t *wloc) {
 size_t		wlen;
 size_t		clen;
 char *		cloc;
 char *		locc;
 wchar_t *	locw;
  if (wloc != NULL) {
    clen = wcstombs (NULL, wloc, 0);
    if (clen == (size_t) -1) {
      return NULL;
    }
    clen++;				/* Include NUL stopper */
    cloc = (char *) malloc (clen);
    if (cloc == NULL) {
      return NULL;
    }
    if (wcstombs (cloc, wloc, clen) == (size_t) -1) {
      free (cloc);
      return NULL;
    }
  } else {
    cloc = NULL;
  }
  locc = setlocale (catg, cloc);
  free (cloc);
  if (locc == NULL) {
    return NULL;
  }
  wlen = mbstowcs (NULL, locc, 0);
  if (wlen == (size_t) -1) {
    return NULL;
  }
  wlen++;				/* Include NUL stopper */
  locw = (wchar_t *) malloc (wlen * sizeof (wchar_t));
  if (locw == NULL) {
    return NULL;
  }
  if (mbstowcs (locw, locc, wlen) == (size_t) -1) {
    free (locw);
    return NULL;
  }
  free (__wCurrentLocale);		/* Release before image */
  return __wCurrentLocale = locw;
}

#endif

/**/
