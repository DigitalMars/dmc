/*_ mbbtype.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <stdio.h>
#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

int __cdecl _mbbtype (unsigned char ch, int type) {
  switch (type) {
  case _MBC_LEAD:
    if (_ismbbtrail (ch)) {
      return _MBC_TRAIL;
    }
    break;
  default:
    if (_ismbblead (ch)) {
      return _MBC_LEAD;
    } else if (_ismbbprint (ch)) {
      return _MBC_SINGLE;
    }
    break;
  }
  return _MBC_ILLEGAL;
}

#endif

