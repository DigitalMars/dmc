/*_ lcsetcd.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

BOOL __cdecl __wSetCurrentDirectory (UINT cPage, LPCWSTR pD) {
 BOOL		ret;
 size_t		sz;
 char *		dp = NULL;
  if (cPage == 0) {
    cPage = __locale_codepage;
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
  ret = SetCurrentDirectoryA (dp);
done:
  free (dp);
  return ret;
error:
  ret = FALSE;
  goto done;
}

#endif

/**/
