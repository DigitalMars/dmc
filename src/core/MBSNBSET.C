/*_ mbsnbset.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

unsigned char * __cdecl _mbsnbset
(unsigned char *s, unsigned int ch, size_t size) {
  if (__mbcodepage != 0) {
   unsigned int		cu;
    cu = ch >> 8;
    if (cu != 0) {
     unsigned char *	p = s;
      while (size != 0) {
        if (*p == '\0') {
          break;
        }
        if (*(p + 1) == '\0' || size < 2) {
          *p = ' ';
          break;
        }
        *p++ = cu;
        *p++ = ch;
        size -= 2;
      }
      return s;
    }
  }
  return (unsigned char *) _strnset ((char *) s, ch, size);
}

#endif

