/*_ mbtowc.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <errno.h>
#include <scrtl.h>

int __CLIB mbtowc (wchar_t *wc, const char *s, size_t n) {
 BYTE	uch;
 int	len = 0;
 size_t	wlen;
  if (s != NULL && n != 0) {
    uch = (BYTE) *s;
    if (uch == '\0') {
      if (wc != NULL) {
        *wc = L'\0';
      }
    } else {
      if (IsCLocale (LC_CTYPE)) {
        if (wc != NULL) {
          *wc = (wchar_t) uch;
        }
        len = sizeof (char);
      } else {
        wlen = wc != NULL ? 1 : 0;
        if (isleadbyte((BYTE) *s)) {
          if (MB_CUR_MAX < 2 || MB_CUR_MAX > n ||
		MultiByteToWideChar (__locale_codepage, __locale_chars,
		s, MB_CUR_MAX, wc, wlen) == 0) {
            goto error;
          }
          len = MB_CUR_MAX;
        } else {
          if (MultiByteToWideChar (__locale_codepage, __locale_chars,
		s, 1, wc, wlen) == 0) {
            goto error;
          }
          len = sizeof (char);
        }
      }
    }
  }
done:
  return len;
error:
  __set_errno (EILSEQ);
  len = -1;
  goto done;
}

#else

/* see MBYTE.C */

#endif

/**/
