/*_ _fmemmve.c				*/
/* Copyright (C) 1986-1994 by Digital Mars	*/
/* All Rights Reserved			*/
/* $Revision: 1.1.1.1 $ 			*/


#include <string.h>


__declspec(naked) void __far * __far _fmemmove(void __far *p1,const void __far *p2,size_t s)
{
   __asm {
     jmp fmemmove
   }
}
