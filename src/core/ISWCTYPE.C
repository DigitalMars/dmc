/*_ iswctype.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <ctype.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

int __CLIB iswctype (wchar_t wch, wctype_t mask) {
 wint_t		flg;
  if (wch != WEOF) {
    if (wch <= 0x00FF) {
      return (int) (_pwctype[wch] & mask);
    } else {
      if (!IsCLocale (LC_CTYPE)) {
        if (__wGetStringType (0, __locale_ident[LC_CTYPE].lgid,
		CT_CTYPE1, &wch, 1, &flg) != 0) {
          return (int) (flg & mask);
        }
      }
    }
  }
  return 0;
}

#endif

/**/
