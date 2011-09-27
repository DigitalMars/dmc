/*_ _tzset.c     */
/* Copyright (C) 1995 by Digital Mars  */
/* All Rights Reserved                                  */

#include <time.h>

#ifdef _tzset
#undef _tzset
#endif

__declspec(naked) void _tzset()
{
    __asm { 
        jmp tzset 
    }
}