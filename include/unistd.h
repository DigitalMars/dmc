/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
/* Unix Header */
/* Not for use for Win32 */

#if _WIN32
#error "unistd.h is not for Windows use"
#endif

#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __UNISTD_H
#define __UNISTD_H 1

#if M_UNIX || M_XENIX

#include <sys/unistd.h>

typedef int     ssize_t;
#include <sys/types.h>

#include <time.h>

#ifndef NULL
#define NULL    0
#endif

#if __cplusplus
extern "C"
{
#endif

void __cdecl _exit(int);
int __cdecl access(const char *, int);
unsigned __cdecl alarm(unsigned);
int __cdecl chdir(const char *);
int __cdecl chown(const char *, uid_t, gid_t);
int __cdecl close(int);
char * __cdecl ctermid(char *);
int __cdecl dup(int);
int __cdecl dup2(int, int);
int __cdecl execl(const char *, const char *, ...);
int __cdecl execle(const char *, const char *, ...);
int __cdecl execlp(const char *, const char *, ...);
int __cdecl execv(const char *, const char *const *);
int __cdecl execve(const char *, const char *const *, const char *const *);
int __cdecl execvp(const char *, const char *const *);
int __cdecl execvpe(const char *, const char *const *, const char *const *);
pid_t __cdecl fork(void);
long __cdecl fpathconf(int, int);
char * __cdecl getcwd(char *, size_t);
gid_t __cdecl getegid(void);
uid_t __cdecl geteuid(void);
gid_t __cdecl getgid(void);
int __cdecl getgroups(int, gid_t *);
char * __cdecl getlogin(void);
pid_t __cdecl getpgrp(void);
pid_t __cdecl getpid(void);
pid_t __cdecl getppid(void);
uid_t __cdecl getuid(void);
int __cdecl isatty(int);
int __cdecl link(const char *, const char *);
off_t __cdecl lseek(int, off_t, int);
long __cdecl pathconf(const char *, int);
int __cdecl pause(void);
int __cdecl pipe(int *);
ssize_t __cdecl read(int, void *, size_t);
int __cdecl rmdir(const char *);
int __cdecl setgid(gid_t);
int __cdecl setpgid(pid_t, pid_t);
pid_t __cdecl setsid(void);
int __cdecl setuid(uid_t);
unsigned __cdecl sleep(unsigned);
#if M_ELF
#define sysconf sysconf4
#endif
long __cdecl sysconf(int);
pid_t __cdecl tcgetpgrp(int);
int __cdecl tcsetpgrp(int, pid_t);
char * __cdecl ttyname(int);
int __cdecl unlink(const char *);
ssize_t __cdecl write(int, const void *, size_t);

#if !defined(_POSIX_SOURCE)
int __cdecl acct(const char *);
int __cdecl brk(void *);
int __cdecl chroot(const char *);
void __cdecl exit(int);
int __cdecl fattach(int, const char *);
int __cdecl fchdir(int);
int __cdecl fchown(int,uid_t, gid_t);
int __cdecl fdetach(const char *);
int __cdecl fsync(int);
int __cdecl ftruncate(int, off_t);
#ifndef OPTFIRST
#define OPTFIRST 1
#endif
#if !OPTFIRST
#define getopt Getopt
int __cdecl nextarg(int argc, char * const argv[], const char *optstring);
#else
#define nextarg(a,b,c) optind++
#endif
int __cdecl getopt(int argc, char * const argv[], const char *optstring);
pid_t __cdecl getpgid(pid_t);
char * __cdecl gettxt(const char *, const char *);
pid_t __cdecl getsid(pid_t);
int __cdecl ioctl(int, int, ...);
int __cdecl lchown(const char *, uid_t, gid_t);
int __cdecl lockf(int, int, long);
int __cdecl nice(int);
void __cdecl profil(unsigned short *, unsigned int, unsigned int, unsigned int);
int __cdecl ptrace(int, pid_t, int, int);
int __cdecl readlink(const char *, void *, int);
int __cdecl rename(const char *, const char *);
void * __cdecl sbrk(size_t);
int __cdecl select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
int __cdecl setgroups(int, const gid_t *);
pid_t __cdecl setpgrp(void);
int __cdecl symlink(const char *, const char *);
void __cdecl sync(void);
int __cdecl truncate(const char *, off_t);
pid_t __cdecl vfork(void);
#endif

#if __cplusplus
}
#endif

#endif

#endif
