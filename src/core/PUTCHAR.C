/*_ putchar.c  Monday June 6, 1994 */
/* Copyright (C) 1994 by Digital Mars	*/
/* All Rights Reserved				*/
/* $Revision: 1.1.1.1 $ */


#include <stdio.h>

/* Function implementation of putchar.c */ 

#undef putchar
int putchar(int c) { return putc(c,stdout); }


