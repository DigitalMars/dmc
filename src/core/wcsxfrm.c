/*_ wcsxfrm.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>
#include <wchar.h>

size_t __CLIB wcsxfrm (wchar_t *ws1, const wchar_t *ws2, size_t size) {
  if (ws1 == NULL) {
    size = 0;
  }
  if (IsCLocale (LC_COLLATE)) {
    wcsncpy (ws1, ws2, size);
    return wcslen (ws2);
  } else {
   wchar_t	wpage[LC_CPSZ];
   int		cpage;
   size_t	sz;
    if (__wGetLocaleInfo (0, __locale_ident[LC_COLLATE].lgid,
	LOCALE_IDEFAULTCODEPAGE, wpage, LC_CPSZ) == 0) {
      goto error;
    }
    cpage = (int) wcstol (wpage, NULL, 10);
    sz = __wLCMapString (cpage, __locale_ident[LC_COLLATE].lgid,
	LCMAP_SORTKEY, ws2, -1, ws1, size);
    if (sz > 0) {
      if ( ws1 != NULL) {		/* convert BYTE to wchar_t */
       int	i;
       BYTE *	bp;
        bp = (BYTE *) ws1;
        for (i = sz - 1; i >= 0; i--) {
          ws1[i] = (wchar_t) bp[i];
        }
      }
    } else {
      sz = __wLCMapString (cpage, __locale_ident[LC_COLLATE].lgid,
	LCMAP_SORTKEY, ws2, -1, NULL, 0);
      if (sz == 0) {
        goto error;
      }
    }
    sz--;
done:
    return sz;
error:
    sz = (size_t) -1;
    goto done;
  }
}

#endif

/**/
