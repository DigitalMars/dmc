/*_ lcgetcd.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

typedef WINBASEAPI DWORD WINAPI fnGetCurrentDirectoryW(
    DWORD nBufferLength, LPWSTR lpBuffer);

static fnGetCurrentDirectoryW* pGetCurrentDirectoryW;
static int initGetCurrentDirectoryW;

DWORD __cdecl __wGetCurrentDirectory (UINT cPage, DWORD nD, LPWSTR pD) {
 BOOL		ret;
 size_t		sz;
 char *		dp = NULL;

  if (!initGetCurrentDirectoryW) {
   pGetCurrentDirectoryW = (fnGetCurrentDirectoryW*)GetProcAddress(GetModuleHandle("kernel32"), "GetCurrentDirectoryW");
   initGetCurrentDirectoryW = 1;
  }
  if (pGetCurrentDirectoryW)
    return (*pGetCurrentDirectoryW)(nD, pD);

  if (cPage == 0) {
    cPage = __locale_codepage;
  }
  if (nD != 0) {
    sz = nD * MB_CUR_MAX;
    dp = GETCMAIN (sz);
    if (dp == NULL) {
      goto error;
    }
    ret = GetCurrentDirectoryA (sz, dp);
    if (ret == 0) {
      goto error;
    }
    ret = mbstowcs (pD, dp, nD);
    if (ret == CPYERR) {
      goto error;
    }
  } else {
    ret = GetCurrentDirectoryA (0, (LPSTR) pD);
    if (ret == 0) {
      goto error;
    }
  }
done:
  free (dp);
  return ret;
error:
  ret = 0;
  goto done;
}

#endif

/**/
