/*_ mbschr.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <string.h>
#include <setmbcp.h>

unsigned char * __cdecl _mbschr (const unsigned char *s, unsigned int ch) {
 unsigned char * q;
 unsigned int	sch;
  if (__mbcodepage != 0) {
    do {
      q = (unsigned char *) s;
      if (_ismbblead (sch = *s)) {
        if (s[1] == '\0') {
          sch = 0;
        } else {
          sch = (sch << 8) | s[1];
          s++;
        }
      }
      if (sch == ch) {
        return q;
      }
      s++;
    } while (sch != 0);
    return NULL;
  }
  return (unsigned char *) strchr ((const char *) s, ch);
}

#endif

