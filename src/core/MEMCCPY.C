/*_ memccpy.c	 September 28, 1993 */
/* Copyright (C) 1985-1989 by Digital Mars	*/
/* All Rights Reserved					*/
/* $Revision: 1.1.1.1 $ */

#include	<string.h>

/**************************
 * Copy memory up to and including first occurance of a specified character.
 * Returns:
 *    if terminating character is copied, returns destination location of
 *       one beyond this character;
 *    otherwise, this function returns null.
 */

void *memccpy (void *dest,const void *src,int c,unsigned int count)
{
   unsigned char byte;
   
   while(1)
   {
      if (count == 0) goto RETURN_NULL;
      count --;
      byte = *((unsigned char *)src)++;
      *((unsigned char *)dest)++ = byte;
      if (byte == c) break;
   }
   return dest;
RETURN_NULL:   
   return NULL;
}

#if __INTSIZE == 2 && (__SMALL__ || __MEDIUM__)
void __far * __far fmemccpy (void __far *dest, const void __far *src,
		             int c, unsigned int count)
{
   unsigned char byte;
   
   while(1)
   {
      if (count == 0) goto RETURN_NULL;
      count --;
      byte = *((unsigned char __far *)src)++;
      *((unsigned char __far *)dest)++ = byte;
      if (byte == c) break;
   }
   return dest;
RETURN_NULL:   
   return NULL;
}
#elif __INTSIZE == 2 && (__COMPACT__ || __LARGE__)
void __far * __far fmemccpy (void __far *dest, const void __far *src,
		             int c, unsigned int count)
{
   return memccpy (dest, src, c, count);
}
#endif
