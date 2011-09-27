/*_ lcgetcl.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

#include "dbgint.h"

LPWSTR __cdecl __wGetCommandLine (UINT cPage) {
 static short	touch = DNOTHING;
retry:
  if (touch == DETECT_W) {		/* NT3.5 */
    return GetCommandLineW ();
  } else if (touch == DETECT_A) {	/* Win95 or Win32s */
   size_t	csz;
   LPCSTR	ccp;
   LPWSTR	wcp = NULL;
    if (cPage == 0) {
      cPage = __locale_codepage;
    }
    ccp = GetCommandLineA ();
    csz = MtoWlcSZ (ccp, -1);
    if (csz == 0) {
      goto error;
    }
//    wcp = GETWMAIN (csz);
    wcp = (wchar_t *) _malloc_crt((csz) * sizeof (wchar_t));
    if (wcp == NULL) {
      goto error;
    }
    if (MtoWcm (ccp, -1, wcp, csz) == 0) {
      goto error;
    }
done:
    return wcp;
error:
    _free_crt (wcp);
    wcp = NULL;
    goto done;
  } else if (touch == DNOTHING) {
    touch = DETECT_A;
    if (GetCommandLineW() != NULL) {
      touch = DETECT_W;
    }
    goto retry;
  }
  return NULL;
}

#endif

/**/
