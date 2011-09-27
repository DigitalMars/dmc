// Copyright (C) 1986-2001 by Digital Mars.
// www.digitalmars.com

#if __DMC__ || __RCC__
#pragma once
#endif

#ifndef __STRING_H
#define __STRING_H 1

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

#if _M_AMD64
typedef unsigned long long size_t;
#else
typedef unsigned size_t;
#endif

#ifndef __STDC__
#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif
#endif

#define _NLSCMPERROR	2147483647

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#if __cplusplus
#define __RESTRICT
#else
#define __RESTRICT restrict
#endif

#if __OS2__ && __INTSIZE == 4
#define __CLIB	__stdcall
#else
#define __CLIB	__cdecl
#endif

void *	__CLIB memcpy(void *,const void *,size_t);
void *  __CLIB _inline_memcpy(void *,const void *,size_t);
void *	__CLIB memmove(void *,const void *,size_t);
char *	__CLIB strcpy(char *,const char *);
char *	__CLIB _inline_strcpy(char *,const char *);
char *	__CLIB strncpy(char *,const char *,size_t);
char *	__CLIB strcat(char *,const char *);
char *	__CLIB strncat(char *,const char *,size_t);
int	__CLIB memcmp(const void *,const void *,size_t);
int     __CLIB _inline_memcmp(const void *,const void *,size_t);
int	__CLIB strcmp(const char *,const char *);
int	__CLIB _inline_strcmp(const char *,const char *);
int	__CLIB strcoll(const char *,const char *);
int	__CLIB strncmp(const char *,const char *,size_t);
size_t	__CLIB strxfrm(char *,const char *,size_t);
size_t	__CLIB strcspn(const char *,const char *);
size_t	__CLIB strspn(const char *,const char *);
char *	__CLIB strtok(char *,const char *);
void *	__CLIB memset(void *,int,size_t);
char *	__CLIB strerror(int);
size_t	__CLIB strlen(const char *);
size_t	__CLIB _inline_strlen(const char *);

#ifndef __STDC__
/* Unicode string routines, these are in wchar.h as well */
size_t __CLIB wcslen(const wchar_t *);
_CRTIMP wchar_t * __CLIB wcscpy(wchar_t *, const wchar_t *);
#ifdef __NT__
wchar_t * __CLIB wmemcpy(wchar_t *,const wchar_t *,size_t);
wchar_t * __CLIB wmemmove(wchar_t *,const wchar_t *,size_t);
wchar_t * __CLIB wmemset(wchar_t *,wchar_t,size_t);
wchar_t * __CLIB wcsncpy(wchar_t *, const wchar_t *, size_t);
wchar_t * __CLIB wcscat(wchar_t *, const wchar_t *);
wchar_t * __CLIB wcsncat(wchar_t *, const wchar_t *, size_t);
int __CLIB wcscmp(const wchar_t *, const wchar_t *);
int __CLIB wcsncmp(const wchar_t *, const wchar_t *, size_t);
int __CLIB wcscoll(const wchar_t *, const wchar_t *);
size_t __CLIB wcsxfrm(wchar_t *, const wchar_t *, size_t);
size_t __CLIB wcscspn(const wchar_t *, const wchar_t *);
size_t __CLIB wcsspn(const wchar_t *, const wchar_t *);
wchar_t * __CLIB wcstok(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, wchar_t ** __RESTRICT ptr);
int __CLIB _wcsicoll(const wchar_t *, const wchar_t *);
int __CLIB _wcsncoll(const wchar_t *, const wchar_t *, size_t);
int __CLIB _wcsnicoll(const wchar_t *, const wchar_t *, size_t);
int __CLIB _wcsicmp(const wchar_t *, const wchar_t *);
_CRTIMP int __CLIB _wcsnicmp(const wchar_t *, const wchar_t *, size_t);
_CRTIMP wchar_t * __CLIB _wcsdup(const wchar_t *);
wchar_t * __CLIB _wcslwr(wchar_t *);
wchar_t * __CLIB _wcsupr(wchar_t *);
wchar_t * __CLIB _wcsnset(wchar_t *, wchar_t, size_t);
wchar_t * __CLIB _wcsrev(wchar_t *);
wchar_t * __CLIB _wcsset(wchar_t *, wchar_t);
#define wcswcs	wcsstr
#define wcsdup	_wcsdup
#define wcsicmp	_wcsicmp
#define wcsnicmp _wcsnicmp
#define wcsnset	_wcsnset
#define wcsrev	_wcsrev
#define wcsset	_wcsset
#define wcslwr	_wcslwr
#define wcsupr	_wcsupr
#define wcsicoll _wcsicoll
#endif
#endif

int  *	__CLIB _memintset(int *, int, size_t);
int	__CLIB memicmp(const void *,const void *,size_t);
void *  __CLIB memccpy(void *,const void *,int,unsigned int);
char *	__CLIB stpcpy(char *,const char *);
int	__CLIB stricmp(const char *,const char *);
int	__CLIB strcmpl(const char *,const char *); /* obsolete */
int	__CLIB strnicmp(const char *, const char *, size_t);
char *	__CLIB strdup(const char *);
char *	__CLIB strlwr(char *);
char *	__CLIB strupr(char *);
char *	__CLIB strnset(char *,int,size_t);
char *	__CLIB strrev(char *);
char *	__CLIB strset(char *,int);
void	__CLIB swab(char *,char *,size_t);
void	__CLIB movedata(unsigned short srcseg,unsigned srcoff,unsigned short destseg,unsigned destoff,size_t nbytes);
char *	__CLIB _strerror(const char *);
int	__CLIB _stricoll(const char *, const char *);
int	__CLIB _strncoll(const char *, const char *, size_t);
int	__CLIB _strnicoll(const char *, const char *, size_t);

#if __INTSIZE == 2
void __far * __CLIB __far fmemccpy(void __far *,const void __far *,int,unsigned int);
void __far * __CLIB __far fmemchr(const void __far *,int,size_t);
int          __CLIB __far fmemcmp(const void __far *,const void __far *,size_t);
int          __CLIB __far _fmemcmp(const void __far *,const void __far *,size_t);
int          __CLIB       _inline_fmemcmp(const void __far *,const void __far *,size_t);
void __far * __CLIB __far fmemcpy(void __far *,const void __far *,size_t);
void __far * __CLIB __far _fmemcpy(void __far *,const void __far *,size_t);
void __far * __CLIB       _inline_fmemcpy(void __far *,const void __far *,size_t);
int          __CLIB __far fmemicmp(const void __far *,const void __far *,size_t);
void __far * __CLIB __far fmemmove(void __far *,const void __far *,size_t);
void __far * __CLIB __far _fmemmove(void __far *,const void __far *,size_t);
void __far * __CLIB __far fmemset(void __far *,int,size_t);
char __far * __CLIB __far fstrcat(char __far *,const char __far*);
char __far * __CLIB __far fstrchr(const char __far *,int);
char __far * __CLIB __far _fstrchr(const char __far *,int);
int	     __CLIB __far fstrcmp(const char __far *,const char __far *);
int	     __CLIB __far _fstrcmp(const char __far *,const char __far *);
int	     __CLIB       _inline_fstrcmp(const char __far *,const char __far *);
char __far * __CLIB __far fstrcpy(char __far *,const char __far *);
char __far * __CLIB __far _fstrcpy(char __far *,const char __far *);
char __far * __CLIB       _inline_fstrcpy(char __far *,const char __far *);
size_t	     __CLIB __far fstrcspn(const char __far *,const char __far *);
char __far * __CLIB __far fstrdup(const char __far *);
char __near * __CLIB __far _nstrdup(const char __far *);
int	     __CLIB __far fstricmp(const char __far *,const char __far *);
size_t	     __CLIB __far fstrlen(const char __far *);
size_t	     __CLIB       _inline_fstrlen(const char __far *);
size_t	     __CLIB __far _fstrlen(const char __far *);
char __far * __CLIB __far fstrlwr(char __far *);
char __far * __CLIB __far fstrncat(char __far *,const char __far *,size_t);
int	     __CLIB __far fstrncmp(const char __far *,const char __far *,size_t);
char __far * __CLIB __far fstrncpy(char __far *,const char __far *,size_t);
int	     __CLIB __far fstrnicmp(const char __far *, const char __far *, size_t);
char __far * __CLIB __far fstrnset(char __far *,int,size_t);
char __far * __CLIB __far fstrpbrk(const char __far *,const char __far *);
char __far * __CLIB __far fstrrchr(const char __far *,int);
char __far * __CLIB __far fstrrev(char __far *);
char __far * __CLIB __far fstrset(char __far *,int);
size_t	     __CLIB __far fstrspn(const char __far *,const char __far *);
char __far * __CLIB __far fstrstr(const char __far *,const char __far *);
char __far * __CLIB __far fstrtok(char __far *,const char __far *);
char __far * __CLIB __far _fstrtok(char __far *,const char __far *);
char __far * __CLIB __far fstrupr(char __far *);
#endif

#ifndef __NT__
#define strcoll         strcmp
#define _stricoll	stricmp
#define _strncoll	strncmp
#define _strnicoll	strnicmp
#endif

#define strncmpl        strnicmp

#if !__STDC__
#define _stricmp stricmp
#define strcmpi stricmp
#define _strcmpi stricmp
#define movmem(src,dest,len) ((void)memmove(dest,src,len))
#define setmem(dest,len,chr) ((void)memset(dest,chr,len))
#define strncmpi strnicmp
#define _strnicmp strnicmp
#define _movedata movedata
int	__CLIB _memicmp(const void *,const void *,size_t);
#define _memccpy memccpy
char *	__CLIB _strdup(const char *);
#define _strlwr strlwr
#define _strnset strnset
char *	__CLIB _strrev(char *);
#define _strset strset
#define _strupr strupr
#define _swab swab

#if __INTSIZE == 2
#define _fmemicmp fmemicmp
#define _fmemccpy fmemccpy
#define _fmemcpy(x,y,n)		_inline_fmemcpy(x,y,n)
#define fmemcpy(x,y,n)		_inline_fmemcpy(x,y,n)
#define _fmemchr fmemchr
#define _fmemset fmemset
#define _fstrcat fstrcat
#define _fstrcat fstrcat
#define _fstrcspn fstrcspn
#define _fstrdup fstrdup
#define _fstricmp fstricmp
#define _fstrcmpl fstricmp
#define _fstrlwr fstrlwr
#define _fstrncat fstrncat
#define _fstrncmp fstrncmp
#define _fstrnicmp fstrnicmp
#define _fstrncpy fstrncpy
#define _fstrnset fstrnset
#define _fstrpbrk fstrpbrk
#define _fstrrchr fstrrchr
#define _fstrrev fstrrev
#define _fstrset fstrset
#define _fstrspn fstrspn
#define _fstrstr fstrstr
#define _fstrupr fstrupr
#define _nmemicmp nmemicmp

#if !__LARGE__
#define _fstrcmp(x,y)		_inline_fstrcmp(x,y)
#define fstrcmp(x,y)		_inline_fstrcmp(x,y)
#define _fstrcpy(x,y)		_inline_fstrcpy(x,y)
#define fstrcpy(x,y)		_inline_fstrcpy(x,y)
#define _fstrlen(s)		_inline_fstrlen(s)
#define fstrlen(s)		_inline_fstrlen(s)
#endif

#endif
#endif

#if !__STDC__
extern int __CLIB sys_nerr;
extern char * __CLIB sys_errlist[];
#endif

#if __cplusplus
}
#endif

#if __STDC_VERSION__ >= 199901L

char *	__CLIB strchr(const char *,int);
char *	__CLIB strrchr(const char *,int);
char *	__CLIB strpbrk(const char *,const char *);
char *	__CLIB strstr(const char *,const char *);
void *	__CLIB memchr(const void *,int,size_t);

#ifndef __STDC__
_CRTIMP wchar_t * __CLIB wcschr(const wchar_t *, wchar_t);
wchar_t * __CLIB wcsrchr(const wchar_t *, wchar_t);
wchar_t * __CLIB wcspbrk(const wchar_t *, const wchar_t *);
wchar_t * __CLIB wcsstr(const wchar_t *, const wchar_t *);
wchar_t * __CLIB wmemchr(const wchar_t *,wchar_t,size_t);
#endif

#elif __cplusplus

extern "C" {
char *	__CLIB strchr(char *,int);
char *	__CLIB strrchr(char *,int);
char *	__CLIB strpbrk(char *,const char *);
char *	__CLIB strstr(char *,const char *);
void *	__CLIB memchr(void *,int,size_t);

#ifndef __STDC__
_CRTIMP wchar_t * __CLIB wcschr(wchar_t *, wchar_t);
wchar_t * __CLIB wcsrchr(wchar_t *, wchar_t);
wchar_t * __CLIB wcspbrk(wchar_t *, const wchar_t *);
wchar_t * __CLIB wcsstr(wchar_t *, const wchar_t *);
wchar_t * __CLIB wmemchr(wchar_t *,wchar_t,size_t);
#endif
}

extern "C++" {
inline const char *strchr(const char *s, int n) { return strchr(const_cast<char *>(s), n); }
inline const char *strrchr(const char *s, int n) { return strrchr(const_cast<char *>(s), n); }
inline const char *strpbrk(const char *s1,const char *s2) { return strpbrk(const_cast<char *>(s1),s2); }
inline const char *strstr(const char *s1,const char *s2) { return strstr(const_cast<char *>(s1), s2); }
inline const void *memchr(const void *s, int c, size_t n) { return memchr(const_cast<void *>(s), c, n); }

#ifndef __WCS_INLINE
#define __WCS_INLINE
inline const wchar_t *wcschr(const wchar_t *s, wchar_t c) { return wcschr(const_cast<wchar_t *>(s), c); }
inline const wchar_t *wcsrchr(const wchar_t *s, wchar_t c) { return wcsrchr(const_cast<wchar_t *>(s), c); }
inline const wchar_t *wcspbrk(const wchar_t *s1,const wchar_t *s2) { return wcspbrk(const_cast<wchar_t *>(s1),s2); }
inline const wchar_t *wcswcs(const wchar_t *s1,const wchar_t *s2) { return wcswcs(const_cast<wchar_t *>(s1), s2); }
inline const void *wmemchr(const void *s, wchar_t c, size_t n) { return wmemchr(const_cast<void *>(s), c, n); }
#endif
}

#else

char *	__CLIB strchr(char *,int);
char *	__CLIB strrchr(char *,int);
char *	__CLIB strpbrk(char *,const char *);
char *	__CLIB strstr(char *,const char *);
void *	__CLIB memchr(void *,int,size_t);

#ifndef __STDC__
_CRTIMP wchar_t * __CLIB wcschr(wchar_t *, wchar_t);
wchar_t * __CLIB wcsrchr(wchar_t *, wchar_t);
wchar_t * __CLIB wcspbrk(wchar_t *, const wchar_t *);
wchar_t * __CLIB wcsstr(wchar_t *, const wchar_t *);
wchar_t * __CLIB wmemchr(wchar_t *,wchar_t,size_t);
#endif

#endif

#endif
