/* _fstrtok.c */
/* Copyright (C) 1995 by Digital Mars */
/* All Rights Reserved */

#include <string.h>

__declspec(naked) char __far * __far _fstrtok(char __far *s1,const char __far *s2)
{
    __asm {
       jmp fstrtok
    }
}

