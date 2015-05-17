/*_ lcmovfl.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

typedef WINBASEAPI BOOL WINAPI fnMoveFileW(LPCWSTR lpExistingFileName,
    LPCWSTR lpNewFileName);

static fnMoveFileW* pMoveFileW;
static int initMoveFileW;

BOOL __cdecl __wMoveFile (UINT cPage, LPCWSTR pS, LPCWSTR pD) {
 BOOL		ret;
 size_t		sz;
 char *		sp = NULL;
 char *		dp = NULL;

  if (!initMoveFileW) {
   pMoveFileW = (fnMoveFileW*)GetProcAddress(GetModuleHandle("kernel32"), "MoveFileW");
   initMoveFileW = 1;
  }
  if (pMoveFileW)
    return (*pMoveFileW)(pS, pD);

  if (cPage == 0) {
    cPage = __locale_codepage;
  }
  sz = WtoMcsSZ (pS, -1);
  if (sz == 0) {
    goto error;
  }
  sz++;
  sp = GETCMAIN (sz);
  if (sp == NULL) {
    goto error;
  }
  if (wcstombs (sp, pS, sz) == CPYERR) {
    goto error;
  }
  sz = WtoMcsSZ (pD, -1);
  if (sz == 0) {
    goto error;
  }
  sz++;
  dp = GETCMAIN (sz);
  if (dp == NULL) {
    goto error;
  }
  if (wcstombs (dp, pD, sz) == CPYERR) {
    goto error;
  }
  ret = MoveFileA (sp, dp);
done:
  free (sp);
  free (dp);
  return ret;
error:
  ret = FALSE;
  goto done;
}

#endif

/**/
