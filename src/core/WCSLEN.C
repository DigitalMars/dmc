/*_ wcscpy.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

size_t __CLIB wcslen (const wchar_t *ws) {
 size_t		ct;
  for (ct = 0; ws[ct] != L'\0'; ct++)
    ;
  return ct;
}

#else

#include <wchar.h>

#define WCHAR_NULL 0x0000

/******************************************************
 * Given a wide character string, return the number of 
 * wide characters that make up the string.
 */
size_t  wcslen(const wchar_t *ws)
{
  size_t count = 0;
  for (count = 0; ws[count] != WCHAR_NULL; count++)
          ;
  return count;
}

#endif

/**/
