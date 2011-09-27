/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */
 
#ifndef _WINDOWS

#include <conio.h>

#ifdef __NT__
#  include <windows.h>
#  include <scio.h>
#endif

int putch (int c)
{
#  ifdef __NT__
    DWORD actual;
    
    if (__conout == INVALID_HANDLE_VALUE)
      return EOF;
    if (!WriteConsole (__conout, &c, 1, &actual, NULL))
      return EOF;
    return c;
#  else
    _AH = 6;
    _DL = (unsigned char)c;
    asm int 21h;
    return c;
#  endif
}

#endif