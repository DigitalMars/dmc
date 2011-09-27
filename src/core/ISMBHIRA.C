/*_ ismbhira.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

int __cdecl _ismbchira (unsigned int ch) {
  if (__mbcodepage == _KANJI_CP) {
    return ch >= 0x829F && ch <= 0x82F1;
  }
  return FALSE;
}

#endif

