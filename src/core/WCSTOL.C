/*_ wcstol.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
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

long __CLIB wcstol (const wchar_t *wp, wchar_t **wep, int ibase) {
 long		val;
 char *		cp;
 char *		ep;
 size_t		wlen;
 int		ret;
  wlen = wcslen (wp);
  wlen++;				/* include NUL stopper */
  cp = (char *) malloc (wlen * sizeof (wchar_t));
  if (cp != NULL) {
    ret = wcstombs (cp, wp, wlen);
    if (ret != (size_t) -1) {
      val = strtol (cp, &ep, ibase);
      if (wep != NULL) {
        *wep = (wchar_t *) wp + _wcsoffset (cp, ep);
      }
    }
    free (cp);
  }
  return val;
}

unsigned long __CLIB wcstoul (const wchar_t *wp, wchar_t **wep, int ibase) {
 unsigned long	val;
 char *		cp;
 char *		ep;
 size_t		wlen;
 int		ret;
  wlen = wcslen (wp);
  wlen++;				/* include NUL stopper */
  cp = (char *) malloc (wlen * sizeof (wchar_t));
  if (cp != NULL) {
    ret = wcstombs (cp, wp, wlen);
    if (ret != (size_t) -1) {
      val = strtoul (cp, &ep, ibase);
      if (wep != NULL) {
        *wep = (wchar_t *) wp + _wcsoffset (cp, ep);
      }
    }
    free (cp);
  }
  return val;
}

#endif

/**/
