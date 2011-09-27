/*_ mbsncoll.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>
#ifdef _WIN32
#include <lcapi32.h>
#endif

int __cdecl _mbsncoll
(const unsigned char *s1, const unsigned char *s2, size_t size) {
#ifdef _WIN32
  if (__mbcodepage != 0) {
   int	ret;
    ret = __aCompareString (__mbcodepage, __mblcid, 0,
	(const char *) s1, _mbsnbcnt (s1, size),
	(const char *) s2, _mbsnbcnt (s2, size));
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
  return _mbsncmp (s1, s2, size);
}

#endif

