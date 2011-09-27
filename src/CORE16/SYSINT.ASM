;_ sysint.asm	Wed May 30 1990   Modified by: Walter Bright
; Windows support added by G. Eric Engstrom Jan 1991	-D_WINDOWS
; OS2 support added by Nikki Locke Jan 1989		-D__OS2__
; Copyright (C) 1984-1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

;Define ROM in order to generate a ROMable version
;ROM	equ	1

nobdos	equ	1	;turn off definition of bdos macro
include	macros.asm
include stdio.asm

if LCODE
extrn __SET_DOS_ERRNO:far
else
extrn __SET_DOS_ERRNO:near
endif

    ifdef _WINDOWS
    extrn DOS3CALL	:far
    endif

ifdef DOS16RM

arg1	equ	[bp+4+2*LCODE]
arg2	equ	[bp+6+2*LCODE]
arg3	equ	[bp+8+2*LCODE]
arg4	equ	[bp+10+2*LCODE]
arg5	equ	[bp+12+2*LCODE]
arg6	equ	[bp+14+2*LCODE]
arg7	equ	[bp+16+2*LCODE]
endif

;struct WORDREGS {unsigned ax,bx,cx,dx,si,di,cflag,flags; };
_ax	equ	0
_bx	equ	2
_cx	equ	4
_dx	equ	6
_si	equ	8
_di	equ	10
_cflag	equ	12
_flags	equ	14

;struct SREGS { unsigned es,cs,ss,ds; };
_es	equ	0
_cs	equ	2
_ss	equ	4
_ds	equ	6


ifdef DOS16RM
_TEXT	segment	byte public 'CODE'
	assume	cs:_TEXT
else
	begcode	sysint
endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get values of segment registers.
;	segread(struct SREGS *sregs);

	c_public	segread
func	segread
	push	BP
	mov	BP,SP
    if SPTR
	mov	BX,P[BP]
	mov	_es[BX],ES
	mov	_cs[BX],CS
	mov	_ss[BX],SS
	mov	_ds[BX],DS
    else
	push	DS
	lds	BX,P[BP]
	mov	_es[BX],ES
	mov	_cs[BX],CS
	mov	_ss[BX],SS
	pop	AX
	mov	_ds[BX],AX
	mov	DS,AX
    endif
	pop	BP
	ret
c_endp	segread

    ifndef __OS2__
ifdef DOS16RM
	c_public int86x
func	int86x	; flags = int86x(intno, far *r, far *r, far *sr)
	push	bp
	mov	bp, sp
	push	0		; [bp-2]  1 = used DS, 2 = used ES
	push	0		; [bp-4]  return ax value
	push	si
	push	di
	push	es
	push	ds
	push	bp		; for after return from interrupt
	pushf
	push	cs
	push	offset int86xr	; iframe (return from interrupt)
    ifdef ROM
	mov	ax, idts
	mov	ds, ax		; address IDT
	assume	ds:nothing
	mov	bl, arg1
	xor	bh, bh
	shl	bx, 3
	cli			; interrupt handler must start disabled
	pushf
	push	ds:[bx+2]
	push	ds:[bx]		; iframe (return into interrupt)
    else
	; call DOS get vector, IDTS selector is not available under DPMI
	mov	AL,arg1
	mov	AH,035h
	int	21h
	cli
	pushf			; interrupt handler must start disabled
	push	ES
	push	BX
	assume	DS:nothing
    endif
	lds	bx, arg2	; input registers
	push	[bx]._ax
	push	[bx]._bx
	mov	cx, [bx]._cx
	mov	dx, [bx]._dx
	mov	si, [bx]._si
	mov	di, [bx]._di
	lds	bx, arg6	; seg registers
	xor	ax, ax		; assume bad es
	lar	ax, [bx]._es	; test validity
	test	ah, 10h
	mov	ax, 0
	jz	i86x_1		; jump if inaccessible or system selector
	mov	ax, [bx]._es	; ok
	mov	byte ptr [bp-2], 2	; mark user ES passed
i86x_1:	mov	es, ax		; set es
	xor	ax, ax
	lar	ax, [bx]._ds
	test	ah, 10h
	mov	ax, 0
	jz	i86x_2		; jump if bad selector
	mov	ax, [bx]._ds	; ok
	or	byte ptr [bp-2], 1	; mark user DS passed
i86x_2:	mov	ds, ax
	pop	bx
	pop	ax		; registers complete, return into int handler
	cli			; interrupt handler must start disabled
	iret
				; interrupt executes now ==========
int86xr: pop	bp
	push	ds
	push	bx
	pushf
	lds	bx, arg4	; output regs
	mov	[bx]._ax, ax
	mov	[bp-4], ax	; save for return
	pop	ax		; flags reg
	mov	[bx]._flags,ax
	and	ax, 1		; isolate carry flag
	mov	[bx]._cflag, ax
	mov	[bx]._cx, cx
	mov	[bx]._dx, dx
	mov	[bx]._si, si
	mov	[bx]._di, di
	pop	[bx]._bx
	lds	bx, arg6	; segment regs
	mov	cx, es
	test	byte ptr [bp-2], 2
	jnz	out_es		; jmp if used input ES, so return output
	jcxz	nout_es		; did not use input ES, skip if return still 0
out_es:	mov	[bx]._es, cx	; return ES
nout_es: pop	cx
	test	byte ptr [bp-2], 1
	jnz	out_ds		; jmp if used input DS, so return output
	jcxz	nout_ds		; did not use input DS, skip if return still 0
out_ds:	mov	[bx]._ds, cx	; return DS
nout_ds:
	pop	ds
	assume	ds:DGROUP
	pop	es
	pop	di
	pop	si
	pop	ax		; return ax
	pop	bp		; lose local var
	pop	bp
	ret
_int86x	endp


	c_public int86
func	int86
	push	bp
	mov	bp, sp
	push	ds
	push	ss
	push	cs
	push	es
	mov	ax, sp
	push	ss
	push	ax		; seg regs arg structure
	push	arg5
	push	arg4
	push	arg3
	push	arg2
	push	arg1
	call	_int86x
	mov	sp, bp
	pop	bp
	ret
_int86	endp

	c_public intdosx
func	intdosx
	push	bp
	mov	bp, sp
	push	arg6
	push	arg5
	push	arg4
	push	arg3
	push	arg2
	push	arg1
	push	21h
	call	_int86x
	mov	sp, bp
	pop	bp
	ret
_intdosx endp

	c_public intdos
func	intdos
	push	bp
	mov	bp, sp
	push	ds
	push	ss
	push	cs
	push	es
	mov	ax, sp
	push	ss
	push	ax		; seg regs arg structure
	push	arg4
	push	arg3
	push	arg2
	push	arg1
	push	21h
	call	_int86x
	mov	sp, bp
	pop	bp
	ret
_intdos endp

	c_public _is_pm
func	_is_pm		; in_protected_mode = _is_pm(void)
	xor	ax, ax		; assume false
	push	sp
	pop	bx
	cmp	bx, sp
	jne	return		; jmp if 8086 or 80186 class
	smsw	ax		; store MSW in ax
	and	ax, 1		; mask all but PE bit
	jz	return
	pushf
	pop	bx
	and	bh, 9Fh		; IOPL to 0
	push	bx
	popf
	pushf
	pop	bx
	and	bh, 60h		; isolate IOPL
	jz	return		; if IOPL 0, return ax=1
	xor	ax, ax		; in virtual 8086, "not protected"
return:
	ret			; 1 if cpu in protected mode
__is_pm	endp

	c_public _intflag
func	_intflag			; old_int_enable = _intflag(new_int_enable)
	push	bp
	mov	bp, sp
	pushf
	pop	ax
	shr	ax, 9
	and	al, 01
	cli
	cmp	word ptr arg1, 0
	jz	ifr
	sti
ifr:	pop	bp
	ret
__intflag endp

else ; DOS16RM

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
	WINENTER
        push	SI
        push	DI
        push	DS

    if SPTR
	mov	BX,P[BP]	;input registers
    else
	push	DS
	lds	BX,P[BP]
    endif
	mov	AX,_ax[BX]
	mov	CX,_cx[BX]
	mov	DX,_dx[BX]
	mov	SI,_si[BX]
	mov	DI,_di[BX]
	mov	BX,_bx[BX]
    if LPTR
	pop	DS
    endif

    ifdef _WINDOWS
	call	DOS3CALL
    else
	int	21h
    endif

intdosfinish:
    if SPTR
	pop	DS		;restore default DS
	push	BX
	mov	BX,P+SIZEPTR[BP]
    else
	push	BX
	lds	BX,P+SIZEPTR[BP]
    endif
	jmp	INTTAIL
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
	WINENTER
        push	SI
        push	DI
        push	DS

    if SPTR
	mov	BX,P+SIZEPTR+SIZEPTR[BP]	;BX = segregs
	;Get ES and DS from *segregs.
	;Can't realistically load SS or CS!
	mov	ES,_es[BX]
	push	_ds[BX]		;value of DS for interrupt
	mov	BX,P[BP]	;input registers
    else
	lds	BX,P+SIZEPTR+SIZEPTR[BP]
	mov	ES,_es[BX]
	push	_ds[BX]
	lds	BX,P[BP]
    endif
	mov	AX,_ax[BX]
	mov	CX,_cx[BX]
	mov	DX,_dx[BX]
	mov	SI,_si[BX]
	mov	DI,_di[BX]
	mov	BX,_bx[BX]
	pop	DS

    ifdef _WINDOWS
	call	DOS3CALL
    else
	int	21h
    endif

	push	BX
	push	DS
    if SPTR
	mov	BX,P+SIZEPTR+SIZEPTR[BP]
	mov	DS,-6[BP]		;restore default DS
    else
	lds	BX,P+SIZEPTR+SIZEPTR[BP]
    endif
	mov	_es[BX],ES
	pop	_ds[BX]
	pop	BX

	jmp	intdosfinish
c_endp	intdosx

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generate an interrupt.
; Use:
;	int int86(unsigned intnum, union REGS *input, union REGS *output);
;	intnum = 8086 software interrupt number
;	input = input register values
;	output = output register values
; Returns:
;	value of flags


	c_public	int86
func	int86
	WINENTER
        push	SI
        push	DI
        push	DS

	;for return from interrupt, we need:
	;	flags
	;	segment
	;	offset

	pushf
	pop	DX
	push	DX		;flags
	push	CS		;segment
	mov	AX,offset intret
	push	AX		;offset
	and	DH,0Ch		;reset T and I flags
	push	DX		;flags
    ifdef ROM
	clr	BX
	mov	ES,BX		;look at base page
	mov	BL,P[BP]	;interrupt number
	shl	BX,1
	shl	BX,1		;*4 to get addr of vector
	push	ES:word ptr 2[BX]	;segment
	push	ES:word ptr [BX]	;offset
    else
	mov	AL,P[BP]
	mov	AH,35h
    ifdef _WINDOWS
	call	DOS3CALL
    else
	int	21h		;get vector in ES:BX
    endif
	push	ES
	push	BX
    endif
    if SPTR
	mov	BX,P+2[BP]		;input registers
    else
	push	DS
	lds	BX,P+2[BP]
    endif
	mov	AX,_ax[BX]
	mov	CX,_cx[BX]
	mov	DX,_dx[BX]
	mov	SI,_si[BX]
	mov	DI,_di[BX]
	mov	BX,_bx[BX]
    if LPTR
	pop	DS
    endif
	iret			;fake an interrupt

intret:
	mov	BP,SP		;BP might have been trashed
        lea	BP,6[BP]	;add BP,6 without affecting flags
intret2:
    if SPTR
	pop	DS		;restore default DS
	push	BX
	mov	BX,P+2+SIZEPTR[BP]
    else
	push	BX
	lds	BX,P+2+SIZEPTR[BP]
    endif

INTTAIL:
	pushf
	pop	_flags[BX]
	mov	_ax[BX],AX
	mov	_cx[BX],CX
	mov	_dx[BX],DX
	mov	_si[BX],SI
	mov	_di[BX],DI
	pop	_bx[BX]
	sbb	CX,CX		;note that status of C is maintained
	mov	_cflag[BX],CX	;set _cflag to non-zero if carry was set
    if LPTR
	pop	DS
    endif
	jnc	INT1		;if no error occurred
	push    AX
	call    __SET_DOS_ERRNO
INT1:
	cld			;C rules state that direction flag is forward
	pop	DI
        pop	SI
        WINLEAVE
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
	WINENTER
        push	SI
        push	DI
        push	DS

	;for return from interrupt, we need:
	;	flags
	;	segment
	;	offset

	pushf
	pop	DX
	push	DX		;flags
	push	CS		;segment
	mov	AX,offset intretx
	push	AX		;offset
	and	DH,0Ch		;reset T and I flags
	push	DX		;flags
    ifdef ROM
	clr	BX
	mov	ES,BX		;look at base page
	mov	BL,P[BP]	;interrupt number
	shl	BX,1
	shl	BX,1		;*4 to get addr of vector
	push	ES:word ptr 2[BX]	;segment
	push	ES:word ptr [BX]	;offset
    else
	mov	AL,P[BP]
	mov	AH,35h
    ifdef _WINDOWS
	call	DOS3CALL
    else
	int	21h		;get vector in ES:BX
    endif
	push	ES
	push	BX
    endif
    if SPTR
	mov	BX,P+2+SIZEPTR+SIZEPTR[BP]	;segregs
	mov	ES,_es[BX]
	push	_ds[BX]
	mov	BX,P+2[BP]		;input registers
    else
	lds	BX,P+2+SIZEPTR+SIZEPTR[BP]
	mov	ES,_es[BX]		;value of ES for interrupt
	push	_ds[BX]			;value of DS for interrupt
	lds	BX,P+2[BP]
    endif
	mov	AX,_ax[BX]
	mov	CX,_cx[BX]
	mov	DX,_dx[BX]
	mov	SI,_si[BX]
	mov	DI,_di[BX]
	mov	BX,_bx[BX]
	pop	DS
	iret			;fake an interrupt

intretx:
	mov	BP,SP
        lea	BP,6[BP]		;add BP,6 without affecting flags

	push	BX
	push	DS
    if SPTR
	mov	DS,-6[BP]			;default DS
	mov	BX,P+2+SIZEPTR+SIZEPTR[BP]	;segregs
    else
	lds	BX,P+2+SIZEPTR+SIZEPTR[BP]
    endif
	mov	_es[BX],ES		;value of ES after interrupt
	pop	_ds[BX]			;value of DS after interrupt
	pop	BX
	jmp	intret2
c_endp	int86x

endif ; DOS16RM

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

    if SPTR
	c_public	bdosx
func	bdosx
c_endp	bdosx		;behaves identical to bdos()
    endif

	c_public	bdosptr
func	bdosptr
c_endp	bdosptr

	c_public	bdos
func	bdos
	WINENTER
	.save	<SI,DI>
	mov	DX,P+2[BP]
	mov	AL,P+4[BP]
	mov	AH,P[BP]
    ifdef _WINDOWS
	call	DOS3CALL
    else
	int	21h
    endif
B1:	
B2:
	.restore <DI,SI>
	WINLEAVE
	ret
c_endp	bdos

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Call BDOS.
; Use:
;	val = bdosx(func,dsdx,al)
;	func = BDOS function number
;	dsdx = value for ds:dx register pair
;	al = value for al register
; Returns:
;	low 8 bits: value returned in AL
;	high 8 bits: 0

    if LPTR
	c_public	bdosx
func	bdosx
	WINENTER
	.save	<SI,DI>
	push	DS
	lds	DX,P+2[BP]
	mov	AL,P+2+SIZEPTR[BP]
	mov	AH,P[BP]
    ifdef _WINDOWS
	call	DOS3CALL
    else
	int	21h
    endif
	pop	DS
	jmp	B1
c_endp	bdosx
    endif

    endif ;__OS2__

ifdef DOS16RM
_TEXT	ends
else
	endcode	sysint
endif
	end

