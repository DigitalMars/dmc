/*_ getws.c						*/
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

wchar_t * __CLIB _getws (wchar_t *s) {
 wint_t		ch;
 wchar_t *	bp = s;
  __fp_lock (stdin);
  ch = getwchar ();
  if (ch == EOF) {
    goto err;
  }
  while (ch != EOF && ch != '\n') {
    *s++ = ch;
    ch = getwchar ();
  }
  *s++ = L'\0';
  if (ferror (stdin)) {
err:
    bp = NULL;
  }
  __fp_unlock (stdin);
  return bp;
}

#endif

/**/
