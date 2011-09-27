#include <iosold\iostream.h>

int _2Comp::xlate(char *s, unsigned char *body,
                        int radix, int negative, int bytes)
{
int result;
#if __INTSIZE == 4
__asm {
	cld
        mov		esi,s
        xor     	bx,bx			; clear for carries/overflow
	mov		al,bl			; zero al
	mov		ecx,bytes
	mov		edi,body
	rep		stosb			; clear destination area

scanloop:
	lodsb					; get next digit
	or		al,al
	jz		done			; terminating null byte
	push		eax
	jmp		short mult		; multiply by radix
nextdigit:
	pop		eax
	cmp		al,'a'
	jnc		hex
	sub		al,'0'
	jmp		short adder
hex:
	or		al,20h			; hex - make lowercase
	sub		al,'a'-10

adder:
	mov		edi,body
	mov     	ecx,bytes
        add     	al,[edi]		; add in digit
        stosb
        dec     	ecx
	jecxz		ovfcheck
addloop:
        mov		al,[edi]
        adc     	al,0
        stosb
        loop    	addloop
ovfcheck:
	xor		bh,bh
	rcl		bh,1			; note any overflow
	jmp		short scanloop

done:
	test	byte ptr negative,1	; needs negating?

	jz		getout

	mov		edi,body
	mov		ecx,bytes
	stc
cloop:
	mov		al,[edi]
	not		al
	adc		al,0
	stosb
	loop		cloop
	xor		al,al
	rcl		al,1
	or		bh,al			; note any overflow

getout:
	xor		eax,eax
	mov     	al,bl			; overflow from multiply
	or		al,bh			; overflow from add/ negate
	mov		result,eax
	jmp exit_function;

mult:
	mov		edi,body
        mov		ecx,bytes
mulloop:
        mov		al,[edi]
        mul     	byte ptr radix
        add	     	al,bl
        adc     	ah,0
        mov    		bl,ah			; note any overflow
        stosb
        loop   		mulloop
	jmp		short nextdigit
	
}
#else	       
__asm {
#if M_I86CM || M_I86LM || M_I86VM
        push    ds
        lds     si,s
#else
        mov     si,s
#endif
	cld
        mov     ax,ds
        mov     es,ax
        xor     bx,bx			; clear for carries/overflow
	mov		al,bl			; zero al
	mov		cx,bytes
#if M_I86CM || M_I86LM || M_I86VM
	les		di,body
#else
	mov		di,body
#endif
	rep		stosb			; clear destination area

scanloop:
	lodsb					; get next digit
	or		al,al
	jz		done			; terminating null byte
	push	ax
	jmp		short mult		; multiply by radix
nextdigit:
	pop		ax
	cmp		al,'a'
	jnc		hex_num
	sub		al,'0'
	jmp		short adder
hex_num:
	or		al,20h			; hex - make lowercase
	sub		al,'a'-10

adder:
#if M_I86CM || M_I86LM || M_I86VM
	les		di,body
#else
	mov		di,body
#endif
        mov     cx,bytes

#if M_I86CM || M_I86LM || M_I86VM
	add	al,es:[di]
#else
        add     al,[di]			; add in digit
#endif
        stosb
        dec     cx
	jcxz	ovfcheck
addloop:

#if M_I86CM || M_I86LM || M_I86VM
	mov		al,es:[di]
#else
        mov		al,[di]
#endif

        adc     al,0
        stosb
        loop    addloop
ovfcheck:
	xor		bh,bh
	rcl		bh,1			; note any overflow
	jmp		short scanloop

done:
	test	byte ptr negative,1	; needs negating?
	jz		getout

#if M_I86CM || M_I86LM || M_I86VM
	les		di,body
#else
	mov		di,body
#endif
	mov		cx,bytes
	stc
cloop:

#if M_I86CM || M_I86LM || M_I86VM
	mov		al,es:[di]
#else
	mov		al,[di]
#endif
	not		al
	adc		al,0
	stosb
	loop	cloop
	xor		al,al
	rcl		al,1
	or		bh,al			; note any overflow
getout:
        mov     	al,bl			; overflow from multiply
	or		al,bh			; overflow from add/ negate
	xor		ah,ah
	mov		result,ax

#if M_I86CM || M_I86LM || M_I86VM
        pop     ds
#endif
	jmp	exit_function
mult:
#if M_I86CM || M_I86LM || M_I86VM
	les		di,body
#else
	mov	di,body
#endif
        mov     cx,bytes
mulloop:
#if M_I86CM || M_I86LM || M_I86VM
	mov		al,es:[di]
#else
        mov		al,[di]
#endif
        mul     byte ptr radix
        add     al,bl
        adc     ah,0
        mov     bl,ah
        stosb
        loop    mulloop
	jmp		short nextdigit
}
#endif
exit_function: return(result);
    
}
