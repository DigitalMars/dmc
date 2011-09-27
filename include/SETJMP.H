/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SETJMP_H
#define __SETJMP_H 1

#if __cplusplus
extern "C"	{
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

#if defined(_WIN32)
#define _JBLEN 16
#elif __INTSIZE == 4
#define _JBLEN 10
#else
#define _JBLEN 9
#endif

typedef int jmp_buf[_JBLEN];

#if __OS2__ && __INTSIZE == 4
#define __CLIB	__stdcall
#else
#define __CLIB	__cdecl
#endif

#if defined(_WINDOWS) && __INTSIZE == 2
int __far __pascal Catch(int __far *);
void __far __pascal Throw(int const __far *,int);
#define _setjmp(A)    Catch(A)
#define longjmp(A,B) Throw(A,B)
#else
int __CLIB _setjmp(jmp_buf);
int __CLIB setjmp(jmp_buf);  /* prototype provided for backward compatability */
void __CLIB longjmp(jmp_buf,int);

#if _WIN32
int __CLIB _inline_setjmp(jmp_buf);
#undef _setjmp
#define _setjmp(x) _inline_setjmp(x)

/*
 * Define jump buffer layout for setjmp/longjmp under NT that unwinds stack
 */
typedef struct __JUMP_BUFFER {
    unsigned long Ebp;
    unsigned long Ebx;
    unsigned long Edi;
    unsigned long Esi;
    unsigned long Esp;
    unsigned long Eip;
    unsigned long Except_Registration; 
    unsigned long TryLevel;
    unsigned long Reserved;
    unsigned long Unwind_Handler;
    unsigned long ExceptData[6];
} _JUMP_BUFFER;

#endif

#if M_UNIX || M_XENIX
#if !__STDC__ || defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE)

#define _SIGJBLEN 128

typedef int sigjmp_buf[_SIGJBLEN];

extern int __CLIB sigsetjmp(sigjmp_buf, int);
extern void __CLIB siglongjmp(sigjmp_buf, int);
#endif
#endif /* M_UNIX || M_XENIX */

#endif /* _WINDOWS */

#define setjmp _setjmp

#if __cplusplus
}
#endif

#endif
