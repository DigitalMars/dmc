/*_ mbclen.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>

size_t __cdecl _mbclen (const unsigned char *s) {
  return _ismbblead (*s) ? 2 : 1;
}

#endif

