;_ cputype.asm   Sat Jan 14 1989   Modified by: Walter Bright */

include	macros.asm

	begdata
	extrn	__osmajor:byte,__osminor:byte
            public ___cputype
        ___cputype  dw  -1    ;; Save value for cputype
	enddata

	begcode	CPUTYPE

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Determine which CPU is running.
; Placed into public domain by Compaq Computers.
;	int cputype(void);
; Returns:
;	0	8088/8086/V20
;	1	80186
;	2	80286
;	3	80386
;	4	80486
;	5	Pentium

	c_public cputype
func	cputype
        cmp     ___cputype, -1
        je      L3
        mov    AX,___cputype
        ret
L3:
	pushf
	clr	AX
	push	AX
	popf		;try forcing flag word to all 0
	pushf
	pop	AX	;retrieve them for testing
	and	AH,0F0h
	_if	AH ne 0F0h, L2	;it's a 286 or 386

	;Distinguish between 8088/8086 and 80186
	clr	AX
	push	SP		;is SP pushed before or after it's decremented?
	pop	BX
	_if	BX ne SP,L1a	;it's an 8088/8086
	inc	AX
L1a:	jmp	L1		;it's an 80186

L2:	mov	AX,0F000h
	push	AX		;try to force high bits on in flags
	popf
	pushf
	pop	AX		;AX = what was actually stored
	and	AH,0F0h
	mov	AX,2
	jz	L1		;no, it's a 286

	;Distinguish between 386 and 486

	;Cannot use extended registers if we're using OS/2 1.0 or 1.1
	;(be careful, we could be in the compatibility box)
	_if	__osmajor ne 10, L4
	_if	__osminor be 1, L1	;can't tell, so assume 286
L4:
	.386
	mov	BX,SP
	and	SP,0FFFCh	;round down to a dword boundary
	pushfd
	pushfd
	pop	EDX
	mov	ECX,EDX
	xor	EDX,40000h	;toggle AC bit
	and	ECX,40000h
	push	EDX
	popfd
	pushfd
	pop	EDX
	popfd			;restore original flags
	mov	SP,BX		;restore original stack pointer
	and	EDX,40000h
	inc	AX		;AX = 3
	_if	EDX e ECX,L1	;it's a 386

	;It's at least a 486.
	;Determine if chip supports CPUID

	pushfd
	pop	EAX
	mov	ECX,EAX
	xor	EAX,200000h	;toggle ID bit
	push	EAX
	popfd
	pushfd
	pop	EAX
	xor	EAX,ECX
	mov	AX,4
	je	L1		;it's a 486 w/o support for CPUID

	;Execute CPUID to determine processor type

	mov	EAX,1
	db	0Fh,0A2h	;CPUID opcode
	mov	AL,AH
	and	AX,0Fh

        mov     ___cputype,AX    ; Save for subsequent calls
L1:	popf			;original flags
	ret
c_endp	cputype

	endcode	CPUTYPE

	end
