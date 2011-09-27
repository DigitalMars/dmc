/*_ mbtohira.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>

unsigned int __cdecl _mbctohira (unsigned int ch) {
  if (_ismbckata (ch)) {
    if (ch <= 0x8393) {
      if (ch >= 0x837F) {
        ch--;
      }
      ch -= 0xA1;
    }
  }
  return ch;
}

#endif

