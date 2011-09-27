/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __IO_H
#define __IO_H 1

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
#define __CLIB  __stdcall
#else
#define __CLIB  __cdecl
#endif

#ifndef __STDC__
#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif
#endif

_CRTIMP int __CLIB read(int,void *,unsigned);
_CRTIMP int __CLIB write(int,const void *,unsigned);
#define _read read
#define _write write

#if __INTSIZE == 2
int __CLIB _readx(int,void *,unsigned,unsigned);
int __CLIB _writex(int,void *,unsigned,unsigned);
#endif

#if M_UNIX || M_XENIX
char __CLIB *ttyname(int filedes);
int __CLIB rdchk(int filedes);
int __cdecl ioctl(int filedes, int cmd,...);
long __cdecl ulimit(int,long);
#else
int __cdecl dos_open(const char *,int);
int __cdecl sopen(const char *, int, int, ...);
int __CLIB setmode(int,int);
int __cdecl _umask(int mask);
int __CLIB chmod(const char *,int);
#define _setmode setmode
#define _sopen sopen
#define umode _umode
#define umask _umask
#endif

struct ftime   
{
   unsigned ft_tsec  : 5;
   unsigned ft_min   : 6;
   unsigned ft_hour  : 5;
   unsigned ft_day   : 5;
   unsigned ft_month : 4;
   unsigned ft_year  : 7;
};

struct _finddata_t
{
    unsigned	attrib;
    long	time_create;
    long	time_access;
    long	time_write;
    unsigned long size;
    char	name[260];
};

#define _A_NORMAL       0x00
#define _A_RDONLY       0x01
#define _A_HIDDEN       0x02
#define _A_SYSTEM       0x04
#define _A_VOLID        0x08
#define _A_SUBDIR       0x10
#define _A_ARCH         0x20

_CRTIMP int __cdecl _findfirst(const char *, struct _finddata_t *);
_CRTIMP int __cdecl _findnext(int, struct _finddata_t *);
_CRTIMP int __cdecl _findclose(int);

#ifndef __NT__
#ifndef DOS386
int __CLIB getftime(int FHdl, struct ftime *FtPtr);
int __CLIB setftime(int handle, struct ftime *ptr);
int __CLIB lock(int FHdl, long Offset, long Size);
int __CLIB unlock(int handle, long offset, long length);
#endif /* DOS386 */
#endif

int __cdecl creatnew(const char *, int);
int __cdecl creattemp(char *, int);
int __cdecl open(const char *,int,...);
int __CLIB creat(const char *,int);
int __CLIB close(int);
int __CLIB locking(int, int, long);
int __CLIB _commit(int);
int __CLIB eof(int handle);
int __CLIB rename(const char *, const char *);
#define _open open
#define _close close
#define _creat creat
#define _locking locking
#define _eof eof

#ifdef __NT__
long __CLIB _get_osfhandle(int);
int __CLIB _open_osfhandle(long,int);
#endif

int __CLIB unlink(const char *);
#define _unlink unlink

int __CLIB remove(const char *);
int __CLIB chsize(int, long);
#define _chsize chsize

#define _chmod chmod

int __CLIB dup(int);
int __CLIB _dup(int);
int __CLIB dup2(int, int);
char * __CLIB mktemp(char *);
#define _mktemp mktemp
#define _dup2 dup2

int __CLIB access(const char *,int);
#define _access access

#define F_OK    0
#define X_OK    1
#define W_OK    2
#define R_OK    4

long __CLIB lseek(int,long,int);
#define _lseek lseek
#define SEEK_SET        0
#define SEEK_CUR        1
#define SEEK_END        2

#ifdef __cplusplus
    inline long tell( int handle ) { return _lseek( handle, 0L, SEEK_CUR ); }
#else
    #define tell( handle ) _lseek( handle, 0L, SEEK_CUR )
#endif
#define _tell tell

long __CLIB filesize(const char *);
long __CLIB filelength(int);
int __CLIB isatty(int);
#define _filelength filelength
#define _isatty isatty

unsigned short __CLIB getDS(void);

#ifndef __STDC__
#define getDS() ((unsigned short)__emit__(0x8C,0xD8))
#endif

#ifdef __NT__
#ifndef __STDC__
int __CLIB _wsopen(const wchar_t *, int, int, ...);
int __CLIB _wchmod(const wchar_t *, int);
int __CLIB _wopen(const wchar_t *,int,...);
int __CLIB _wcreat(const wchar_t *,int);
int __CLIB _wrename(const wchar_t *, const wchar_t *);
int __CLIB _wunlink(const wchar_t *);
wchar_t * __CLIB _wmktemp(wchar_t *);
int __CLIB _waccess(const wchar_t *, int);
#endif
#endif

#if __cplusplus
}
#endif

#endif
