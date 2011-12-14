/*_ wcsnicol.c						*/
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
#include <wchar.h>

int __CLIB _wcsnicoll (const wchar_t *ws1, const wchar_t *ws2, size_t size) {
 int		ret = 0;
 wchar_t	wpage[LC_CPSZ];
  if (size != 0) {
    if (IsCLocale (LC_COLLATE)) {
      ret = wcsnicmp (ws1, ws2, size);
    } else {
      if (__wGetLocaleInfo (0, __locale_ident[LC_COLLATE].lgid,
		LOCALE_IDEFAULTCODEPAGE, wpage, LC_CPSZ) == 0) {
        goto error;
      }
      ret = __wCompareString ((int) wcstol (wpage, NULL, 10),
	__locale_ident[LC_COLLATE].lgid, NORM_IGNORECASE, ws1, size, ws2, size);
      if (ret == 0) {
        goto error;
      }
      ret -= CMPEQ;
    }
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
