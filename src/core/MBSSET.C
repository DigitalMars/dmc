/*_ mbsset.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbstring.h>
#include <setmbcp.h>

unsigned char * __cdecl _mbsset (unsigned char *s, unsigned int ch) {
  if (__mbcodepage != 0) {
   unsigned int		cu;
    cu = ch >> 8;
    if (cu != 0) {
     unsigned char *	p = s;
      while (*p != '\0') {
        if (*(p + 1) == '\0') {
          *p = ' ';
          break;
        }
        *p++ = cu;
        *p++ = ch;
      }
      return s;
    }
  }
  return (unsigned char *) _strset ((char *) s, ch);
}

#endif

