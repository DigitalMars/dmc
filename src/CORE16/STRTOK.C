/*_ strtok.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<string.h>

#if __INTSIZE == 4 && (M_UNIX || M_XENIX || __OS2__ || __NT__)
#define __FAR
#else
#define __FAR __far
#endif

char *strtok(p1,p2)
char *p1;
const char *p2;
{
   if (p1 == NULL)
      return (char *)fstrtok (NULL, (const char __FAR *)p2);
   else   
      return (char *)fstrtok ((char __FAR *)p1, (const char __FAR *)p2);
}


