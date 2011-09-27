#include <iosold\iostream.h>

void _2Comp::negate(unsigned char *body, int bytes)
{
#if __INTSIZE == 4
    __asm {
	cld
        mov     esi,body

        mov     ecx,bytes
        mov     edi,esi
	stc
negloop:
        lodsb
		not		al
        adc     al,0
        stosb
        loop    negloop
    }
#else
    __asm {
#if M_I86CM || M_I86LM || M_I86VM
        mov     bx,ds
        lds     si,body
#else
        mov     si,body
#endif
        mov     cx,bytes
        mov     di,si
        mov     ax,ds
        mov     es,ax
	stc
	cld
negloop:
        lodsb
	not		al
        adc     al,0
        stosb
        loop    negloop

#if M_I86CM || M_I86LM || M_I86VM
        mov     ds,bx
#endif
    }
#endif
}
