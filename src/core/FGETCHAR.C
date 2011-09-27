/*_ fgetchar.c    */
/* Copyright (C) 1994 Digital Mars	*/
/* All Rights Reserved					*/

#include <stdio.h>

int fgetchar(void)
{
   return fgetc(stdin);
}
