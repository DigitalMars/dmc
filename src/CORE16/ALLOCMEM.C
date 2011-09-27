/*_ allocmem.c   Mon Jul 26 1993 */
/* Copyright (C) 1985-1991 by Digital Mars */
/* All Rights Reserved				*/
/* $Revision: 1.1.1.1 $ */

#include <dos.h>

int allocmem(unsigned size, unsigned *segp)
{
   unsigned sg, rc;
   
   sg = *segp;
   rc = _dos_allocmem(size, &sg);
   if (rc == 0) 
   {
      *segp = sg;
      return -1;
   }
   return sg;
}

int freemem(unsigned segx)
{
   if (_dos_freemem(segx) == 0)
      return 0;
   else
      return -1;   
}
