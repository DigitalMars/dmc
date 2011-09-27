/*_ _fptchar.c    */
/* Copyright (C) 1995 Digital Mars	*/
/* All Rights Reserved					*/

#include <stdio.h>

int _fputchar(int c)
{
  return fputc(c,stdout);
}


