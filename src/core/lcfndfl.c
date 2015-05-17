/*_ lcfndfl.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

static BOOL ConvertAsciiToWide (LPWIN32_FIND_DATAW pW, LPWIN32_FIND_DATAA pA) {
  pW->dwFileAttributes = pA->dwFileAttributes;
  pW->ftCreationTime   = pA->ftCreationTime;
  pW->ftLastAccessTime = pA->ftLastAccessTime;
  pW->ftLastWriteTime  = pA->ftLastWriteTime;
  pW->nFileSizeHigh    = pA->nFileSizeHigh;
  pW->nFileSizeLow     = pA->nFileSizeLow;
  pW->dwReserved0      = pA->dwReserved0;
  pW->dwReserved1      = pA->dwReserved1;
  if (mbstowcs (pW->cFileName, pA->cFileName, MAX_PATH) == CPYERR) {
    return FALSE;
  }
  if (mbstowcs (pW->cAlternateFileName, pA->cAlternateFileName, 14)
	== CPYERR) {
    return FALSE;
  }
  return TRUE;
}

typedef WINBASEAPI HANDLE WINAPI fnFindFirstFileW(
    LPCWSTR lpFileName,
    LPWIN32_FIND_DATAW lpFindFileData);

static fnFindFirstFileW* pFindFirstFileW;
static int initFindFirstFileW;

HANDLE __cdecl __wFindFirstFile
(UINT cPage, LPCWSTR pF, LPWIN32_FIND_DATAW pD) {
 HANDLE			ret;
 WIN32_FIND_DATAA	aD;
 size_t			sz;
 char *			cp = NULL;
  
  if (!initFindFirstFileW) {
   pFindFirstFileW = (fnFindFirstFileW*)GetProcAddress(GetModuleHandle("kernel32"), "FindFirstFileW");
   initFindFirstFileW = 1;
  }
  if (pFindFirstFileW)
    return (*pFindFirstFileW)(pF, pD);

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
  ret = FindFirstFileA (cp, &aD);
  if (ret == INVALID_HANDLE_VALUE) {
    goto error;
  }
  if (ConvertAsciiToWide(pD, &aD) == FALSE) {
    goto error;
  }
done:
  free (cp);
  return ret;
error:
  ret = INVALID_HANDLE_VALUE;
  goto done;
}

typedef WINBASEAPI BOOL WINAPI fnFindNextFileW(
    HANDLE hFindFile,
    LPWIN32_FIND_DATAW lpFindFileData);

static fnFindNextFileW* pFindNextFileW;
static int initFindNextFileW;

BOOL __cdecl __wFindNextFile (UINT cPage, HANDLE hF, LPWIN32_FIND_DATAW pD) {
 BOOL			ret;
 WIN32_FIND_DATAA	aD;

  if (!initFindNextFileW) {
   pFindNextFileW = (fnFindNextFileW*)GetProcAddress(GetModuleHandle("kernel32"), "FindNextFileW");
   initFindNextFileW = 1;
  }
  if (pFindNextFileW)
    return (*pFindNextFileW)(hF, pD);

  if (cPage == 0) {
    cPage = __locale_codepage;
  }
  ret = FindNextFileA (hF, &aD);
  if (ret != TRUE) {
    goto error;
  }
  if (ConvertAsciiToWide(pD, &aD) == FALSE) {
    goto error;
  }
done:
  return ret;
error:
  ret = FALSE;
  goto done;
}

#endif

/**/
