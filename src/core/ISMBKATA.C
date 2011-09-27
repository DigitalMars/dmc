/*_ ismbkata.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

int __cdecl _ismbckata (unsigned int ch) {
  if (__mbcodepage == _KANJI_CP) {
    return ch >= 0x8340 && ch <= 0x8396 && ch != 0x837F;
  }
  return FALSE;
}

#endif

