/*_ atexit.c   Sat Sep  1 1990   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars */
/* All Rights Reserved                         */
/* $Revision: 1.1.1.1 $ */

#include <stdlib.h>
#include <scrtl.h>
#if (__SMALL__ || __COMPACT__) && !(M_UNIX || M_XENIX || __NT__ || (__OS2__ && __INTSIZE == 4))
#  include <dos.h>
#endif

int atexit(void (*func)(void))
{
# if __INTSIZE == 2 && (__SMALL__ || __COMPACT__)
    return _fatexit ((void (__cdecl _32BIT *)(void))_MK_FP (0, func));
# else         
    return _fatexit (func);
# endif    
}
