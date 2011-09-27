/*_ lconv.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <limits.h>
#include <locale.h>
#include <setlocal.h>

struct lconv	__locale_conv_const = {
  __locale_decimal_const,	/* decimal_point */
  __locale_null_const,		/* thousands_sep */
  __locale_null_const,		/* grouping */
  __locale_null_const,		/* int_curr_symbol */
  __locale_null_const,		/* currency_symbol */
  __locale_null_const,		/* mon_decimal_point */
  __locale_null_const,		/* mon_thousands_sep */
  __locale_null_const,		/* mon_grouping */
  __locale_null_const,		/* positive_sign */
  __locale_null_const,		/* negative_sign */
  CHAR_MAX,			/* int_frac_digits */
  CHAR_MAX,			/* frac_digits */
  CHAR_MAX,			/* p_cs_precedes */
  CHAR_MAX,			/* p_sep_by_space */
  CHAR_MAX,			/* n_cs_precedes */
  CHAR_MAX,			/* n_sep_by_space */
  CHAR_MAX,			/* p_sign_posn */
  CHAR_MAX,			/* n_sign_posn */
};

struct lconv *	__locale_conv = &__locale_conv_const;

struct lconv * __CLIB localeconv (void) {
  return __locale_conv;
}

extern int __charmax;	/* used for checking if user compiled with -J */

void _STI_lconv (void) {
  if (__charmax == 0) {
    __locale_conv_const.int_frac_digits = UCHAR_MAX;
    __locale_conv_const.frac_digits = UCHAR_MAX;
    __locale_conv_const.p_cs_precedes = UCHAR_MAX;
    __locale_conv_const.p_sep_by_space = UCHAR_MAX;
    __locale_conv_const.n_cs_precedes = UCHAR_MAX;
    __locale_conv_const.n_sep_by_space = UCHAR_MAX;
    __locale_conv_const.p_sign_posn = UCHAR_MAX;
    __locale_conv_const.n_sign_posn = UCHAR_MAX;
  }
}

#else

/*_ lconv.c   Thu Sep 28 1989   Modified by: Walter Bright */

#include	"locale.h"
#include	<limits.h>
#include	<string.h>

/* Some strings to reduce redundancy	*/
static char NA[] = "";		/* not available			*/
static char DOT[] = ".";
extern int __charmax;  /* used for checking if user compiled with -J */

/* At program startup, this is initialized to the "C" locale	*/
struct lconv __current_lconv =
{	DOT,NA,NA,NA,NA,NA,NA,NA,NA,NA,
	CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,
	{ _LOCALE_C,_LOCALE_C,_LOCALE_C,
	  _LOCALE_C,_LOCALE_C,_LOCALE_C
	}
};


/*************************************
 * Return pointer to current state of locale.
 */

struct lconv *localeconv(void)
{
    return &__current_lconv;
}

void _STI_lconv()
{
   /* if __charmax is 0, then user compiled with -J, fixup current_lconv */
   if (!__charmax)
   {
      __current_lconv.int_frac_digits = UCHAR_MAX;
      __current_lconv.frac_digits = UCHAR_MAX;
      __current_lconv.p_cs_precedes = UCHAR_MAX;
	   __current_lconv.p_sep_by_space = UCHAR_MAX;
	   __current_lconv.n_cs_precedes  = UCHAR_MAX;
	   __current_lconv.n_sep_by_space = UCHAR_MAX;
	   __current_lconv.p_sign_posn   =  UCHAR_MAX;
	   __current_lconv.n_sign_posn   = UCHAR_MAX;
   }
}

#endif

/**/
