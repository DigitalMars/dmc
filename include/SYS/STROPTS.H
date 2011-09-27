/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SYS_STROPTS_H
#define __SYS_STROPTS_H 1

#include <sys/types.h>	

#define SNDZERO         0x001
#define SNDPIPE         0x002

#define RNORM           0x000
#define RMSGD           0x001
#define RMSGN           0x002
#define RMODEMASK       0x003
#define RPROTDAT        0x004
#define RPROTDIS        0x008
#define RPROTNORM	0x010
#define RPROTMASK       0x01c

#define FLUSHR          0x01
#define FLUSHW          0x02
#define FLUSHRW         0x03
#define FLUSHBAND       0x04

#define S_INPUT         0x0001
#define S_HIPRI         0x0002
#define S_OUTPUT        0x0004
#define S_MSG           0x0008
#define S_ERROR         0x0010
#define S_HANGUP        0x0020
#define S_RDNORM        0x0040
#define S_WRNORM	S_OUTPUT
#define S_RDBAND        0x0080
#define S_WRBAND        0x0100
#define S_BANDURG       0x0200

#define RS_HIPRI        0x01
#define	DONT_BLOCK      0x02		

#define MSG_HIPRI       0x01
#define MSG_ANY         0x02
#define MSG_BAND        0x04

#define MORECTL         1
#define MOREDATA        2

#define MUXID_ALL	(-1)

#define ANYMARK		0x01
#define LASTMARK	0x02

/*  Stream Ioctl defines */
#define	STR		('S'<<8)
/* (STR|000) in use */
#define I_NREAD		(STR|01)
#define I_PUSH		(STR|02)
#define I_POP		(STR|03)
#define I_LOOK		(STR|04)
#define I_FLUSH		(STR|05)
#define I_SRDOPT	(STR|06)
#define I_GRDOPT	(STR|07)
#define I_STR		(STR|010)
#define I_SETSIG	(STR|011)
#define I_GETSIG	(STR|012)
#define I_FIND		(STR|013)
#define I_LINK		(STR|014)
#define I_UNLINK	(STR|015)
/* (STR|016) in use */
#define I_PEEK		(STR|017)
#define I_FDINSERT	(STR|020)
#define I_SENDFD	(STR|021)
#define I_RECVFD        (STR|022)

#define I_SWROPT	(STR|023)
#define I_GWROPT	(STR|024)
#define I_LIST		(STR|025)
#define I_PLINK		(STR|026)
#define I_PUNLINK	(STR|027)
#define I_FLUSHBAND	(STR|034)
#define I_CKBAND	(STR|035)
#define I_GETBAND	(STR|036)
#define I_ATMARK	(STR|037)
#define I_SETCLTIME	(STR|040)
#define I_GETCLTIME	(STR|041)
#define I_CANPUT	(STR|042)
#define	I_S_RECVFD	(STR|043)

/* Same ioctl as O_MODESWITCH */
#define	X_STR			('S'<<8)	/* SCO XENIX Streams ioctl's */
#define	X_I_BASE		(X_STR|0200)
#define	X_I_NREAD		(X_STR|0201)
#define	X_I_PUSH		(X_STR|0202)
#define	X_I_POP			(X_STR|0203)
#define	X_I_LOOK		(X_STR|0204)
#define	X_I_FLUSH		(X_STR|0205)
#define	X_I_SRDOPT		(X_STR|0206)
#define	X_I_GRDOPT		(X_STR|0207)
#define	X_I_STR			(X_STR|0210)
#define	X_I_SETSIG		(X_STR|0211)
#define	X_I_GETSIG		(X_STR|0212)
#define	X_I_FIND		(X_STR|0213)
#define	X_I_LINK		(X_STR|0214)
#define	X_I_UNLINK		(X_STR|0215)
#define	X_I_PEEK		(X_STR|0217)
#define	X_I_FDINSERT		(X_STR|0220)
#define	X_I_SENDFD		(X_STR|0221)
#define	X_I_RECVFD		(X_STR|0222)

struct strioctl
        {
        int     ic_cmd;
        int     ic_timout;
        int     ic_len;
        char    *ic_dp;
        };

#define INFTIM		-1

struct strbuf
        {
        int     maxlen;
        int     len;
        char    *buf;
        };

struct strpeek
        {
	struct strbuf ctlbuf;
	struct strbuf databuf;
	long	      flags;
        };

struct strfdinsert
        {
	struct strbuf ctlbuf;
	struct strbuf databuf;
	long	      flags;
	int	      fildes;
	int	      offset;
        };

struct strrecvfd
        {
	int fd;
	o_uid_t uid;
	o_gid_t gid;
	char fill[8];
        };


#ifndef FMNAMESZ
#define FMNAMESZ	8
#endif

struct str_mlist
        {
	char l_name[FMNAMESZ+1];
        };

struct str_list
        {
	int sl_nmods;
	struct str_mlist *sl_modlist;
        };

struct bandinfo
        {
	unsigned char	bi_pri;
	int		bi_flag;
        };

#if __cplusplus
extern "C"
{
#endif

#if !__STDC__
int __cdecl getmsg(int, struct strbuf *, struct strbuf *, int *);
int __cdecl putmsg(int, const struct strbuf *, const struct strbuf *, int);
#endif

#if __cplusplus
}
#endif

#endif

