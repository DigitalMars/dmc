/*_ strncoll.c						*/
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

int __CLIB _strncoll (const char *s1, const char *s2, size_t size) {
 int	ret = 0;
 char	apage[LC_CPSZ];
  if (size != 0) {
    if (IsCLocale (LC_COLLATE)) {
      ret = strncmp (s1, s2, size);
    } else {
      if (__aGetLocaleInfo (0, __locale_ident[LC_COLLATE].lgid,
		LOCALE_IDEFAULTCODEPAGE, apage, LC_CPSZ) == 0) {
        goto error;
      }
      ret = __aCompareString ((int) strtol (apage, NULL, 10),
	__locale_ident[LC_COLLATE].lgid, 0, s1, size, s2, size);
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

#else

# if 0
#include <stdlib.h>
#include <string.h>

int (__CLIB _strncoll) (const char *s1, const char *s2, size_t size) {
  return strncmp (s1, s2, size);
}
# endif

#endif

/**/
