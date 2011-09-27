/* Copyright (C) 1993 by Digital Mars		*/
/* All Rights Reserved				*/
/* January 5, 1993	*/

#include <dos.h>
#include <errno.h>
#include <scrtl.h>

unsigned _dos_getdiskfree(unsigned driveno, struct diskfree_t *dinfo)
{
#if __INTSIZE == 4
	__asm
	{
			mov AH,36h
			mov DL,byte ptr driveno
			int 21h
			cmp AX,-1
			jz	err
			mov ESI,dinfo
			mov [ESI],EDX		; clusters per drive
			mov 4[ESI],EBX		; number of available clusters
			mov 0Ch[ESI],ECX		; bytes per sector
			mov 8[ESI],EAX   ; sectors per cluster
			jmp done
			

	}
#else
	__asm
	{
			mov AH,36h

			mov DL,byte ptr driveno
			int 21h
			cmp AX,-1
			jz		err
#if __LARGE__ || __COMPACT__
			push	DS
			lds SI,dinfo
#else
			mov SI,dinfo
#endif
			mov [SI],DX			; clusters per drive
			mov 2[SI],BX		; number of available clusters
			mov 4[SI],AX		; sectors per sector
			mov 6[SI],CX	    ; bytes per cluster
#if __LARGE__ || __COMPACT__
			pop DS
#endif
			jmp done
			

	}
#endif
err:
      __set_errno(EINVAL);
      return -1;
done:
      return 0;     
}



