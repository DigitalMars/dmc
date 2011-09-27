/* nmsize.c   Thu Jul 22 1993                            */
/* Copyright (C) 1986-1993 Digital Mars		*/
/* All Rights Reserved					*/
/* $Revision: 1.1.1.1 $ */

#include <stdlib.h>
#ifdef _WINDOWS
#include <windows.h>
#endif

/*********************************************************
 * Return size of allocated memory block
 */

   size_t _nmsize(void __near *memblock)
   {
#ifdef _WINDOWS 
      return (size_t)LocalSize((HANDLE)memblock);
#else
      return ((size_t *)memblock)[-1] - sizeof(size_t);
#endif
   }

