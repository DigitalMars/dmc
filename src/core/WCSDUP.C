/*_ wcsdup.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>
#include <malloc.h>

wchar_t * __CLIB _wcsdup (const wchar_t *sws) {
 wchar_t *	dws;
  if (sws != NULL) {
    dws = (wchar_t *) malloc ((wcslen (sws) + 1) * sizeof (wchar_t));
    if (dws != NULL) {
      return wcscpy (dws, sws);
    }
  }
  return NULL;
}

#endif

/**/
