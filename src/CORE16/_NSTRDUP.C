/*_ _nstrdup.c	24 Sep 1994   */
/* Copyright (C) 1994 by Digital Mars	*/
/* All Rights Reserved							*/

#include <string.h>
#include <stdlib.h>


#define DOSLARGE ((__LARGE__ || __COMPACT__) && (!_WINDOWS))



static char __near * __near fartonearcpy(char __near *, const char __far *);


/*****
 *  fartonearcpy takes a far string pointer and copies its
 * contents to a near string pointer. 
 */ 
static char __near * __near fartonearcpy(char __near *dest, const char __far *src)
{

   while ((*dest++ = *src++) != 0) 
      ;
}


char __near * __far _nstrdup(const char __far *p)
{
   char __near *pnew=0L;

#if !DOSLARGE
	pnew = _nmalloc(fstrlen(p) + 1);
	if (pnew)
		fartonearcpy(pnew,p);
#endif
	return pnew;
}


   


   

