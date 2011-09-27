/*_ vwprintf.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#define UNICODE		1	/* Enable Windows Unicode API	*/
#define _UNICODE	1	/* Enable C Unicode Runtime	*/

#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <scio.h>
#include <sctchar.h>

int vwprintf (const wchar_t *format, va_list args) {
 int	status;
  __fp_lock (stdout);
  status = _wpformat ((fp_t)_fputwc_nlock, stdout, format, args);
  __fp_unlock (stdout);
  if (stdout->_flag & _IOLBF) {
    fflush (stdout);
  }
  return status;
}

#endif

/**/
