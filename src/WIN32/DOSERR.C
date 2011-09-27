/*_ doserr.c   Modified by: Walter Bright */
/*
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include	<dos.h>
#include	<errno.h>

#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <windows.h>
#endif



/**************************************
 * Get dos extended error information.
 * OS/2 version assumes _errno contains
 * the OS/2 error code
 */

int dosexterr(struct DOSERROR *err) { return dos_exterr(err); }

int dos_exterr(struct DOSERROR *err)
{
#ifdef	__OS2__
    int errcode = errno;
    unsigned eclass,eaction,elocus;

    if (err)
    {
	DosErrClass(errcode,&eclass,&eaction,&elocus);
	err->exterror = errcode;
	err->eclass = eclass;
	err->action = eaction;
	err->locus = elocus;
    }
    return errcode;
#endif
#ifdef	__NT__
    if (err)
    {
	err->exterror = GetLastError ();
	err->eclass = 0;
	err->action = 0;
	err->locus = 0;
    }
    return errno;
#endif
}
