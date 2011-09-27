/*_ mbsrev.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

unsigned char * __cdecl _mbsrev (unsigned char *s) {
  if (__mbcodepage != 0) {
   unsigned char	ch;
   unsigned char *	r = s;
   unsigned char *	p = s;
    while ((ch = *r) != '\0') {
      if (_ismbblead (ch)) {
        if (*(r + 1) == '\0') {
          break;
        }
        *r = *(r + 1);
        *(r + 1) = ch;
        r++;
      }
      r++;
    }
    r--;
    while (p < r) {
      ch = *p;
      *p++ = *r;
      *r-- = ch;
    }
    return s;
  }
  return (unsigned char *) _strrev ((char *) s);
}

#endif

