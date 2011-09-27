/*_ wcsstr.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

wchar_t * __CLIB wcsstr (const wchar_t *ws1, const wchar_t *ws2) {
 size_t		sz1;
 size_t		sz2;
 size_t		sz2b;
 wchar_t	wc2;
  sz2 = wcslen (ws2);
  if (sz2 == 0) {
    return (wchar_t *) ws1;
  }
  sz2b = sz2 * sizeof (wchar_t);
  wc2 = *ws2;
  sz1 = wcslen (ws1);
  while (sz1 >= sz2) {
    if (*ws1 == wc2) {
      if (memcmp (ws1, ws2, sz2b) == 0) {
        return (wchar_t *) ws1;
      }
    }
    ws1++;
    sz1--;
  }
  return NULL;
}

#endif

/**/
