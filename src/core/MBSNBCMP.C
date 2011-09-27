/*_ mbsnbcmp.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <string.h>
#include <setmbcp.h>

int __cdecl _mbsnbcmp
(const unsigned char *s1, const unsigned char *s2, size_t size) {
  if (__mbcodepage != 0) {
   int		ret = 0;
   unsigned int	sch1 = 0;
   unsigned int	sch2 = 0;
    while (size != 0) {
      size--;
      sch1 = *s1;
      if (_ismbblead (sch1)) {
        if (size == 0) {
          sch1 = 0;
          goto break1;
        }
        if (s1[1] == '\0') {
          sch1 = 0;
        } else {
          sch1 = (sch1 << 8) | s1[1];
          s1++;
        }
      }
      s1++;
break1:
      sch2 = *s2;
      if (_ismbblead (sch2)) {
        if (size == 0) {
          sch2 = 0;
          goto break2;
        }
        size--;
        if (s2[1] == '\0') {
          sch2 = 0;
        } else {
          sch2 = (sch2 << 8) | s2[1];
          s2++;
        }
      }
      s2++;
break2:
      if (sch1 != sch2) {
        break;
      }
      if (sch1 == 0) {
        break;
      }
    }
    if (sch1 < sch2) {
      ret = -1;
    } else if (sch1 > sch2) {
      ret = 1;
    }
    return ret;
  }
  return strncmp ((const char *) s1, (const char *) s2, size);
}

#endif

