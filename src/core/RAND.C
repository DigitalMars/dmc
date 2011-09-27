/*_ rand.c   Thu Feb 25 1988   Modified by: Walter Bright */
/* ANSI Standard random number generator	*/
/* Copyright (C) 1985-1994 by Digital Mars */
/* $Revision: 1.1.1.1 $ */

#include	<stdlib.h>
#include	<time.h>

static unsigned long int next = 1;
#define step_next (next = next * 1103515245 + 12345)

int __CLIB rand(void)
{
	step_next;
	return ((unsigned int) (next / 0x10000) & 0x7FFF);
}

int __CLIB random (int num) // negative arguments treated
		            // as unsigned
{
        step_next;
	
	return ((long) (next / 0x10000) * (unsigned)num) >>
	        16;
}

void __CLIB srand(unsigned int seed)
{
	next = seed;
}

void __CLIB randomize (void)
{
	next = (unsigned long)time(NULL);
}

