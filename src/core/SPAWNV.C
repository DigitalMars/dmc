/* Copyright (C) 1994-1995 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#include <tchar.h>
#include <sctchar.h>
#include <process.h>
#include <stdio.h>
#include <scrtl.h>

int _tspawnv (int mode, const _TCHAR *path, const _TCHAR *const *argv)
{
  return __tspawn (mode, path, argv, NULL, 0);
}
