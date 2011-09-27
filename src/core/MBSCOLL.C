/*_ mbscoll.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>
#ifdef _WIN32
#include <lcapi32.h>
#endif

int __cdecl _mbscoll (const unsigned char *s1, const unsigned char *s2) {
#ifdef _WIN32
  if (__mbcodepage != 0) {
   int	ret;
    ret = __aCompareString (__mbcodepage, __mblcid, 0,
	(const char *) s1, -1, (const char *) s2, -1);
    if (ret == 0) {
      goto error;
    }
    ret -= CMPEQ;
done:
    return ret;
error:
    ret = _NLSCMPERROR;
    goto done;
  }
#endif
  return _mbscmp (s1, s2);
}

#endif

