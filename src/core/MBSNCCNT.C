/*_ mbsnccnt.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>

size_t __cdecl _mbsnccnt (const unsigned char *s, size_t size) {
 size_t	count = 0;
 size_t	i;
 int	mode = 0;
  for (i = 0; i < size; i++) {
    if (s[i] == '\0') {
      break;
    }
    if (mode != _M1) {
      if (_ismbblead (s[i])) {
        mode = _M1;
      } else {
        mode = 0;
        count++;
      }
    } else {
      mode = _M2;
      count++;
    }
  }
  return count;
}

#endif

