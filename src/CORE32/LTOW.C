/*_ ltow.c						*/
/* Copyright (C) 1995 by Digital Mars Corporation		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>

#define LONGSTRSIZE	64

wchar_t * __CLIB _ltow (long value, wchar_t *wbuf, int radix) {
 char		cbuf[LONGSTRSIZE];
  _ltoa (value, cbuf, radix);
  mbstowcs (wbuf, cbuf, LONGSTRSIZE);
  return wbuf;
}

#endif

/**/
