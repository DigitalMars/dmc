/*_ lcapi32.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

int __cdecl __wCompareString (UINT cPage,
LCID lId, DWORD fVal, LPCWSTR pS1, int nS1, LPCWSTR pS2, int nS2) {
 static short	touch = DNOTHING;
 size_t		i;
  for (i = 0; i < nS1; i++) {
    if (pS1[i] == L'\0') {
      nS1 = i;
      break;
    }
  }
  for (i = 0; i < nS2; i++) {
    if (pS2[i] == L'\0') {
      nS2 = i;
      break;
    }
  }
  if (nS1 == 0 || nS2 == 0) {
    return nS1 > nS2 ? CMPGT : nS1 < nS2 ? CMPLT : CMPEQ;
  }
retry:
  if (touch == DETECT_W) {		/* NT3.5 */
    return CompareStringW (lId, fVal, pS1, nS1, pS2, nS2);
  } else if (touch == DETECT_A) {	/* Win95 or Win32s */
   int		ret;
   int		sz1;
   int		sz2;
   char *	cp1 = NULL;
   char *	cp2 = NULL;
    if (cPage == 0) {
      cPage = __locale_codepage;
    }
    sz1 = WtoMcsSZ (pS1, nS1);
    if (sz1 == 0) {
      goto error;
    }
    cp1 = GETCMAIN (sz1);
    if (cp1 == NULL) {
      goto error;
    }
    if (WtoMcs (pS1, nS1, cp1, sz1) == 0) {
      goto error;
    }
    sz2 = WtoMcsSZ (pS2, nS2);
    if (sz2 == 0) {
      goto error;
    }
    cp2 = GETCMAIN (sz2);
    if (cp2 == NULL) {
      goto error;
    }
    if (WtoMcs (pS2, nS2, cp2, sz2) == 0) {
      goto error;
    }
    ret = CompareStringA (lId, fVal, cp1, sz1, cp2, sz2);
done:
    free (cp1);
    free (cp2);
    return ret;
error:
    ret = APIERR;
    goto done;
  } else if (touch == DNOTHING) {
    touch = DETECT_A;
    if (CompareStringW (0, 0, L"", 1, L"", 1) != APIERR) {
      touch = DETECT_W;
    }
    goto retry;
  }
  return APIERR;
}

int __cdecl __wGetLocaleInfo
(UINT cPage, LCID lId, LCTYPE lType, LPWSTR pL, int nL) {
 static short	touch = DNOTHING;
retry:
  if (touch == DETECT_W) {		/* NT3.5 */
    return GetLocaleInfoW (lId, lType, pL, nL);
  } else if (touch == DETECT_A) {	/* Win95 or Win32s */
   int		ret;
   int		csz;
   char *	cp = NULL;
    if (cPage == 0) {
      cPage = __locale_codepage;
    }
    csz = GetLocaleInfoA (lId, lType, NULL, 0);
    if (csz == 0) {
      goto error;
    }
    cp = GETCMAIN (csz);
    if (cp == NULL) {
      goto error;
    }
    if (GetLocaleInfoA (lId, lType, cp, csz) == 0) {
      goto error;
    }
    if (nL == 0) {
      ret = MtoWcmSZ (cp, -1);		/* return length only */
      if (ret == 0) {
        goto error;
      }
    } else {
      ret = MtoWcm (cp, -1, pL, nL);
      if (ret == 0) {
        goto error;
      }
    }
done:
    free (cp);
    return ret;
error:
    ret = APIERR;
    goto done;
  } else if (touch == DNOTHING) {
    touch = DETECT_A;
    if (GetLocaleInfoW (0, LOCALE_ILANGUAGE, NULL, 0) != APIERR) {
      touch = DETECT_W;
    }
    goto retry;
  }
  return APIERR;
}

BOOL __cdecl __wGetStringType
(UINT cPage, LCID lId, DWORD fVal, LPCWSTR pS, int nS, LPWORD pC) {
 static short	touch = DNOTHING;
retry:
  if (touch == DETECT_W) {		/* NT3.5 */
    return GetStringTypeW (fVal, pS, nS, pC);
  } else if (touch == DETECT_A) {	/* Win95 or Win32s */
   BOOL		ret;
   int		csz;
   char *	cp = NULL;
   WORD *	wp = NULL;
    if (cPage == 0) {
      cPage = __locale_codepage;
    }
    csz = WtoMcsSZ (pS, nS);
    if (csz == 0) {
      goto error;
    }
    cp = GETCMAIN (csz);
    if (cp == NULL) {
      goto error;
    }
    if (WtoMcs (pS, nS, cp, csz) == 0) {
      goto error;
    }
    wp = (WORD *) malloc ((csz + 1) * sizeof (WORD));
    if (wp == NULL) {
      goto error;
    }
    wp[nS - 1] = wp[nS] = 0xFFFF;
    ret = GetStringTypeA (lId, fVal, cp, csz, wp);
    if (wp[nS - 1] == 0xFFFF || wp[nS] != 0xFFFF) {
      goto error;
    }
    memcpy (pC, wp, nS * sizeof(WORD));
done:
    free (cp);
    free (wp);
    return ret;
error:
    ret = FALSE;
    goto done;
  } else if (touch == DNOTHING) {
   wchar_t	dw[4];
    touch = DETECT_A;
    if (GetStringTypeW (CT_CTYPE1, L"", 1, dw) != APIERR) {
      touch = DETECT_W;
    }
    goto retry;
  }
  return FALSE;
}

int __cdecl __wLCMapString
(UINT cPage, LCID lId, DWORD fVal, LPCWSTR pS, int nS, LPWSTR pD, int nD) {
 static short	touch = DNOTHING;
 size_t		i;
  for (i = 0; i < nS; i++) {
    if (pS[i] == L'\0') {
      nS = i;
      break;
    }
  }
retry:
  if (touch == DETECT_W) {		/* NT3.5 */
    return LCMapStringW (lId, fVal, pS, nS, pD, nD);
  } else if (touch == DETECT_A) {	/* Win95 or Win32s */
   int		ret;
   int		ssz;
   int		dsz;
   char *	csp = NULL;
   char *	cdp = NULL;
    if (cPage == 0) {
      cPage = __locale_codepage;
    }
    ssz = WtoMcsSZ (pS, nS);
    if( ssz == 0) {
      goto error;
    }
    csp = GETCMAIN (ssz);
    if (csp == NULL) {
      goto error;
    }
    if (WtoMcs (pS, nS, csp, ssz) == 0) {
      goto error;
    }
    dsz = LCMapStringA (lId, fVal, csp, ssz, NULL, 0);
    if (dsz == 0) {
      goto error;
    }
    cdp = GETCMAIN (dsz);
    if (cdp == NULL) {
      goto error;
    }
    if (LCMapStringA (lId, fVal, csp, ssz, cdp, dsz) == 0) {
      goto error;
    }
    if (fVal & LCMAP_SORTKEY) {
      ret = dsz;
      if (nD != 0) {
        strncpy ((char *) pD, cdp, nD < dsz ? nD : dsz);
      }
    } else {
      if (nD == 0) {
        ret = MtoWcmSZ (cdp, dsz);
        if (ret == 0) {
          goto error;
        }
      } else {
        ret = MtoWcm (cdp, dsz, pD, nD);
        if (ret == 0) {
          goto error;
        }
      }
    }
done:
    free (csp);
    free (cdp);
    return ret;
error:
    ret = APIERR;
    goto done;
  } else if (touch == DNOTHING) {
    touch = DETECT_A;
    if (LCMapStringW (0, LCMAP_UPPERCASE, L"", 1, NULL, 0) != APIERR) {
      touch = DETECT_W;
    }
    goto retry;
  }
  return APIERR;
}

#endif

/**/
