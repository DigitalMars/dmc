/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 */

#ifdef __NT__

#include <scio.h>

HANDLE __conin = 0, __conout = 0;

void _STI_conio ()
{
  SECURITY_ATTRIBUTES security;
  
  security.nLength = sizeof (SECURITY_ATTRIBUTES);
  security.lpSecurityDescriptor = NULL;
  security.bInheritHandle = TRUE;
  __conin = CreateFile ("CONIN$", GENERIC_READ|GENERIC_WRITE,
    FILE_SHARE_READ|FILE_SHARE_WRITE, &security, OPEN_EXISTING, 0, 0);
  __conout = CreateFile ("CONOUT$", GENERIC_READ|GENERIC_WRITE,
    FILE_SHARE_READ|FILE_SHARE_WRITE, &security, OPEN_EXISTING, 0, 0);
}

void _STD_conio ()
{
  CloseHandle (__conout);
  CloseHandle (__conin);
}

#endif
