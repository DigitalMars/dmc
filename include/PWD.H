/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __PWD_H
#define __PWD_H 1

#if __cplusplus
extern "C" {
#endif

#include <sys/types.h>

struct passwd
        {
	char	*pw_name;
	char	*pw_passwd;
	uid_t	pw_uid;
	gid_t	pw_gid;
	char	*pw_age;
	char	*pw_comment;
	char	*pw_gecos;
	char	*pw_dir;
	char	*pw_shell;
        };

#if !defined(_POSIX_SOURCE) 
struct comment
        {
	char	*c_dept;
	char	*c_name;
	char	*c_acct;
	char	*c_bin;
        };
#endif /* !defined(_POSIX_SOURCE) */ 

#ifndef __STDC__

#if !defined(_POSIX_SOURCE) 
struct passwd * __cdecl getpwent(void);
void __cdecl setpwent(void);
void __cdecl endpwent(void);
#ifdef __FILE_DEFINED
struct passwd *  __cdecl fgetpwent(FILE *);
#else
struct passwd *  __cdecl fgetpwent(void *);
#endif
#endif /* !defined(_POSIX_SOURCE) */ 

struct passwd * __cdecl getpwnam(const char *);
struct passwd * __cdecl getpwuid(uid_t);

#endif /* __STDC__ */

#if __cplusplus
}
#endif

#endif
