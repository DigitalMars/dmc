/*_ wcsrchr.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

wchar_t * __CLIB wcsrchr (const wchar_t *sws, wchar_t wch) {
 const wchar_t *	fws = NULL;
  do {
    if (*sws == wch) {
      fws = sws;
    }
  } while (*sws++ != L'\0');
  return (wchar_t *) fws;
}

#endif

/**/
