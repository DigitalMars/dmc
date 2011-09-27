/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#include <io.h>
#include <sys\stat.h>
#include <scrtl.h>

int __umask_pmode = 0;

int _umask (int pmode)
{
  int old_pmode;

  old_pmode = __umask_pmode;
  __umask_pmode = pmode & (_S_IWRITE | _S_IREAD);
  return old_pmode;
}
