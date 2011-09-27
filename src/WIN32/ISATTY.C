/*_ isatty.c
 * Copyright (C) 1991 by Digital Mars
 * All Rights Reserved
 * Written by Walter Bright
 */
 
#include <scio.h> 

#ifdef __OS2__
#include <os2lib.h>
#endif
#ifdef __NT__
#include <windows.h>
#endif

/********************************
 * Determine if handle is a tty.
 * Returns:
 *	!=0	character device
 *	0	not a character device or error
 */

int isatty(int fd)
{
#if __NT__
    return (GetFileType (_osfhnd[fd]) == FILE_TYPE_CHAR);
#else
    unsigned info;
    unsigned attribute;
    int result;

#if __I86__ >= 3
    result = DosQueryHType(fd,&info,&attribute);
#else
    result = DosQHandType(fd,&info,&attribute);
#endif
    if (result)
    {	__set_errno (result);
	result = 0;
    }
    else
    {
	if (info & 0xFF)
	    result++;
    }
    return result;
#endif
}
