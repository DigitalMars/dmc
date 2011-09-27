/*_ wcsrev.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

wchar_t * __CLIB _wcsrev (wchar_t *sws) {
 wchar_t *	prw = sws;
 wchar_t *	plw = sws;
 wchar_t	wch;
 size_t		size = 0;
  while (*prw != L'\0') {
    size++;
    prw++;
  }
  size /= 2;
  while (size > 0) {
    prw--;
    wch = *plw;
    *plw = *prw;
    *prw = wch;
    plw++;
    size--;
  }
  return sws;
}

#endif

/**/
