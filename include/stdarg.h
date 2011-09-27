/* Copyright (C) 1986-2005 by Digital Mars */
/* www.digitalmars.com */

#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __STDARG_H
#define __STDARG_H 1

#if __INTSIZE == 4
#define __VA_ALIGN	3
#else
#define __VA_ALIGN	1
#endif

#define __va_size(type) ((sizeof(type) + __VA_ALIGN) & ~__VA_ALIGN)
#define _INTSIZEOF(n)    ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#if 1
#define __SS
#define _FARARG_
#else
#define __SS	__ss
#define _FARARG_ __far
#endif

typedef char __SS *va_list;

#if __cplusplus
    #define va_start(ap, parmn) (static_cast<void>((ap) = static_cast<va_list>(&(parmn)) + __va_size(parmn)))
    #define va_end(ap)  (static_cast<void>(0))
#else
    #define va_start(ap,parmn) ((void)((ap) = (va_list)&(parmn)+__va_size(parmn)))
    #define va_end(ap)  ((void)0)
#endif

#define va_arg(ap,type)	   (*(type __SS *)(((ap)+=__va_size(type))-(__va_size(type))))
#define va_copy(dest,src)  ((dest) = (src))

#endif

