/*_ wcscspn.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

size_t __CLIB wcscspn (const wchar_t *sws, const wchar_t *cws) {
 const wchar_t *	psw = sws;
 const wchar_t *	pcw;
  while (*psw != L'\0') {
    for (pcw = cws; *pcw != L'\0'; pcw++) {
      if (*pcw == *psw) {
        goto done;
      }
    }
    psw++;
  }
done:
  return psw - sws;
}

#endif

/**/
