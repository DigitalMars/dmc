/*_ lccrtfl.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

typedef WINBASEAPI HANDLE WINAPI fnCreateFileW(
    LPCWSTR lpFileName, DWORD dwDesiredAccess,
    DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile);

static fnCreateFileW* pCreateFileW;
static int initCreateFileW;

HANDLE __cdecl __wCreateFile (UINT cPage, LPCWSTR pF, DWORD fA, DWORD fS,
LPSECURITY_ATTRIBUTES pS, DWORD fC, DWORD fT, HANDLE hT) {
 HANDLE		ret;
 size_t		sz;
 char *		cp = NULL;
 
  if (!initCreateFileW)
  {
   pCreateFileW = (fnCreateFileW*)GetProcAddress(GetModuleHandle("kernel32"), "CreateFileW");
   initCreateFileW = 1;
  }
  if (pCreateFileW)
    return (*pCreateFileW)(pF, fA, fS, pS, fC, fT, hT);

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
  ret = CreateFileA (cp, fA, fS, pS, fC, fT, hT);
done:
  free (cp);
  return ret;
error:
  ret = (HANDLE) -1;
  goto done;
}

#endif

/**/
