/*_ wtoi.c						*/
/* Copyright (C) 1995 by Digital Mars Corporation		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>

#define INTSTRSIZE	32

int __CLIB _wtoi (const wchar_t *wp) {
 char		cbuf[INTSTRSIZE];
 int		value;
 int		ret;
  ret = wcstombs (cbuf, wp, INTSTRSIZE - 1);
  if (ret != -1) {
    cbuf[INTSTRSIZE - 1] = '\0';
    value = atoi (cbuf);
  } else {
    value = 0;
  }
  return value;
}

#endif

/**/
