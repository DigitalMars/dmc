/*_ wcsset.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

wchar_t * __CLIB _wcsset (wchar_t *sws, wchar_t val) {
 wchar_t *	psw = sws;
  while (*psw != L'\0') {
    *psw++ = val;
  }
  return sws;
}

#endif

/**/
