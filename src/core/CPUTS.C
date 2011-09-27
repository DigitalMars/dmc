/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#ifndef _WINDOWS

#include <conio.h>
#ifdef __NT__
# include <scio.h>
# include <string.h>
#endif

int _cputs (const char *string)
{
# ifdef __NT__
    DWORD actual;
    
    if (__conout == INVALID_HANDLE_VALUE)
      return -1;
    if (!WriteConsole (__conout, string, strlen (string), &actual, NULL))
      return -1;
    return 0;
# else
    for (; *string != '\0'; string += 1)
      if (putch (*string) == EOF)
        return -1;
    return 0;
# endif
}

#endif