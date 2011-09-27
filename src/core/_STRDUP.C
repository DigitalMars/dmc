/*_ _strdup.c   */
/* Copyright (C) 1995	*/
/* All rights reserved. */


#include <string.h>

__declspec(naked) char * _strdup(const char *s)
{
    __asm {
       jmp strdup
    }
}


