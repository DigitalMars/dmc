/*_ ismbbyte.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

#define _ismb(c,m)	(_mbctype+1)[(unsigned char)c] & (m)
#define _ismbc(c,m,a)	((_mbctype+1)[(unsigned char)c] & (m)) || ((_ctype+1)[(unsigned char)c] & (a))

/*
 *	Non-Macro version.
 */

#ifdef _ismbbkalnum
#undef _ismbbkalnum
#endif

int __cdecl _ismbbkalnum (unsigned int c) {
  return _ismb (c, _MS);
}

#ifdef _ismbbkprint
#undef _ismbbkprint
#endif

int __cdecl _ismbbkprint (unsigned int c) {
  return _ismb (c, _MS | _MP);
}

#ifdef _ismbbkpunct
#undef _ismbbkpunct
#endif

int __cdecl _ismbbkpunct (unsigned int c) {
  return _ismb (c, _MP);
}

#ifdef _ismbbalnum
#undef _ismbbalnum
#endif

int __cdecl _ismbbalnum (unsigned int c) {
  return _ismbc (c, _MS, _ALP | _DIG);
}

#ifdef _ismbbalpha
#undef _ismbbalpha
#endif

int __cdecl _ismbbalpha (unsigned int c) {
  return _ismbc (c, _MS, _ALP);
}

#ifdef _ismbbgraph
#undef _ismbbgraph
#endif

int __cdecl _ismbbgraph (unsigned int c) {
  return _ismbc (c, _MS | _MP, _PNC | _ALP | _DIG);
}

#ifdef _ismbbkana
#undef _ismbbkana
#endif

int __cdecl _ismbbkana (unsigned int c) {
  if (__mbcodepage == _KANJI_CP) {
    return _ismb (c, _MS | _MP);
  }
  return 0;
}

#ifdef _ismbbprint
#undef _ismbbprint
#endif

int __cdecl _ismbbprint (unsigned int c) {
  return _ismbc (c, _MS | _MP, _BLK | _PNC | _ALP | _DIG);
}

#ifdef _ismbbpunct
#undef _ismbbpunct
#endif

int __cdecl _ismbbpunct (unsigned int c) {
  return _ismbc (c, _MP, _PNC);
}

#endif

