#include <iosold\iostream.h>

int _2Comp::div(unsigned char *body, int d, int bytes)
{
int result;
#if __INTSIZE == 4
    __asm {
		mov		esi,body

		mov		ebx,d
		mov		ecx,bytes

		add		esi,ecx
		dec		esi
		mov		edi,esi
		xor		dx,dx
		xor		ah,ah
		std

divloop:
		lodsb
		div		bl
		or		al,al
		jz		noquot
		inc		dx
noquot:
		stosb
		loop		divloop
		cld

		mov		al,dl
		xchg		ah,al
		cwde
		mov		result,eax
    }
		return(result);
#else
    __asm {
#if M_I86CM || M_I86LM || M_I86VM
		push		ds
		lds		si,body
#else	
		mov		si,body
#endif
		mov		bx,d
		mov		cx,bytes
		add		si,cx
		dec		si
		mov		di,si
		mov		ax,ds
		mov		es,ax
		xor		dx,dx
		xor		ah,ah
		std

divloop:
		lodsb
		div		bl
		or		al,al
		jz		noquot
		inc		dx
noquot:
		stosb
		loop		divloop
		cld

		mov		al,dl
		xchg		ah,al
		mov		result,ax
#if M_I86CM || M_I86LM || M_I86VM
		pop	   	ds
#endif
    }
		return(result);
#endif
}
