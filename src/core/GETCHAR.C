/*_ getchar.c  Monday June 6, 1994 */
/* Copyright (C) 1994 by Digital Mars	*/
/* All Rights Reserved				*/
/* $Revision: 1.1.1.1 $ */


#include <stdio.h>

/* Function implementation of getchar.c */ 

#undef getchar
int getchar(void) { return getc(stdin); }


