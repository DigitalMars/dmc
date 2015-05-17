/*_ lccrtpc.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

static char *__make_ascii_envptr (wchar_t *wep) {
 UINT		cPage = __locale_codepage;
 size_t		tsz;
 size_t		csz;
 size_t		rsz;
 char *		tcp;
 char *		cp;
 wchar_t *	wp;
  wp = wep;
  tsz = 0;
  while (*wp != L'\0') {
    csz = WtoMcsSZ (wp, -1);
    if (csz == 0) {
      return NULL;
    }
    wp += wcslen (wp) + 1;
    tsz += csz;
  }
  tsz++;
  tcp = GETCMAIN (tsz);
  if (tcp == NULL) {
    return;
  }
  rsz = tsz - 1;
  wp = wep;
  cp = tcp;
  while (*wp != L'\0') {
    csz = wcstombs (cp, wp, rsz);
    if (csz == CPYERR) {
      free (tcp);
      return NULL;
    }
    wp += wcslen (wp) + 1;
    cp += csz + 1;
    rsz -= csz;
  }
  *cp = '\0';
  return tcp;
}

typedef WINBASEAPI BOOL WINAPI fnCreateProcessW(
    LPCWSTR lpApplicationName,
    LPCWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles,
    DWORD dwCreationFlags,
    LPVOID lpEnvironment,
    LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation);

static fnCreateProcessW* pCreateProcessW;
static int initCreateProcessW;

BOOL __cdecl __wCreateProcess (UINT cPage, LPCWSTR pM, LPCWSTR pC,
LPSECURITY_ATTRIBUTES pP, LPSECURITY_ATTRIBUTES pS, BOOL fH, DWORD fC,
LPVOID pE, LPCWSTR pD, LPSTARTUPINFO pU, LPPROCESS_INFORMATION pI) {
 BOOL		ret;
 size_t		sz;
 char *		mp = NULL;
 char *		cp = NULL;
 char *		dp = NULL;
 char *		ep = NULL;

  if (!initCreateProcessW) {
   pCreateProcessW = (fnCreateProcessW*)GetProcAddress(GetModuleHandle("kernel32"), "CreateProcessW");
   initCreateProcessW = 1;
  }
  if (pCreateProcessW)
    // startup info always empty, so we can just cast it UNICODE
    return (*pCreateProcessW)(pM, pC, pP, pS, fH, fC, pE, pD, (LPSTARTUPINFOW)pU, pI);

  if (cPage == 0) {
    cPage = __locale_codepage;
  }
  if (pM != NULL) {
    sz = WtoMcsSZ (pM, -1);
    if (sz == 0) {
      goto error;
    }
    sz++;
    mp = GETCMAIN (sz);
    if (mp == NULL) {
      goto error;
    }
    if (wcstombs (mp, pM, sz) == CPYERR) {
      goto error;
    }
  }
  if (pC != NULL) {
    sz = WtoMcsSZ (pC, -1);
    if (sz == 0) {
      goto error;
    }
    sz++;
    cp = GETCMAIN (sz);
    if (cp == NULL) {
      goto error;
    }
    if (wcstombs (cp, pC, sz) == CPYERR) {
      goto error;
    }
  }
  if (pD != NULL) {
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
  }
  if (fC & CREATE_UNICODE_ENVIRONMENT) {
    if (pE != NULL) {
      ep = __make_ascii_envptr ((wchar_t *) pE);
      if (ep == NULL) {
        goto error;
      }
      pE = ep;
    }
    fC &= ~CREATE_UNICODE_ENVIRONMENT;
  }
  ret = CreateProcessA (mp, cp, pP, pS, fH, fC, pE, dp, pU, pI);
done:
  free (mp);
  free (cp);
  free (dp);
  free (ep);
  return ret;
error:
  ret = FALSE;
  goto done;
}

#endif

/**/
