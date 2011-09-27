/*_ wctomb.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <errno.h>
#include <scrtl.h>

int __CLIB wctomb (char *s, wchar_t wch) {
 int	len = 0;
 BOOL	stus;
  if (s != NULL) {
    if (IsCLocale (LC_CTYPE)) {
      if (wch > 0x00FF) {
        goto error;
      }
      *s = (char) wch;
      len = sizeof (char);
    } else {
      len = WideCharToMultiByte (__locale_codepage,
	WC_COMPOSITECHECK | WC_SEPCHARS, &wch, 1, s, MB_CUR_MAX, NULL, &stus);
      if (len == 0 || stus != 0) {
        goto error;
      }
    }
  }
done:
  return len;
error:
  __set_errno (EILSEQ);
  len = -1;
  goto done;
}

#else

/* see MBYTE.C */

#endif

/**/
