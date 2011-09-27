/*_ mbslen.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

size_t __cdecl _mbslen (const unsigned char *s) {
 size_t		n;
  if (__mbcodepage != 0) {
    n = 0;
    while (*s != '\0') {
      if (_ismbblead (*s)) {
        if (s[1] == '\0') {
          break;
        }
        s++;
      }
      s++;
      n++;
    }
  } else {
    n = strlen ((const char *) s);
  }
  return n;
}

#endif

