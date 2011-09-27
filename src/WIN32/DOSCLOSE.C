/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */
 
#ifdef __NT__

#include <dos.h>
#include <fcntl.h>
#include <share.h>
#include <io.h>

int dos_close (int fd)
{
  return _close (fd);
}

#endif

