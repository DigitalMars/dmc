/*_ lcgetmf.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>
#include <limits.h>

DWORD __cdecl __wGetModuleFileName
 (UINT cPage, HMODULE hM, LPWSTR pP, DWORD nP) {
 static short	touch = DNOTHING;
retry:
  if (touch == DETECT_W) {		/* NT3.5 */
    return GetModuleFileNameW (hM, pP, nP);
  } else if (touch == DETECT_A) {	/* Win95 or Win32s */
   DWORD	ret;
   size_t	csz;
   LPSTR	ccp = NULL;
    if (cPage == 0) {
      cPage = __locale_codepage;
    }
    csz = nP * MB_LEN_MAX;
    ccp = GETCMAIN (csz);
    if (ccp == NULL) {
      goto error;
    }
    ret = GetModuleFileNameA (hM, ccp, csz);
    if (ret == 0) {
      goto error;
    }
    ret = MtoWcm (ccp, -1, pP, nP);
    if (ret == 0) {
      goto error;
    }
done:
    return ret;
error:
    ret = 0;
    goto done;
  } else if (touch == DNOTHING) {
   wchar_t	buf[8];
    touch = DETECT_A;
    if (GetModuleFileNameW (NULL, buf, 8) > 0) {
      touch = DETECT_W;
    }
    goto retry;
  }
  return 0;
}

#endif

/**/
