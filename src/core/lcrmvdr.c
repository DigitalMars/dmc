/*_ lcrmvdr.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

BOOL __cdecl __wRemoveDirectory (UINT cPage, LPCWSTR pD) {
 BOOL		ret;
 size_t		sz;
 char *		cp = NULL;
  if (cPage == 0) {
    cPage = __locale_codepage;
  }
  sz = WtoMcsSZ (pD, -1);
  if (sz == 0) {
    goto error;
  }
  sz++;
  cp = GETCMAIN (sz);
  if (cp == NULL) {
    goto error;
  }
  if (wcstombs (cp, pD, sz) == CPYERR) {
    goto error;
  }
  ret = RemoveDirectoryA (cp);
done:
  free (cp);
  return ret;
error:
  ret = FALSE;
  goto done;
}

#endif

/**/
