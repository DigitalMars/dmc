/*_ wgetenv.c						*/
/* Copyright (C) 1995 by Digital Mars Corporation		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setlocal.h>
#include <lcapi32.h>
#include "semlock.h"
#include "mt.h"

extern wchar_t ** _wenviron;
extern void _setenvp (void);

wchar_t * __CLIB _wgetenv (const wchar_t *var) {
 wchar_t *	wep = NULL;
 wchar_t * *	envptr;
 wchar_t *	p;
 size_t		vlen;
  LockSemaphore (_semEnvironment);
  if (_wenviron == NULL) {
    _setenvp();				/* for DLL */
  }
  if (_wenviron != NULL && var != NULL) {
    vlen = wcslen (var);
    for (envptr = _wenviron; *envptr != NULL; envptr++) {
      p = wcschr (*envptr, L'=');
      if (p != NULL) {
        if (p - *envptr == vlen) {
          if (wcsnicmp (*envptr, var, vlen) == 0) {
            wep = p + 1;
            break;
          }
        }
      }
    }
  }
  UnlockSemaphore (_semEnvironment);
  return wep;
}

#endif

/**/
