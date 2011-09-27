//_ wcstod.c
// Copyright (C) 1995-2001 by Digital Mars
// All Rights Reserved
// www.digitalmars.com

#ifdef _WIN32

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>
#include <scrtl.h>

#define iSBCS		0
#define iMBCS		1
#define jMBCS		2

static int __near __pascal _wcsoffset (char *s, char *ep) {
 int	woff;
 int	mode;
 char *	sp;
 char	ch;
  woff = 0;
  if (ep != NULL) {
    mode = iSBCS;
    for (sp = s; sp != ep; sp++) {
      ch = *sp;
      if (ch == '\0') {
        break;
      }
      if (mode != iMBCS) {
        if (isleadbyte ((unsigned char) ch)) {
          mode = iMBCS;
        } else {
          woff++;
          mode = iSBCS;
        }
      } else {
        woff++;
        mode = jMBCS;
      }
    }
  }
  return woff;
}

double __CLIB wcstod (const wchar_t *wp, wchar_t **wep) {
 double		value = 0.0;
 char *		cp;
 char *		ep;
 size_t		wlen;
 int		ret;
  wlen = wcslen (wp);
  wlen++;				/* include NUL stopper */
  cp = (char *) alloca (wlen * sizeof (wchar_t));
  if (cp != NULL) {
    ret = wcstombs (cp, wp, wlen);
    if (ret != (size_t) -1) {
      value = strtod (cp, &ep);
      if (wep != NULL) {
        *wep = (wchar_t *) wp + _wcsoffset (cp, ep);
      }
    }
  }
  return value;
}

long double __CLIB wcstold (const wchar_t *wp, wchar_t **wep) {
 long double	value = 0.0;
 char *		cp;
 char *		ep;
 size_t		wlen;
 int		ret;
  wlen = wcslen (wp);
  wlen++;				/* include NUL stopper */
  cp = (char *) alloca (wlen * sizeof (wchar_t));
  if (cp != NULL) {
    ret = wcstombs (cp, wp, wlen);
    if (ret != (size_t) -1) {
      value = strtold (cp, &ep);
      if (wep != NULL) {
        *wep = (wchar_t *) wp + _wcsoffset (cp, ep);
      }
    }
  }
  return value;
}

#endif

/**/
