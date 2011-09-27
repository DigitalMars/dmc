/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#ifndef _WINDOWS

#include <conio.h>
#include <scio.h>

int _cscanf (const char *format, ...)
{
  return _sfmt ((fp_t)ungetch, (fp2_t)getche, NULL ,format,
    (va_list)&(format) + sizeof(format));
}

#endif
