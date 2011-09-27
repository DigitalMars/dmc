/*_ mbstok.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>
#if _MT
#include <mt.h>
#endif

unsigned char * __cdecl _mbstok (unsigned char *s, const unsigned char *c) {
#if _MT
 unsigned char * * _ppn = (unsigned char * *) &_getthreaddata() -> t_strtok;
#define _pn	(*_ppn)
#else
static unsigned char * _pn = NULL;
#endif
  if (__mbcodepage != 0) {
    if (s == NULL) {
      s = _pn;
      if (s == NULL) {
        return NULL;
      }
    }
    s = _mbsspnp (s, c);
    if (s == NULL) {
      return NULL;
    }
    if (*s == '\0' || _ismbblead (*s) && *(s + 1) == '\0') {
      return NULL;
    }
    _pn = _mbspbrk (s, c);
    if (_pn != NULL) {
      if (_ismbblead (*_pn)) {
        *_pn++ = '\0';
      }
      *_pn++ = '\0';
    }
    return s;
  }
  return (unsigned char *) strtok ((char *) s, (const char *) c);
}

#endif

