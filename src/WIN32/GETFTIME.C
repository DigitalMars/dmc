/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#ifdef __NT__

#include <windows.h>
#include <io.h>
#include <dos.h>

int getftime (int fd, struct ftime *FtPtr)
{
  _dos_getftime (fd, (unsigned *)((short *)FtPtr + 1), (unsigned *)((short *)FtPtr + 0));
}

#endif