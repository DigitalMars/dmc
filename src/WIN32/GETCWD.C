/*_ getcwd.c   Modified by: Walter Bright
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<direct.h>
#include	<errno.h>
#include	<scrtl.h>


#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <tchar.h>
#include <sctchar.h>
#include <windows.h>
#include <lcapi32.h>
#endif


/********************************
 * Get current working directory.
 * Input:
 *	pbuf	where to store the pathname
 *	n	maximum length of pathname including terminating 0
 * Returns:
 *	if successful
 *		pbuf
 *	else
 *		NULL
 *		errno = ENOMEM (out of memory), ERANGE (pathname longer
 *			than n-1 characters)
 */

#ifdef __OS2__

char *getcwd(char *pbuf,size_t Nbytes)
{
    unsigned Buflen;
    unsigned Drive;
    unsigned long Drmap;
    int result;
    char myalloc = 0;

    /* Determine number of bytes required to hold path	*/
    Buflen = 0;

#if __I86__ >= 3
    result = DosQueryCurrentDir(0,NULL,&Buflen);
#else
    result = DosQCurDir(0,NULL,&Buflen);
#endif
    if (result != 111)			/* ERROR_BUFFER_OVERFLOW	*/
	goto err;

    Buflen += 3;			/* for D:\			*/
    if (!pbuf)
    {
	/* malloc the larger of Buflen or Nbytes	*/
	myalloc = 1;
	if (Nbytes < Buflen)
	    Nbytes = Buflen;
	pbuf = malloc(Nbytes);
	if (!pbuf)
	{   result = ENOMEM;
	    goto err;
	}
    }
    else if (Buflen > Nbytes)		/* if insufficient buffer size	*/
    {	result = ERANGE;
	goto err;
    }

#if __I86__ >= 3
    result = DosQueryCurrentDisk(&Drive,&Drmap);
#else
    result = DosQCurDisk(&Drive,&Drmap); /* get current drive letter	*/
#endif
    if (result)
	goto err;
    pbuf[0] = Drive + 'A' - 1;
    pbuf[1] = ':';
    pbuf[2] = '\\';
    Buflen = Nbytes - 3;		/* leave room for D:\		*/
#if __I86__ >= 3
    result = DosQueryCurrentDir(0,pbuf + 3,&Buflen);
#else
    result = DosQCurDir(0,pbuf + 3,&Buflen);
#endif
    if (result)
    {
     err:
	if (myalloc)
	    free(pbuf);
	__set_errno (result);
	pbuf = NULL;
    }
    return pbuf;
}

#elif __NT__

_TCHAR *_tgetcwd (_TCHAR *pbuf, size_t Nbytes)
{
  unsigned Buflen;
  unsigned Drive;
  char myalloc = 0;

    /* Determine number of bytes required to hold path	*/
  Buflen = __tGetCurrentDirectory (0, (LPTSTR)&Drive);	/* use "Drive" since not documented whether this can be NULL */
  if (Buflen == 0)
    goto nt_err;
  Buflen++;						/* Add space for terminator */
  Buflen += 3;			/* for D:\			*/
  if (!pbuf)
  {
    if (Nbytes < Buflen)
      Nbytes = Buflen;
    pbuf = malloc (Nbytes * sizeof (_TCHAR));
    if (!pbuf)
    {
      __set_errno (ENOMEM);
      goto my_err;
    }
    myalloc = 1;
  }
  else if (Buflen > Nbytes)
  {
    __set_errno (ERANGE);
    goto my_err;
  }
  if (__tGetCurrentDirectory (Nbytes, (LPTSTR)pbuf) == 0)
    goto nt_err;
  return pbuf;
nt_err:
  __set_nt_errno ();
my_err:
  if (myalloc)
    free (pbuf);
  return NULL;
}

#endif
