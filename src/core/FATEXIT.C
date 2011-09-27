/*_ fatexit.c   Sat Sep 1 1990   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars */
/* All Rights Reserved                         */
/* $Revision: 1.1.1.1 $ */

#include <stdlib.h>
#include <scrtl.h>

/********************************
 * Register function to be called by exit().
 * The functions are called in the reverse order that they are registered.
 * A maximum of _atexit_max - 1 functions can be registered.
 * Returns:
 *     0       succeeds
 *     !=0     failed to register the function
 */

int _32BIT _fatexit (void (__cdecl _32BIT *func)(void))
{
    if (!_atexitp)
       _atexitp = _atexit_tbl;
    if (_atexitp >= _atexit_tbl + _atexit_max - 1)
       return 1;
/* Position 0 is left NULL as a sentinal for the end            */
    *++_atexitp = func;
    return 0;
}
