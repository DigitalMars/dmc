/*_ wcsupr.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

#include "dbgint.h"

wchar_t * __CLIB _wcsupr (wchar_t *ws) {
 wchar_t *	wtmp = NULL;
  if (IsCLocale (LC_CTYPE)) {
   wchar_t *	p;
    for (p = ws; *p != L'\0'; p++) {
      if (iswlower (*p)) {
        *p -= (L'a' - L'A');
      }
    }
  } else {
   int		sz;
    sz = __wLCMapString (0, __locale_ident[LC_CTYPE].lgid,
	LCMAP_UPPERCASE, ws, -1, NULL, 0);
    if (sz == 0) {
      goto error;
    }
//    wtmp = GETWMAIN (sz);
    wtmp = (wchar_t *) _malloc_crt((sz) * sizeof (wchar_t));
    if (wtmp == NULL) {
      goto error;
    }
    if (__wLCMapString (0, __locale_ident[LC_CTYPE].lgid,
	LCMAP_UPPERCASE, ws, -1, wtmp, sz) == 0) {
      goto error;
    }
    memcpy (ws, wtmp, sz * sizeof (wchar_t));
  }
done:
  _free_crt (wtmp);
  return ws;
error:
  ws = NULL;
  goto done;
}

#endif

/**/
