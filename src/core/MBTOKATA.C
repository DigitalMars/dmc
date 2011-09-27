/*_ mbtokata.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>

unsigned int __cdecl _mbctokata (unsigned int ch) {
  if (_ismbchira (ch)) {
    ch += 0xA1;
    if (ch >= 0x837F) {
      ch++;
    }
  }
  return ch;
}

#endif

