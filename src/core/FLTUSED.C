/*_ fltused.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

typedef char __near *PFM(int,int,int,double *,char __ss *,int __ss *,int);
extern PFM __floatfmt;
PFM *__pfloatfmt;

typedef int __pascal SFM(struct scanf_state __ss *);
extern SFM __scanfloat;
SFM *__pscanfloat;

#ifdef __NT__
#include <wchar.h>
typedef wchar_t __near *WPFM (int, int, int, double *,
				wchar_t __ss *, int __ss *, int);
extern WPFM	__wfloatfmt;
WPFM *		__wpfloatfmt;
typedef int __pascal WSFM (struct scanf_state __ss *);
extern WSFM	__wscanfloat;
SFM *		__wpscanfloat;
#endif

/***************************
 * This definition will cause this module to be linked in.
 * We really are interested in the static constructor below.
 */

void __cdecl _fltused()
{
}

void _STI_fltused()
{
	__pfloatfmt = __floatfmt;
	__pscanfloat = __scanfloat;
#ifdef __NT__
	__wpfloatfmt = __wfloatfmt;
	__wpscanfloat = __wscanfloat;
#endif
}
