/*_ mbsnbcnt.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>

size_t __cdecl _mbsnbcnt (const unsigned char *s, size_t size) {
 const unsigned char * p = s;
 size_t		i;
  for (i = 0; i < size; i++) {
    if (*p == '\0') {
      break;
    }
    if (_ismbblead (*p)) {
      if (*(p + 1) == '\0') {
        break;
      }
      p++;
    }
    p++;
  }
  return p - s;
}

#endif

