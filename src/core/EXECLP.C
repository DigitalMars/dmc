/* Copyright (C) 1994-1995 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#include <tchar.h>
#include <sctchar.h>
#include <process.h>
#include <stdio.h>
#include <scrtl.h>

int _texeclp (const _TCHAR *path, const _TCHAR *arglist, ...)
{
  return __tspawn (_P_OVERLAY, path, &arglist, NULL, 1);
}

