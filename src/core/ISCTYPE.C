/*_ isctype.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <ctype.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

int __CLIB _isctype (int c, int mask) {
 int	i;
 WORD	ct;
 char	buf[4];
 char	ch;
  if (((unsigned int)(c + 1)) <= 0x0100) {
    return _pctype[c] & mask;
  }
  i = 0;
  ch = c >> 8;
  if (isleadbyte ((BYTE) ch)) {
    buf[i++] = ch;
  }
  buf[i++] = c;
  buf[i] = '\0';
  if (__aGetStringType (0, __locale_ident[LC_CTYPE].lgid,
	CT_CTYPE1, buf, i, &ct) != 0) {
    return (int) (ct & mask);
  }
  return 0;
}

#endif

/**/
