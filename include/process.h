/* Copyright (C) 1992-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __PROCESS_H
#define __PROCESS_H 1

#if __cplusplus
extern "C" {
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

#if __OS2__ && __INTSIZE == 4
#define __CLIB __stdcall
#else
#define __CLIB __cdecl
#endif

#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif

#ifdef _WINDOWS
          void __CLIB _exec_showset(unsigned short style);
          void __CLIB _exec_showreset(void);
unsigned short __CLIB _exec_showget(void);
#endif

void __CLIB exit(int);
void __CLIB _c_exit(void);
void __CLIB _cexit(void);
void __CLIB _exit(int);
void __CLIB abort(void);
void __CLIB _dodtors(void);
#if !(M_UNIX || M_XENIX)
int __CLIB getpid(void);
#endif
#define _getpid getpid

int __CLIB system(const char *);

int __CLIB spawnl(int,const char *,const char *,...);
int __CLIB spawnle(int,const char *,const char *,...);
int __CLIB spawnlp(int,const char *,const char *,...);
int __CLIB spawnlpe(int,const char *,const char *,...);
int __CLIB spawnv(int,const char *,const char *const *);
int __CLIB spawnve(int,const char *,const char *const *,const char *const *);
int __CLIB spawnvp(int,const char *,const char *const *);
int __CLIB spawnvpe(int,const char *,const char *const *, const char *const *);

#define _spawnl   spawnl
#define _spawnle  spawnle
#define _spawnlp  spawnlp
#define _spawnlpe spawnlpe
#define _spawnv   spawnv
#define _spawnve  spawnve
#define _spawnvp  spawnvp
#define _spawnvpe spawnvpe

#define P_WAIT    0
#define P_NOWAIT  1
#define P_OVERLAY 2

#define _P_WAIT    P_WAIT
#define _P_NOWAIT  P_NOWAIT
#define _P_OVERLAY P_OVERLAY

#if !(M_UNIX || M_XENIX)
int __CLIB execl(const char *,const char *,...);
int __CLIB execle(const char *,const char *,...);
int __CLIB execlp(const char *,const char *,...);
int __CLIB execlpe(const char *,const char *,...);
int __CLIB execv(const char *,const char *const *);
int __CLIB execve(const char *,const char *const *,const char *const *);
int __CLIB execvp(const char *,const char *const *);
int __CLIB execvpe(const char *,const char *const *,const char *const *);
#endif
#define _execl execl
#define _execle execle
#define _execlp execlp
#define _execlpe execlpe
#define _execv execv
#define _execve execve
#define _execvp execvp
#define _execvpe execvpe

#define WAIT_CHILD 0
#define WAIT_GRANDCHILD 1

int __CLIB cwait(int *,int,int);
int __CLIB wait(int *);

#ifdef _MT
#if __NT__


unsigned long __CLIB _beginthread(void (__CLIB *)(void *),unsigned,void *);
unsigned long __CLIB _beginthreadex(void *, unsigned,
                unsigned (__stdcall *) (void *), void *, unsigned, unsigned *);
void __CLIB _endthread(void);
void __CLIB _endthreadex(unsigned);

#elif __INTSIZE == 4

int __CLIB _beginthread(void (__CLIB *)(void *),unsigned,void *);
void __CLIB _endthread(void);

#else
int __CLIB _beginthread(void (__far *)(void __far *),
       void __far *, unsigned,void __far *);
void __CLIB _endthread(void);

#endif
#endif

//#if defined(_WIN32)
//int __stdcall _CRT_INIT(void *,unsigned long,void *);
//#endif

#ifdef __NT__
int __CLIB _wsystem(const wchar_t *);
int __CLIB _wspawnl(int, const wchar_t *, const wchar_t *, ...);
int __CLIB _wspawnle(int, const wchar_t *, const wchar_t *, ...);
int __CLIB _wspawnlp(int, const wchar_t *, const wchar_t *, ...);
int __CLIB _wspawnlpe(int, const wchar_t *, const wchar_t *, ...);
int __CLIB _wspawnv(int, const wchar_t *, const wchar_t * const *);
int __CLIB _wspawnve(int, const wchar_t *, const wchar_t * const *, const wchar_t * const *);
int __CLIB _wspawnvp(int, const wchar_t *, const wchar_t * const *);
int __CLIB _wspawnvpe(int, const wchar_t *, const wchar_t * const *, const wchar_t * const *);
#if !(M_UNIX || M_XENIX)
int __CLIB _wexecl(const wchar_t *, const wchar_t *, ...);
int __CLIB _wexecle(const wchar_t *, const wchar_t *, ...);
int __CLIB _wexeclp(const wchar_t *, const wchar_t *, ...);
int __CLIB _wexeclpe(const wchar_t *, const wchar_t *, ...);
int __CLIB _wexecv(const wchar_t *, const wchar_t * const *);
int __CLIB _wexecve(const wchar_t *, const wchar_t * const *, const wchar_t * const *);
int __CLIB _wexecvp(const wchar_t *, const wchar_t * const *);
int __CLIB _wexecvpe(const wchar_t *, const wchar_t * const *, const wchar_t * const *);
#endif
#endif

#ifdef __cplusplus
}
#endif


#endif
