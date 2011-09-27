/* Copyright (C) 1992-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SEARCH_H
#define __SEARCH_H 1

#if __cplusplus
extern "C" {
#endif

#if _M_AMD64
typedef unsigned long long size_t;
#else
typedef unsigned size_t;
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

void *	__CLIB bsearch(const void *,const void *,size_t,size_t,
	int (__CLIB *)(const void *,const void *));
void	__CLIB qsort(void *,size_t,size_t,
	int (__CLIB *)(const void *,const void *));
void *	__CLIB _lfind(const void *,const void *,unsigned int *,unsigned int,
	int (__cdecl *)(const void *,const void *));
void *	__CLIB _lsearch(const void *,const void *,unsigned int *,unsigned int,
	int (__cdecl *)(const void *,const void *));

/* synonymns */
#define lsearch _lsearch
#define lfind _lfind
	
#if __cplusplus
}
#endif

#endif

