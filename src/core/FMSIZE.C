/* fmsize.c   Thu Jul 22 1993                            */
/* Copyright (C) 1986-1993 Digital Mars		*/
/* All Rights Reserved					*/
/* $Revision: 1.1.1.1 $ */

#include <stdlib.h>
#include <dos.h>
size_t _dos_blocksize (unsigned);

/*********************************************************
 * Return size of allocated memory block
 */
 

#if __INTSIZE == 2 && (__SMALL__ || __MEDIUM__)
   size_t _fmsize (void __far *memblock)
   {
      unsigned para_size;
      
      para_size = _dos_blocksize(FP_SEG(memblock));
      if (para_size >= 0x1000) // can happen in WIN due to rounding up of siz
         return 0xFFFF;        // better than returning 0!
      else
         return para_size << 4;   
   }
#else
   size_t _fmsize (void __far *memblock)
   {
      return ((size_t *)memblock)[-1] - sizeof(size_t);
   }
#endif

