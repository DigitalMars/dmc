/*_ wctype.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <ctype.h>

int (__CLIB isleadbyte) (int c) {
  return isleadbyte (c);
}

int (__CLIB iswalpha) (wchar_t c) {
  return iswalpha (c);
}

int (__CLIB iswupper) (wchar_t c) {
  return iswupper (c);
}

int (__CLIB iswlower) (wchar_t c) {
  return iswlower (c);
}

int (__CLIB iswdigit) (wchar_t c) {
  return iswdigit (c);
}

int (__CLIB iswxdigit) (wchar_t c) {
  return iswxdigit (c);
}

int (__CLIB iswspace) (wchar_t c) {
  return iswspace (c);
}

int (__CLIB iswpunct) (wchar_t c) {
  return iswpunct (c);
}

int (__CLIB iswalnum) (wchar_t c) {
  return iswalnum (c);
}

int (__CLIB iswprint) (wchar_t c) {
  return iswprint (c);
}

int (__CLIB iswgraph) (wchar_t c) {
  return iswgraph (c);
}

int (__CLIB iswcntrl) (wchar_t c) {
  return iswcntrl (c);
}

int (__CLIB iswascii) (wchar_t c) {
  return iswascii (c);
}

#else

# ifdef _MBCS
#include <mbctype.h>
#include <locale.h>
extern struct lconv __current_lconv;
int (__CLIB isleadbyte) (int c) {
  if (__current_lconv.lc[LC_ALL] != _LOCALE_C ||
      __current_lconv.lc[LC_CTYPE] != _LOCALE_C) {
    return _ismbblead ((unsigned char) c);
  }
  return 0;
}
# else
int (__CLIB isleadbyte) (int c) {
  return 0;
}
# endif

# if 0
#include <tchar.h>
#include <sctchar.h>
int (__CLIB isleadbyte) (int c) {
  return isleadbyte (c);
}
# endif

#endif

/**/
