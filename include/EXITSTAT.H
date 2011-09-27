/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __EXITSTAT_H
#define __EXITSTAT_H 1

#ifdef __cplusplus
extern "C"
  {
#endif

#include <setjmp.h>

#ifndef __STDC__
int __cdecl exit_PUSHSTATE(void);
int __cdecl exit_popstate(void);
#else
int exit_PUSHSTATE(void);
int exit_popstate(void);
#endif

extern jmp_buf _exit_state;

#define exit_pushstate(RESULT) \
((exit_PUSHSTATE()!=0)?RESULT=2:\
(((RESULT=setjmp(_exit_state))!=0)?exit_popstate(),RESULT:RESULT))

#ifdef __cplusplus
  }
#endif

#endif
