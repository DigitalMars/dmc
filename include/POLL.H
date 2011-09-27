/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __POLL_H
#define __POLL_H 1

#if __cplusplus
extern "C" {
#endif

struct pollfd 
{
	int fd;
	short events;
	short revents;
};

#define POLLIN		001
#define POLLPRI		002
#define	POLLOUT		004
#define POLLERR		010
#define POLLHUP		020
#define POLLNVAL	040
#define NPOLLFILE	20

int __cdecl poll(struct pollfd[], unsigned long, int);

#if __cplusplus
}
#endif

#endif
