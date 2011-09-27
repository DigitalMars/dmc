/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved				*/
/* January 25, 1994  */
/* $Revision: 1.1.1.1 $ */

#include <string.h>

__declspec(naked) int _memicmp(const void *m1,const void *m2,size_t l)
{
    __asm {
			jmp memicmp
    }
}

