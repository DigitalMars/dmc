/*_ wcsncat.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

wchar_t * __CLIB wcsncat (wchar_t *dws, const wchar_t *sws, size_t size) {
 wchar_t *	pdw = dws;
  while (*pdw++ != L'\0')
    ;
  pdw--;
  while (size > 0) {
    if (*sws == L'\0') {
      break;
    }
    *pdw = *sws;
    pdw++;
    sws++;
    size--;
  }
  *pdw = L'\0';
  return dws;
}

#endif

/**/
