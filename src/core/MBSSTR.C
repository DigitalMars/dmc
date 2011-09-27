/*_ mbsstr.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <string.h>
#include <setmbcp.h>

unsigned char * __cdecl _mbsstr
(const unsigned char *s, const unsigned char *f) {
  if (__mbcodepage != 0) {
   size_t	sn;
   size_t	fn;
   size_t	i;
   unsigned int	fch;
    fn = strlen ((const char *) f);
    if (fn == 0) {
      return (unsigned char *) s;
    }
    sn = strlen ((const char *) s);
    if (sn >= fn) {
      sn -= fn;
      fch = *f;
      for (i = 0; i <= sn; i++) {
        if (s[i] == fch) {
          if (memcmp (s + i, f, fn) == 0) {
            return (unsigned char *) s + i;
          }
        }
        if (_ismbblead (s[i]) && (s + 1)[i] != '\0') {
          i++;
        }
      }
    }
    return NULL;
  }
  return (unsigned char *) strstr ((const char *) s, (const char *) f);
}

#endif

