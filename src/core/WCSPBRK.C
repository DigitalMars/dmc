/*_ wcspbrk.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

wchar_t * __CLIB wcspbrk (const wchar_t *sws, const wchar_t *cws) {
 const wchar_t *	pcw;
  while (*sws != L'\0') {
    for (pcw = cws; *pcw != L'\0'; pcw++) {
      if (*pcw == *sws) {
        return (wchar_t *) sws;
      }
    }
    sws++;
  }
  return NULL;
}

#endif

/**/
