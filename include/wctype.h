
/* Copyright (C) 1986-2003 by Digital Mars.
 * All Rights Reserved
 * www.digitalmars.com
 */

#if __DMC__ || __RCC__
#pragma once
#endif

#ifndef __WCTYPE_H
#define __WCTYPE_H 1

#if _WIN32

#if __cplusplus
extern "C" {
#endif

#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif

#if !defined(_WCTYPE_T_DEFINED)
typedef wchar_t wint_t;
typedef wchar_t wctype_t;
#define _WCTYPE_T_DEFINED 1
#endif

typedef wchar_t wctrans_t;

#ifndef WEOF
#define WEOF (wint_t) (0xFFFF)
#endif

#if __OS2__ && __INTSIZE == 4
#define __CLIB	__stdcall
#else
#define __CLIB	__cdecl
#endif

/* Define _CRTAPI1 (for compatibility with the NT SDK) */
#ifndef _CRTAPI1
#define _CRTAPI1 __cdecl
#endif


/* Define _CRTAPI2 (for compatibility with the NT SDK) */
#ifndef _CRTAPI2
#define _CRTAPI2 __cdecl
#endif

/* Define CRTIMP  */
#ifndef _CRTIMP
#if defined(_WIN32) && defined(_DLL)
#define _CRTIMP  __declspec(dllimport)
#else
#define _CRTIMP
#endif
#endif

#define _SPC	8
#define _CTL	0x20
#define _BLK	0x40
#define _HEX	0x80
#define _UC	1
#define _LC	2
#define _PNC	0x10
#define _DIG	4
#ifdef __NT__
#define _ALP	(0x0100|_UC|_LC)
#define _LEADBYTE 0x8000
#else
#define _ALP	(_UC|_LC)
#endif

#define _UPPER _UC
#define _LOWER _LC
#define _DIGIT _DIG
#define _SPACE _SPC
#define _PUNCT _PNC
#define _CONTROL _CTL
#define _BLANK _BLK
#define _ALPHA _ALP

#ifdef M_ELF
#define _pwctype __pwctype
#endif

#ifndef __STDC__
extern wctype_t * __CLIB	_pwctype;
#endif


#ifndef iswalpha

int __CLIB iswalpha(wint_t);
int __CLIB iswblank(wint_t);
int __CLIB iswupper(wint_t);
int __CLIB iswlower(wint_t);
int __CLIB iswdigit(wint_t);
int __CLIB iswxdigit(wint_t);
int __CLIB iswspace(wint_t);
int __CLIB iswpunct(wint_t);
int __CLIB iswalnum(wint_t);
int __CLIB iswprint(wint_t);
int __CLIB iswgraph(wint_t);
int __CLIB iswcntrl(wint_t);
int __CLIB iswascii(wint_t);
#undef isleadbyte
int __CLIB isleadbyte(int);
wchar_t __CLIB towupper(wchar_t);
_CRTIMP wchar_t __CLIB towlower(wchar_t);
int __CLIB iswctype(wint_t, wctype_t);
#define is_wctype	iswctype


#define iswalpha(c)	(iswctype(c, _ALPHA))
#define iswupper(c)	(iswctype(c, _UPPER))
#define iswlower(c)	(iswctype(c, _LOWER))
#define iswdigit(c)	(iswctype(c, _DIGIT))
#define iswxdigit(c)	(iswctype(c, _HEX))
#define iswspace(c)	(iswctype(c, _SPACE))
#define iswpunct(c)	(iswctype(c, _PUNCT))
#define iswalnum(c)	(iswctype(c, _ALPHA|_DIGIT))
#define iswprint(c)	(iswctype(c, _BLANK|_PUNCT|_ALPHA|_DIGIT))
#define iswgraph(c)	(iswctype(c, _PUNCT|_ALPHA|_DIGIT))
#define iswcntrl(c)	(iswctype(c, _CONTROL))
#define iswascii(c)	((unsigned)(c) < 0x80)

#endif

#if __cplusplus
}
#endif

#endif

#endif
