
/* Copyright (C) 1992-2003 by Digital Mars */

#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __LIMITS_H 
#define __LIMITS_H 1

#define	CHAR_BIT	8
#define	SCHAR_MAX	127
#define	SCHAR_MIN	(-128)
#define	UCHAR_MAX	255
#if _CHAR_UNSIGNED
#define	CHAR_MAX	255
#define	CHAR_MIN	0
#else
#define	CHAR_MAX	127
#define	CHAR_MIN	(-128)
#endif

#define	SHRT_MAX	32767
#define	SHRT_MIN	(-32768)
#define	USHRT_MAX	65535

#define LONG_MAX        2147483647
#define LONG_MIN        (-2147483647L - 1)
#define	ULONG_MAX	4294967295U

#if __INTSIZE == 2
#define	INT_MAX		32767
#define	INT_MIN		(-32768)
#define	UINT_MAX	65535
#else
#define	INT_MAX		2147483647
#define	INT_MIN		(-2147483647L - 1)
#define	UINT_MAX	4294967295
#endif

#if __INTSIZE == 4
#define LLONG_MIN       (-9223372036854775807LL-1LL)
#define LLONG_MAX       9223372036854775807LL
#define ULLONG_MAX      18446744073709551615ULL
#endif


#define MB_LEN_MAX	2

#if M_UNIX || M_XENIX
#if defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE) || !__STDC__

#define _POSIX_ARG_MAX     4096
#define _POSIX_CHILD_MAX      6
#define _POSIX_LINK_MAX       8
#define _POSIX_MAX_CANON    255
#define _POSIX_MAX_INPUT    255
#define _POSIX_NAME_MAX      14
#define _POSIX_NGROUPS_MAX    0
#define _POSIX_OPEN_MAX      16
#define _POSIX_PATH_MAX     255
#define _POSIX_PIPE_BUF     512
#define _POSIX_SSIZE_MAX  32767
#define _POSIX_STREAM_MAX     8
#define _POSIX_TZNAME_MAX     3

#define ARG_MAX            5120

#define NGROUPS_MAX           8 

#define MAX_CANON       256
#define MAX_INPUT       256
#define PIPE_BUF        5120
#define PIPE_MAX        PIPE_BUF
#define SSIZE_MAX       INT_MAX
#endif /* _POSIX_SOURCE || _XOPEN_SOURCE || !__STDC__ */

#define FCHR_MAX    1048576
#define PID_MAX       30000
#define STD_BLK        1024
#define SYS_NMLN          9
#define UID_MAX       60000
#define USI_MAX  4294967295

#if defined(_XOPEN_SOURCE) || !__STDC__
#define LONG_BIT         32
#ifndef TMP_MAX
#define TMP_MAX       17576
#endif
#define NZERO		 20
#define PASS_MAX          8
#define WORD_BIT         32
#endif

#define NL_ARGMAX         9
#define NL_LANGMAX       14
#define NL_MSGMAX     32767
#define NL_SETMAX       255
#define NL_TEXTMAX      255
#define NL_NMAX           0

#endif /* M_UNIX || M_XENIX */

#endif
