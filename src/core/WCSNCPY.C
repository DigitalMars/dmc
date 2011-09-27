/*_ wcsncpy.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

wchar_t * __CLIB wcsncpy (wchar_t *dws, const wchar_t *sws, size_t size) {
 wchar_t *	pdw = dws;
  while (size > 0) {
    *pdw = *sws;
    if(*pdw == L'\0') {
      break;
    }
    sws++;
    pdw++;
    size--;
  }
  while (size > 0) {
    *pdw = L'\0';
    pdw++;
    size--;
  }
  return dws;
}

#endif

/**/
