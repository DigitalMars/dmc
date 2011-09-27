/*_ fputchar.c    */
/* Copyright (C) 1994 Digital Mars	*/
/* All Rights Reserved					*/

#include <stdio.h>

int fputchar(int c)
{
   return fputc(c,stdout);
}
