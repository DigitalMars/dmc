
/* Copyright (C) 2003 by Digital Mars.
 * All Rights Reserved
 * www.digitalmars.com
 * written by Walter Bright
 */

#include <windows.h>
#include <io.h>
#include <errno.h>
#include <string.h>
#include <time.h>

static long FILETIME_to_time_t(FILETIME *ft)
{
    long long ll;

    ll = *(long long *)ft;
    return (time_t)((ll - 116444736000000000LL) / 10000000LL);
}

static void finddata_to_finddata_t(WIN32_FIND_DATA *fileinfo, struct _finddata_t *fd)
{
    fd->attrib = fileinfo->dwFileAttributes;
    fd->time_create = FILETIME_to_time_t(&fileinfo->ftCreationTime);
    fd->time_access = FILETIME_to_time_t(&fileinfo->ftLastAccessTime);
    fd->time_write =  FILETIME_to_time_t(&fileinfo->ftLastWriteTime);
    fd->size = fileinfo->nFileSizeLow;
    strcpy(fd->name, fileinfo->cFileName);
}

int _findfirst(const char *name, struct _finddata_t *fd)
{
    HANDLE h;
    WIN32_FIND_DATA fileinfo;

    h = FindFirstFile(name, &fileinfo);
    if (h != INVALID_HANDLE_VALUE)
    {
	finddata_to_finddata_t(&fileinfo, fd);
    }
    return (int)h;
}

int _findnext(int handle, struct _finddata_t *fd)
{
    WIN32_FIND_DATA fileinfo;

    if (FindNextFile((HANDLE)handle, &fileinfo) != FALSE)
    {
	finddata_to_finddata_t(&fileinfo, fd);
	return 0;
    }
    else
    {
	errno = ENOENT;
	return -1;
    }
}

int _findclose(int handle)
{
    if (FindClose((HANDLE)handle))
    {
	return 0;	// successful
    }
    else
    {
	errno = ENOENT;
	return -1;
    }
}


