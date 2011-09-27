/* freect.c   Thu Jul 22 1993                           */
/* Copyright (C) 1986-1993 Digital Mars		*/
/* All Rights Reserved					*/
/* $Revision: 1.1.1.1 $ */


/*********************************************************
 * Return number of times an item can be malloc'd
 */

#if __SMALL__ || __MEDIUM__ || _WINDOWS
#include <stdlib.h>

unsigned _freect(size_t sz)
{
   return _memavl() / sz;
}

#endif
