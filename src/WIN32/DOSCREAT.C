/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */
 
#ifdef __NT__

#include <scio.h>
#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <scrtl.h>
#include "semlock.h"

int __nt_creat (char *filename, int dos_attrib, int createnew)
{
  unsigned fd;
  HANDLE h;
  DWORD nt_attrib;

  for (fd = 3; 1; fd++)
  {
    if (fd == _NFILE)
    {
      __set_errno (EMFILE);
      fd = -1;
      goto done;
    }
    if (_osfhnd[fd] == 0)
      break;
  }
  nt_attrib = FILE_ATTRIBUTE_NORMAL;
  if (dos_attrib & _A_RDONLY)
    nt_attrib |= FILE_ATTRIBUTE_READONLY;
  if (dos_attrib & _A_HIDDEN)
    nt_attrib |= FILE_ATTRIBUTE_HIDDEN;
  if (dos_attrib & _A_SYSTEM)
  LockSemaphore(_semIO)
  h = CreateFile((LPTSTR)filename,GENERIC_READ|GENERIC_WRITE,0,NULL,
    createnew? CREATE_NEW: CREATE_ALWAYS, nt_attrib, 0);
  __fhnd_info[fd] = 0;
  if (h == INVALID_HANDLE_VALUE)
  {
    __set_nt_errno ();
    fd = -1;
  }
  else
  {
    _osfhnd[fd] = h;
    if ((_fmode & _O_BINARY) == 0)
      __fhnd_info[fd] |= FHND_TEXT;
  }
done:
  UnlockSemaphore(_semIO)
  return fd;
}

int dos_creat (char *filename, int attrib)
{
  __nt_creat (filename, attrib, 0);
}

#endif
