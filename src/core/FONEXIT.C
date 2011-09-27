/*_ fonexit.c   Sat Sep	1 1990	 Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars */
/* All Rights Reserved				*/
/* $Revision: 1.1.1.1 $ */

#include	<stdlib.h>

_fonexit_t _fonexit(_fonexit_t func)
{
  if (_fatexit (func) == 0)
    return func;
  return NULL;
}
