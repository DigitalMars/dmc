/*_ lcsetfa.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

typedef WINBASEAPI BOOL WINAPI fnSetFileAttributesW(LPCWSTR pF, DWORD fA);

static fnSetFileAttributesW* pSetFileAttributesW;
static int initSetFileAttributesW;

BOOL __cdecl __wSetFileAttributes (UINT cPage, LPCWSTR pF, DWORD fA) {
 BOOL		ret;
 size_t		sz;
 char *		cp = NULL;

  if (!initSetFileAttributesW) {
   pSetFileAttributesW = (fnSetFileAttributesW*)GetProcAddress(GetModuleHandle("kernel32"), "SetFileAttributesW");
   initSetFileAttributesW = 1;
  }
  if (pSetFileAttributesW)
    return (*pSetFileAttributesW)(pF, fA);
  
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
  ret = SetFileAttributesA (cp, fA);
done:
  free (cp);
  return ret;
error:
  ret = FALSE;
  goto done;
}

#endif

/**/
