/* Copyright (C) 2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __MBCTYPE_H
#define __MBCTYPE_H 1

#ifndef __CTYPE_H
#include <ctype.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Define _CRTAPI1 (for compatibility with the NT SDK) */
#ifndef _CRTAPI1
#define _CRTAPI1 __cdecl
#endif

/* Define _CRTAPI2 (for compatibility with the NT SDK) */
#ifndef _CRTAPI2
#define _CRTAPI2 __cdecl
#endif

/* Define CRTIMP */
#ifndef _CRTIMP
#if defined(_WIN32) && defined(_DLL)
#define _CRTIMP  __declspec(dllimport)
#else
#define _CRTIMP
#endif
#endif

extern unsigned char _mbctype[];

/* bit masks for MBCS character types */
#define _MS	0x01		/* MBCS single-byte symbol */
#define _MP	0x02		/* MBCS punct */
#define _M1	0x04		/* MBCS 1st (lead) byte */
#define _M2	0x08		/* MBCS 2nd byte*/

/* byte types  */
#define _MBC_SINGLE	0	/* valid single byte char */
#define _MBC_LEAD	1	/* lead byte */
#define _MBC_TRAIL	2	/* trailing byte */
#define _MBC_ILLEGAL	(-1)	/* illegal byte */

#define _KANJI_CP   932

/* _setmbcp parameter defines */
#define _MB_CP_SBCS     0
#define _MB_CP_OEM      -2
#define _MB_CP_ANSI     -3

#ifndef _MBCTYPE_DEFINED

/* MB control routines */
int __cdecl _setmbcp(int);
int __cdecl _getmbcp(void);

/* byte routines */
int __cdecl _ismbbkalnum(unsigned int);
int __cdecl _ismbbkprint(unsigned int);
int __cdecl _ismbbkpunct(unsigned int);
int __cdecl _ismbbalnum(unsigned int);
int __cdecl _ismbbalpha(unsigned int);
int __cdecl _ismbbgraph(unsigned int);
int __cdecl _ismbbkana(unsigned int);
int __cdecl _ismbbprint(unsigned int);
int __cdecl _ismbbpunct(unsigned int);
#ifndef _MBBLEAD_DEFINED
int __cdecl _ismbblead(unsigned int);
int __cdecl _ismbbtrail(unsigned int);
int __cdecl _ismbslead(const unsigned char *, const unsigned char *);
int __cdecl _ismbstrail(const unsigned char *, const unsigned char *);
#define _MBBLEAD_DEFINED
#endif
#define _MBCTYPE_DEFINED
#endif

/* char byte classification macros */
#define _ismbbkalnum(_c) ((_mbctype+1)[(unsigned char)(_c)] & _MS)
#define _ismbbkprint(_c) ((_mbctype+1)[(unsigned char)(_c)] & (_MS|_MP))
#define _ismbbkpunct(_c) ((_mbctype+1)[(unsigned char)(_c)] & _MP)

#define _ismbbalnum(_c)  (((_ctype+1)[(unsigned char)(_c)] & (_ALP|_DIG))||_ismbbkalnum(_c))
#define _ismbbalpha(_c)  (((_ctype+1)[(unsigned char)(_c)] & _ALP)||_ismbbkalnum(_c))
#define _ismbbgraph(_c)  (((_ctype+1)[(unsigned char)(_c)] & (_PNC|_ALP|_DIG))||_ismbbkprint(_c))
#define _ismbbprint(_c)  (((_ctype+1)[(unsigned char)(_c)] & (_BLK|_PNC|_ALP|_DIG))||_ismbbkprint(_c))
#define _ismbbpunct(_c)  (((_ctype+1)[(unsigned char)(_c)] & _PNC)||_ismbbkpunct(_c))

#define _ismbblead(_c)   ((_mbctype+1)[(unsigned char)(_c)] & _M1)
#define _ismbbtrail(_c)  ((_mbctype+1)[(unsigned char)(_c)] & _M2)

#define _ismbbkana(_c)   ((_mbctype+1)[(unsigned char)(_c)] & (_MS|_MP))

#ifdef __cplusplus
}
#endif

#endif
