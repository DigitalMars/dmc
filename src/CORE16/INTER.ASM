;_ inter.asm   Mon Aug 20 1990	 Modified by: Walter Bright
; Windows support added by G. Eric Engstrom Jan 1991	-D_WINDOWS
; Copyright (c) 1986-1990 by Walter Bright
; All Rights Reserved
; Written by Walter Bright
; Many thanks to Wayne Mandrus for the modifications for DOS16RM.

; Interrupt handling package

; If you are developing ROM applications, and MS-DOS won't be present,
; define ROM. This is to bypass DOS and manipulate the interrupt vector
; table directly.

;ROM	equ	1	;bypass DOS when dealing with the vector table

include	MACROS.ASM

    if LCODE
	c_extrn	malloc,far, free,far
    else
	c_extrn	malloc,near, free,near
    endif
    ifdef DOS16RM
	c_extrn D16SegCSAlias,far
	c_extrn D16SegCancel,far
	PROTECTED equ 1
    endif
    ifdef _WINDOWS
	extrn	ALLOCDSTOCSALIAS : far
	extrn	FREESELECTOR : far
	PROTECTED equ 1
    endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Silly macro to fake a POPF instruction, which doesn't always work.
_popf	macro
	local	L1,L2
	jmps	L2
L1:	iret
L2:	push	cs
	call	L1
	endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Macro to read interrupt vector vecnum from the interrupt vector table
; into vecseg and vecoff.

getvec	macro	vecnum,vecseg,vecoff
    ifdef ROM
	clr	AX
	mov	ES,AX		;vector table is at segment 0
	mov	AL,vecnum
	mov	DI,AX
	shl	DI,1
	shl	DI,1		;offset of vector is (vecnum * 4)
	mov	vecoff,ES:[DI]
	mov	vecseg,ES:2[DI]
    else
      ifdif <vecnum>,<AL>
	mov	AL,vecnum
      endif
	bdos	35h		;read vector in ES:BX
      ifdif <vecoff>,<BX>
	mov	vecoff,BX
      endif
      ifdif <vecseg>,<ES>
	mov	vecseg,ES
      endif
    endif
	endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Macro to set interrupt vector vecnum into the interrupt vector table
; from vecseg and vecoff.

setvec	macro	vecnum,vecseg,vecoff
    ifdef ROM
	push	ES
      ifdif <vecnum>,<AL>
	mov	AL,vecnum
      endif
	clr	AH
	mov	DI,AX
	shl	DI,1
	shl	DI,1
      ifdif <vecseg>,<DS>
	mov	CX,vecseg
      endif
	clr	AX
	mov	ES,AX
	mov	ES:[DI],vecoff
      ifdif <vecseg>,<DS>
	mov	ES:2[DI],CX
      else
	mov	ES:2[DI],vecseg
      endif
	pop	ES
    else
      ifdif <vecseg>,<DS>
	push	DS
      endif
      ifdif <vecnum>,<AL>
	mov	AL,vecnum
      endif
      ifdif <vecoff>,<DX>
	mov	DX,vecoff
      endif
      ifdif <vecseg>,<DS>
	mov	DS,vecseg
      endif
	bdos	025h
      ifdif <vecseg>,<DS>
	pop	DS
      endif
    endif
	endm

;These structures should match union REGS and struct SREGS in dos.h
REGS	macro
	_ax		dw	?
	_bx		dw	?
	_cx		dw	?
	_dx		dw	?
	_si		dw	?
	_di		dw	?
	_cflag		dw	?
	_flags		dw	?
	endm

SREGS	macro
	_es		dw	?
	_cs		dw	?
	_ss		dw	?
	_ds		dw	?
	endm

; For unfathomable reasons, if's don't work in structs.

    ifdef I8086S
intstr	struc
	callf		db	?	;far call
	faraddr		dw	?,?	;far address of interrupt service routine
	prevvec		dw	?,?	;previous vector
	stacksize	dw	?	;size of newstack
	newstack	dw	?,?	;new value for SS and SP
	oldstack	dw	?,?	;old value of SS and SP
	funcptr		dw	?	;function pointer (offset portion)
	_ax		dw	?
	_bx		dw	?
	_cx		dw	?
	_dx		dw	?
	_si		dw	?
	_di		dw	?
	_cflag		dw	?
	_flags		dw	?
	_es		dw	?
	_cs		dw	?
	_ss		dw	?
	_ds		dw	?
    ifdef _WINDOWS
	datasel		dw	?
	csalias		dw	?
    endif
intstr	ends
    endif

    ifdef I8086M
intstr	struc
	callf		db	?	;far call
	faraddr		dw	?,?	;far address of interrupt service routine
	prevvec		dw	?,?	;previous vector
	stacksize	dw	?	;size of newstack
	newstack	dw	?,?	;new value for SS and SP
	oldstack	dw	?,?	;old value of SS and SP
	funcptr		dw	?,?	;function pointer
	_ax		dw	?
	_bx		dw	?
	_cx		dw	?
	_dx		dw	?
	_si		dw	?
	_di		dw	?
	_cflag		dw	?
	_flags		dw	?
	_es		dw	?
	_cs		dw	?
	_ss		dw	?
	_ds		dw	?
    ifdef _WINDOWS
	datasel		dw	?
	csalias		dw	?
    endif
intstr	ends
    endif

    ifdef I8086P
intstr	struc
	callf		db	?	;far call
	faraddr		dw	?,?	;far address of interrupt service routine
	prevvec		dw	?,?	;previous vector
	stacksize	dw	?	;size of newstack
	newstack	dw	?,?	;new value for SS and SP
	oldstack	dw	?,?	;old value of SS and SP
	funcptr		dw	?,?	;function pointer
	_ax		dw	?
	_bx		dw	?
	_cx		dw	?
	_dx		dw	?
	_si		dw	?
	_di		dw	?
	_cflag		dw	?
	_flags		dw	?
	_es		dw	?
	_cs		dw	?
	_ss		dw	?
	_ds		dw	?
intstr	ends
    endif

    ifdef I8086C
intstr	struc
	callf		db	?	;far call
	faraddr		dw	?,?	;far address of interrupt service routine
	prevvec		dw	?,?	;previous vector
	stacksize	dw	?	;size of newstack
	newstack	dw	?,?	;new value for SS and SP
	oldstack	dw	?,?	;old value of SS and SP
	staticseg	dw	?	;value for DS
	funcptr		dw	?	;function pointer (offset portion)
	_ax		dw	?
	_bx		dw	?
	_cx		dw	?
	_dx		dw	?
	_si		dw	?
	_di		dw	?
	_cflag		dw	?
	_flags		dw	?
	_es		dw	?
	_cs		dw	?
	_ss		dw	?
	_ds		dw	?
    ifdef _WINDOWS
	datasel		dw	?
	csalias		dw	?
    endif
intstr	ends
    endif

    ifdef I8086L
intstr	struc
	callf		db	?	;far call
	faraddr		dw	?,?	;far address of interrupt service routine
	prevvec		dw	?,?	;previous vector
	stacksize	dw	?	;size of newstack
	newstack	dw	?,?	;new value for SS and SP
	oldstack	dw	?,?	;old value of SS and SP
	staticseg	dw	?	;value for DS
	funcptr		dw	?,?	;function pointer
	_ax		dw	?
	_bx		dw	?
	_cx		dw	?
	_dx		dw	?
	_si		dw	?
	_di		dw	?
	_cflag		dw	?
	_flags		dw	?
	_es		dw	?
	_cs		dw	?
	_ss		dw	?
	_ds		dw	?
    ifdef PROTECTED
	datasel		dw	?
    endif
    ifdef _WINDOWS
	csalias		dw	?
    endif
intstr	ends
    endif

    ifdef I8086V
intstr	struc
	callf		db	?	;far call
	faraddr		dw	?,?	;far address of interrupt service routine
	prevvec		dw	?,?	;previous vector
	stacksize	dw	?	;size of newstack
	newstack	dw	?,?	;new value for SS and SP
	oldstack	dw	?,?	;old value of SS and SP
	staticseg	dw	?	;value for DS
	funcptr		dw	?,?	;function pointer
	_ax		dw	?
	_bx		dw	?
	_cx		dw	?
	_dx		dw	?
	_si		dw	?
	_di		dw	?
	_cflag		dw	?
	_flags		dw	?
	_es		dw	?
	_cs		dw	?
	_ss		dw	?
	_ds		dw	?
intstr	ends
    endif

	begcode	int

    ifndef __OS2__

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get interrupt vector.
; Use:
;	void int_getvector(vectornumber,&offset,&segment);

	c_public	int_getvector
func	int_getvector
	WINENTER
	.save	<SI,DI>
    if SPTR
	push	ES
	getvec	P[BP],ES,BX	;get vector in ES:BX
	mov	SI,P+2[BP]
	mov	[SI],BX
	mov	SI,P+2+SIZEPTR[BP]
	mov	[SI],ES
	pop	ES
    else
	getvec	P[BP],AX,BX	;get vector in AX:BX
	les	SI,P+2[BP]
	mov	ES:[SI],BX
	les	SI,P+2+SIZEPTR[BP]
	mov	ES:[SI],AX
    endif
G1:	.restore <DI,SI>
	WINLEAVE
	ret
c_endp	int_getvector

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set interrupt vector.
; Use:
;	void int_setvector(vectornumber,offset,segment);

	c_public	int_setvector
func	int_setvector
	WINENTER
	.save	<SI,DI>
	mov	DX,P+2[BP]
	setvec	P[BP],P+2+2[BP],DX
	jmps	G1
c_endp	int_setvector

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generic interrupt service routine.
; Figure out who called us, set up the stack, and then call the
; specified function.
; Chain to previous vector if funcptr returns a 0.
; This function is not callable by C, it is public so debuggers will
; know about it.

	c_public	int_service
	even
func	int_service
	push	BP
	mov	BP,SP
	push	DS
	push	BX
	lds	BX,2[BP]		;DS:BX -> intstr+5 (!)
     ifdef PROTECTED
	mov	DS,datasel-5[BX]	;must use DSAlias not CSAlias!
     endif
	mov	_ax-5[BX],AX
	pop	_bx-5[BX]
	mov	_cx-5[BX],CX
	mov	_dx-5[BX],DX
	mov	_si-5[BX],SI
	mov	_di-5[BX],DI
;	pushf
;	pop	_flags-5[BX]
	mov	AX,10[BP]		;get flags from caller
	mov	_flags-5[BX],AX
	mov	_es-5[BX],ES
	mov	_cs-5[BX],CS
	mov	_ss-5[BX],SS
	pop	_ds-5[BX]
	_if	<newstack-5[BX]> e 0, S4	;if keep existing stack
	mov	oldstack-5[BX],SP
	mov	oldstack+2-5[BX],SS	;remember old stack
	cli				;for bug in old 8088's
	mov	SS,newstack+2-5[BX]
	mov	SP,newstack-5[BX]	;set new stack
	;sti				;for bug in old 8088's
S4:	push	BX
	cld				;no bugs with backwards direction flag

    if SPTR
	push	DS
	pop	ES			;set ES to be same as DS
	push	BX			;push offset of struct INT_DATA
    if LCODE
	call	dword ptr funcptr-5[BX]
    else
	call	word ptr funcptr-5[BX]
    endif
	add	SP,SIZEPTR
    else ;LPTR
	push	DS
	push	DS
	pop	ES
	mov	DS,staticseg-5[BX]	;set static data segment
	push	ES			;push segment of struct SREGS
	push	BX			;push offset of struct INT_DATA
    if LCODE
	call	ES:dword ptr funcptr-5[BX]
    else
	call	ES:word ptr funcptr-5[BX]
    endif
	add	SP,SIZEPTR
	pop	DS
    endif
	pop	BX
	_if	<newstack-5[BX]> e 0, S5	;if keep existing stack
	cli				;for bug in old 8088's
	mov	SS,oldstack+2-5[BX]
	mov	SP,oldstack-5[BX]	;restore interrupted stack
;	sti				;for bug in old 8088's
S5:	tst	AX			;chain?
	jnz	S1			;no
	mov	AX,prevvec+2-5[BX]	;segment of previous vector
	mov	2+2[BP],AX
	mov	AX,prevvec-5[BX]	;offset of previous vector
	mov	2[BP],AX		;so retf at S2 will jump to prev vector
	;Get register return values
	push	_flags-5[BX]		;so _popf will pick up this for flags
	jmps	S3

S1:	mov	AX,_flags-5[BX]
	mov	10[BP],AX		;so iret will pick up this for flags

S3:	push	_ds-5[BX]
	mov	ES,_es-5[BX]
	mov	DI,_di-5[BX]
	mov	SI,_si-5[BX]
	mov	DX,_dx-5[BX]
	mov	CX,_cx-5[BX]
	mov	AX,_ax-5[BX]
	mov	BX,_bx-5[BX]
	pop	DS

	jz	S2		;if chain
;_popf
	pop	BP
	add	SP,4		;pop off address of far call to us
	iret

S2:	_popf
	pop	BP
	.retf
c_endp      int_service

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Intercept a vector.
; Use:
;	int int_intercept(unsigned vectornumber,int (*funcptr)(struct INT_DATA *),
;		unsigned stacksize);
; Input:
;	funcptr	Pointer to a function which returns:
;		0:	chain to previous interrupt vector
;		!=0:	return from interrupt
;	stacksize	Size of stack to allocate for interrupt service
;			routine. If 0, no new stack is allocated, and
;			the existing stack is used for the isr.
; Returns:
;	0	success
;	-1	failure

	c_public	int_intercept
func	int_intercept
	WINENTER
	.save	<SI,DI>
	mov	AX,size intstr
	push	AX
	callm	malloc			;allocate thunk
	pop	BX
    if SPTR
	tst	AX			;error allocating?
	jz	err1			;yes
	mov	SI,AX
	mov	callf[SI],09Ah			;opcode for far call
	mov	faraddr[SI],offset int_service	;offset of interrupt server
	mov	faraddr+2[SI],CS		;segment of interrupt server
    if LCODE
	mov	AX,P+2+4[BP]		;get stacksize
    else
	mov	AX,P+2+2[BP]		;get stacksize
    endif
	mov	stacksize[SI],AX	; RPJ - fix problem with 0-sized stacks
	cwd				;in case AX is 0, set DX to 0
	tst	AX			;special value?
	jz	i4			;no
	_if	AX ae 128, i2
	mov	AX,128			;128 bytes minimum size
i2:
	mov	stacksize[SI],AX	;remember stack size
	push	AX
	callm	malloc
	pop	BX
	tst	AX
	jnz	noe
	jmp	freethunk

err1:	jmp	err			;error allocating for stack

noe:	add	AX,stacksize[SI]	;adjust SP to be past top of stack
	mov	DX,SS			;value for SS
i4:	mov	newstack[SI],AX		;value for SP at start of interrupt
	mov	newstack+2[SI],DX	;value for SS
	mov	AX,P+2[BP]
	mov	funcptr[SI],AX
    if LCODE
	mov	AX,P+2+2[BP]
	mov	funcptr+2[SI],AX	;get segment of function
    endif

	getvec	P[BP],ES,BX		;get old vector in ES:BX
	mov	prevvec[SI],BX
	mov	prevvec+2[SI],ES	;store in struct

    ifdef _WINDOWS
	mov	datasel[SI],DS
	mov	word ptr csalias[SI],0
	push	AX		; vector number
	push	SI		; offset of thunk
	push	DS		; DS selector for segment of thunk
	push	DS		; pass DS selector to alias maker
	call	ALLOCDSTOCSALIAS	; Create CS alias to thunk
	pop	DS		; restore DS selector for segment of thunk
	pop	SI		; restore offset of thunk
	pop	DX		; restore vector number
	tst	AX		; AX == 0, no selector created
	jz	aliasCreationError
	push	DS		; save DS
	mov	csalias[SI],AX	; save CS alias for FREESELECTOR when original function is restored
	mov	DS,AX		; move CS alias to DS
	mov	AX,DX		; AX = vector number
    endif ;_WINDOWS
	setvec	AL,DS,SI		;set vector to DS:SI
    ifdef _WINDOWS
	pop	DS
    endif ;_WINDOWS
    else ;LPTR
	tst	DX			;error allocating?
	jz	err1			;yes
	mov	ES,DX
	mov	SI,AX
	mov	ES:callf[SI],09Ah		;opcode for far call
	mov	ES:faraddr[SI],offset int_service ;offset of interrupt server
	mov	ES:faraddr+2[SI],CS		;segment of interrupt server
	mov	ES:staticseg[SI],DS		;segment of static data
    if LCODE
	mov	AX,P+2+4[BP]		;get stacksize
    else
	mov	AX,P+2+2[BP]		;get stacksize
    endif
	mov	ES:stacksize[SI],AX	; RPJ - fix problem with 0-sized stacks
	cwd				;in case AX is 0, set DX to 0
	tst	AX			;use existing stack?
	jz	i4			;yes
	_if	AX ae 128, i2
	mov	AX,128			;128 bytes minimum size
i2:
	mov	ES:stacksize[SI],AX	;remember stack size
	_push	<ES>
	push	AX
	callm	malloc
	pop	BX
	_pop	<ES>
	tst	DX
	jnz	noe
	jmp	freethunk

err1:	jmp	err			;error allocating for stack

noe:	add	AX,ES:stacksize[SI]	;adjust SP to be past top of stack
i4:	mov	ES:newstack[SI],AX	;value for SP at start of interrupt
	mov	ES:newstack+2[SI],DX	;value for SS
	mov	AX,P+2[BP]		;get offset of function
	mov	ES:funcptr[SI],AX
    if LCODE
	mov	AX,P+2+2[BP]
	mov	ES:funcptr+2[SI],AX	;get segment of function
    endif

	push	ES
	getvec	P[BP],CX,BX		;get old vector in CX:BX
	pop	ES
	mov	ES:prevvec[SI],BX
	mov	ES:prevvec+2[SI],CX	;store in struct

    ifdef DOS16RM
	mov	ES:datasel[SI],ES
	push	AX			;vector number
	push	ES
	push	SI			;ES:SI -> thunk
	callm	D16SegCSAlias		;create CS alias for thunk
	tst	DX
	pop	SI
	pop	ES
	pop	AX
	jz	aliasCreationError
	push	DS
	mov	DS,DX			;interrupt vector must be set to CSAlias
    else ;DOS16RM
    ifdef _WINDOWS
	mov	ES:datasel[SI],ES
	mov	word ptr ES:csalias[SI],0
	push	AX		; vector number
	push	SI		; offset of thunk
	push	ES		; DS selector for segment of thunk
	push	ES		; pass DS selector to alias maker
	call	ALLOCDSTOCSALIAS	; Create CS alias to thunk
	pop	ES		; restore DS selector for segment of thunk
	pop	SI		; restore offset of thunk
	pop	DX		; restore vector number
	tst	AX		; AX == 0, no selector created
	jz	aliasCreationError
	push	DS		; save DS
	mov	DS,AX		; move CS alias to DS
	mov	ES:csalias[SI],AX	; save CS alias for FREESELECTOR when original function is restored
	mov	AX,DX		; AX = vector number
    else ;_WINDOWS
	push	DS
	push	ES
	pop	DS
    endif ;_WINDOWS
    endif ;DOS16RM
	setvec	AL,DS,SI	;set vector to DS:SI
	pop	DS		; restore DS
    endif ;SPTR
	clr	AX		;success

i1:
	.restore <DI,SI>
	WINLEAVE
	ret

    ifdef PROTECTED
aliasCreationError:
    ifdef SPTR
	push	DS
	pop	ES
    endif
	mov	AX,ES:stacksize[SI]
	tst	AX
	jz	freethunk
	sub	AX,ES:newstack[SI]
	neg	AX			;AX = offset of start of stack
    ifdef LPTR
	push	ES:newstack+2[SI]	;segment of stack
    endif
	push	AX
	callm	free				;free isr stack
	add	SP,SIZEPTR
    endif

freethunk:
    if LPTR
	push	ES
    endif
	push	SI
	callm	free			;free thunk
	add	SP,SIZEPTR

err:	mov	AX,-1
	jmp	i1
c_endp	int_intercept

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Restore previous interrupt vector.
; Use:
;	void int_restore(unsigned vectornumber);

	c_public	int_restore
func	int_restore
	WINENTER
	.save	<SI,DI>
	getvec	P[BP],ES,BX	;ES:BX -> intstr (also, ES == DS if req'd)
    if SPTR
    ifdef _WINDOWS
        push	ES:datasel[BX]
        pop	ES
    endif ;_WINDOWS
	mov	AX,stacksize[BX]
	tst	AX
	jz	nostack
	mov	AX,newstack[BX]
	sub	AX,stacksize[BX]
	push	BX		;save BX
	push	AX
	callm	free		;free allocated stack
	pop	CX
	pop	BX
nostack:
    ifdef _WINDOWS
	cmp	word ptr csalias[BX],0
	je	noselector
	  push	  ES
	  push	  BX
	  push	  word ptr csalias[BX]	; pass in CS alias selector that we no longer need
	  call	  FREESELECTOR
	  pop	  BX
	  pop	  ES
noselector:
    endif ;_WINDOWS
	mov	DX,prevvec[BX]
	setvec	P[BP],prevvec+2[BX],DX	;restore previous vector
	push	BX
	callm	free		;free intstr
	pop	BX
    else
	mov	AX,ES:stacksize[BX]
	tst	AX
	jz	nostack
	mov	AX,ES:newstack[BX]
	sub	AX,ES:stacksize[BX]
	_push	<ES,BX>			;save ES,BX
	push	ES:newstack+2[BX]	;segment of allocated stack
	push	AX			;offset of allocated stack
	callm	free			;free allocated stack
	add	SP,SIZEPTR
	_pop	<BX,ES>
nostack:
	mov	DX,ES:prevvec[BX]
	setvec	P[BP],ES:prevvec+2[BX],DX	;restore previous vector
     ifdef PROTECTED
	push	word ptr ES:datasel[BX]	;data segment of intstr
	push	BX			;offset of intstr
     ifdef _WINDOWS
	cmp	word ptr ES:csalias[BX],0
	je	noselector
	  push	  word ptr ES:csalias[BX]	; pass in CS alias selector that we no longer need
	  call	  FREESELECTOR
noselector:
     endif
     ifdef DOS16RM
	push	ES		; ES is the code segment alias
	push	BX		; garbage, this function needs a far pointer,
				; only affects selector though
	call	D16SegCancel
	pop	BX		; clean up stack
	pop	BX
     endif
	;note that now top of stack contains ES:datasel[BX],BX
     else ;PROTECTED
	push	ES		;segment of intstr
	push	BX		;offset of intstr
     endif ;PROTECTED
	callm	free		;free intstr
	add	SP,SIZEPTR
    endif ;SPTR
	clr	AX		;backwards compatibility, old version returned 0
	.restore <DI,SI>
	WINLEAVE
	ret
c_endp	int_restore

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Turn interrupts on/off.

	c_public int_on
func	int_on
	sti
	ret
c_endp	int_on

	c_public int_off
func	int_off
	cli
	ret
c_endp	int_off

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Call previous interrupt service routine (isr) before we intercepted it.
;	long int_prev(struct INT_DATA *p);
; Input:
;	p	Pointer to struct INT_DATA, which gives the address of the
;		routine to call and the values for the registers.
; Output:
;	*p	Is filled in with the values of the registers and flags
;		after the routine returned.
; Returns:
;	values that DX and AX had after the isr returned

	c_public	int_prev
func	int_prev
	WINENTER
	.save	<SI,DI>
	push	DS

	;for return from interrupt, we need:
	;	flags
	;	segment
	;	offset

	;Set up so return from interrupt goes to intretx
	pushf
	pop	DX
	push	DX		;flags
	push	CS		;segment
	mov	AX,offset intretx
	push	AX		;offset

    if SPTR
	mov	BX,P[BP]	;INT_DATA *
    else
	lds	BX,P[BP]
    endif

	;Put flags,seg,off on stack so an iret will call the interrupt
	and	DH,0Ch		;reset T and I flags
	push	DX		;flags
	push	prevvec+2-5[BX]	;seg
	push	prevvec-5[BX]	;offset

	mov	ES,_es-5[BX]	;value of ES for isr
	push	_ds-5[BX]	;value of DS for isr
	mov	AX,_ax-5[BX]
	mov	CX,_cx-5[BX]
	mov	DX,_dx-5[BX]
	mov	SI,_si-5[BX]
	mov	DI,_di-5[BX]
	mov	BX,_bx-5[BX]
	pop	DS
	iret			;call the previous isr

intretx:
	mov	BP,SP
	push	BX
	push	DS
    if SPTR
	mov	DS,[BP]			;default DS
	mov	BX,6+P[BP]		;INTDATA *
    else
	lds	BX,6+P[BP]		;INTDATA *
    endif
	pop	_ds-5[BX]		;value of DS after isr
	mov	_es-5[BX],ES		;value of ES after isr

	pushf
	pop	_flags-5[BX]
	mov	_ax-5[BX],AX
	mov	_cx-5[BX],CX
	mov	_dx-5[BX],DX
	mov	_si-5[BX],SI
	mov	_di-5[BX],DI
	pop	_bx-5[BX]
	sbb	CX,CX		;note that status of C is maintained
	mov	_cflag-5[BX],CX	;set _cflag to non-zero if carry was set
	cld			;C rules state that direction flag is forward
	pop	DS
	.restore <DI,SI>
	WINLEAVE
	ret
c_endp	int_prev

    endif ;__OS2__

	endcode	int

	end

