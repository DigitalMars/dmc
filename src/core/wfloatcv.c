/*_ wfloatcv.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <float.h>

#define BUF_DIGSZ	(LDBL_DIG * 2 + 16)

char * __pascal __floatcvt (int cnvflag, char *digstr, long double val, int ndig, int *pdecpt, int *psign);

wchar_t * __pascal __wfloatcvt
(int cnvflag, wchar_t *digstr, long double val, int ndig, int *pdecpt, int *psign) {
 char		cbuf[BUF_DIGSZ];
  __floatcvt (cnvflag, cbuf, val, ndig, pdecpt, psign);
  if (mbstowcs (digstr, cbuf, BUF_DIGSZ) == (size_t) -1) {
    digstr[0] = L'\0';
  }
  return digstr;
}

#endif

/**/
