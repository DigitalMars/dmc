/*_ _new.c   Tue May 30 1989   Modified by: Walter Bright */
/* Copyright (C) 1988-1989 by Northwest Software	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/

#include	<stdlib.h>
#include	<new.h>

/********************
 * _new_handler can be modified to point to a function in case
 * the allocation fails. _new_handler can attempt to repair things.
 */

extern _PNH _new_handler;
extern int __new_handler_type;
#define ANSI_COMPAT 1
#define MS_COMPAT 2

//
// Near versions of new and delete for use in large model.  They are only
// implemented for windows.
//

#if _WINDOWS
#if __LARGE__

void __near * operator new(size_t size)
{
    if (size == 0)
        size++;
    return _nmalloc(size);
}

#endif
#endif

