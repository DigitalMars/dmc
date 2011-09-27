
// Copyright (C) 2001 by Digital Mars
// All Rights Reserved
// www.digitalmars.com

#if __DMC__ || __RCC__
#pragma once
#endif

#ifndef __UTIL_H
#define __UTIL_H 1

#if __cplusplus
extern "C" {
#endif

#if _M_AMD64
typedef unsigned long long size_t;
#else
typedef unsigned size_t;
#endif

int __cdecl file_read(const char *name, void **pbuffer, size_t *psize);
int __cdecl file_write(const char *name, void *buffer, size_t size);
int __cdecl file_append(const char *name, void *buffer, size_t size);

#if __cplusplus
}
#endif

#endif

