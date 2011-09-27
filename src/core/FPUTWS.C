/*_ fputws.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#if MSDOS || __OS2__ || _WINDOWS || __NT__
#include <dos.h>
#endif
#include "mt.h"

int __CLIB fputws (const wchar_t *s, FILE *fp) {
 int	ret;
  __fp_lock (fp);
  while (*s != L'\0') {
    if (_fputwc_nlock (*s,fp) == WEOF) {
      ret = EOF;
      goto ret;
    }
    s++;
  }
  ret = 0;
ret:
  __fp_unlock (fp);
  return ret;
}

#endif

/**/
