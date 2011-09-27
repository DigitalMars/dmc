/*_ mbstrlen.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>

size_t __CLIB _mbstrlen (const char *s) {
 size_t		n;
  if (MB_CUR_MAX != 1) {
    if (MultiByteToWideChar (__locale_codepage, __locale_chars,
		s, -1, NULL, 0) == 0) {
      goto error;
    }
    n = 0;
    while (*s != '\0') {
      if (isleadbyte((BYTE) *s)) {
        if (s[1] == '\0') {
          break;
        }
        s++;
      }
      s++;
      n++;
    }
  } else {
    n = strlen (s);
  }
done:
  return n;
error:
  n = -1;
  goto done;
}

#endif

