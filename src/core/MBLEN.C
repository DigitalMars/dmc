/*_ mblen.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>

int __CLIB mblen (const char * s, size_t n) {
 int	len = 0;
  if (s != NULL && n != 0) {
    if (*s != '\0') {
      if (isleadbyte ((BYTE) *s)) {
        if (MB_CUR_MAX < 2 || MB_CUR_MAX > n ||
		MultiByteToWideChar (__locale_codepage, __locale_chars,
		s, MB_CUR_MAX, NULL, 0) == 0) {
          goto error;
        }
        len = MB_CUR_MAX;
      } else {
        if (MultiByteToWideChar (__locale_codepage, __locale_chars,
		s, 1, NULL, 0) == 0) {
          goto error;
        }
        len = sizeof (char);
      }
    }
  }
done:
  return len;
error:
  len = -1;
  goto done;
}

#else

/* see MBYTE.C */

#endif

