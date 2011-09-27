/* eof.c  Created 9/28/93			*/
/* Copyright (C) 1993 by Digital Mars	*/
/* All Rights Reserved				*/
/* Written by Richard S. Sadowsky		*/
/* eof() routine				*/
#include <io.h>
#include <errno.h>
#include <scrtl.h>

int eof(int handle)
{
	long currentPos, endPos;
	
	if (isatty (handle)) goto NotEOF;
	currentPos = lseek(handle, 0, SEEK_CUR);
	if (currentPos == -1L)
		goto IOError;
	endPos = lseek(handle, 0, SEEK_END);
	if (endPos == -1L)
		goto IOError;
	if (lseek(handle, currentPos, SEEK_SET) == -1L)
		goto IOError;
	if (endPos <= currentPos)
		return 1;
NotEOF:
	return 0;
IOError:
	__set_errno (EBADF);
	return -1;
}
