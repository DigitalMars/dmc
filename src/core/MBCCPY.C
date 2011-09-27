/*_ mbccpy.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>

void __cdecl _mbccpy (unsigned char *d, const unsigned char *s) {
  if (_ismbblead (d[0] = s[0])) {
    d[1] = s[1];
  }
}

#endif

