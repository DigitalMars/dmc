/*_ mbtolowe.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>
#ifdef _WIN32
#include <lcapi32.h>
#endif

unsigned int __cdecl _mbctolower (unsigned int ch) {
  if (ch >= 0x0100) {
    if (__mbcodepage != 0) {
#ifdef _WIN32
     char		tmp[3];
     char		map[3];
     int		ret;
      tmp[0] = ch >> 8;
      tmp[1] = ch;
      ret = __aLCMapString (__mbcodepage, __mblcid,
		LCMAP_LOWERCASE, tmp, 2, map, 2);
      if (ret != 0) {
        return ((unsigned char) map[0] << 8) | (unsigned char) map[1];
      }
#else
     int		i;
     int		crn;
     MBCASERANG *	crp;
      if (__mbcpinfo != NULL) {
        crn = __mbcpinfo->crn;
        crp = __mbcpinfo->crp;
        for (i = 0; i < crn; i++) {
          if (ch >= crp[i].ustart && ch <= crp[i].uend) {
            return ch + (crp[i].lstart - crp[i].ustart);
          }
        }
      }
#endif
    }
    return ch;
  }
  if (isupper (ch)) {
    ch += ('a' - 'A');
  }
  return ch;
}

#endif

