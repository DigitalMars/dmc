/* Copyright (C) 1992-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __VARARGS_H
#define __VARARGS_H 1

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

#ifndef _VA_LIST_DEFINED
#define _VA_LIST_DEFINED 1
typedef char __SS *va_list;
#endif

#define va_dcl		int va_alist;
#define va_start(ap)	((ap) = (va_list)&(va_alist))
#define va_arg(ap,type)	(*(type __SS *)(((char __SS *)(ap)+=__va_size(type))-(__va_size(type))))
#define va_end(ap)	((void)0)

#endif


