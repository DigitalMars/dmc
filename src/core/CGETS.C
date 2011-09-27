/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#ifndef _WINDOWS

#include <conio.h>
#ifdef __NT__
# include <scio.h>
#else
# include <string.h>
# include <scrtl.h>
#endif

char *_cgets (char *buffer)
{
# ifdef __NT__
    DWORD old_mode, actual;
    char *end;
    
    buffer[1] = 0;
    if (__conin != INVALID_HANDLE_VALUE)
    {
      GetConsoleMode (__conin, &old_mode);
      SetConsoleMode (__conin, ENABLE_PROCESSED_INPUT|ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT);
      if (ReadConsole (__conin, buffer + 2, (DWORD)buffer[0], &actual, NULL))
      {
        if (actual == *buffer) /* need room for nul */
          actual -= 1;
        end = buffer + 2 + actual;
        *end-- = '\0';
        if (*end == '\n')
          *end-- = '\0';
        if (*end == '\r')
          *end-- = '\0';
        buffer[1] = (unsigned char)(end - (buffer + 1));
      }
      SetConsoleMode (__conin, old_mode);
    }
    return buffer + 2;
# else
    asm
    {
#     if __LARGE__ || __COMPACT__
        push ds
        lds  dx,buffer
#     else
        mov  INTDX,buffer
#     endif
      mov  ah,0ah
      int  21h
#     if __LARGE__ || __COMPACT__
        pop  ds
#     endif
    }
    *strchr (buffer + 2, '\r') = '\0';
    return buffer + 2;
# endif
}

#endif