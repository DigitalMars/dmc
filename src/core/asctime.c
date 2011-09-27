/*_ asctime.c
 * Copyright (C) 1986-2003 by Digital Mars, www.digitalmars.com
 * All Rights Reserved
 * Written by Walter Bright
 */

#include        <tchar.h>
#include	<sctchar.h>
#include        <stdlib.h>
#include        <stdio.h>
#include        <time.h>
#include        <errno.h>
#include        <string.h>

#if _MT
#include        "mt.h"
#endif

#if M_UNIX || M_XENIX
#define DefaultTimeZone 0L
#else
#define DefaultTimeZone 8L
#endif

/******************************
 * C99 7.23.3.1
 * Convert broken-down time into a string of the form:
 *      Wed Apr 16 15:14:03 1986\n\0
 * Returns:
 *      A pointer to that string (statically allocated).
 *      The string is overwritten by each call to asctime() or ctime().
 */

static void __near __pascal decput(_TCHAR near *s,int n)
{
	*s = (n / 10) + _T('0');
	*(s + 1) = (n % 10) + _T('0');
}

static void __near __pascal decput1(_TCHAR near *s,int n)
{
	*s = (n /1000) + _T('0');
	n %= 1000;
	*(s+1) = (n /100) + _T('0');
	n %= 100;
	*(s+2) = (n /10) + _T('0');
	n %= 10;
	*(s+3) = n  + _T('0');
}


_TCHAR *_tasctime(const struct tm *ptm)
{
	static _TCHAR months[12*3+1] =
		_T("JanFebMarAprMayJunJulAugSepOctNovDec");
	static _TCHAR days[7*3+1] =
		_T("SunMonTueWedThuFriSat");

#if _MT
	_TCHAR *string;
	if (ptm == NULL)
		return NULL;
	string = _getthreaddata()->
# ifndef _UNICODE
		t_asctime;
# else
		t_wasctime;
# endif
	_stprintf(string,
		_T("%.3s %.3s%3d %.2d:%.2d:%.2d %d\n"),
		//_T("%.3s %.3s %02d %02d:%02d:%02d %4d\n"),
		days + ptm->tm_wday * 3,months + ptm->tm_mon * 3,ptm->tm_mday,
		ptm->tm_hour,ptm->tm_min,ptm->tm_sec,1900 + ptm->tm_year);
#else
	static _TCHAR string[26] = _T("ddd mmm dd hh:mm:ss yyyy\n");

	if (ptm == NULL)
		return NULL;
	memcpy(string,&days[ptm->tm_wday * 3],3 * sizeof (_TCHAR));
	memcpy(&string[4],&months[ptm->tm_mon * 3],3 * sizeof (_TCHAR));
	string[8] = (ptm->tm_mday >= 10) ?
			(ptm->tm_mday / 10) + _T('0') : _T('0');
	string[9] = (ptm->tm_mday % 10) + _T('0');
	decput(&string[11],ptm->tm_hour);
	decput(&string[14],ptm->tm_min);
	decput(&string[17],ptm->tm_sec);
	decput1(&string[20],ptm->tm_year+1900);
#endif
	return string;
}

