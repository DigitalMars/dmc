;_ sysint.asm   1991   Modified by: Walter Bright */
;Copyright (C) 1984-1991 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

nobdos	equ	1	;turn off definition of bdos macro
include	macros.asm
include stdio.asm

extrn __SET_ERRNO:near

;struct WORDREGS {unsigned ax,bx,cx,dx,si,di,cflag,flags; };
_ax	equ	0
_bx	equ	_ax+4
_cx	equ	_bx+4
_dx	equ	_cx+4
_si	equ	_dx+4
_di	equ	_si+4
_cflag	equ	_di+4
_flags	equ	_cflag+4

;struct SREGS { unsigned short es,cs,ss,ds,fs,gs; };
_es	equ	0
_cs	equ	2
_ss	equ	4
_ds	equ	6
_fs	equ	8
_gs	equ	10


	begcode	sysint

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get values of segment registers.
;	void __cdecl segread(struct SREGS *sregs);

	c_public	segread
func	segread
	mov	EAX,PS[ESP]
	mov	_es[EAX],ES
	mov	_cs[EAX],CS
	mov	_ss[EAX],SS
	mov	_ds[EAX],DS
	mov	_fs[EAX],FS
	mov	_gs[EAX],GS
	ret
c_endp	segread


comment ~ --- REPLACED FOLLOWING WITH INT86.ASM, INT86X.ASM ---


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generate an interrupt.
; Use:
;	int int86(unsigned intnum, union REGS *input, union REGS *output);
;	intnum = 8086 software interrupt number
;	input = input register values
;	output = output register values
; Returns:
;	value of flags


; BP offset to first parameter passed to int86()


	c_public	int86
func	int86
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX,ESI,EDI,ES,DS>
	push	DS
	push	EBP

	;for return from interrupt, we need:
	;	flags
	;	segment
	;	offset

	pushfd
	pop	EDX
	push	EDX		;flags
	push	CS		;segment
	push	offset intret	;offset return address to get us back here

	and	DH,0Ch		;reset T and I flags
	push	EDX		;flags
	mov	CL,P[EBP]
	mov	AX,2502h
	int	21h		;get protected mode vector in ES:EBX
	push	ES
	push	EBX		;offset	return address to get us to interrupt we want to call

	mov	EBX,P+4[EBP]	;input registers
	mov	EAX,_ax[EBX]
	mov	ECX,_cx[EBX]
	mov	EDX,_dx[EBX]
	mov	ESI,_si[EBX]
	mov	EDI,_di[EBX]
	mov	EBX,_bx[EBX]

	iretd			;fake an interrupt so we end up at the interrupt we want to call

intret:
	pop	EBP		;BP might have been trashed
intret2:
	pop	DS		;restore default DS
	push	EBX
	mov	EBX,P+4+SIZEPTR[EBP]
	
INTTAIL:
	pushfd
	pop	dword ptr _flags[EBX]
	mov	_ax[EBX],EAX
	mov	_cx[EBX],ECX
	mov	_dx[EBX],EDX
	mov	_si[EBX],ESI
	mov	_di[EBX],EDI
	pop	dword ptr _bx[EBX]
	sbb	ECX,ECX		;note that status of C is maintained
	mov	_cflag[EBX],ECX	;set _cflag to non-zero if carry was set
	jnc	INT1		;if no error occurred
	errno_set
INT1:
	cld			;C rules state that direction flag is forward
	unuse	<DS,ES,EDI,ESI,EDX,ECX,EBX>
	pop	EBP
	ret
c_endp	int86

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generate an interrupt.
; Use:
;	intnum = BDOS interrupt number
;	input = & struct regval, input register values
;	output = & struct regval, output register values
;	segregs = & struct SREGS, input segment registers
;	val = int86x(intnum,input,output,segregs);
; Returns:
;	value of flags


	c_public	int86x
func	int86x
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX,ESI,EDI,ES,DS>
	push	DS
	push	EBP

	;for return from interrupt, we need:
	;	flags
	;	segment
	;	offset

	pushfd
	pop	EDX
	push	EDX		;flags
	push	CS		;segment
	push	offset intretx	;offset

	and	DH,0Ch		;reset T and I flags
	push	EDX		;flags
	mov	CL,P[EBP]
	mov	AX,2502h
	int	21h		;get protected mode vector in ES:EBX
	push	ES
	push	EBX
	mov	EBX,P+4+SIZEPTR+SIZEPTR[EBP]	;segregs
	mov	ES,_es[EBX]
	mov	FS,_fs[EBX]
	mov	GS,_gs[EBX]
	push	dword ptr _ds[EBX]
	mov	EBX,P+4[EBP]		;input registers
	mov	EAX,_ax[EBX]
	mov	ECX,_cx[EBX]
	mov	EDX,_dx[EBX]
	mov	ESI,_si[EBX]
	mov	EDI,_di[EBX]
	mov	EBX,_bx[EBX]
	pop	DS
	iretd			;fake an interrupt

intretx:
	pop	EBP
	push	EBX
	push	DS
	mov	DS,-20[EBP]			;restore original DS
	mov	EBX,P+4+SIZEPTR+SIZEPTR[EBP]	;segregs
	mov	_es[EBX],ES		;value of ES after interrupt
	mov	_fs[EBX],FS
	mov	_gs[EBX],GS
	pop	dword ptr _ds[EBX]	;value of DS after interrupt
	pop	EBX
	jmp	intret2
c_endp	int86x


--- END OF COMMENT --- ~


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generate a real mode interrupt.
; Use:
;	int int86_real(unsigned intnum, union REGS *input, union REGS *output);
;	intnum = 8086 software interrupt number
;	input = input register values
;	output = output register values
; Returns:
;	value of flags

	c_public	int86_real
func	int86_real
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX,ESI,EDI>
	mov	EBX,P+4[EBP]	;input registers
	mov	ECX,_cx[EBX]
	push	_dx[EBX]
	push	_ax[EBX]
	push	0                   ;value for GS and FS
	push	0                   ;value for DS and ES
	push	word ptr P[EBP]     ;interrupt #
	mov	EDX,ESP              ;points to parameter block
	mov	ESI,_si[EBX]
	mov	EDI,_di[EBX]
	mov	EBX,_bx[EBX]
	push	EBP
	mov	AX,2511h             ;Issue real mode interrupt
	int	21h
	pop	EBP
	lea	ESP,[ESP+14]	;pop interrupt number,eax,segs
intr2:
	mov	EDX,P+SIZEPTR+4[EBP]	;output registers
	mov	_ax[EDX],EAX
	mov	_bx[EDX],EBX
	mov	_cx[EDX],ECX
	pop	_dx[EDX]
	mov	_si[EDX],ESI
	mov	_di[EDX],EDI
	pushfd
	pop	_flags[EDX]
	sbb	ECX,ECX		;note that status of C is maintained
	mov	_cflag[EDX],ECX	;set _cflag to non-zero if carry was set
	jnc	intr1		;if no error occurred
	push    EAX
	call    __SET_ERRNO
intr1:
	cld			;C rules state that direction flag is forward
	unuse	<EDI,ESI,EDX,ECX,EBX>
	pop	EBP
	ret
c_endp	int86_real

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generate a real mode interrupt.
; Use:
;	intnum = interrupt number
;	input = & struct regval, input register values
;	output = & struct regval, output register values
;	segregs = & struct SREGS, input segment registers
;	val = int86x_real(intnum,input,output,segregs);
; Returns:
;	value of flags

	c_public	int86x_real
func	int86x_real
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX,ESI,EDI>
	mov	EAX,P+4[EBP]		;input registers
	mov	ECX,_cx[EAX]
	push	_dx[EAX]
	push	_ax[EAX]
	mov	ESI,_si[EAX]
	mov	EDI,_di[EAX]
	mov	EBX,_bx[EAX]
	mov	EAX,P+4+SIZEPTR+SIZEPTR[EBP]	;segregs
	push	word ptr _gs[EAX]
	push	word ptr _fs[EAX]
	push	word ptr _es[EAX]
	push	word ptr _ds[EAX]
	push	word ptr P[EBP]    ;interrupt number
	mov	EDX,ESP             ;pointer to parameter block
	push	EBP
	mov	AX,2511h             ;issue real mode interrupt
	int	21h
	pop	EBP
	lea	ESP,[ESP+2]	;pop interrupt number
	mov	EDX,P+4+SIZEPTR+SIZEPTR[EBP]	;segregs
	pop	word ptr _ds[EDX]
	pop	word ptr _es[EDX]
	pop	word ptr _fs[EDX]
	pop	word ptr _gs[EDX]
	lea	ESP,[ESP+4]
	jmp	intr2
c_endp	int86x_real

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generate a DOS interrupt.
; Use:
;	input = & struct regval, input register values
;	output = & struct regval, output register values
;	val = intdos(input,output);
; Returns:
;	value of flags

	c_public	intdos
func	intdos
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX,ESI,EDI,ES,DS>
	push	DS

	mov	EBX,P[EBP]	;input registers
	mov	EAX,_ax[EBX]
	mov	ECX,_cx[EBX]
	mov	EDX,_dx[EBX]
	mov	ESI,_si[EBX]
	mov	EDI,_di[EBX]
	mov	EBX,_bx[EBX]
	push	EBP		;work around Novell bug which trashes EBP
	int	21h
	pop	EBP

	pop	DS		;restore default DS

intdosfinish:
	push	EBX
	mov	EBX,P+SIZEPTR[EBP]
	jmp	INTTAIL
	
	
; added this bit...


INTTAIL:
	pushfd
	pop	dword ptr _flags[EBX]
	mov	_ax[EBX],EAX
	mov	_cx[EBX],ECX
	mov	_dx[EBX],EDX
	mov	_si[EBX],ESI
	mov	_di[EBX],EDI
	pop	dword ptr _bx[EBX]
	sbb	ECX,ECX		;note that status of C is maintained
	mov	_cflag[EBX],ECX	;set _cflag to non-zero if carry was set
	jnc	INT1		;if no error occurred
	push    EAX
	call    __SET_ERRNO
INT1:
	cld			;C rules state that direction flag is forward
	unuse	<DS,ES,EDI,ESI,EDX,ECX,EBX>
	pop	EBP
	ret


; ...that's it	
	
	
c_endp	intdos

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generate a DOS interrupt.
; Use:
;	input = & struct regval, input register values
;	output = & struct regval, output register values
;	segregs = & struct SREGS, input segment registers
;	val = intdosx(input,output,segregs);
; Returns:
;	value of flags


	c_public	intdosx
func	intdosx
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX,ESI,EDI,ES,DS>

	mov	EBX,P+SIZEPTR+SIZEPTR[EBP]	;BX = segregs
	;Get ES and DS from *segregs.
	;Can't realistically load SS or CS!
	mov	ES,_es[EBX]
	mov	FS,_fs[EBX]
	mov	GS,_gs[EBX]
	push	dword ptr _ds[EBX]		;value of DS for interrupt
	mov	EBX,P[EBP]			;input registers
	mov	EAX,_ax[EBX]
	mov	ECX,_cx[EBX]
	mov	EDX,_dx[EBX]
	mov	ESI,_si[EBX]
	mov	EDI,_di[EBX]
	mov	EBX,_bx[EBX]
	pop	DS

	push	EBP		;work around Novell bug which trashes EBP
	int	21h
	pop	EBP

	push	EBX
	push	DS
	mov	EBX,P+SIZEPTR+SIZEPTR[EBP]
	;NOTE the following offset from EBP depends on the implementation
	;of the uses macro above.
	mov	DS,-20[EBP]			;restore default DS
	mov	_es[EBX],ES
	mov	_fs[EBX],FS
	mov	_gs[EBX],GS
	pop	dword ptr _ds[EBX]
	pop	EBX

	jmp	intdosfinish
c_endp	intdosx

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Call BDOS.
; Use:
;	val = bdos(func,dx,al)
;	func = BDOS function number
;	dx = value for dx register
;	al = value for al register
; Returns:
;	low 8 bits: value returned in AL
;	high 8 bits: 0

	c_public	bdosx
func	bdosx
;	jmp	_bdos
c_endp	bdosx

	c_public	bdosptr
func	bdosptr
c_endp	bdosptr

	c_public	bdos
func	bdos
	push	EBP
	mov	EBP,ESP
	uses	<EBX,EDX,ESI,EDI>
	mov	EDX,P+4[EBP]
	mov	AL,P+4+4[EBP]
	mov	AH,P[EBP]
	int	21h
	movzx	EAX,AX
B1:	
B2:	unuse	<EDI,ESI,EDX,EBX>
	pop	EBP
	ret
c_endp	bdos

	endcode	sysint
	end
