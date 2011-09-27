/*_ towupper.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <ctype.h>
#include <stdio.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

wchar_t __CLIB towupper (wchar_t ch) {
 wchar_t	wch;
  if (ch != WEOF) {
    if (IsCLocale (LC_CTYPE)) {
      if (ch >= L'a' && ch <= L'z') {
        ch -= (L'a' - L'A');
      }
    } else {
      if (ch > 0x00FF || iswlower (ch)) {
        if (__wLCMapString (0, __locale_ident[LC_CTYPE].lgid,
		LCMAP_UPPERCASE, &ch, 1, &wch, 1) != 0) {
          ch = wch;
        }
      }
    }
  }
  return ch;
}

#endif

