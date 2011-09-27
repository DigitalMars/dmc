/*_ mbsbtype.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

int __cdecl _mbsbtype (const unsigned char *s, size_t off) {
  if (__mbcodepage != 0) {
   int	type;
    type = _MBC_ILLEGAL;
    do {
      if (*s == '\0') {
        return _MBC_ILLEGAL;
      }
      type = _mbbtype (*s, type);
      s++;
    } while (off-- != 0);
    return type;
  }
  return _MBC_SINGLE;
}

#endif

