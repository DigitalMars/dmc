/*_ unlink.c 
 * Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 */

#include <tchar.h>
#include <sctchar.h>
#include <windows.h>
#include <stdio.h>
#include <scrtl.h>
#include <lcapi32.h>

/*****************************
 * Delete a file.
 * Returns:
 *	-1	error
 *	0	successful
 */
int _tunlink(const _TCHAR *name)
{
    if (__tDeleteFile ((LPTSTR)name))
	    return 0;
    __set_nt_errno ();
    return -1;
}

