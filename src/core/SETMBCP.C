/*_ setmbcp.c						*/
/* Copyright (C) 1994-1995 by Digital Mars	*/
/* All Rights Reserved					*/

#include <mbctype.h>
#include <mbstring.h>
#include <string.h>
#ifdef _WIN32
#include <setlocal.h>
#endif
#include <setmbcp.h>

#define ARRAY_SIZE	257

static void __setmbctype (void) {
  _setmbcp (_MB_CP_ANSI);
}
void (*__init_mbctype_ptr)(void) = __setmbctype;

unsigned char _mbctype[257] = { 0 };
MBCPINFO * __mbcpinfo = NULL;	/* Global CodePage information */
UINT __mbcodepage = 0;		/* Global Code page maintained by RTL */
#ifdef _WIN32
int __mblcid = 0;		/* Global Locale ident */
#endif

#ifndef _WIN32
static UINT GetGlobalCodePage (void) {
 unsigned short  gcp;		/* Global codepage */
  asm {
	mov	ax, 6601h
	int	21h
	mov	gcp, bx
  }
  return gcp;
}
#endif

static void fillrange (BYTE *bufp, BYTE start, BYTE end, BYTE value) {
 int	i;
  for (i = start; i <= end; i++) {
    bufp[i + 1] |= value;
  }
}

static BOOL checkcpinfo (UINT cp) {
 int	i;
 int	cpi;
  for (cpi = 0; cpi < __mb_cpinfoN; cpi++) {
    if (__mb_cpinfo[cpi].cp == cp) {
      memset (_mbctype, 0, ARRAY_SIZE);
      for (i = 0; i < __mb_cpinfo[cpi].brn; i++) {
        fillrange (_mbctype, __mb_cpinfo[cpi].brp[i].start,
		__mb_cpinfo[cpi].brp[i].end, __mb_cpinfo[cpi].brp[i].flag);
      }
      __mbcodepage = cp;
#ifdef _WIN32
      __mblcid = __mb_cpinfo[cpi].lid;
#endif
      __mbcpinfo = __mb_cpinfo + cpi;
      return TRUE;
    }
  }
  return FALSE;				/* not found */
}

int _getmbcp (void) {
   return __mbcodepage;
}

int _setmbcp (int codepage) {
 UINT	cp;
  if (codepage == __mbcodepage) {
    return 0;
  }
  switch (codepage) {
  case _MB_CP_SBCS:
    memset (_mbctype, 0, ARRAY_SIZE);
    __mbcodepage = codepage;
#ifdef _WIN32
    __mblcid = 0;
#endif
    __mbcpinfo = NULL;
    return 0;
  case _MB_CP_ANSI:
#ifdef _WIN32
    cp = GetACP();
#else
    cp = GetGlobalCodePage ();
#endif
    break;
#ifdef _WIN32
  case _MB_CP_OEM:
    cp = GetOEMCP();
    break;
#endif
  default:
    cp = codepage;
    break;
  }
  if (checkcpinfo (cp)) {
    return 0;
  }
#ifdef _WIN32
  {
   CPINFO	cpInfo;
    if (GetCPInfo (cp, &cpInfo)) {
      memset (_mbctype, 0, ARRAY_SIZE);
      if (cpInfo.MaxCharSize > 1){
       BYTE *	p;
        for (p = cpInfo.LeadByte; p[0] != 0 && p[1] != 0; p += 2) {
          fillrange (_mbctype, p[0], p[1], _M1);
        }
        fillrange (_mbctype, 0x01, 0xFF, _M2);
      }
      __mbcodepage = cp;
      __mblcid = 0;
      __mbcpinfo = NULL;
      return 0;
    }
  }
#endif
  return -1;
}

