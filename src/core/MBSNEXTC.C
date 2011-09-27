/*_ mbsnextc.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>

unsigned int __cdecl _mbsnextc (const unsigned char *s) {
 unsigned int	sch = *s;
  return _ismbblead (sch) ? (sch << 8) | s[1] : sch;
}

#endif

