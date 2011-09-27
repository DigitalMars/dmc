
// $Header$
// Copyright (C) 1995-2001 by Digital Mars
// All Rights Reserved
// www.digitalmars.com
// Written by Walter Bright

// Most of what used to be here was moved into math.h
// per the C99 standard.

#if __DMC__ || __RCC__
#pragma once
#endif

#ifndef __FLTPNT_H
#define __FLTPNT_H 1

#ifndef __MATH_H
#include <math.h>
#endif

#if __cplusplus
extern "C" {
#endif

double	__CLIB scalb(double, int);
float	__CLIB scalbf(float, int);
long double __CLIB scalbl(long double, int);
#define scalbl		scalb

double __cdecl nans(const char *);
float __cdecl nansf(const char *);
long double __cdecl nansl(const char *);
#if !_WIN32
#define nansl nans
#endif

long int __cdecl rndtol(long double);
long int __cdecl rndtonl(long double);

/* Compiler Intrinsics  */
#if __INLINE_8087
short __cdecl _inline_rndtos(double);
long __cdecl _inline_rndtol(long double);
#if __INTSIZE == 4
long long __cdecl _inline_rndtoll(long double);
#endif
#define rndtol(d) _inline_rndtol(d)
#endif

#if __cplusplus
}
#endif

#endif

