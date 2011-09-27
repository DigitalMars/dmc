/* getdate.c	 Created 9/21/93		*/
/* Copyright (C) 1993 by Digital Mars	*/
/* All Rights Reserved				*/
/* Written by Richard S. Sadowsky		*/
/* getdate and gettime routines			*/

void getdate(struct date *dateP)
{
	asm {
		mov	ah,0x2A
		int	0x21
#if __COMPACT__ || __LARGE__
		les	bx,dateP
		mov	es:[bx],cx
		mov	es:[bx+2],dx
#elif DOS386
		mov	ebx,dateP
		mov	[ebx],cx
		mov	[ebx+4],dx
#else
		mov	bx,dateP
		mov	[bx],cx
		mov	[bx+2],dx
#endif
	}
}

void gettime(struct time *timeP)
{
	asm {
		mov	ah,0x2C
		int	0x21
#if __COMPACT__ || __LARGE__
		les	bx,timeP
		mov	es:[bx],cx
		mov	es:[bx+2],dx
#elif DOS386
		mov	ebx,timeP
		mov	[ebx],cx
		mov	[ebx+2],dx
#else
		mov	bx,timeP
		mov	[bx],cx
		mov	[bx+2],dx
#endif
	}
}
