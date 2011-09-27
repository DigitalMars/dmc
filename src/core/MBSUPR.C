/*_ mbsupr.c						*/
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

unsigned char * __cdecl _mbsupr (unsigned char *s) {
  if (__mbcodepage != 0) {
   unsigned char * p = s;
   unsigned int	sch;
#ifdef _WIN32
   int		ret;
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
    while (*p != '\0') {
      sch = *p;
      if (_ismbblead (sch)) {
        if (*(p + 1) != '\0') {
#ifdef _WIN32
          ret = __aLCMapString (__mbcodepage, __mblcid,
		LCMAP_UPPERCASE, (const char *) p, 2, tmp, 2);
          if (ret == 0) {
            return NULL;
          }
          *p++ = (unsigned char) tmp[0];
          sch = (unsigned char) tmp[1];
#else
          sch = (sch << 8) | *(p + 1);
          if (__mbcpinfo != NULL) {
            for (i = 0; i < crn; i++) {
              if (sch >= crp[i].lstart && sch <= crp[i].lend) {
                sch -= (crp[i].lstart - crp[i].ustart);
                break;
              }
            }
          }
          *p++ = sch >> 8;
#endif
        }
      } else {
        if (islower (sch)) {
          sch -= ('a' - 'A');
        }
      }
      *p++ = sch;
    }
    return s;
  }
  return (unsigned char *) strupr ((char *) s);
}

#endif

