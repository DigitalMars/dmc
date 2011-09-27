/*_ chsize.c   Modified by: Bill Andrews
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include <tchar.h>
#include <sctchar.h>
#include <sys\stat.h>
#include <io.h>
#include <errno.h>
#include <scrtl.h>

#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef _WIN32
#include <windows.h>
#include <lcapi32.h>
extern void *_osfhnd[];
#endif

#ifndef _NOCHSIZE

/**********************************
 *  Truncate or extend an opened file
 *
 *  Arguments: 1 - file handle
 *       2 - Position for truncation
 *
 *  Returns: 0 if successful, else -1
 */

int chsize(int fd, long posn)
{   
    int result;

#ifdef __OS2__
#if __I86__ >= 3
    result = DosSetFileSize(fd,posn);
#else
    result = DosNewSize(fd,posn);
#endif
    if (result)
    { 
        __set_errno (result);
        result = -1;
    }
    return result;
#endif
#ifdef _WIN32
    if (SetFilePointer (_osfhnd[fd], posn, 0, FILE_BEGIN) != posn || !SetEndOfFile(_osfhnd[fd]))
    { 
        __set_nt_errno ();
        return -1;
    }
    return 0;
#endif
}

#endif

#ifndef _NOCHMOD

/**********************************
 * Change file access mode to that specified by mode.
 * Mode is one of the following values:
 *     S_IREAD   Set file to read-only.
 *     S_IWRITE  Set file to write-only.
 *     S_IREAD | S_IWRITE Set file to read or write.
 *     S_IREAD is ignored under DOS.
 * Return Value:
 *     0 means chmod was successful. -1 means it is not,
 *     and errno is set.
 * See Also:
 *     access, fstat, creat, stat, open
 */

int _tchmod(const _TCHAR *filename,int mode)
{   
    int result;

#if __OS2__
#if __I86__ >= 3
    FileInfoBuf fileinfo;

    /* get file info */
    result = DosQueryPathInfo(filename,1,&fileinfo,sizeof(FileInfoBuf));
    if (result)
        goto err;

    /* Set file info */
    fileinfo.attribute = ((mode & S_IWRITE) ? 0 : 1);
    result = DosSetPathInfo(filename,1,&fileinfo,sizeof(FileInfoBuf),0x10);
    if (result)
    {
    err:
        __set_errno (result);
    }
#else
    result = DosSetFileMode(filename,((mode & S_IWRITE) ? 0 : 1),0L);
    if (result)
    { 
        __set_errno (result);
        result = -1;
    }
#endif
#elif _WIN32
    DWORD attr;
    
    if ((attr = __tGetFileAttributes((LPTSTR)filename)) == 0xFFFFFFFF)
    {
        __set_nt_errno();
        result = -1;
        goto ret;
    }
    
    if (!__tSetFileAttributes ((LPTSTR)filename,
      ((mode & S_IWRITE) ? attr & ~FILE_ATTRIBUTE_READONLY : attr | FILE_ATTRIBUTE_READONLY)))
    { 
        __set_nt_errno ();
        result = -1;
    }
    else
        result = 0;

#endif
ret:
    return result;
}

#endif

