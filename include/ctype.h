
/* Copyright (C) 1986-2002 by Digital Mars.
 * All Rights Reserved
 * www.digitalmars.com
 */

#if __DMC__ || __RCC__
#pragma once
#endif

#ifndef __CTYPE_H
#define __CTYPE_H 1

#if __cplusplus
extern "C" {
#endif

#ifndef __STDC__

#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif

#if !defined(_WCTYPE_T_DEFINED)
typedef wchar_t wint_t;
typedef wchar_t wctype_t;
#define _WCTYPE_T_DEFINED 1
#endif

#ifndef WEOF
#define WEOF (wint_t) (0xFFFF)
#endif

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

#ifdef __NT__
#ifdef M_ELF
#define _pctype  __pctype
#ifndef __STDC__
#define _pwctype __pwctype
#endif
#endif

extern unsigned short * __CLIB	_pctype;
#ifndef __STDC__
extern wctype_t * __CLIB	_pwctype;
#endif
extern unsigned short __CLIB	_ctype[];
#else
#ifdef M_ELF
#define _ctype __ctype
#endif

extern const unsigned char __CLIB _ctype[];
#endif

#ifdef __NT__
int	__CLIB _isctype(int, int);
#endif

int	__CLIB isalnum(int);
int	__CLIB isalpha(int);
int	__CLIB isblank(int);
int	__CLIB iscntrl(int);
int	__CLIB isdigit(int);
int	__CLIB isgraph(int);
int	__CLIB islower(int);
int	__CLIB isprint(int);
int	__CLIB ispunct(int);
int	__CLIB isspace(int);
int	__CLIB isupper(int);
int	__CLIB isxdigit(int);
int	__CLIB toupper(int);
int	__CLIB tolower(int);

#ifndef __STDC__
int	__CLIB isascii(int);
int	__CLIB __isascii(int);
int	__CLIB iscsym(int);
int	__CLIB __iscsym(int);
int	__CLIB iscsymf(int);
int	__CLIB __iscsymf(int);
int	__CLIB toascii(int);
int	__CLIB __toascii(int);
int 	__CLIB _toupper(int);
int	__CLIB _tolower(int);
#endif

#ifdef __NT__
#ifndef __STDC__
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
int __CLIB isleadbyte(int);
wchar_t __CLIB towupper(wchar_t);
_CRTIMP wchar_t __CLIB towlower(wchar_t);
int __CLIB iswctype(wint_t, wctype_t);
#define is_wctype	iswctype
#endif
#endif
#endif

#ifdef __NT__
extern int __CLIB __locale_mbsize;
#define MB_CUR_MAX	__locale_mbsize
#define isalnum(c)	(MB_CUR_MAX>1?_isctype(c,_ALP|_DIG):_pctype[c]&(_ALP|_DIG))
#define isalpha(c)	(MB_CUR_MAX>1?_isctype(c,_ALP):_pctype[c]&_ALP)
#define iscntrl(c)	(MB_CUR_MAX>1?_isctype(c,_CTL):_pctype[c]&_CTL)
#define isdigit(c)	(MB_CUR_MAX>1?_isctype(c,_DIG):_pctype[c]&_DIG)
#define isgraph(c)	(MB_CUR_MAX>1?_isctype(c,_ALP|_DIG|_PNC):_pctype[c]&(_ALP|_DIG|_PNC))
#define islower(c)	(MB_CUR_MAX>1?_isctype(c,_LC):_pctype[c]&_LC)
#define isprint(c)	(MB_CUR_MAX>1?_isctype(c,_ALP|_DIG|_PNC|_BLK):_pctype[c]&(_ALP|_DIG|_PNC|_BLK))
#define ispunct(c)	(MB_CUR_MAX>1?_isctype(c,_PNC):_pctype[c]&_PNC)
#define isspace(c)	(MB_CUR_MAX>1?_isctype(c,_SPC):_pctype[c]&_SPC)
#define isupper(c)	(MB_CUR_MAX>1?_isctype(c,_UC):_pctype[c]&_UC)
#define isxdigit(c)	(MB_CUR_MAX>1?_isctype(c,_HEX):_pctype[c]&_HEX)
#define isleadbyte(c)	(_pctype[(unsigned char)(c)]&_LEADBYTE)
#else
#define MB_CUR_MAX	1
#define isalnum(c)	(_ctype[(c)+1]&(_UC|_LC|_DIG))
#define isalpha(c)	(_ctype[(c)+1]&(_UC|_LC))
#define iscntrl(c)	(_ctype[(c)+1]&_CTL)
#define isdigit(c)	(_ctype[(c)+1]&_DIG)
#define isgraph(c)	(_ctype[(c)+1]&(_UC|_LC|_DIG|_PNC))
#define islower(c)	(_ctype[(c)+1]&_LC)
#define isprint(c)	(_ctype[(c)+1]&(_UC|_LC|_DIG|_PNC|_BLK))
#define ispunct(c)	(_ctype[(c)+1]&_PNC)
#define isspace(c)	(_ctype[(c)+1]&_SPC)
#define isupper(c)	(_ctype[(c)+1]&_UC)
#define isxdigit(c)	(_ctype[(c)+1]&_HEX)
# ifndef _MBCS
#define isleadbyte(c)	(0)
# endif
#endif

#ifndef __STDC__
#if !defined(_WIN32)
#define tolower(c)	(isupper(c) ? (c)+('a'-'A') : (c))
#define toupper(c)	(islower(c) ? (c)-('a'-'A') : (c))
#endif
#define isascii(c)	((unsigned)(c)<0200)
#define __isascii(c)	((unsigned)(c)<0200)
#define iscsym(c)	((_ctype[(c)+1]&(_UC|_LC|_DIG))||((c)=='_'))
#define __iscsym(c)	((_ctype[(c)+1]&(_UC|_LC|_DIG))||((c)=='_'))
#define iscsymf(c)	((_ctype[(c)+1]&(_UC|_LC))||((c)=='_'))
#define __iscsymf(c)	((_ctype[(c)+1]&(_UC|_LC))||((c)=='_'))
#define toascii(c)	((c)&0x7F)
#define __toascii(c)	((c)&0x7F)
#define _tolower(c)	((c)+('a'-'A'))
#define _toupper(c)	((c)-('a'-'A'))
#endif

#ifdef __NT__
#ifndef __STDC__
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
#endif

#if __cplusplus
}
#endif

#endif
