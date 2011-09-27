/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __FCNTL_H
#define __FCNTL_H 1

#if __cplusplus
extern "C" {
#endif

#define O_RDONLY	0
#define O_WRONLY	1
#define O_RDWR		2
#define O_APPEND	8
#define O_CREAT		0x100
#define O_TRUNC		0x200
#define O_EXCL		0x400

#define _O_RDONLY       0
#define _O_WRONLY       1
#define _O_RDWR         2
#define _O_APPEND       8
#define _O_CREAT        0x100
#define _O_TRUNC        0x200
#define _O_EXCL         0x400

#if M_UNIX || M_XENIX

#define O_ACCMODE       3
#if !defined(_XOPEN_SOURCE) && !defined(_POSIX_SOURCE)
#define O_NDELAY        4
#endif
#define O_NONBLOCK      0x80

#if M_XOUT

#define CF_RDLCK	1
#define CF_WRLCK	2
#define CF_UNLCK	3

#define CMN_RDLCK	3
#define CMN_WRLCK	1
#define CMN_UNLCK	0
#define F_RDLCK CMN_RDLCK
#define F_WRLCK CMN_WRLCK
#define F_UNLCK CMN_UNLCK

#define O_SYNCW 0020
#define O_SYNC	O_SYNCW

#else /* M_XOUT */

#define F_RDLCK 01
#define F_WRLCK 02
#define F_UNLCK 03

#if !defined(_POSIX_SOURCE)
#define O_SYNC    0x10
#endif

#endif	/* M_XOUT */

#define O_NOCTTY 0x800

#define F_DUPFD	  0
#define F_GETFD	  1
#define F_SETFD	  2
#define F_GETFL	  3
#define F_SETFL	  4
#define F_GETLK	  5
#define F_SETLK	  6
#define F_SETLKW  7
#if !defined(_XOPEN_SOURCE) && !defined(_POSIX_SOURCE)
#define F_CHKFL   8
#define F_ALLOCSP 10
#define F_FREESP  11
#define F_CHSIZE  0x6000
#define F_RDCHK	  0x6001
#endif

#define FD_CLOEXEC	0x01

#if !defined(_XOPEN_SOURCE) && !defined(_POSIX_SOURCE)
#define F_LK_NBRLCK     0x6100
#define F_LK_NBLCK	0x6200
#define F_LK_UNLCK	0x6300
#define F_LK_RLCK	0x7100
#define F_LK_LOCK	0x7200

#define LK_LCKTYPE(a)	((a>>8)&7)
#define LK_CMDTYPE(a)	((a>>12)&7)
#endif

#include <sys/types.h>

struct flock {
        short   l_type;
	short	l_whence;
	off_t	l_start;
        off_t   l_len;
	short   l_sysid;
	pid_t  	l_pid;
};

int __cdecl rdchk(int filedes);
int __cdecl fcntl(int filedes, int cmd, ...);

#else	/* M_XENIX || M_UNIX */

#define O_NOINHERIT     0x80
#define O_BINARY        0x8000
#define O_TEXT          0x4000

#define _O_TEXT         0x4000
#define _O_BINARY	0x8000
#define _O_RAW          _O_BINARY
#define _O_NOINHERIT	0x0080

#endif /* M_UNIX || M_XENIX */

#if __cplusplus
}
#endif

#endif
