/*_ _fgtchar.c    */
/* Copyright (C) 1995 Digital Mars	*/
/* All Rights Reserved					*/

#include <stdio.h>

int _fgetchar(void)
{
  return fgetc(stdin);
}


