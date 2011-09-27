/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */
 
#ifdef __NT__

#include <dos.h>
#include <fcntl.h>
#include <share.h>
#include <io.h>

int dos_open (const char *filename, int flags)
{
  return _sopen (filename, flags & (_O_RDONLY|_O_WRONLY|_O_RDWR|_O_APPEND),
    flags & (_SH_COMPAT|_SH_DENYRW|_SH_DENYWR|_SH_DENYRD|_SH_DENYNO));
}

#endif

