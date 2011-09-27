/*_ wscanf.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#define UNICODE		1	/* Enable Windows Unicode API	*/
#define _UNICODE	1	/* Enable C Unicode Runtime	*/
#ifdef  _MBCS
#undef  _MBCS			/* Disable MBCS code		*/
#endif

#include <tchar.h>
#include <scio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sctchar.h>

int fwscanf (FILE *fp,const wchar_t *format,...) {
  return _wsfmt ((fp_t)ungetwc, (fp2_t)fgetwc, fp, format, (va_list)&(format) + sizeof (format));
}

int wscanf(const wchar_t *format, ...) {
  return _wsfmt ((fp_t)ungetwc, (fp2_t)fgetwc, stdin, format, (va_list)&(format) + sizeof (format));
}

static int sgetwc (void *ps) {
  return (*(*(wchar_t **)ps)) ? *(*(wchar_t **)ps)++ : EOF;
}

static int sungetwc (int c, void *ps) {
  return (c == EOF) ? c : (*--(*(wchar_t **)ps) = c);
}

int swscanf (const wchar_t *s, const wchar_t *format, ...) {
  return _wsfmt((fp_t)sungetwc, (fp2_t)sgetwc, &s, format, (va_list)&(format) + sizeof (format));
}

#include "scanf.c"

#endif

/**/
