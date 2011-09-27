/* Copyright (C) 1994-1995 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#include <tchar.h>
#include <sctchar.h>
#include <process.h>
#include <stdio.h>
#include <scrtl.h>

int _texecvp (const _TCHAR *cmd, const _TCHAR * const *argv)
{
  return __tspawn (_P_OVERLAY, cmd, argv, NULL, 1);
}
