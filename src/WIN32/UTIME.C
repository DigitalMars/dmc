/*_ time2.c   Modified by: Walter Bright
 * Copyright (C) 1991 by Walter Bright
 * All Rights Reserved
 * Written by Walter Bright
 */

#include <tchar.h>
#include <sctchar.h>
#include <stddef.h>
#include <time.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <scrtl.h>

#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <windows.h>
#include <lcapi32.h>
#endif

#ifndef _UNICODE

# if __NT__

void __UnixToSystemTime(time_t *pt,SYSTEMTIME *pst)
{
	struct tm *bd;

	bd = localtime(pt);
	pst->wYear		= bd->tm_year + 1900;
	pst->wMonth		= bd->tm_mon + 1;
	pst->wDayOfWeek = bd->tm_wday;
	pst->wDay		= bd->tm_mday;
	pst->wHour		= bd->tm_hour;
	pst->wMinute	= bd->tm_min;
	pst->wSecond	= bd->tm_sec;
	pst->wMilliseconds	= 0;
}

# endif

#else

# if __NT__

void __UnixToSystemTime (time_t *pt,SYSTEMTIME *pst);

# endif

#endif

int _tutime(const _TCHAR *filespec,time_t timep[2])
{
	int result;
#if __NT__
	FILETIME laft,lmft;
	SYSTEMTIME last,lmst;
	HANDLE h;
	time_t lat,lmt;

	if (timep)
	{	lat = timep[0];			/* get 'last accessed' time */
	lmt = timep[1];			/* get 'last modified' time */
	}
	else
	lat = lmt = time(NULL);		/* default to current time	*/

	/* Convert Unix time to NT system time	*/
	__UnixToSystemTime(&lat,&last);
	__UnixToSystemTime(&lmt,&lmst);

	h = __tCreateFile((LPTSTR)filespec,GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if (h == (HANDLE)-1)
	goto err;
	if (!SystemTimeToFileTime (&last, &laft) ||
	!SystemTimeToFileTime (&lmst, &lmft) ||
		!SetFileTime (h, NULL, &laft, &lmft))
	goto err;
	if (!CloseHandle(h))
	{
err:
        __set_nt_errno ();
	if (h != (HANDLE)-1)
		CloseHandle(h);
	result = -1;
	}
	else
	return 0;
	return result;
#else
	int fd;
	struct tm *bd;
	FileInfoBuf fileinfo;
	time_t t;
	unsigned short date;
	unsigned short tm;

	if (timep)
	t = timep[1];		/* get 'last modified' time */
	else
	t = time(NULL);

	bd = localtime(&t);

	date = ((((bd->tm_year - 80) << 4) + bd->tm_mon + 1) << 5) + bd->tm_mday;
	tm = (((bd->tm_hour << 6) + bd->tm_min) << 5) + (bd->tm_sec >> 1);

	fd = open(filespec,O_RDWR);
	if (fd == -1)			/* -1 = error (errno already set) */
	{	result = -1;
	goto L1;
	}

	/* OS2 1.1 demands that the rest of the fields be 0 */
	fileinfo.wdate = date;
	fileinfo.wtime = tm;
	fileinfo.cdate = 0;
	fileinfo.ctime = 0;
	fileinfo.adate = 0;
	fileinfo.atime = 0;

	result = DosSetFileInfo(fd,1,&fileinfo,12);
	if (result)
	{
	__set_errno (result);
	close(fd);
	result = -1;
	}
	else
	{
	result = close(fd);
	if (result != -1)
		result = 0;
	}
L1:
	return result;
#endif
}
