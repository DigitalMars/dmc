/*_ fgetws.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#if MSDOS || __OS2__ || _WINDOWS || __NT__
#include <dos.h>
#endif
#include "mt.h"

wchar_t * __CLIB fgetws (wchar_t *s, int n, FILE *fp) {
 wint_t		c;
 wchar_t *	bp = s;
  __fp_lock (fp);
  if (n == 0)				/* if n is zero then return NULL */
    goto err;
  while (--n > 0) {
    c = _fgetwc_nlock (fp);
    if (c != WEOF) {
      if ((*s = c) != L'\n') {
      	s++;
        continue;
      }
      s++;
      break;
    } else {
      if (s == bp) {
        goto err;
      } else {
        break;
      }
    }
  }
  *s = L'\0';
  if (ferror (fp)) {
err:
    bp = NULL;
  }
ret:
  __fp_unlock (fp);
  return bp;
}

#endif

/**/
