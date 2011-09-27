/* Copyright (C) 1995 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SETMBCP_H
#define __SETMBCP_H

# if __cplusplus
extern "C" {
# endif

#include <windows.h>

typedef struct {		/* local code ranges */
 BYTE		flag;
 BYTE		start;
 BYTE		end;
} BYTERANG;
#define _BCountof(x)	(sizeof(x) / sizeof(BYTERANG))

typedef struct {		/* multibyte case code ranges */
 WORD		ustart;
 WORD		uend;
 WORD		lstart;
 WORD		lend;
} MBCASERANG;
#define _MCCountof(x)	(sizeof(x) / sizeof(MBCASERANG))

typedef struct {		/* multibyte digit code ranges */
 WORD		dstart;
 WORD		dend;
} MBDIGITRANG;
#define _MDCountof(x)	(sizeof(x) / sizeof(MBDIGITRANG))

typedef struct {		/* multibyte informations */
 UINT		cp;
#ifdef _WIN32
 LCID		lid;
#endif
 short		brn;
 BYTERANG *	brp;
 short		crn;
 MBCASERANG *	crp;
 short		drn;
 MBDIGITRANG *	drp;
} MBCPINFO;
#define _MICountof(x)	(sizeof(x) / sizeof(MBCPINFO))

extern MBCPINFO		__mb_cpinfo[];	/* CP Control table */
extern int		__mb_cpinfoN;	/* CP Control table item number */
extern MBCPINFO *	__mbcpinfo;	/* Current CP table */
extern UINT		__mbcodepage;	/* Current CP */
#ifdef _WIN32
extern int		__mblcid;	/* Current locale ident */
#endif

# if __cplusplus
}
# endif

#endif

