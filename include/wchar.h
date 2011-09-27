/* Copyright (C) 2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __WCHAR_H
#define __WCHAR_H 1

#if __cplusplus
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

typedef char *__VA_LIST;

#if _M_AMD64
typedef unsigned long long size_t;
#else
typedef unsigned size_t;
#endif

#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif

typedef wchar_t wint_t;
struct tm;

#define WCHAR_MIN	0
#define WCHAR_MAX	0xFFFF

#define WEOF (wint_t) (0xFFFF)

#ifndef NULL
#ifdef __cplusplus
#define NULL	0
#else
#define NULL	((void *)0)
#endif
#endif

#if __cplusplus
#define __RESTRICT
#else
#define __RESTRICT restrict
#endif

#if __cplusplus
}
#endif

#ifndef __STDIO_H
#include <stdio.h>
#endif

#ifndef __STDLIB_H
#include <stdlib.h>
#endif

#ifndef __STRING_H
#include <string.h>
#endif

#if __cplusplus
extern "C" {
#endif

#if _WIN32

int __CLIB fwprintf(FILE * __RESTRICT stream, const wchar_t * __RESTRICT format, ...);
int __CLIB fwscanf(FILE * __RESTRICT stream, const wchar_t * __RESTRICT format, ...);
int __CLIB swprintf(wchar_t * __RESTRICT s, size_t n, const wchar_t * __RESTRICT format, ...);
int __CLIB swscanf(const wchar_t * __RESTRICT s, const wchar_t * __RESTRICT format, ...);
int __CLIB vfwprintf(FILE * __RESTRICT stream, const wchar_t * __RESTRICT format, __VA_LIST arg);
//int vfwscanf(FILE * __RESTRICT stream, const wchar_t * __RESTRICT format, __VA_LIST arg);
int __CLIB vswprintf(wchar_t * __RESTRICT s, size_t n, const wchar_t * __RESTRICT format, __VA_LIST arg);
//int vswscanf(const wchar_t * __RESTRICT s, const wchar_t * __RESTRICT format, __VA_LIST arg);
int __CLIB vwprintf(const wchar_t * __RESTRICT format, __VA_LIST arg);
//int vwscanf(const wchar_t * __RESTRICT format, __VA_LIST arg);
int __CLIB wprintf(const wchar_t * __RESTRICT format, ...);
int __CLIB wscanf(const wchar_t * __RESTRICT format, ...);
wint_t __CLIB fgetwc(FILE *stream);
//wchar_t *fgetws(wchar_t * __RESTRICT s, int n, FILE * __RESTRICT stream);
wint_t __CLIB fputwc(wchar_t c, FILE *stream);
//int fputws(const wchar_t * __RESTRICT s, FILE * __RESTRICT stream);
int __CLIB fwide(FILE *stream, int mode);
//wint_t getwc(FILE *stream);
//wint_t getwchar(void);
//wint_t putwc(wchar_t c, FILE *stream);
//wint_t putwchar(wchar_t c);
wint_t __CLIB ungetwc(wint_t c, FILE *stream);

double __CLIB wcstod(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr);
//float wcstof(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr);
long double __CLIB wcstold(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr);
long __CLIB wcstol(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr, int base);
long long __CLIB wcstoll(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr, int base);
unsigned long __CLIB wcstoul(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr, int base);
unsigned long long __CLIB wcstoull(const wchar_t * __RESTRICT nptr, wchar_t ** __RESTRICT endptr, int base);

_CRTIMP wchar_t * __CLIB wcscpy(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2);
wchar_t * __CLIB wcsncpy(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, size_t n);
wchar_t * __CLIB wmemcpy(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, size_t n);
wchar_t * __CLIB wmemmove(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, size_t n);

wchar_t * __CLIB wcscat(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2);
wchar_t * __CLIB wcsncat(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, size_t n);

int __CLIB wcscmp(const wchar_t *s1, const wchar_t *s2);
int __CLIB wcscoll(const wchar_t *s1, const wchar_t *s2);
int __CLIB wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n);
size_t __CLIB wcsxfrm(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, size_t n);
//int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n);

#if __STDC_VERSION__ >= 199901L

_CRTIMP wchar_t * __CLIB wcschr(const wchar_t *, wchar_t);
wchar_t * __CLIB wcsrchr(const wchar_t *, wchar_t);
wchar_t * __CLIB wcspbrk(const wchar_t *, const wchar_t *);
wchar_t * __CLIB wcsstr(const wchar_t *, const wchar_t *);
wchar_t * __CLIB wmemchr(const wchar_t *,wchar_t,size_t);

#elif __cplusplus

extern "C" {
_CRTIMP wchar_t * __CLIB wcschr(wchar_t *, wchar_t);
wchar_t * __CLIB wcsrchr(wchar_t *, wchar_t);
wchar_t * __CLIB wcspbrk(wchar_t *, const wchar_t *);
wchar_t * __CLIB wcsstr(wchar_t *, const wchar_t *);
wchar_t * __CLIB wmemchr(wchar_t *,wchar_t,size_t);
}

#ifndef __WCS_INLINE
#define __WCS_INLINE
extern "C++" {
inline const wchar_t *wcschr(const wchar_t *s, wchar_t c) { return wcschr((wchar_t *)s, c); }
inline const wchar_t *wcsrchr(const wchar_t *s, wchar_t c) { return wcsrchr((wchar_t *)s, c); }
inline const wchar_t *wcspbrk(const wchar_t *s1,const wchar_t *s2) { return (wchar_t *) wcspbrk((const wchar_t *)s1,s2); }
inline const wchar_t *wcswcs(const wchar_t *s1,const wchar_t *s2) { return wcswcs((wchar_t *)s1, s2); }
inline const void *wmemchr(const void *s, wchar_t c, size_t n) { return wmemchr((void *)s, c, n); }
}
#endif

#else

_CRTIMP wchar_t * __CLIB wcschr(wchar_t *, wchar_t);
wchar_t * __CLIB wcsrchr(wchar_t *, wchar_t);
wchar_t * __CLIB wcspbrk(wchar_t *, const wchar_t *);
wchar_t * __CLIB wcsstr(wchar_t *, const wchar_t *);
wchar_t * __CLIB wmemchr(wchar_t *,wchar_t,size_t);

#endif


size_t __CLIB wcscspn(const wchar_t *s1, const wchar_t *s2);
size_t __CLIB wcsspn(const wchar_t *s1, const wchar_t *s2);
wchar_t * __CLIB wcstok(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, wchar_t ** __RESTRICT ptr);

_CRTIMP size_t __CLIB wcslen(const wchar_t *s);
wchar_t * __CLIB wmemset(wchar_t *s, wchar_t c, size_t n);

size_t __cdecl wcsftime(wchar_t * __RESTRICT s1, size_t maxsize,
	const wchar_t * __RESTRICT format,
	const struct tm * __RESTRICT timeptr);

typedef int mbstate_t;
wint_t btowc(int c);
int wctob(wint_t c);
int mbsinit(const mbstate_t *ps);
size_t mbrlen(const char * __RESTRICT s, size_t n, mbstate_t * __RESTRICT ps);
size_t mbrtowc(wchar_t * __RESTRICT pwc, const char * __RESTRICT s,
	size_t n, mbstate_t * __RESTRICT ps);
size_t wcrtomb(char * __RESTRICT s, wchar_t wc, mbstate_t * __RESTRICT ps);

size_t mbsrtowcs(wchar_t * __RESTRICT dst, const char ** __RESTRICT src,
	size_t len, mbstate_t * __RESTRICT ps);
size_t wcsrtombs(char * __RESTRICT dest, const wchar_t ** __RESTRICT src,
	size_t len, mbstate_t * __RESTRICT ps);


#ifndef _FSIZE_T_DEFINED
typedef unsigned long _fsize_t;
#define _FSIZE_T_DEFINED
#endif

#ifndef __TIME_T_DEFINED
typedef long time_t;
#define __TIME_T_DEFINED
#endif

struct _wfinddata_t
{
        unsigned attrib;
        time_t   time_create;
        time_t   time_access;
        time_t   time_write;
        _fsize_t size;
        wchar_t  name[260];
};

#endif

#if __cplusplus
}
#endif


#endif
