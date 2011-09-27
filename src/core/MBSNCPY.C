/*_ mbsncpy.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

unsigned char * __cdecl _mbsncpy
(unsigned char *d, const unsigned char *s, size_t size) {
  if (__mbcodepage != 0) {
   unsigned char * p = d;
    while (size != 0) {
      if (*s == '\0') {
        break;
      }
      if (_ismbblead (*s)) {
        if (*(s + 1) == '\0') {
          break;
        }
        *p++ = *s++;
      }
      *p++ = *s++;
      size--;
    }
    while (size != 0) {
      *p++ = '\0';
      size--;
    }
    return d;
  }
  return (unsigned char *) strncpy ((char *) d, (const char *) s, size);
}

#endif

