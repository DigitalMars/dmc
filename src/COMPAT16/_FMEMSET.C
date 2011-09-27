/*_ _fmemset.c   Tues. May 24 1994	 */
/* Copyright (C) 1985-1994 by Digital Mars */
/* All Rights Reserved				*/

#include <string.h>

#ifdef _fmemset
#undef _fmemset
#endif

void __far * __far _fmemset(void __far *p,int c,size_t n)
{
      return fmemset(p,c,n);   // Simply call fmemset.
}

