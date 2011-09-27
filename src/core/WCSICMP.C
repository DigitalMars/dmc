/*_ wcsicmp.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include <setlocal.h>

int __CLIB _wcsicmp (const wchar_t *ws1, const wchar_t *ws2) {
 wchar_t	wc1;
 wchar_t	wc2;
 int		ret = 0;
  if (IsCLocale (LC_CTYPE)) {
    do {
      wc1 = *ws1 >= L'A' && *ws1 <= L'Z' ? *ws1 + (L'a' - L'A') : *ws1;
      wc2 = *ws2 >= L'A' && *ws2 <= L'Z' ? *ws2 + (L'a' - L'A') : *ws2;
      if (wc1 != wc2) {
        break;
      }
      ws1++;
      ws2++;
    } while (wc1 != L'\0');
  } else {
    do {
      wc1 = towlower (*ws1);
      wc2 = towlower (*ws2);
      if (wc1 != wc2) {
        break;
      }
      ws1++;
      ws2++;
    } while (wc1 != L'\0');
  }
  if (wc1 < wc2) {
    ret = -1;
  } else if (wc1 > wc2) {
    ret = 1;
  }
  return ret;
}

#endif

/**/
