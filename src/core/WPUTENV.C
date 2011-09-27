/*_ wputenv.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>
#include <string.h>

int __CLIB _wputenv (const wchar_t *newstr) {
 char *		cp;
 size_t		clen;
 int		ret = -1;
  clen = wcstombs (NULL, newstr, 0);
  clen++;				/* include NUL stopper */
  cp = (char *) malloc (clen);
  if (cp != NULL) {
    if (wcstombs (cp, newstr, clen) != (size_t) -1) {
      ret = putenv (cp);
    }
    free (cp);
  }
  return ret;
}

#endif

