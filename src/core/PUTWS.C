/*_ putws.c						*/
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

int __CLIB _putws (const wchar_t *s) {
 int	ret;
  __fp_lock (stdout);
  while (*s != L'\0') {
    if (putwchar (*s) == WEOF) {
      ret = 1;
      goto L1;
    }
    s++;
  }
  ret = putwchar (L'\n') == EOF;
L1:
  __fp_unlock (stdout);
  return ret;
}

#endif

/**/
