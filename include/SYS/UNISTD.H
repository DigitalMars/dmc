/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SYS_UNISTD_H
#define __SYS_UNISTD_H 1

#define R_OK    004
#define W_OK    002
#define X_OK    001
#define F_OK    000

#if !defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE)
#define EFF_ONLY_OK     010
#define EX_OK           020
#define _PASS_MAX       80
#define _POSIX_SYMLOOP  32
#define GF_PATH         "/etc/group"
#define PF_PATH         "/etc/passwd"
#endif

#if !defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE)
#define F_ULOCK 0
#define F_LOCK  1
#define F_TLOCK 2
#define F_TEST  3
#endif 

#define SEEK_SET        0
#define SEEK_CUR        1
#define SEEK_END        2

#define _POSIX_JOB_CONTROL      1
#define _POSIX_SAVED_IDS        1

#ifndef _POSIX_VDISABLE
#define _POSIX_VDISABLE         0
#endif

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

#ifdef M_ELF

#define _SC_ARG_MAX	1
#define _SC_CHILD_MAX	2
#define _SC_CLK_TCK	3
#define _SC_NGROUPS_MAX 4
#define _SC_OPEN_MAX	5
#define _SC_JOB_CONTROL 6
#define _SC_SAVED_IDS	7
#define _SC_VERSION	8
#define _SC_PASS_MAX	9
#define _SC_LOGNAME_MAX	10
#define _SC_PAGESIZE	11
#define _SC_XOPEN_VERSION 12
#define _SC_NACLS_MAX   13

#define _PC_LINK_MAX    1
#define _PC_MAX_CANON	2
#define _PC_MAX_INPUT	3
#define _PC_NAME_MAX	4
#define _PC_PATH_MAX	5
#define _PC_PIPE_BUF	6
#define _PC_CHOWN_RESTRICTED	9
#else
#define _SC_ARG_MAX     0
#define _SC_CHILD_MAX   1
#define _SC_CLK_TCK     2
#define _SC_NGROUPS_MAX 3
#define _SC_OPEN_MAX    4
#define _SC_JOB_CONTROL 5
#define _SC_SAVED_IDS   6
#define _SC_VERSION     7
#define _SC_PASS_MAX    8
#define _SC_XOPEN_VERSION 9

#define _PC_LINK_MAX    0
#define _PC_MAX_CANON   1
#define _PC_MAX_INPUT   2
#define _PC_NAME_MAX    3
#define _PC_PATH_MAX    4
#define _PC_PIPE_BUF    5
#define _PC_CHOWN_RESTRICTED 6
#endif

#define _PC_NO_TRUNC    7
#define _PC_VDISABLE    8

#ifndef _POSIX_VERSION
#define _POSIX_VERSION	198808L
#endif

#ifndef _XOPEN_VERSION
#define _XOPEN_VERSION 3
#endif

#endif

