/*_ mbsnbcat.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

unsigned char * __cdecl _mbsnbcat
(unsigned char *d, const unsigned char *s, size_t size) {
  if (__mbcodepage != 0) {
   unsigned char * p = d;
    while (*p != '\0') {
      if (_ismbblead (*p)) {
        if (p[1] == '\0') {
          break;
        }
        p++;
      }
      p++;
    }
    while (size != 0) {
      if (*s == '\0') {
        break;
      }
      if (_ismbblead (*s)) {
        if (s[1] == '\0') {
          break;
        }
        if (size < 2) {
          break;
        }
        *p++ = *s++;
        size--;
      }
      *p++ = *s++;
      size--;
    }
    *p = '\0';
    return d;
  }
  return (unsigned char *) strncat ((char *) d, (const char *) s, size);
}

#endif

