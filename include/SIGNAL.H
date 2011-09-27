/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SIGNAL_H
#define __SIGNAL_H 1

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
#define __CLIB	__stdcall
#else
#define __CLIB	__cdecl
#endif

typedef volatile int sig_atomic_t;

void (__CLIB * __CLIB signal(int,void (__CLIB *)(int)))(int);

#if M_UNIX || M_XENIX
#define SIGHUP	 1
#define SIGINT	 2
#define SIGQUIT	 3
#define SIGILL	 4
#define SIGTRAP	 5
#define SIGIOT	 6
#define SIGABRT	 6
#define SIGEMT	 7
#define SIGFPE	 8
#define SIGKILL	 9
#define SIGBUS	 10
#define SIGSEGV	 11
#define SIGSYS	 12
#define SIGPIPE	 13
#define SIGALRM	 14
#define SIGTERM	 15
#define SIGUSR1	 16
#define SIGUSR2	 17
#define SIGCLD	 18
#define SIGPWR	 19
#define SIGWINCH 20

#if M_XOUT
#define SIGPOLL 20
#else
#define SIGPOLL 22
#endif

#define SIGCHLD SIGCLD

#if M_UNIX
#define SIGURG  21
#define SIGSTOP 23
#define SIGTSTP 24
#define SIGCONT 25
#define SIGTTIN 26
#define SIGTTOU 27
#define SIGVTALRM 28
#define SIGPROF 29
#define SIGXCPU 30
#define SIGXFSZ 31
#define _NSIG	32
#else
#define _NSIG	21
#endif /* M_UNIX */

#if !__STDC__
#define NSIG _NSIG
#endif

#if !__STDC__ || defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE)
#include <sys/types.h>
typedef long    sigset_t;

#define SIGALL (~(sigset_t)0L) /* All signals.    */

#define sigbit(n)	(1L << ((n) - 1))
#define sigemptyset(s)	*(s) = ~SIGALL
#define sigfillset(s)	*(s) = SIGALL
#if 0
#define sigaddset(s,n)  *(s) |= sigbit(n)
#define sigdelset(s,n)  *(s) &= ~sigbit(n)
#define sigismember(set,n) ((*(set) & sigbit(n)) == sigbit(n))
#else
int __CLIB sigaddset(sigset_t *,int);
int __CLIB sigdelset(sigset_t *,int);
int __CLIB sigismember(const sigset_t *,int);
#endif
int __CLIB sigpending(sigset_t *);
int __CLIB sigprocmask(int, const sigset_t *, sigset_t *);
int __CLIB sigsuspend(const sigset_t *);

struct  sigaction
        {
        void    (*sa_handler)(int);
        sigset_t sa_mask;
        int     sa_flags;
        };

int __CLIB sigaction(int sig, const struct sigaction *act,
                                     const struct sigaction *oact);
int __CLIB kill(pid_t,int);
pid_t __CLIB getpid(void);

#define SA_NOCLDSTOP    1

#define SIG_BLOCK       1
#ifdef M_ELF
#define SIG_SETMASK     3
#else
#define SIG_SETMASK     0
#endif
#define SIG_UNBLOCK     2

#endif

#define SIG_ERR  (void(__CLIB *)(int))-1
#define SIG_DFL  (void(__CLIB *)(int))0
#define SIG_IGN  (void(__CLIB *)(int))1
#define SIG_HOLD (void(__CLIB *)(int))2

int __CLIB pause(void);
unsigned int __CLIB alarm(unsigned int seconds);

#else /* M_UNIX || M_XENIX */


#define _NSIG 23 /* = SIGABRT + 1 */

#if !__STDC__
#define NSIG _NSIG
#endif

#define SIGINT	2		/* interrupt			*/
#define SIGILL	4		/* illegal instruction		*/
#define SIGFPE	8		/* floating point error		*/
#define SIGSEGV 11		/* segment violation		*/
#define SIGTERM 15		/* terminate			*/

#if _WIN32
#define SIGBREAK 21		/* ctrl-break under NT		*/
#else
#define SIGBREAK  SIGINT  /* Under DOS, SIGBREAK is SIGINT */
#endif

#define SIGABRT 22		/* abort			*/


#define SIG_DFL		(void (__CLIB *)(int)) 0
#define SIG_ERR		(void (__CLIB *)(int)) -1
#define SIG_IGN		(void (__CLIB *)(int)) 1

#endif

int __CLIB raise(int);

#if __cplusplus
}
#endif

#endif
