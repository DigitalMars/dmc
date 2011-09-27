/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __HANDLE_H
#define __HANDLE_H 1

#if __cplusplus
extern "C" {
#endif

#define HANDLE_BASE	0xFE000000
#define HANDLE_MAX	((int)(0x10000 - (HANDLE_BASE >> 16)))
#define HANDLE_PAGESIZE		(16*1024)
int handle_ishandle(void __handle *h);
#define handle_ishandle(h) ((int)(((unsigned long) (h) >= HANDLE_BASE) != 0))
void __handle * __cdecl handle_malloc(unsigned);
void __handle * __cdecl handle_calloc(unsigned);
void __handle * __cdecl handle_realloc(void __handle *,unsigned);
char __handle * __cdecl handle_strdup(char __handle *);
void __cdecl handle_free(void __handle *);
int __cdecl handle_usingemm(void);
void __cdecl handle_remap(void);
#if NO_HANDLE || DOS16RM || __INTSIZE == 4 || __OS2__ || __NT__
#define __handle
#define handle_malloc(n)	malloc(n)
#define handle_calloc(n)	calloc((n),1)
#define handle_realloc(h,n)	realloc((h),(n))
#define handle_free(h)		free(h)
#define handle_strdup(h)	strdup(h)
#define handle_usingemm()	0
#define handle_remap()
#undef handle_ishandle
#define handle_ishandle(h)	0
#include	<stdlib.h>
#include	<string.h>
#endif

#if __cplusplus
}
#endif

#endif
