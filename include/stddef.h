/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __STDDEF_H
#define __STDDEF_H 1

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

#if __OS2__ && __INTSIZE == 4
#define __CLIB	__stdcall
#else
#define __CLIB	__cdecl
#endif

#if defined(_MT) || defined(_DLL) || __cplusplus
extern int * __CLIB _errno(void);
#define errno (*_errno())
#else
extern int __cdecl errno;
#endif

#if defined(_MT)

#if __INTSIZE == 4
unsigned long * __CLIB __threadid(void);
#if __NT__
#define _threadid	(__threadid())
#else
			/* MOV EAX,FS:[0Ch] */
#define _threadid	((unsigned long *)__emit__(0x64,0xA1,0x0C,0,0,0))
#endif
#else
extern int __far * __cdecl _threadid;
#endif

#endif

#if _M_AMD64
typedef long long ptrdiff_t;
typedef unsigned long long size_t;
#else
typedef int ptrdiff_t;
typedef unsigned size_t;
#endif

#if __cplusplus
typedef decltype(nullptr) nullptr_t;
#endif

#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif

#define offsetof(t,i)	((size_t)((char *)&((t *)0)->i - (char *)0))

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else 
#define NULL ((void *)0)
#endif 
#endif 

#if __cplusplus && !__NEW_DEFINED
#define __NEW_DEFINED 1
extern "C++" {
   void *operator new(size_t s, void *at);
#if _ENABLE_ARRAYNEW
   void *operator new[](size_t s, void *at);
#endif
}
#endif

#if __cplusplus
}
#endif

#endif
