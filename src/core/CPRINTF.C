/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#ifndef _WINDOWS

#include <conio.h>
#include <scio.h>

int _cprintf (const char *format, ...)
{
  int count;

  count = _pformat ((fp_t)_putch, NULL, format, (va_list)&(format) + sizeof (format));
  if (count < 0)
    return 0;
  return count;
}

#endif