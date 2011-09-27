/*_ wcsncmp.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

int __CLIB wcsncmp (const wchar_t *ws1, const wchar_t *ws2, size_t size) {
 int		ret = 0;
  if (size != 0) {
    do {
      if (*ws1 != *ws2) {
        break;
      }
      if (*ws1 == L'\0') {
        break;
      }
      ws1++;
      ws2++;
      size--;
    } while (size != 0);
    if (*ws1 < *ws2) {
      ret = -1;
    } else if (*ws1 > *ws2) {
      ret = 1;
    }
  }
  return ret;
}

#endif

/**/
