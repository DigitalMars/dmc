/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __DMC__ || __RCC__
#pragma once
#endif


#ifndef __ERRNO_H
#define __ERRNO_H 1

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

#if defined(_MT) || defined(_DLL) || __cplusplus
extern int * __CLIB _errno(void);
#define errno (*_errno())
#else
extern int __cdecl errno;
#endif

#if M_UNIX || M_XENIX

#if defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE) || !__STDC__
#define EPERM           1
#define ENOENT		2
#define ESRCH		3
#define EINTR		4
#define EIO		5
#define ENXIO		6
#define E2BIG		7
#define ENOEXEC		8
#define EBADF		9
#define ECHILD		10
#define EAGAIN		11
#define ENOMEM		12
#define EACCES		13
#define EFAULT		14

#if !defined(_POSIX_SOURCE)
#define ENOTBLK         15
#endif

#define EBUSY           16
#define EEXIST		17
#define EXDEV		18
#define ENODEV		19
#define ENOTDIR		20
#define EISDIR		21
#define EINVAL		22
#define ENFILE		23
#define EMFILE		24
#define ENOTTY		25

#if !defined(_POSIX_SOURCE)
#define ETXTBSY         26
#endif

#define EFBIG           27
#define ENOSPC		28
#define ESPIPE		29
#define EROFS		30
#define EMLINK		31
#define EPIPE		32
#endif /* defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE) || !__STDC__ */

#define EDOM            33

#ifndef ERANGE
#define ERANGE		34
#endif

#if defined(_XOPEN_SOURCE) || !__STDC__
#define ENOMSG          35
#define EIDRM		36
#endif

#if !__STDC__
#define ECHRNG          37
#define EL2NSYNC	38
#define EL3HLT		39
#define EL3RST		40
#define ELNRNG		41
#define EUNATCH		42
#define ENOCSI		43
#define EL2HLT		44
#endif

#if defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE) || !__STDC__
#define EDEADLK         45
#define ENOLCK		46
#endif

#define EBADE           50
#define EBADR           51
#define EXFULL          52
#define ENOANO          53
#define EBADRQC         54
#define EBADSLT         55
#define EDEADLOCK       56
#define EBFONT          57
#define ENOSTR          60
#define ENODATA         61
#define ETIME           62
#define ENOSR           63
#define ENONET          64
#define ENOPKG          65
#define EREMOTE         66
#define ENOLINK         67
#define EADV            68
#define ESRMNT          69
#define ECOMM           70
#define EPROTO          71
#define EMULTIHOP       74
#define ELBIN           75
#define EDOTDOT         76
#define EBADMSG         77

#if defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE) || !__STDC__
#define ENAMETOOLONG    78
#endif 

#define EOVERFLOW       79   
#define ENOTUNIQ        80
#define EBADFD          81
#define EREMCHG         82
#define ELIBACC         83
#define ELIBBAD         84
#define ELIBSCN         85
#define ELIBMAX         86
#define ELIBEXEC        87
#define EILSEQ          88

#if defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE) || !__STDC__
#define ENOSYS		89
#endif /* defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE) || !__STDC__ */

#if !__STDC__

#ifdef SCOUNIX
#define TCPERR          90
#define EWOULDBLOCK     (TCPERR+0)
#define EINPROGRESS     (TCPERR+1)
#define EALREADY        (TCPERR+2)
#define ENOTSOCK        (TCPERR+3)
#define EDESTADDRREQ    (TCPERR+4)
#define EMSGSIZE        (TCPERR+5)
#define EPROTOTYPE      (TCPERR+6)
#define EPROTONOSUPPORT (TCPERR+7)
#define ESOCKTNOSUPPORT (TCPERR+8)
#define EOPNOTSUPP      (TCPERR+9)
#define EPFNOSUPPORT    (TCPERR+10)
#define EAFNOSUPPORT    (TCPERR+11)
#define EADDRINUSE      (TCPERR+12)
#define EADDRNOTAVAIL   (TCPERR+13)
#define ENETDOWN        (TCPERR+14)
#define ENETUNREACH     (TCPERR+15)
#define ENETRESET       (TCPERR+16)
#define ECONNABORTED    (TCPERR+17)
#define ECONNRESET      (TCPERR+18)
#define ENOBUFS         ENOSR
#define EISCONN         (TCPERR+20)
#define ENOTCONN        (TCPERR+21)
#define ESHUTDOWN       (TCPERR+22)
#define ETOOMANYREFS    (TCPERR+23)
#define ETIMEDOUT       (TCPERR+24)
#define ECONNREFUSED    (TCPERR+25)
#define EHOSTDOWN       (TCPERR+26)
#define EHOSTUNREACH    (TCPERR+27)
#define ENOPROTOOPT     (TCPERR+28)

#else   /* SCOUNIX */

#define ELOOP           90
#define ERESTART        91
#define ESTRPIPE        92
#define ENOTEMPTY       93
#define EUSERS          94

#define ENOTSOCK        95
#define EDESTADDRREQ    96
#define EMSGSIZE        97
#define EPROTOTYPE      98
#define ENOPROTOOPT     99

#define EPROTONOSUPPORT 120
#define ESOCKTNOSUPPORT 121
#define EOPNOTSUPP      122
#define EPFNOSUPPORT    123
#define EAFNOSUPPORT    124

#define EADDRINUSE      125
#define EADDRNOTAVAIL   126
#define ENETDOWN        127
#define ENETUNREACH     128
#define ENETRESET       129

#define ECONNABORTED    130
#define ECONNRESET      131
#define ENOBUFS         132
#define EISCONN         133
#define ENOTCONN        134
#endif /* SCOUNIX */

/* XENIX error numbers */
#define EUCLEAN         135
#define ENOTNAM         137
#define ENAVAIL         138
#define EISNAM          139
#define EREMOTEIO       140
#define EINIT           141
#define EREMDEV         142

#define ESHUTDOWN       143
#define ETOOMANYREFS    144
#define ETIMEDOUT       145
#define ECONNREFUSED    146
#define EHOSTDOWN       147
#define EHOSTUNREACH    148
#define EWOULDBLOCK	EAGAIN
#define EALREADY        149
#define EINPROGRESS     150

#define ESTALE          151

#define ENOLOAD         152
#define ERELOC          153
#define ENOMATCH        154
#define EBADVER         156
#define ECONFIG         157
#endif /* !__STDC__ */
#else  /* M_UNIX || M_XENIX */

#define EZERO        0
#define EPERM        1
#define ENOENT       2
#define ESRCH        3
#define EINTR        4
#define EIO          5
#define ENXIO        6
#define E2BIG        7
#define ENOEXEC      8
#define EBADF        9
#define ECHILD       10
#define EAGAIN       11
#define ENOMEM       12
#define EACCES       13
#define EFAULT       14
#define ENOTBLK      15
#define EBUSY        16
#define EEXIST       17
#define EXDEV        18
#define ENODEV       19
#define ENOTDIR      20
#define EISDIR       21
#define EINVAL       22
#define ENFILE       23
#define EMFILE       24
#define ENOTTY       25
#define ETXTBSY      26
#define EFBIG        27
#define ENOSPC       28
#define ESPIPE       29
#define EROFS        30
#define EMLINK       31
#define EPIPE        32
#define EDOM         33
#define ERANGE       34
#define EUCLEAN      35
#define EDEADLOCK    36
#define EDEADLK      36
#define ENAMETOOLONG 38
#define ENOLCK       39
#define ENOSYS       40
#define ENOTEMPTY    41
#define EILSEQ       42

#endif /* M_UNIX || M_XENIX */

#if __cplusplus
}
#endif

#endif
