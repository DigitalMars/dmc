/*_ ultow.c						*/
/* Copyright (C) 1995 by Digital Mars Corporation		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>

#define LONGSTRSIZE	64

wchar_t * __CLIB _ultow (unsigned long value, wchar_t *wbuf, int radix) {
 char		cbuf[LONGSTRSIZE];
  _ultoa (value, cbuf, radix);
  mbstowcs (wbuf, cbuf, LONGSTRSIZE);
  return wbuf;
}

#endif

/**/
