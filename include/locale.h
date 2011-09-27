/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __LOCALE_H
#define __LOCALE_H 1

#if __cplusplus
extern "C" {
#endif

#ifndef __STDC__
#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif
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

#if __OS2__ && __INTSIZE == 4
#define __CLIB	__stdcall
#else
#define __CLIB	__cdecl
#endif

#ifndef __NT__
enum _LOCALE
{   _LOCALE_C,
    _LOCALE_USA,
    _LOCALE_ITALY,
    _LOCALE_NETHERLANDS,
    _LOCALE_NORWAY,
    _LOCALE_SWITZERLAND,
    _LOCALE_UK,
    _LOCALE_JAPAN,
    _LOCALE_KOREA,
    _LOCALE_CHINA,
    _LOCALE_TAIWAN,
    _LOCALE_MAX
};
#define _LOCALE_NATIVE	_LOCALE_USA
#endif

#pragma pack(1)
struct lconv
{
	char *decimal_point;
	char *thousands_sep;
	char *grouping;
	char *int_curr_symbol;
	char *currency_symbol;
	char *mon_decimal_point;
	char *mon_thousands_sep;
	char *mon_grouping;
	char *positive_sign;
	char *negative_sign;
	char int_frac_digits;
	char frac_digits;
	char p_cs_precedes;
	char p_sep_by_space;
	char n_cs_precedes;
	char n_sep_by_space;
	char p_sign_posn;
	char n_sign_posn;
#ifndef __NT__
	char lc[6];
#endif
	char int_p_cs_precedes;
	char int_n_cs_precedes;
	char int_p_sep_by_space;
	char int_n_sep_by_space;
	char int_p_sign_posn;
	char int_n_sign_posn;
};

#pragma pack()

#define LC_ALL		0
#define LC_COLLATE	1
#define LC_CTYPE	2
#define LC_MONETARY	3
#define LC_NUMERIC	4
#define LC_TIME		5

#define LC_MIN          LC_ALL
#define LC_MAX          LC_TIME

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

char * __CLIB setlocale(int category, const char *locale);
struct lconv * __CLIB localeconv(void);

#ifdef _WIN32
#ifndef __STDC__
wchar_t * __CLIB _wsetlocale(int, const wchar_t *);
#endif
#endif

#if _CHAR_UNSIGNED
/*
   The standard states localeconv should return valuse of CHAR_MAX,
   well with -J, CHAR_MAX is 255 and not 127. To deal with this, we
   need this extern to cause RTL startup code to fix up the lconv
   structure returned from localeconv
*/
int __charmax;
#endif

#if __cplusplus
}
#endif

#endif
