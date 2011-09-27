/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SYS_STAT_H
#define __SYS_STAT_H 1

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

#ifndef __STDC__
#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif
#endif

#define _S_IFMT	  0xF000
#define _S_IFDIR  0x4000
#define _S_IFCHR  0x2000
#define _S_IFIFO  0x1000
#define _S_IFREG  0x8000
#define _S_IREAD  0x0100
#define _S_IWRITE 0x0080
#define _S_IEXEC  0x0040

#ifndef __STDC__
#define S_IFMT	 _S_IFMT
#define S_IFDIR  _S_IFDIR
#define S_IFCHR  _S_IFCHR
#define S_IFREG  _S_IFREG
#define S_IREAD  _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC  _S_IEXEC
#define S_IFBLK  0x6000
#define S_IFNAM  0x5000
#endif

#define S_ISREG(m)  (((m)&(S_IFMT))==S_IFREG)
#define S_ISBLK(m)  (((m)&(S_IFMT))==S_IFBLK)
#define S_ISNAM(m)  (((m)&(S_IFMT))==S_IFNAM)
#define S_ISDIR(m)  (((m)&(S_IFMT))==S_IFDIR)
#define S_ISCHR(m)  (((m)&(S_IFMT))==S_IFCHR)

#if M_UNIX || M_XENIX
#define S_IFIFO 0x1000
#define S_ISFIFO(m) (((m)&(S_IFMT))==S_IFIFO)

#define S_IXOTH 0x0001
#define S_IWOTH 0x0002
#define S_IROTH 0x0004
#define S_IRWXO 0x0007
#define S_INSEM 0x0001
#define S_INSHD 0x0002
#define S_ISUID 0x0800
#define S_ISGID 0x0400
#define S_ISVTX 0x0200
#define S_ENFMT 0x0400
#define S_IRWXU 0x01C0
#define S_IRUSR 0x0100
#define S_IWUSR 0x0080
#define S_IXUSR 0x0040
#define S_IRWXG 0x0038
#define S_IRGRP 0x0020
#define S_IWGRP 0x0010
#define S_IXGRP 0x0008

#if !defined(_POSIX_SOURCE)
#define S_IFLNK  0xA000
#define S_IFSOCK 0xC000
#endif

#include <sys/types.h>
#include <time.h>

#ifndef _STYPES
#if !defined(M_ELF)
#define _STYPES 1
#endif
#endif

#if !defined(_STYPES)

#define _ST_FSTYPSZ     16

/* SVR4 stat */

struct  stat {
	dev_t	st_dev;
	long	st_pad1[3];
	ino_t	st_ino;
	mode_t 	st_mode;
	nlink_t	st_nlink;
	uid_t 	st_uid;
	gid_t 	st_gid;
	dev_t	st_rdev;
	long	st_pad2[2];
	off_t	st_size;
	long	st_pad3;
        union
              {
                time_t          st__sec;
                timestruc_t     st__tim;
              } st_atim,
                st_mtim,
                st_ctim;
	long	st_blksize;
	long	st_blocks;
	char	st_fstype[_ST_FSTYPSZ];
	int	st_aclcnt;
	level_t	st_level;
	ulong_t	st_flags;
	lid_t	st_cmwlevel;
	long	st_pad4[4];
};

#define st_atime st_atim.st__sec
#define st_mtime st_mtim.st__sec
#define st_ctime st_ctim.st__sec

#else   /* !defined(_STYPES) */

/* SVID 2 stat */

struct  stat {
        o_dev_t st_dev;
        o_ino_t st_ino;
        o_mode_t st_mode;
        o_nlink_t st_nlink;
        o_uid_t st_uid;
        o_gid_t st_gid;
        o_dev_t st_rdev;

        off_t   st_size;
        time_t  st_atime;
        time_t  st_mtime;
        time_t  st_ctime;
};

int __cdecl stat(const char *,struct stat *);
int __cdecl fstat(int,struct stat *);

#if !defined(_POSIX_SOURCE)
int __cdecl lstat(const char *, struct stat *);
int __cdecl mknod(const char *, mode_t, dev_t);
#endif

#endif /* !defined(_STYPES) */

#if defined(M_ELF)

int __cdecl _fxstat(const int, int, struct stat *);
int __cdecl _xstat(const int, const char *, struct stat *);

#if !defined(_POSIX_SOURCE) 
int __cdecl _lxstat(const int, const char *, struct stat *);
int __cdecl _xmknod(const int, const char *, mode_t, dev_t);
#endif

#if !defined(_STYPES)
#define _MKNOD_VER      2
#define _STAT_VER       2
/* Map to SVR4 functions */
#define fstat(fd,buf) _fxstat(_STAT_VER, fd, buf)

#if !defined(_POSIX_SOURCE) 
#define lstat(path,buf) _lxstat(_STAT_VER, path, buf)
#define mknod(path, mode, dev) _xmknod(_MKNOD_VER, path, mode, dev)
#endif

static int __cdecl stat( const char *path, struct stat *buf)
{
	return _xstat(_STAT_VER, path, buf);
}


#endif /* !defined(_STYPES) */

#endif /* M_ELF */

int __cdecl chmod(const char *, mode_t);
int __cdecl mkdir(const char *, mode_t);
int __cdecl mkfifo(const char *, mode_t);
/* mode_t __cdecl umask(mode_t); defined in io.h */ 
int __cdecl fchmod(int, mode_t);

#else /* M_UNIX || M_XENIX */

#pragma pack (2)
struct stat
{	short	st_dev;
	unsigned short	st_ino;
	unsigned short st_mode;
	short	st_nlink;
	unsigned short	st_uid;
	unsigned short	st_gid;
	short	st_rdev;
#if __INTSIZE == 4
	short	dummy;                  /* for alignment                */
#endif
	long	st_size;
	long	st_atime;
	long	st_mtime;
	long	st_ctime;
};
#pragma pack ()

#define _stat stat

int __cdecl stat(const char *,struct stat *);
int __cdecl fstat(int,struct stat *);
#define _fstat fstat
int __cdecl _stat(const char *,struct stat *);

#endif /* M_UNIX || M_XENIX */

/* For MicroSoft and Borland compatibility */
#define _S_IFMT	0xF000
#define _S_IFREG 0x8000
#define _S_IFBLK 0x6000
#define _S_IFNAM 0x5000
#define _S_IFDIR 0x4000
#define _S_IFCHR 0x2000
#define _S_IREAD 0x0100
#define _S_IWRITE 0x0080
#define _S_IEXEC 0x0040

#ifdef __NT__
#ifndef __STDC__
int __cdecl _wstat(const wchar_t *, struct stat *);
#endif
#endif

#if __cplusplus
}
#endif

#endif

