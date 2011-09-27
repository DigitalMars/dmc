/* Copyright (C) 1994-1995 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#include <tchar.h>
#include <sctchar.h>
#include <process.h>
#include <stdio.h>
#include <scrtl.h>

int _tspawnlpe (int mode, const _TCHAR *path, const _TCHAR *arglist, ...)
{
  const _TCHAR *const *kludge = __tseek_envp (&arglist);
  return __tspawn (mode, path, &arglist, kludge, 1);
}

