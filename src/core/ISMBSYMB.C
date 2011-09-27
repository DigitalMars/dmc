/*_ ismbsymb.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

int __cdecl _ismbcsymbol (unsigned int ch) {
  if (__mbcodepage == _KANJI_CP) {
    return ch >= 0x8141 && ch <= 0x81AC && ch != 0x817F;
  }
  return FALSE;
}

#endif

