/* Copyright (C) 1992-2001 by Digital Mars. $Revision: 1.1.1.2 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __UNMANGLE_H
#define __UNMANGLE_H 1

#if __cplusplus
extern "C" {
#endif


#if __OS2__ && __INTSIZE == 4
#define __CLIB	__stdcall
#else
#define __CLIB	__cdecl
#endif

#if !defined(__UNMANGLE_DEFINED)
char * __CLIB unmangle_pt (char **);
char * __CLIB unmangle_ident (char *ident);
#endif

#if __cplusplus
}
#endif

#endif
