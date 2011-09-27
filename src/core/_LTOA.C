/*_ _ltoa.c   */
/* Copyright (C) 1995	*/
/* All rights reserved. */


#include <stdlib.h>


__declspec(naked) char * _ltoa(long l, char * s,int j)
{
    __asm {
      jmp ltoa
    }
}

