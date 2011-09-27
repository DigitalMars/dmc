// fmalloc.c
// Copyright (C) 1985-1993 by Digital Mars
// All rights reserved
// $Revision: 1.1.1.1 $

// DOS386 far malloc routines

#include <stdlib.h>

void __far * __cdecl _fmalloc(size_t numbytes)
{
   void *memptr;
   
   memptr = _nmalloc (numbytes);
   if (memptr == NULL)
      return NULL;
   else
      return (void __far *)memptr;   
}

void __cdecl _ffree(void __far *p)
{
   _nfree ((void __near *)p);
}
