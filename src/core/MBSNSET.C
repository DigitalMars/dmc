/*_ mbsnset.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

unsigned char * __cdecl _mbsnset
(unsigned char *s, unsigned int ch, size_t size) {
  if (__mbcodepage != 0) {
   BOOL			mode = FALSE;
   unsigned char *	p = s;
   unsigned char	cu;
    cu = ch >> 8;
    if (cu != 0) {
      while (size != 0) {
        if (*p == '\0') {
          break;
        }
        mode = mode ? FALSE : _ismbblead (*p);
        if (*(p + 1) == '\0') {
          *p = ' ';
          return s;
        }
        *p++ = cu;
        mode = mode ? FALSE : _ismbblead (*p);
        *p++ = ch;
        size--;
      }
    } else {
      while (size != 0) {
        if (*p == '\0') {
          break;
        }
        mode = mode ? FALSE : _ismbblead (*p);
        *p++ = ch;
        size--;
      }
    }
    if (mode && *p != '\0') {
      *p = ' ';
    }
    return s;
  }
  return (unsigned char *) _strnset ((char *) s, ch, size);
}

#endif

