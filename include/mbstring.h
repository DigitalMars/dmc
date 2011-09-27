/* Copyright (C) 2001 by Digital Mars. $Revision: 1.1.1.2 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __MBSTRING_H
#define __MBSTRING_H 1

#ifndef __STRING_H
#include <string.h>
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

#if _M_AMD64
typedef unsigned long long size_t;
#else
typedef unsigned size_t;
#endif

#define _NLSCMPERROR    2147483647	/* currently == INT_MAX */

/* string routines */
int __cdecl _mbbtype(unsigned char, int);
int __cdecl _mbsbtype(const unsigned char *, size_t);
unsigned int __cdecl _mbsnextc (const unsigned char *);
unsigned char * __cdecl _mbsinc(const unsigned char *);
unsigned char * __cdecl _mbsninc(const unsigned char *, size_t);
unsigned char * __cdecl _mbsdec(const unsigned char *, const unsigned char *);
size_t __cdecl _mbsnbcnt(const unsigned char *, size_t);
size_t __cdecl _mbsnccnt(const unsigned char *, size_t);
unsigned int __cdecl _mbbtombc(unsigned int);
unsigned int __cdecl _mbctombb(unsigned int);
size_t __cdecl _mbclen(const unsigned char *);
size_t __cdecl _mbslen(const unsigned char *);
/*unsigned char * __cdecl _mbscat(unsigned char *, const unsigned char *);*/
#define _mbscat(s1,s2)	((unsigned char *)strcat((char *)s1, (const char *)s2))
unsigned char * __cdecl _mbsncat(unsigned char *, const unsigned char *, size_t);
unsigned char * __cdecl _mbsnbcat(unsigned char *, const unsigned char *, size_t);
void __cdecl _mbccpy(unsigned char *, const unsigned char *);
/*unsigned char * __cdecl _mbscpy(unsigned char *, const unsigned char *);*/
#define _mbscpy(s1,s2)	((unsigned char *)strcpy((char *)s1, (const char *)s2))
unsigned char * __cdecl _mbsncpy(unsigned char *, const unsigned char *, size_t);
unsigned char * __cdecl _mbsnbcpy(unsigned char *, const unsigned char *, size_t);
_CRTIMP unsigned char * __cdecl _mbschr(const unsigned char *, unsigned int);
unsigned char * __cdecl _mbsrchr(const unsigned char *, unsigned int);
int __cdecl _mbscmp(const unsigned char *, const unsigned char *);
int __cdecl _mbsncmp(const unsigned char *, const unsigned char *, size_t);
int __cdecl _mbsicmp(const unsigned char *, const unsigned char *);
int __cdecl _mbsnicmp(const unsigned char *, const unsigned char *, size_t);
int __cdecl _mbsnbcmp(const unsigned char *, const unsigned char *, size_t);
_CRTIMP int __cdecl _mbsnbicmp(const unsigned char *, const unsigned char *, size_t);
#define _mbccmp(_cpc1, _cpc2) _mbsncmp((_cpc1),(_cpc2),1)
int __cdecl _mbscoll(const unsigned char *, const unsigned char *);
int __cdecl _mbsncoll(const unsigned char *, const unsigned char *, size_t);
int __cdecl _mbsicoll(const unsigned char *, const unsigned char *);
int __cdecl _mbsnicoll(const unsigned char *, const unsigned char *, size_t);
int __cdecl _mbsnbcoll(const unsigned char *, const unsigned char *, size_t);
int __cdecl _mbsnbicoll(const unsigned char *, const unsigned char *, size_t);
size_t __cdecl _mbscspn(const unsigned char *, const unsigned char *);
size_t __cdecl _mbsspn(const unsigned char *, const unsigned char *);
unsigned char * __cdecl _mbsspnp(const unsigned char *, const unsigned char *);
unsigned char * __cdecl _mbspbrk(const unsigned char *, const unsigned char *);
unsigned char * __cdecl _mbstok(unsigned char *, const unsigned char *);
/*unsigned char * __cdecl _mbsdup(const unsigned char *);*/
#define _mbsdup(s) ((unsigned char *)strdup((const char *)s))
unsigned char * __cdecl _mbsrev(unsigned char *);
unsigned char * __cdecl _mbsstr(const unsigned char *, const unsigned char *);
unsigned char * __cdecl _mbslwr(unsigned char *);
unsigned char * __cdecl _mbsupr(unsigned char *);
unsigned char * __cdecl _mbsset(unsigned char *, unsigned int);
unsigned char * __cdecl _mbsnset(unsigned char *, unsigned int, size_t);
unsigned char * __cdecl _mbsnbset(unsigned char *, unsigned int, size_t);

/* character routines */
int __cdecl _ismbcalnum(unsigned int);
int __cdecl _ismbcalpha(unsigned int);
int __cdecl _ismbcdigit(unsigned int);
int __cdecl _ismbcgraph(unsigned int);
int __cdecl _ismbclegal(unsigned int);
int __cdecl _ismbclower(unsigned int);
int __cdecl _ismbcprint(unsigned int);
int __cdecl _ismbcpunct(unsigned int);
int __cdecl _ismbcspace(unsigned int);
int __cdecl _ismbcupper(unsigned int);
unsigned int __cdecl _mbctolower(unsigned int);
unsigned int __cdecl _mbctoupper(unsigned int);

/* Also declared in mbctype.h, here for MSVC Compatability */
#ifndef _MBBLEAD_DEFINED
int __cdecl _ismbblead(unsigned int);
int __cdecl _ismbbtrail(unsigned int);
int __cdecl _ismbslead(const unsigned char *, const unsigned char *);
int __cdecl _ismbstrail(const unsigned char *, const unsigned char *);
#define _MBBLEAD_DEFINED
#endif

/* Kanji specific prototypes */
int __cdecl _ismbchira(unsigned int);
int __cdecl _ismbckata(unsigned int);
int __cdecl _ismbcsymbol(unsigned int);
int __cdecl _ismbcl0(unsigned int);
int __cdecl _ismbcl1(unsigned int);
int __cdecl _ismbcl2(unsigned int);
unsigned int __cdecl _mbcjistojms(unsigned int);
unsigned int __cdecl _mbcjmstojis(unsigned int);
unsigned int __cdecl _mbctohira(unsigned int);
unsigned int __cdecl _mbctokata(unsigned int);

#ifdef __cplusplus
}
#endif

#endif
