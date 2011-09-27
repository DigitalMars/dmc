/*_ mbsdec.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

unsigned char * _mbsdec (const unsigned char *sp, const unsigned char *cp) {
  if (cp != sp) {
    if (cp != NULL) {
     unsigned char * p;
      cp--;
      if (__mbcodepage != 0) {
        if (_ismbblead (cp[0])) {
          return (unsigned char *) cp - 1;
        }
        p = (unsigned char *) cp;
        while (p != sp) {
          if (!_ismbblead (p[-1])) {
            break;
          }
          p--;
        }
        return (unsigned char *) cp - ((p - cp) & 1);
      }
      return (unsigned char *) cp;
    }
  }
  return NULL;
}

#endif

