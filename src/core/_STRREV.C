/*_ _strrev.c   */
/* Copyright (C) 1995	*/
/* All rights reserved. */


#include <string.h>

__declspec(naked) char * _strrev(char *s)
{
    __asm {
       jmp strrev
    }
}


