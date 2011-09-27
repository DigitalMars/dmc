/* _fstrchr.c */
/* Copyright (C) 1995 by Digital Mars */
/* All Rights Reserved */

#include <string.h>

__declspec(naked) char __far * __far _fstrchr(const char __far *s,int i)
{
   __asm {
     jmp fstrchr
   }
}

