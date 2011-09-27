/*_ wcsnset.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

wchar_t * __CLIB _wcsnset (wchar_t *sws, wchar_t val, size_t size) {
 wchar_t *	psw = sws;
  while (size > 0) {
    if (*sws == L'\0') {
      break;
    }
    *sws = val;
    sws++;
    size--;
  }
  return psw;
}

#endif

/**/
