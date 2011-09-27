// fcalloc.c
// Copyright (C) 1985-1993 by Digital Mars
// All rights reserved
// $Revision: 1.1.1.1 $

// DOS386 far malloc routines

#include <stdlib.h>

void __far * __cdecl _fcalloc(size_t num, size_t size)
{
   void *memptr;
   
   memptr = _ncalloc (num, size);
   if (memptr == NULL)
      return NULL;
   else
      return (void __far *)memptr;
}
