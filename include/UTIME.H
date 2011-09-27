/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __UTIME_H
#define __UTIME_H 1

#if __cplusplus
extern "C" {
#endif

#if !__STDC__

#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif

#if M_UNIX || M_XENIX

#include <sys/types.h>

typedef long time_t;

struct utimbuf
{
   time_t actime;
   time_t modtime;
};

#ifndef UTIME_DEFINED
#define UTIME_DEFINED
int __cdecl utime(const char *filename, struct utimbuf *times);
#endif

#else /* M_UNIT || M_XENIX */

#include <sys\types.h>

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

struct utimbuf 
{
   time_t actime;   /* access time -- ignored by DOS */
   time_t modtime;  /* modification time */
};

#ifndef UTIME_DEFINED
#define UTIME_DEFINED
int __cdecl utime(const char *filename, struct utimbuf *times);
#endif

#ifndef WUTIME_DEFINED
#define WUTIME_DEFINED
int __cdecl _wutime(wchar_t *filename, struct utimbuf *times);
#endif

#define _utimbuf utimbuf
#define _utime utime

#endif /* M_UNIX || M_XENIX */
#endif /* !__STDC__ */

#if __cplusplus
}
#endif

#endif
