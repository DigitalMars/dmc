/*_ _dup.c   */
/* Copyright (C) 1995	*/
/* All rights reserved. */

#include <io.h>

__declspec(naked) int _dup(int fd)
{
    __asm {
        jmp dup
    }
}


