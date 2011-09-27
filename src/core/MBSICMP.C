/*_ mbsicmp.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <string.h>
#include <setmbcp.h>
#ifdef _WIN32
#include <lcapi32.h>
#endif

int __cdecl _mbsicmp (const unsigned char *s1, const unsigned char *s2) {
  if (__mbcodepage != 0) {
   int		ret = 0;
   unsigned int	sch1;
   unsigned int	sch2;
#ifdef _WIN32
   int		map;
   char		tmp[3];
#else
   int		i;
   int		crn;
   MBCASERANG *	crp;
    if (__mbcpinfo != NULL) {
      crn = __mbcpinfo->crn;
      crp = __mbcpinfo->crp;
    }
#endif
    do {
      sch1 = *s1;
      if (_ismbblead (sch1)) {
        if (s1[1] == '\0') {
          sch1 = 0;
        } else {
#ifdef _WIN32
          map = __aLCMapString (__mbcodepage, __mblcid,
		LCMAP_LOWERCASE, (const char *) s1, 2, tmp, 2);
          if (map == 0) {
            return 0;
          }
          sch1 = ((unsigned char) tmp[0] << 8) | (unsigned char) tmp[1];
#else
          sch1 = (sch1 << 8) | s1[1];
          if (__mbcpinfo != NULL) {
            for (i = 0; i < crn; i++) {
              if (sch1 >= crp[i].ustart && sch1 <= crp[i].uend) {
                sch1 += (crp[i].lstart - crp[i].ustart);
                break;
              }
            }
          }
#endif
          s1++;
        }
      } else {
        if (isupper (sch1)) {
          sch1 += ('a' - 'A');
        }
      }
      s1++;
      sch2 = *s2;
      if (_ismbblead (sch2)) {
        if (s2[1] == '\0') {
          sch2 = 0;
        } else {
#ifdef _WIN32
          map = __aLCMapString (__mbcodepage, __mblcid,
		LCMAP_LOWERCASE, (const char *) s2, 2, tmp, 2);
          if (map == 0) {
            return 0;
          }
          sch2 = ((unsigned char) tmp[0] << 8) | (unsigned char) tmp[1];
#else
          sch2 = (sch2 << 8) | s2[1];
          if (__mbcpinfo != NULL) {
            for (i = 0; i < crn; i++) {
              if (sch2 >= crp[i].ustart && sch2 <= crp[i].uend) {
                sch2 += (crp[i].lstart - crp[i].ustart);
                break;
              }
            }
          }
#endif
          s2++;
        }
      } else {
        if (isupper (sch2)) {
          sch2 += ('a' - 'A');
        }
      }
      s2++;
      if (sch1 != sch2) {
        break;
      }
    } while (sch1 != 0);
    if (sch1 < sch2) {
      ret = -1;
    } else if (sch1 > sch2) {
      ret = 1;
    }
    return ret;
  }
  return _stricmp ((const char *) s1, (const char *) s2);
}

#endif

