/*_ ismblowe.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>
#ifdef _WIN32
#include <lcapi32.h>
#endif

int __cdecl _ismbclower (unsigned int ch) {
  if (ch >= 0x0100) {
    if (__mbcodepage != 0) {
#ifdef _WIN32
     char		temp[3];
     unsigned short	type[2];
      temp[0] = ch >> 8;
      temp[1] = ch;
      temp[2] = '\0';
      if (__aGetStringType (__mbcodepage, __mblcid, CT_CTYPE1,
		temp, 2, type)) {
        return type[0] & _LC;
      }
#else
     int		i;
     int		crn;
     MBCASERANG *	crp;
      if (__mbcpinfo != NULL) {
        crn = __mbcpinfo->crn;
        crp = __mbcpinfo->crp;
        for (i = 0; i < crn; i++) {
          if (ch >= crp[i].lstart && ch <= crp[i].lend) {
            return TRUE;
          }
        }
      }
#endif
    }
    return FALSE;
  }
  return islower (ch);
}

#endif

