/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __GRP_H
#define __GRP_H 1

#if __cplusplus
extern "C" {
#endif

#include <sys/types.h>
struct group
        {
        char *gr_name;
        char *gr_passwd;
        gid_t gr_gid;
        char **gr_mem;
        };
#if !defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE)
#include <stdio.h>
#endif
#ifndef __STDC__
#if !defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE)
void __CLIB endgrent(void);
struct group * __CLIB fgetgrent(FILE *);
struct group * __CLIB getgrent(void);
void __CLIB setgrent(void);
int __CLIB initgroups(const char *,gid_t);
#endif /* !_POSIX_SOURCE && !_XOPEN_SOURCE */
extern struct group * __CLIB getgrgid(gid_t);
extern struct group * __CLIB getgrnam(const char *);
#endif  /* __STDC__ */

#if __cplusplus
};
#endif

#endif
