/*_ wcscoll.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <setlocal.h>
#include <errno.h>
#include <scrtl.h>
#include <lcapi32.h>

int __CLIB wcscoll (const wchar_t *ws1, const wchar_t *ws2) {
 int		ret;
 wchar_t	wpage[LC_CPSZ];
  if (IsCLocale (LC_COLLATE)) {
    ret = wcscmp (ws1, ws2);
  } else {
    if (__wGetLocaleInfo (0, __locale_ident[LC_COLLATE].lgid,
	LOCALE_IDEFAULTCODEPAGE, wpage, LC_CPSZ) == 0) {
      goto error;
    }
    ret = __wCompareString ((int) wcstol (wpage, NULL, 10),
	__locale_ident[LC_COLLATE].lgid, 0, ws1, -1, ws2, -1);
    if (ret == 0) {
      goto error;
    }
    ret -= CMPEQ;
  }
done:
  return ret;
error:
  __set_errno (EINVAL);
  ret = _NLSCMPERROR;
  goto done;
}

#endif

/**/
