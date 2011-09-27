/*_ mallocf.c   Wed Aug 11 1993 */
/* Copyright (C) 1985-1991 by Digital Mars */
/* All Rights Reserved				*/
/* $Revision: 1.1.1.1 $ */

#include <stdlib.h>

void __far * __far __CLIB __mallocf(size_t size)
{
   return (void __far *)malloc(size);
}

void __far __CLIB __freef(void __far *ptr)
{
#if __SMALL__ || __MEDIUM__
   free ((void __near *)ptr);
#else
   free (ptr);
#endif
}
