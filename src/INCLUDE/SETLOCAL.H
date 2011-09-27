/* Copyright (C) 1995 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SETLOCAL_H
#define __SETLOCAL_H 1

# if __cplusplus
extern "C" {
# endif

#include <windows.h>

#define _LOCALE_C	0		/* "C" Locale Ident */
#define _LOCALE_C_CP	CP_ACP		/* "C" ANSI Code Page */
# if 0
#define _LOCALE_C_CP	CP_OEMCP	/* "C" OEM Code page */
# endif
#define IsCLocale(x)	(__locale_ident[x].lgid == _LOCALE_C)

#define LC_LGSZ		32
#define LC_CYSZ		32
#define LC_CPSZ		6
#define LC_LCSZ		(LC_LGSZ+1+LC_CYSZ+1+LC_CPSZ+1)

typedef struct {
// union {
  LCID	lgid;
//  struct {
  WORD	lang;
  WORD	lsrt;
//  };
// };
// union {
  LCID	cyid;
//  struct {
  WORD	cnty;
  WORD	csrt;
//  };
// };
  WORD	cpge;
} LCIDENT;				/* Locale Ident */

typedef struct {
  char	lang[LC_LGSZ];
  char	cnty[LC_CYSZ];
  char	cpge[LC_CPSZ];
} LCSTRING;				/* Locale String */

typedef struct {
  char *	lgweek[7];
  char *	shweek[7];
  char *	lgmont[12];
  char *	shmont[12];
  char *	lgdate;
  char *	shdate;
  char *	lyampm[2];
  char *	lytime;
} LCTIME;				/* Locale Time */

# if 0
extern int	__charmax;				/* CHARMAX.C */
# endif

extern int	__locale_chars;				/* SETCHARS.C */

# ifndef MB_CUR_MAX
extern int __CLIB	__locale_mbsize;		/* NLSDATA.C */
#define MB_CUR_MAX	__locale_mbsize
# endif

extern UINT	__locale_codepage;			/* NLSDATA.C */
extern LCIDENT	__locale_ident[];			/* NLSDATA.C */

extern char	__locale_null_const[];			/* NULDATA.C */

extern char	__locale_decimal_const[];		/* DECDATA.C */
extern char *	__locale_decpoint;			/* DECDATA.C */

extern LCTIME	__locale_time_const;			/* TIMDATA.C */
extern LCTIME *	__locale_time;				/* TIMDATA.C */

extern struct lconv	__locale_conv_const;		/* LCONV.C */
extern struct lconv *	__locale_conv;			/* LCONV.C */

# if __cplusplus
}
# endif

#endif

