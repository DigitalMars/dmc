/*_ wcschr.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

wchar_t * __CLIB wcschr (const wchar_t *sws, wchar_t wch) {
  do {
    if (*sws == wch) {
      return (wchar_t *) sws;
    }
  } while (*sws++ != L'\0');
  return NULL;
}

#endif

/**/
