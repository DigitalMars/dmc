/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __MACROS_H
#define __MACROS_H 1

extern void _far _pascal Dos3Call(void);

#define LPTR M_I86CM || M_I86LM || M_I86VM
#define SPTR M_I86SM || M_I86MM

#ifdef _WINDOWS
#define bdos	__asm call Dos3Call
#else
#define bdos	__asm int 21h
#endif

#define bdos1(fun)    __asm mov ah,fun bdos

#if __INTSIZE == 4
#define errno_set   __asm mov errno, EAX
#else
#define errno_set   __asm mov errno, AX
#endif

#define errno_set1(value)	__asm mov errno, value

#define clr1(reg1)  __asm xor reg1, reg1

#define clr2( reg1, reg2 ) __asm xor reg1,reg1	clr1(reg2)

#define clr3( reg1, reg2, reg3) __asm xor reg1, reg1 clr2( reg2, reg3 )

#define _ifs(arg1,cond,arg2,lbl) __asm cmp arg1,arg2 __asm j##cond short lbl

#if __I386>=3 || (LPTR)
#define SIZEPTR 4
#else
#define SIZEPTR 2
#endif
#endif
