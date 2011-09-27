
/* Copyright (C) 1995-2003 by Digital Mars, www.digitalmars.com */

#if __DMC__ || __RCC__
#pragma once
#endif

#ifndef __FLTENV_H
#define __FLTENV_H 1

#if __cplusplus
extern "C" {
#endif

typedef struct __fenv_t
{
    unsigned short status;
    unsigned short control;
    unsigned short round;
    unsigned short reserved[2];
} fenv_t;

typedef int fexcept_t;

#define FE_INVALID	1
#define FE_DENORMAL	2
#define FE_DIVBYZERO	4
#define FE_OVERFLOW	8
#define FE_UNDERFLOW	0x10
#define FE_INEXACT	0x20
#define FE_ALL_EXCEPT	0x3F

#define FE_TONEAREST	0
#define FE_UPWARD	0x800
#define FE_DOWNWARD	0x400
#define FE_TOWARDZERO	0xC00

extern fenv_t __cdecl _FE_DFL_ENV;

#define FE_DFL_ENV (&_FE_DFL_ENV)

#define FE_FLTPREC	0
#define FE_DBLPREC	0x200
#define FE_LDBLPREC	0x300

int  __cdecl fetestexcept(int excepts);
int __cdecl feraiseexcept(int excepts);
int __cdecl feclearexcept(int excepts);
//int  __cdecl fegetexcept(fexcept_t *flagp,int excepts);
//int  __cdecl fesetexcept(const fexcept_t *flagp,int excepts);
int  __cdecl fegetround(void);
int  __cdecl fesetround(int round);
int  __cdecl fegetprec(void);
int  __cdecl fesetprec(int prec);
int __cdecl fegetenv(fenv_t *envp);
int __cdecl fesetenv(const fenv_t *envp);
//void __cdecl feprocentry(fenv_t *envp);
//void __cdecl feprocexit(const fenv_t *envp);

int __cdecl fegetexceptflag(fexcept_t *flagp,int excepts);
int __cdecl fesetexceptflag(const fexcept_t *flagp,int excepts);
int  __cdecl feholdexcept(fenv_t *envp);
int __cdecl feupdateenv(const fenv_t *envp);

#if __cplusplus
}
#endif

#endif





