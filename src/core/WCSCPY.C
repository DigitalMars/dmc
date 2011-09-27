/*_ wcscpy.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <string.h>

wchar_t * __CLIB wcscpy (wchar_t *ws1, const wchar_t *ws2) {
  return (wchar_t *) memcpy (ws1, ws2, (wcslen (ws2) + 1) * sizeof(wchar_t));
}

#else

#include <wchar.h>
#include <string.h>

/******************************************************
 * Given 2 wide character strings, copy the contents of 
 * string 2 to string 1 and return string 1.
 */
wchar_t *wcscpy(wchar_t *string1, const wchar_t *string2)
{
  /* Count is number of bytes to copy counting zero byte */
  size_t count = (wcslen(string2) + 1) * sizeof(wchar_t);
  memcpy(string1,string2,count);
  return string1;
}

#endif

/**/
