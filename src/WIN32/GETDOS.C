/*_ getdos.c   Modified by: Walter Bright
 * Copyright (C) 1991-2001 by Digital Mars
 * All rights reserved.
 * www.digitalmars.com
 * Written by Walter Bright
 */

#include <tchar.h>
#include <sctchar.h>
#include <stdio.h>
#include <dos.h>
#include <errno.h>
#include <direct.h>
#include <scrtl.h>

#ifdef __OS2__
# include "os2lib.h"
#endif
#ifdef __NT__
# include <scio.h>
# include <lcapi32.h>
#endif

#ifndef _UNICODE

void _flushterm(void);

unsigned char extchar;	/* extended character waiting	*/
static unsigned char char_is_waiting = 0;
static unsigned char waiting_char = 0;


/******************************
 * Return 1 if key is available from console, else 0
 */

int kbhit()
{
# if __NT__
	DWORD num;
	INPUT_RECORD irBuffer;

	if (char_is_waiting)
	   return 1;
	if (extchar)
	   return 1;
    L1:
	if (!PeekConsoleInput(__conin, &irBuffer, 1, &num) || num == 0)
	    return 0;
	if (irBuffer.EventType == KEY_EVENT)
	{
	    if (!irBuffer.Event.KeyEvent.bKeyDown)
		goto Lignore;
	    if (!(irBuffer.Event.KeyEvent.uChar.UnicodeChar))
		goto Lignore;
	    return 1;
	}

    Lignore:
	ReadConsoleInput(__conin, &irBuffer, 1, &num);
	goto L1;
# elif __OS2__
#  if __I86__
	unsigned long openmode;
	int result;

	if (char_is_waiting)
	   return 1;
	//result = DosQueryFHState(_confh,&openmode);
	return (result == 0 && openmode & 0x200000) ? 0 : -1;
#  else
	KbdCharData chardata;
	int flag;

	if (char_is_waiting)
	   return 1;
	if (extchar)
	flag = 1;
	else
	{
	flag = KbdPeek(&chardata,0);
	if (flag)
		flag = 0;
	else
	{
		if (chardata.status & 0xC0)
		flag = 1;
		else
		flag = 0;
	}
	}
	return flag;
#  endif
# else
	*** unknown
# endif
}

/******************************
 * Get character from keyboard, don't echo it.
 */

int getch()
{
	int c;

# if __NT__
	DWORD num;
	DWORD savemode;

	if (char_is_waiting)
	{
	   char_is_waiting = 0;
	   return waiting_char;
	}
	c = extchar;
	if (c)
	    extchar = 0;
	else
	{
	    /* This is not really correct since ReadConsole will not
	       return function keys etc. so we should use
	       ReadConsoleInput.
	     */

	    GetConsoleMode (__conin, &savemode);
	    SetConsoleMode (__conin, 0);
	    if (!ReadConsole (__conin, &c, 1, &num, NULL))
		 c = -1;
	    SetConsoleMode (__conin, savemode);
	}
# elif __OS2__

#  if __I86__ >= 3
	/* not implemented */
#  else
	if (char_is_waiting)
	{
	   char_is_waiting = 0;
	   return waiting_char;
	}
	c = extchar;
	if (c)
	extchar = 0;
	else
	{	KbdCharData chardata;

	c = KbdCharIn(&chardata,0,0);
	if (c)
		c = -1;		/* error		*/
	else
	{
		c = chardata.charcode;
		/* If extended character or extra cursor pad	*/
		if (c == 0 || c == 224)
		extchar = chardata.scancode;
	}
	}
#  endif
# endif
	return c;
}

/******************************
 * Get character from keyboard, echo it.
 */

int getche()
{	int c;

# if __NT__
	DWORD num;
	DWORD cmode, savemode;

	_flushterm();
	if (char_is_waiting)
	{
	   char_is_waiting = 0;
	   return waiting_char;
	}
	GetConsoleMode (__conin, &savemode);
	cmode = ENABLE_ECHO_INPUT;
	SetConsoleMode (__conin, cmode);

	if (!ReadConsole (__conin, &c, 1, &num, NULL))
	c = -1;
	SetConsoleMode (__conin, savemode);

# elif __OS2__
	_flushterm();
	if (char_is_waiting)
	{
	   char_is_waiting = 0;
	   return waiting_char;
	}
	c = getch();
	if (c >= 0)			/* If no error reading keyboard		*/
	{
#  if __I86__ >= 3
	/* not implemented */
#  else
	VioWrtTTY(&c,1,0);
#  endif
	c &= 0xFF;
	}
# endif
	return c;
}

/******************************
 * Push one character back into console queue
 */

int ungetch (int c)
{
   if (char_is_waiting || c == -1) return -1;
   char_is_waiting = 1;
   waiting_char = (char)c;
   return c;
}

/******************************
 * Set/reset verify switch for disk writes.
 * Input:
 *	1	turn on verify
 *	0	turn off verify
 */

void dos_set_verify(int flag)
{
# ifndef __NT__
	DosSetVerify(flag);
# endif
}

/******************************
 * Get disk verify state.
 * Returns:
 *	1 if disk verify is on, 0 if it is off
 */

int dos_get_verify()
{	unsigned flag;

# if __NT__
	flag = 1;
# elif __OS2__
#  if __I86__ >= 3
	DosQueryVerify(&flag);
#  else
	DosQVerify(&flag);
#  endif
# endif
	return flag;
}

/******************************
 * Set/reset Ctrl-Break check.
 * Input:
 *	1	turn on ctrl-break check
 *	0	turn it off
 */

void dos_set_ctrl_break(int flag)
{
	/* Can't turn it off under OS/2 & NT    */
}

/******************************
 * Get state of Ctrl-Break checking.
 * Returns:
 *	1	on
 *	0	off
 */

int dos_get_ctrl_break()
{
	return 1;		/* always on under OS/2 & NT	*/
}

# if __NT__
unsigned _dos_getdiskfree(unsigned driveno, struct diskfree_t *dinfo)
{
	static char drv[] = "?:\\";
	DWORD secsPerCluster, bytesPerSector, availClusters, totalClusters;

	drv[0] = (char)(driveno + 'A' - 1);
	if (GetDiskFreeSpace ((driveno ? drv : NULL), &secsPerCluster, &bytesPerSector, &availClusters, &totalClusters))
	{
		dinfo->total_clusters = totalClusters;
		dinfo->avail_clusters = availClusters;
		dinfo->sectors_per_cluster = secsPerCluster;
		dinfo->bytes_per_sector = bytesPerSector;
		return 0;
	}
	else
	{
		__set_errno (EINVAL);
		return 1;
	}
}
# endif

/******************************
 * Get disk free space in bytes.
 * Input:
 *	drive == 0: default drive
 *	drive == 1: drive = A:, etc.
 * Returns:
 *	a long
 *	-1L if drive was invalid
 */

long dos_getdiskfreespace(int drive)
{
	int result;
	long space;

# if __NT__
	static char drv[] = "?:\\";
	DWORD secsPerCluster, bytesPerSector, availClusters, totalClusters;

	drv[0] = (char)(drive + 'A' - 1);
	if (GetDiskFreeSpace ((drive ? drv : NULL), &secsPerCluster, &bytesPerSector, &availClusters, &totalClusters))
	space = secsPerCluster * availClusters * bytesPerSector;
	else
	space = -1;
# elif __OS2__
	FSInfoBuf fsinfo;

#  if __I86__ >= 3
	result = DosQueryFSInfo(drive,1,&fsinfo,sizeof(FSInfoBuf));
#  else
	result = DosQFSInfo(drive,1,&fsinfo,sizeof(FSInfoBuf));
#  endif

	if (result)
	space = -1;
	else
	space = fsinfo.SecsPerCluster * fsinfo.AvailClusters *
		fsinfo.BytesPerSector;
# endif
	return space;
}

/******************************
 Absolute disk read.
	drive		drive number 0=A, 1=B, ...
	numsec		number of sectors to write
	startsec	starting logical sector number
	buffer		where the result will be stored
 Returns:
	0		success
	!= 0		failure (the value is what is described in
			the DOS manual)
 */

int dos_abs_disk_read(int drive,int numsec,long startsec,char *buffer)
{	int result;

# if __NT__			/* Not available in NT */
	result = EDOM;

# elif __OS2__
	FSInfoBuf fsinfo;

#  if __I86__ >= 3
	result = DosQueryFSInfo(drive,1,&fsinfo,sizeof(FSInfoBuf));
#  else
	result = DosQFSInfo(drive,1,&fsinfo,sizeof(FSInfoBuf));
#  endif
	if (result == 0)
	{	char name[3];
	unsigned fd;
	unsigned Action;

	/* Open drive for absolute read
		No inherit, Return errors, Deny none, Read Only
	 */
	name[0] = drive + 'A';	/* drive letter */
	name[1] = ':';
	name[2] = 0;
	result = DosOpen(name,&fd,&Action,0L,0,1,0b01010000011000000,0L);
	if (result == 0)
	{	unsigned long pos,NewPos;

		pos = startsec * fsinfo.BytesPerSector;
#  if __I86__ >= 3
		result = DosSetFilePtr(fd,pos,0,&NewPos);
#  else
		result = DosChgFilePtr(fd,pos,0,&NewPos);
#  endif
		if (result == 0 && pos != NewPos)
		result = EDOM;		/* our error code	*/
		else if (result == 0)
		{	unsigned nbytes;
		unsigned nread;

		nbytes = numsec * fsinfo.BytesPerSector;
		result = DosRead(fd,buffer,nbytes,&nread);
		if (result == 0 && nbytes != nread)		/* if not all bytes read	*/
			result = EDOM;			/* our own error code	*/
		}
		DosClose(fd);
	}
	}
# else
	result = EDOM;
# endif
	return result;
}

/******************************
 * Absolute disk write.
 *	drive		drive number 0=A, 1=B, ...
 *	numsec		number of sectors to write
 *	startsec	starting logical sector number
 *	buffer		where the result will be read from
 * Returns:
 *	0		success
 *	!= 0		failure (the value is what is described in
 *			the DOS manual)
 */

int dos_abs_disk_write(int drive,int numsec,long startsec,const char *buffer)
{	int result;

# if __NT__			/* Not available in NT */
	result = EDOM;

# elif __OS2__
	FSInfoBuf fsinfo;

#  if __I86__ >= 3
	result = DosQueryFSInfo(drive,1,&fsinfo,sizeof(FSInfoBuf));
#  else
	result = DosQFSInfo(drive,1,&fsinfo,sizeof(FSInfoBuf));
#  endif
	if (result == 0)
	{	char name[3];
	unsigned fd;
	unsigned Action;

	/* Open drive for absolute write:
		No inherit, Return errors, Deny none, Write Only
	 */
	name[0] = drive + 'A';	/* drive letter */
	name[1] = ':';
	name[2] = 0;
	result = DosOpen(name,&fd,&Action,0L,0,1,0b01010000011000001,0L);
	if (result == 0)
	{	unsigned long pos,NewPos;

		pos = startsec * fsinfo.BytesPerSector;
#  if __I86__ >= 3
		result = DosSetFilePtr(fd,pos,0,&NewPos);
#  else
		result = DosChgFilePtr(fd,pos,0,&NewPos);
#  endif
		if (result == 0 && pos != NewPos)
		result = EDOM;		/* our error code	*/
		else if (result == 0)
		{	unsigned nbytes;
		unsigned nwritten;

		nbytes = numsec * fsinfo.BytesPerSector;
		result = DosWrite(fd,buffer,nbytes,&nwritten);
		if (result == 0 && nbytes != nwritten)	/* if not all bytes written */
			result = EDOM;			/* our own error code		*/
		}
		DosClose(fd);
	}
	}
# else
	result = EDOM;
# endif

	return result;
}

#endif

#ifndef _NOCHDIR

/************************
 * Change directory to that specified by pathname.
 * Returns:
 *	0	success
 *	-1	failure (errno is set to ENOENT)
 */

int _tchdir(const _TCHAR *pathname)
{	int result;

# if __NT__

	if (!__tSetCurrentDirectory ((LPTSTR)pathname))
	{
        __set_nt_errno ();
	result = -1;
	}
	else
	result = 0;
# elif __OS2__
#  if __I86__ >= 3
	result = DosSetCurrentDir(pathname);
#  else
	result = DosChDir(pathname,0L);
#  endif
	if (result)
	{	__set_errno (result);
	result = -1;
	}
# endif
	return result;
}

#endif

#ifndef _NOMKDIR

/************************
 * Create subdirectory given by pathname.
 * Returns:
 *	0	success
 *	-1	failure, in which case errno is set to:
 *		ENOENT: path not found
 *		EACCES: access denied
 */

int _tmkdir(const _TCHAR *pathname)
{	int result;

# if __NT__
	if (!__tCreateDirectory ((LPTSTR)pathname, NULL))
	{
        __set_nt_errno ();
	result = -1;
	}
	else
	result = 0;

# elif __OS2__

#  if __I86__ >= 3
	result = DosCreateDir(pathname,NULL);
#  else
	result = DosMkDir(pathname,0L);
#  endif
	if (result)
	{	__set_errno (result);
	result = -1;
	}
# endif
	return result;
}

#endif

#ifndef _NORDDIR

/************************
 * Remove subdirectory given by pathname.
 * Returns:
 *	0	success
 *	-1	failure, in which case errno is set to:
 *		ENOENT: path not found
 *		EACCES: access denied or directory is not empty
 */

int _trmdir(const _TCHAR *pathname)
{	int result;

# if __NT__
	if (!__tRemoveDirectory ((LPTSTR)pathname))
	{
        __set_nt_errno ();
	result = -1;
	}
	else
	result = 0;

# elif __OS2__

#  if __I86__ >= 3
	result = DosDeleteDir(pathname);
#  else
	result = DosRmDir(pathname,0L);
#  endif
	if (result)
	{	__set_errno (result);
	result = -1;
	}
# endif
	return result;
}

#endif

#ifndef _UNICODE

/************************
 * Returns:
 *	0	success
 *	-1	failure
 */

int _chdrive(int drive)
{	int result;
# if __NT__
	char dir[4];

	dir[0] = drive + 'A' - 1;
	dir[1] = ':';
	dir[2] = 0;
	result = SetCurrentDirectory (dir) ? 0 : -1;

# elif __OS2__
#  if __I86__ >= 3
	result = DosSetDefaultDisk(drive);
#  else
	result = DosSelectDisk(drive);
	if (result)
	result = -1;
#  endif
# endif
	return result;
}

/************************
 * Return default disk drive
 * 1 = A, 2 = B etc.
 */

int _getdrive()
{
	unsigned drive;

# if __NT__
	dos_getdrive (&drive);
# elif __OS2__
	unsigned long map;		/* drive bitmap		*/

#  if __I86__ >= 3
	DosQueryCurrentDisk(&drive,&map);
#  else
	DosQCurDisk(&drive,&map);
#  endif
# endif
	return drive;
}

#endif

