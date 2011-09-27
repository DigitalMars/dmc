/*_ find.c	 Modified by: Walter Bright */
/*
 * Copyright (C) 1991-1992 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include	<tchar.h>
#include	<sctchar.h>
#include	<dos.h>
#include	<errno.h>
#include	<string.h>
#include	<scrtl.h>

#include <windows.h>
#include <lcapi32.h>

static HANDLE FindHandle = INVALID_HANDLE_VALUE;
static struct FIND dosfind;
static WIN32_FIND_DATA ntfind;
static int __attribute;

static BOOL get_another ()
{
  if (__tFindNextFile (FindHandle, &ntfind))
    return 1;
  FindClose (FindHandle);
  FindHandle = INVALID_HANDLE_VALUE;
  __set_nt_errno ();
  return 0;
}

/* Believe it or not, this is the algorithm DOS uses to find files */

static struct FIND *find_tail ()
{
  int subdir, hidsys;
  
  subdir = __attribute & _A_SUBDIR;
  hidsys = __attribute & (_A_HIDDEN | _A_SYSTEM);
  while (!subdir && (ntfind.dwFileAttributes & _A_SUBDIR)
    || hidsys != (ntfind.dwFileAttributes & (_A_HIDDEN | _A_SYSTEM)))
  {
    if (!get_another ())
      return NULL;
  }
  FileTimeToDosDateTime (&ntfind.ftLastWriteTime, &dosfind.date, &dosfind.time);
  dosfind.attribute = (unsigned short)ntfind.dwFileAttributes;
  dosfind.size = ntfind.nFileSizeLow;
#if 0
  ntfind.cFileName[sizeof (dosfind.name) - 1] = 0;
#else
  ntfind.cFileName[sizeof (ntfind.cFileName) / sizeof (_TCHAR) - 1] = 0;
#endif
  _tcscpy(dosfind.name, ntfind.cFileName);
  return &dosfind;
}

struct FIND *__tfindfirst (const _TCHAR *name, int attribute)
{
  __attribute = attribute;
  if (FindHandle != INVALID_HANDLE_VALUE)
	  FindClose (FindHandle);
  FindHandle = __tFindFirstFile ((LPTSTR)name, &ntfind);
  if (FindHandle == INVALID_HANDLE_VALUE)
  {
    __set_nt_errno ();
    return NULL;
  }
  return find_tail ();
}

struct FIND *__tfindnext()
{
  if (!get_another ())
    return NULL;
  return find_tail ();
}
