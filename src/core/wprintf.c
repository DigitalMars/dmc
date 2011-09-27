//_ wprintf.c
// Copyright (C) 1995-2003 by Digital Mars, www.digitalmars.com
// All Rights Reserved

#ifdef _WIN32

#define UNICODE		1	/* Enable Windows Unicode API	*/
#define _UNICODE	1	/* Enable C Unicode Runtime	*/
#ifdef  _MBCS
#undef  _MBCS			/* Disable MBCS code		*/
#endif

#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <scio.h>
#include <dos.h>
#include <sctchar.h>

# if __SMALL__ || __MEDIUM__
#  if __INTSIZE == 4
#   if M_UNIX || M_XENIX || __NT__ || __OS2__
#define ALWAYS_NEAR_DATA 1
#define FAR
#define FSTRLEN _tcslen
#   else
#define DFT_NEAR_DATA 1
#define FAR __far
#define FSTRLEN _tsclen
#   endif
#  else
#define DFT_NEAR_DATA 1
#define FAR __far
#define FSTRLEN _tcslen
#  endif
# else
#define DFT_FAR_DATA 1
#define FAR __far
#define FSTRLEN _tcslen
# endif

int fwprintf (FILE *fp, const wchar_t *format, ...) {
  return vfwprintf (fp, format, (va_list)&(format) + sizeof (format));
}

int vfwprintf (FILE *fp, const wchar_t *format, va_list args) {
 int status;
  __fp_lock(fp);
  status = _wpformat ((fp_t)_fputwc_nlock, fp, format, args);
  __fp_unlock(fp);
  return status;
}

static int __cdecl __swprintf (wint_t c, void *s) {
  *(*(wchar_t **)s)++ = c;
  return c & 0xFFFF;
}


/*************************
 * Return number of chars written in array, excluding trailing 0.
 */

int _swprintf(wchar_t *s,const wchar_t *format,...)
{
	return _vswprintf(s,format,(va_list)&(format) + sizeof(format));
}

int _vswprintf(wchar_t *s,const wchar_t *format,va_list args)
{	int i;
	wchar_t *sstart = s;

	if ((i = _wpformat((fp_t)__swprintf,&s,format,args)) < 0)
		i = 0;		/* an error occurred		*/
	sstart[i] = 0;
	return i;
}

/***************
 * C99 7.24.2.3
 */

int swprintf (wchar_t *s, size_t n, const wchar_t *format, ...)
{
    return _vsnwprintf (s, n, format, (va_list)&(format) + sizeof (format));
}

/***************
 * C99 7.24.2.7
 */

int vswprintf (wchar_t *s, size_t n, const wchar_t *format, va_list args)
{
    return _vsnwprintf(s, n, format, args);
}

struct WVSN { wchar_t *s; size_t count; };

static wint_t __cdecl __snwprintf (wint_t c, void *pvsn)
{
    if (((struct WVSN *)pvsn)->count)
    {
	((struct WVSN *)pvsn)->count--;
	*(((struct WVSN *)pvsn)->s)++ = c;
	return c & 0xFFFF;
    }
    return WEOF;
}

int __CLIB _snwprintf (wchar_t *s, size_t count, const wchar_t *format, ...) {
  return _vsnwprintf (s, count, format, (va_list)&(format) + sizeof (format));
}

int __CLIB _vsnwprintf (wchar_t *s, size_t count, const wchar_t *format, va_list args)
{
    int i;
    struct WVSN vsn;
    vsn.s = s;
    vsn.count = count;

    if ((i = _wpformat ((fp_t)__snwprintf, &vsn, format, args)) < 0)
    {
	i = 0;		/* an error occurred */
    }
    if (vsn.count == 0)	/* if no space for terminating 0 */
    {
	if (count == 0 || i != count)
	{
	    i = -1;
	}
    }
    else
    {
	s[i] = 0;
    }
    return i;
}

#include "printf.c"

#endif

/**/
