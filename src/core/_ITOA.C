/*_ _itoa.c   */
/* Copyright (C) 1995	*/
/* All rights reserved. */


#include <stdlib.h>


__declspec(naked) char * _itoa(int i, char * s,int j)
{
    __asm {
      jmp itoa
    }
}

