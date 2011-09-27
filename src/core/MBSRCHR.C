/*_ mbsrchr.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <string.h>
#include <setmbcp.h>

unsigned char * __cdecl _mbsrchr (const unsigned char *s, unsigned int ch) {
 unsigned char * p;
 unsigned char * q;
 unsigned int	sch;
  if (__mbcodepage != 0) {
    p = NULL;
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
        p = q;
      }
      s++;
    } while (sch != 0);
    return p;
  }
  return (unsigned char *) strrchr ((const char *) s, ch);
}

#endif

