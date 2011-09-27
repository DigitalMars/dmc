/*_ toupper.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include <ctype.h>

#undef toupper
#undef _toupper

#ifdef __NT__
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

int __cdecl toupper (int ch) {
  if (IsCLocale (LC_CTYPE) || ch < 0x0100) {
    if (islower (ch)) {
      ch -= ('a' - 'A');
    }
  } else {
   int  i = 0;
   char uch = ch >> 8;
   char stmp[4];
   char dtmp[4];
    if (isleadbyte ((BYTE) uch)) {
      stmp[i++] = uch;
    }
    stmp[i++] = ch;
    stmp[i] = '\0';
    if (__aLCMapString (0, __locale_ident[LC_CTYPE].lgid,
		LCMAP_UPPERCASE, stmp, i, dtmp, 4) == 0) {
    }
    ch = ((BYTE) dtmp[0] << 8) | (BYTE) dtmp[1];
  }
  return ch;
}

#else

toupper(c)	{ return (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c; }

#endif

_toupper(c)	{ return (c - ('a' - 'A')); }

