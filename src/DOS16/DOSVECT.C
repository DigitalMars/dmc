/*_ dosvect.c   Sunday December 4, 1993 */
/* Copyright (C) 1993 by Digital Mars */
/* All Rights Reserved				*/
/* $Revision: 1.1.1.1 $ */

#ifndef _WINDOWS
#include <dos.h>

void (__interrupt __far * _dos_getvect(unsigned vector))()
{
   void (__interrupt __far * funcptr)();
   
   _asm mov AL,byte ptr vector
   _asm mov AH,0x35
   _asm int 0x21
   _asm mov word ptr funcptr,BX
   _asm mov word ptr funcptr+2,ES
   
   return funcptr;
}

void _dos_setvect(unsigned vector, void (__interrupt __far * funcptr)())
{
   _asm mov  AL,byte ptr vector
   _asm mov  AH,0x25
   _asm push DS
   _asm lds  DX,dword ptr funcptr
   _asm int  0x21
   _asm pop  DS
}
#endif
