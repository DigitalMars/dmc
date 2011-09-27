/*_ _fcvt.c   */
/* Copyright (C) 1995	*/
/* All rights reserved. */


#include <stdlib.h>


__declspec(naked) char * _fcvt(double d,int i1,int *ip1,int *ip2) 
{
  __asm {
     jmp fcvt
  }
}
