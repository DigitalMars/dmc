/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __DIRECT_H
#define __DIRECT_H 1

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

/* Define CRTIMP */
#ifndef _CRTIMP
#if defined(_WIN32) && defined(_DLL)
#define _CRTIMP  __declspec(dllimport)
#else
#define _CRTIMP
#endif
#endif

#if __cplusplus
extern "C" {
#endif

#ifndef __NT__
#define MAXPATH		80
#define MAXDRIVE	3
#define MAXDIR		66
#define MAXFILE		9
#define MAXEXT		5
#else
#define MAXPATH 260
#define MAXDIR 260
#define MAXDRIVE 260
#define MAXFILE 260
#define MAXEXT 260
#endif

#ifndef ZTC_COMPAT
/* also #define'd in direct.h for something different */
#undef WILDCARDS
#define WILDCARDS 0x01
#endif

#define EXTENSION 0x02
#define FILENAME  0x04
#define DIRECTORY 0x08
#define DRIVE	  0x10

#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif

#if _M_AMD64
typedef unsigned long long size_t;
#else
typedef unsigned size_t;
#endif

char *searchpath(const char *);
void fnmerge( char  *,const char  *,const char  *, const char  *,const char  *);
int fnsplit( const char  *,char  *,char  *,char  *,char  *);

char * __cdecl getcwd(char *,size_t);
char * __CLIB _getdcwd(int drive,char *Pbuf,int PbufLen);
int __CLIB getcurdir(int drive,char *dir);
int __cdecl _chdrive(int);
int __cdecl _getdrive(void);
int __cdecl chdir(const char *);
#if !(M_UNIX || M_XENIX)
int __cdecl mkdir(const char *);
#endif
int __cdecl rmdir(const char *);
#define _chdir(direc) chdir(direc)
#define _mkdir mkdir
#define _rmdir rmdir
#define _getcwd(direc,len) getcwd(direc,len)
#define getdrive _getdrive

#ifdef __NT__
wchar_t * __CLIB _wgetcwd(wchar_t *, size_t);
wchar_t * __CLIB _wgetdcwd(int, wchar_t *, int);
int __CLIB _wchdir(const wchar_t *);
int __CLIB _wmkdir(const wchar_t *);
int __CLIB _wrmdir(const wchar_t *);
#endif

#if __cplusplus
}
#endif

#endif
