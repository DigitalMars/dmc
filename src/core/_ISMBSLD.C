/*_ _ismbsld.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

int __cdecl _ismbslead (const unsigned char *s, const unsigned char *c) {
  if (__mbcodepage != 0) {
    while (s <= c) {
      if (*s == '\0') {
        break;
      }
      if (_ismbblead (*s)) {
        if (s == c) {
          return TRUE;
        }
        if (*(s + 1) == '\0') {
          break;
        }
        s++;
      }
      s++;
    }
  }
  return FALSE;
}

#endif

