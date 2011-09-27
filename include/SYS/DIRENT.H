/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */

#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SYS_DIRENT_H
#define __SYS_DIRENT_H 1

#if __cplusplus
extern "C"
{
#endif

#if __OS2__ && __INTSIZE == 4
#define __CLIB	__stdcall
#else
#define __CLIB	__cdecl
#endif

#if !defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE)
#define MAXNAMLEN	512		/* maximum filename length */
#define DIRBUF		2048		/* buffer size for fs-indep. dirs */
#endif

#include <sys/types.h>

typedef struct	/* #ifdefs protect opaque type's members */
	{
	int		dd_fd;		/* file descriptor */
	int		dd_loc;		/* location offset in block */
	int		dd_size;	/* amount of valid data */
	char		*dd_buf;	/* directory block */
	} DIR;			/* stream data from opendir() */

DIR * __CLIB opendir(const char *);
struct dirent * __CLIB readdir(DIR *);

#if !defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE)
long __CLIB telldir(DIR *);
void __CLIB seekdir(DIR *,long);
#endif 

void __CLIB rewinddir(DIR *);
int __CLIB closedir(DIR *);

#if !defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE)
#define rewinddir(dirp)	seekdir(dirp,0L)
#endif

/* File-system independent directory entry. */
struct dirent {
	ino_t		d_ino;		/* "inode number" of entry */
	off_t		d_off;		/* offset of disk directory entry */
	unsigned short	d_reclen;	/* length of this record */
	char		d_name[1];	/* name of file */
};

typedef	struct	dirent	dirent_t;

#if !defined(_POSIX_SOURCE)
int __CLIB getdents(int, struct dirent *, unsigned);
#endif 

#if __cplusplus
}
#endif

#endif

