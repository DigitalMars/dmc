/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SYS_TYPES_H
#define __SYS_TYPES_H 1

#ifndef __TIME_T_DEFINED
typedef long		time_t;
#define __TIME_T_DEFINED
#endif

#if !defined(M_UNIX) && !defined(M_XENIX)

typedef unsigned short _ino_t;
typedef short _dev_t;
typedef long _off_t;
#ifndef __STDC__
typedef unsigned short ino_t;
typedef short dev_t;
typedef long off_t;
#endif 

#else /* M_UNIX || M_XENIX */
typedef char *		addr_t; 	
typedef char *		caddr_t;
typedef long		daddr_t;
typedef char *		faddr_t;
typedef short		cnt_t;
typedef unsigned long	paddr_t;
typedef unsigned char	use_t;
typedef short		sysid_t;
typedef short		index_t;
typedef short		lock_t;	
typedef	unsigned short	sel_t;
typedef unsigned long	k_sigset_t;
typedef unsigned long	k_fltset_t;

#if !defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE)
typedef struct _label 
	{ 
	int val[6]; 
	} label_t;

typedef enum boolean 
	{ B_FALSE, B_TRUE 
	} boolean_t;
#endif

/* POSIX Extensions */

typedef unsigned char	uchar_t;
typedef unsigned short	ushort_t;
typedef unsigned int	uint_t;
typedef unsigned long	ulong_t;

typedef long		id_t;		
typedef unsigned long	pvec_t;		
typedef unsigned long	lid_t;		
typedef lid_t		level_t;	

typedef unsigned long	major_t;
typedef unsigned long	minor_t;

/* POSIX and XOPEN required typedefs */

typedef long            UID_T;
typedef unsigned long	DEV_T;
typedef UID_T		GID_T;
typedef unsigned long	INO_T;
typedef int		KEY_T;
typedef unsigned long	MODE_T;
typedef unsigned long	NLINK_T;
typedef long		off_t;
typedef long		PID_T;
typedef unsigned int	size_t;
typedef int		ssize_t;

typedef long		clock_t;

/* pre-SVR4 types */
typedef long            o_key_t;
typedef unsigned short  o_mode_t;
typedef short           o_dev_t;
typedef unsigned short  o_uid_t;
typedef o_uid_t         o_gid_t;
typedef short           o_nlink_t;
typedef short           o_pid_t;
typedef unsigned short  o_ino_t;

#if !defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE)

typedef struct { int r[1]; } * physadr;
typedef unsigned char	unchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef int		spl_t;

typedef unsigned char	u_char;
typedef unsigned short	u_short;
typedef unsigned int	u_int;
typedef unsigned long	u_long;

#define NBBY    8

#ifndef	FD_SETSIZE
#ifdef NOFILES_MAX
#define  FD_SETSIZE NOFILES_MAX
#else
#define  FD_SETSIZE 150
#endif
#endif

typedef long	fd_mask;
#define NFDBITS (sizeof(fd_mask) * NBBY)

#ifndef howmany
#define  howmany(x, y)   (((x)+((y)-1))/(y))
#endif

typedef struct fd_set
        {
	fd_mask	fds_bits[howmany(FD_SETSIZE, NFDBITS)];
        } fd_set;

#define	FD_SET(n, p)	((p)->fds_bits[(n)/NFDBITS] |= (1L << ((n) % NFDBITS)))
#define	FD_CLR(n, p)	((p)->fds_bits[(n)/NFDBITS] &= ~(1L << ((n) % NFDBITS)))
#define	FD_ISSET(n, p)	((p)->fds_bits[(n)/NFDBITS] & (1L << ((n) % NFDBITS)))
#define FD_ZERO(p)	memset((char *)(p), '\0', sizeof(*(p)))

typedef unsigned long   mask_t;
typedef mask_t          priv_t;

#endif  

#ifndef _STYPES
#if !defined(M_ELF)
#define _STYPES 1
#endif
#endif

#if !defined(_STYPES)
#define key_t KEY_T
#define mode_t MODE_T
#define dev_t DEV_T
#define uid_t UID_T
#define gid_t GID_T
#define nlink_t NLINK_T
#define pid_t PID_T
#define ino_t INO_T
#else
#define key_t o_key_t
#define mode_t o_mode_t
#define dev_t o_dev_t
#define uid_t o_uid_t
#define gid_t o_gid_t
#define nlink_t o_nlink_t
#define pid_t o_pid_t
#define ino_t o_ino_t
#endif

typedef long hostid_t;
#define P_MYHOSTID ((hostid_t)(-1))

#endif  /* M_UNIX || M_XENIX */

#endif

