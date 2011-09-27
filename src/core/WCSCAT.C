/*_ wcscat.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

wchar_t * __CLIB wcscat (wchar_t *dws, const wchar_t *sws) {
  wcscpy (dws + wcslen (dws), sws);
  return dws;
}

#endif

/**/
