/*_ mbscspn.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

size_t __cdecl _mbscspn (const unsigned char *s, const unsigned char *c) {
 const unsigned char *	sp;
 const unsigned char *	cp;
 unsigned int		sch;
 int			i;
 int			cn;
  if (__mbcodepage != 0) {
    cn = 0;
    for (cp = c; *cp != '\0'; cp++) {
      if (_ismbblead (*cp)) {
        if (*(cp + 1) == '\0') {
          break;
        }
        cp++;
      }
      cn++;
    }
    for (sp = s; *sp != '\0'; sp++) {
      sch = *sp;
      cp = c;
      for (i = 0; i < cn; i++) {
        if (_ismbblead (*cp)) {
          if (*cp == sch && *(cp + 1) == *(sp + 1)) {
            break;
          }
          cp++;
        } else {
          if (*cp == sch) {
            break;
          }
        }
        cp++;
      }
      if (i < cn) {
        break;
      }
      if (_ismbblead (sch)) {
        if (sp[1] == '\0') {
          break;
        }
        sp++;
      }
    }
    return sp - s;
  }
  return strcspn ((const char *) s, (const char *) c);
}

#endif

