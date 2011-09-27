/*_ ismblega.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>

int __cdecl _ismbclegal (unsigned int ch) {
  return _ismbblead (ch >> 8) && _ismbbtrail ((unsigned char) ch);
}

#endif

