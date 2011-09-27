/* Copyright (C) 1993 by Digital Mars*/
/* All Rights Reserved				      */

#include <io.h>
#include <dos.h>
#include <stdlib.h>
#include <errno.h>
#include <scrtl.h>

#if __NT__
#include	<windows.h>
#endif

#if __OS2__
#include	<os2lib.h>
#endif

int _commit(int handle)
{
    return _dos_commit(handle) == 0 ? 0 : -1;
}	  	

unsigned _dos_commit(int handle)
{
#if __NT__
	/* Does not appear to be an NT API function for this	*/
	return 0;			/* so assume success	*/


#elif __OS2__
	int result;

#if __INTSIZE == 4
	result = DosResetBuffer(handle);
#else
	result = DosBufReset(handle);
#endif
	if (result)
	    __set_errno (EBADF);		/* bad file handle	*/
	return result;


#else // MSDOS
#if __INTSIZE == 4
	if ((_osmajor << 8) + _osminor < 0x303)
	{
		_EAX = EINVAL;		/* function number invalid	*/
		goto F2;
	}
	__asm
	{	mov	AH,68h		;commit to file or device
		mov	EBX,handle	;EBX = file handle
		int21h						
		jnc	F1		;no error
	F2:
		push	EAX
		call    __set_dos_errno
		jmp	F3
	F1:	xor 	EAX,EAX		;no error return 0
	F3:
	}
#else
	if ((_osmajor << 8) + _osminor < 0x303)
	{
		_AX = EINVAL;		/* function number invalid	*/
		goto F2;
	}
	__asm
	{	mov	AH,68h		;commit to file or device
		mov	BX,handle	;BX = file handle
		int21h						
		jnc	F1		;no error
	F2:
		push	AX
		call    __set_dos_errno
		jmp	F3
	F1:	xor 	AX,AX		;no error return 0
	F3:
	}
#endif
#endif
}
