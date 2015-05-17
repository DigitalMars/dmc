/*_ lcgetfa.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

typedef WINBASEAPI DWORD WINAPI fnGetFileAttributesW(LPCWSTR lpFileName);

static fnGetFileAttributesW* pGetFileAttributesW;
static int initGetFileAttributesW;

DWORD __cdecl __wGetFileAttributes (UINT cPage, LPCWSTR pF) {
 DWORD		ret;
 size_t		sz;
 char *		cp = NULL;

  if (!initGetFileAttributesW) {
   pGetFileAttributesW = (fnGetFileAttributesW*)GetProcAddress(GetModuleHandle("kernel32"), "GetFileAttributesW");
   initGetFileAttributesW = 1;
  }
  if (pGetFileAttributesW)
    return (*pGetFileAttributesW)(pF);

  if (cPage == 0) {
    cPage = __locale_codepage;
  }
  sz = WtoMcsSZ (pF, -1);
  if (sz == 0) {
    goto error;
  }
  sz++;
  cp = GETCMAIN (sz);
  if (cp == NULL) {
    goto error;
  }
  if (wcstombs (cp, pF, sz) == CPYERR) {
    goto error;
  }
  ret = GetFileAttributesA (cp);
done:
  free (cp);
  return ret;
error:
  ret = 0xFFFFFFFF;
  goto done;
}

#endif

/**/
