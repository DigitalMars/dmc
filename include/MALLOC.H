/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.2 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __MALLOC_H
#define __MALLOC_H 1

#include <stdlib.h>

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

#include <heapstat.h>

int __cdecl _heapchk(void);
int __cdecl _heapset(unsigned);
#if __INTSIZE == 2
int __cdecl _heapadd(void __far *,size_t);
int __cdecl _fheapmin(void);
#else
int __cdecl _heapadd(void *,size_t);
#endif
int __cdecl _heapmin(void);

_CRTIMP void * __CLIB realloc(void *,size_t);
_CRTIMP void * __CLIB calloc(size_t, size_t);
_CRTIMP void   __CLIB free(void *);
_CRTIMP void * __CLIB malloc(size_t);
_CRTIMP size_t __CLIB _msize(void *);

#ifdef __NT__
#define _expand realloc
#else
void * __cdecl _expand(void *,size_t);
#endif




#ifdef __cplusplus
}
#endif

#endif
