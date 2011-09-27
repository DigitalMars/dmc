/* _exit.c  
 * Copyright (C) 1985-2004 Digital Mars
 * www.digitalmars.com
 * All rights reserved.
 * Exit process
 */

#include <stdlib.h>

#ifdef __OS2__
#include <os2lib.h>
#endif
#ifdef _WIN32
#include <windows.h>
//extern int __stdcall ExitProcess(unsigned);
#endif

#ifdef DOS386
__declspec(naked) void __CLIB _Exit(int exitcode)
{
    _asm
    {
	mov	AL,4[ESP]
	mov	AH,0x4C
	int	0x21
	hlt
    }
}
#endif

#ifdef __OS2__
void __CLIB _Exit(int exitcode)
{
    DosExit(exitcode, 1);	// 1 = terminate all threads in this process
    _asm
    {
	hlt
    }
}
#endif

#ifdef _WIN32
void __CLIB _Exit(int exitcode)
{
    ExitProcess(exitcode);

/*
    _asm
    {
	hlt
    }
*/
}
#endif
