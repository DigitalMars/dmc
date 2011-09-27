/*_ wtol.c						*/
/* Copyright (C) 1995 by Digital Mars Corporation		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>

#define LONGSTRSIZE	64

long __CLIB _wtol (const wchar_t *wp) {
 char		cbuf[LONGSTRSIZE];
 long		value;
 int		ret;
  ret = wcstombs (cbuf, wp, LONGSTRSIZE - 1);
  if (ret != -1) {
    cbuf[LONGSTRSIZE - 1] = '\0';
    value = atol (cbuf);
  } else {
    value = 0;
  }
  return value;
}

#endif

/**/
