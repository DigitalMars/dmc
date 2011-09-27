/*_ mbsninc.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>

unsigned char * __cdecl _mbsninc (const unsigned char *s, size_t size) {
  if (s != NULL) {
    return (unsigned char *) s + _mbsnbcnt (s, size);
  }
  return NULL;
}

#endif

