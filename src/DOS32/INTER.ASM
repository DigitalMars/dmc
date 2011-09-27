;_ inter.asm   Mon Aug 20 1990   Modified by: Walter Bright */
; Copyright (c) 1986-1990 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

; Interrupt handling package

include	MACROS.ASM

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Macro to set interrupt vector vecnum into the interrupt vector table
; from vecseg and vecoff.

setvec	macro	vecnum,vecseg,vecoff
      ifdif <vecseg>,<DS>
	push	DS
      endif
      ifdif <vecnum>,<CL>
	mov	CL,vecnum
      endif
      ifdif <vecoff>,<EDX>
	mov	EDX,vecoff
      endif
      ifdif <vecseg>,<DS>
	mov	DS,vecseg
      endif
	mov	AX,2506h	;set protected mode interrupt vector
	int	21h
      ifdif <vecseg>,<DS>
	pop	DS
      endif
	endm

;These structures should match union REGS and struct SREGS in dos.h
REGS	macro
	_ax		dd	?
	_bx		dd	?
	_cx		dd	?
	_dx		dd	?
	_si		dd	?
	_di		dd	?
	_cflag		dd	?
	_flags		dd	?
	endm

SREGS	macro
	_es		dw	?
	_cs		dw	?
	_ss		dw	?
	_ds		dw	?
	_fs		dw	?
	_gs		dw	?
	endm

; For unfathomable reasons, if's don't work in structs.

TSIZE	equ	7
    ifdef I8086S
INT_DATA	struc
	callf		db	?	;far call
	faraddr		df	?	;far address of interrupt service routine
	dummy		db	?	;to dword align the rest
	prevvec		df	?	;previous vector
	prevvecr	dd	?	;previous real mode vector
	stacksize	dd	?	;size of newstack
	newstack	df	?	;new value for SS and SP
	oldstack	df	?	;old value of SS and SP
	funcptr		dd	?	;function pointer (offset portion)
	_ax		dd	?
	_bx		dd	?
	_cx		dd	?
	_dx		dd	?
	_si		dd	?
	_di		dd	?
	_cflag		dd	?
	_flags		dd	?
	_es		dw	?
	_cs		dw	?
	_ss		dw	?
	_ds		dw	?
	_fs		dw	?
	_gs		dw	?
	datasel		dw	?
INT_DATA	ends
    endif

    ifdef I8086M
INT_DATA	struc
	callf		db	?	;far call
	faraddr		df	?	;far address of interrupt service routine
	dummy		db	?	;to dword align the rest
	prevvec		df	?	;previous vector
	prevvecr	dd	?	;previous real mode vector
	stacksize	dd	?	;size of newstack
	newstack	df	?	;new value for SS and SP
	oldstack	df	?	;old value of SS and SP
	funcptr		df	?	;function pointer
	_ax		dd	?
	_bx		dd	?
	_cx		dd	?
	_dx		dd	?
	_si		dd	?
	_di		dd	?
	_cflag		dd	?
	_flags		dd	?
	_es		dw	?
	_cs		dw	?
	_ss		dw	?
	_ds		dw	?
	_fs		dw	?
	_gs		dw	?
INT_DATA	ends
    endif

    ifdef I8086C
INT_DATA	struc
	callf		db	?	;far call
	faraddr		df	?	;far address of interrupt service routine
	dummy		db	?	;to dword align the rest
	prevvec		df	?	;previous vector
	prevvecr	dd	?	;previous real mode vector
	stacksize	dd	?	;size of newstack
	newstack	df	?	;new value for SS and SP
	oldstack	df	?	;old value of SS and SP
	staticseg	dw	?	;value for DS
	funcptr		dd	?	;function pointer (offset portion)
	_ax		dd	?
	_bx		dd	?
	_cx		dd	?
	_dx		dd	?
	_si		dd	?
	_di		dd	?
	_cflag		dd	?
	_flags		dd	?
	_es		dw	?
	_cs		dw	?
	_ss		dw	?
	_ds		dw	?
	_fs		dw	?
	_gs		dw	?
INT_DATA	ends
    endif

    ifdef I8086L
INT_DATA	struc
	callf		db	?	;far call
	faraddr		df	?	;far address of interrupt service routine
	dummy		db	?	;to dword align the rest
	prevvec		df	?	;previous vector
	prevvecr	dd	?	;previous real mode vector
	stacksize	dd	?	;size of newstack
	newstack	df	?	;new value for SS and SP
	oldstack	df	?	;old value of SS and SP
	staticseg	dw	?	;value for DS
	funcptr		df	?	;function pointer
	_ax		dd	?
	_bx		dd	?
	_cx		dd	?
	_dx		dd	?
	_si		dd	?
	_di		dd	?
	_cflag		dd	?
	_flags		dd	?
	_es		dw	?
	_cs		dw	?
	_ss		dw	?
	_ds		dw	?
	_fs		dw	?
	_gs		dw	?
	datasel		dw	?
INT_DATA	ends
    endif
    begdata
intercept_count dd 0    ;; For memory locking and unlocking purporses
    enddata

    if LCODE
	extrn	_malloc:far, _free:far
    else
	extrn	_malloc:near, _free:near, __x386_memlock:near, __x386_memunlock:near
    endif

	begcode	int

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get protected mode interrupt vector.
; Use:
;	void int_getvector(unsigned vectornumber,unsigned *offset,unsigned *segment);

	c_public	int_getvector
func	int_getvector
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ESI>
	push	ES
 	mov	CL,P[EBP]
	mov	AX,2502h
	int	21h		;get interrupt vector in ES:EBX
	mov	ESI,P+4[EBP]
	mov	[ESI],EBX
	mov	ESI,P+4+SIZEPTR[EBP]
	clr	EAX
	mov	EAX,ES
	mov	[ESI],EAX
	pop	ES
	unuse	<ESI,EBX>
	pop	EBP
	ret
c_endp	int_getvector

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set protected mode interrupt vector for both real and protected mode
; interrupts.
; Use:
;	void __cdecl int_setvector(unsigned vectornumber,unsigned offset,
;		unsigned segment);

	c_public	int_setvector
func	int_setvector
	push	EBP
	mov	EBP,ESP
	
        push	DS
	mov	CL,P[EBP]
	lds	EDX,P+4[EBP]	;vector
	mov	AX,2506h	;set protected mode interrupt vector
	int	21h
	pop	DS

	pop	EBP
	ret
c_endp	int_setvector

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generic interrupt service routine.
; Figure out who called us, set up the stack, and then call the
; specified function.
; Chain to previous vector if funcptr returns a 0.
; This function is not callable by C or C++, it is public so debuggers will
; know about it.

	c_public	int_service
	_align
func    int_service  
	push	EBP
	mov	EBP,ESP
	push	DS
	push	EBX
	lds	EBX,4[EBP]		;DS:EBX -> INT_DATA+TSIZE (!)
	mov	DS,word ptr datasel[EBX-TSIZE]	;must use DSAlias not CSAlias!
	mov	_ax[EBX-TSIZE],EAX
	pop	_bx[EBX-TSIZE]
	mov	_cx[EBX-TSIZE],ECX
	mov	_dx[EBX-TSIZE],EDX
	mov	_si[EBX-TSIZE],ESI
	mov	_di[EBX-TSIZE],EDI
;	pushfd
;	pop	dword ptr _flags[EBX-TSIZE]
	mov	EAX,20[EBP]		;get flags from caller
	mov	_flags[EBX-TSIZE],EAX
	pop	dword ptr _ds[EBX-TSIZE]	;this will step on _fs
	mov	_fs[EBX-TSIZE],FS
	mov	_gs[EBX-TSIZE],GS
	mov	_es[EBX-TSIZE],ES
	mov	_cs[EBX-TSIZE],CS
	mov	_ss[EBX-TSIZE],SS
	_ifs	<word ptr newstack+4[EBX-TSIZE]> e 0, S4 ;if keep existing stack
	mov	dword ptr oldstack[EBX-TSIZE],ESP
	mov	word  ptr oldstack+4[EBX-TSIZE],SS	;remember old stack
	lss	ESP,newstack[EBX-TSIZE]	;set new stack
S4:	push	EBX
	cld				;no bugs with backwards direction flag

    if SPTR
	push	DS
	pop	ES			;set ES to be same as DS
	push	EBX			;push offset of struct INT_DATA
    if LCODE
	call	fword ptr funcptr[EBX-TSIZE]
    else
	call	dword ptr funcptr[EBX-TSIZE]
    endif
	add	ESP,SIZEPTR
    else ;LPTR
	push	DS
	push	DS
	pop	ES
	mov	DS,staticseg[EBX-TSIZE]	;set static data segment
	push	ES			;push segment of struct SREGS
	push	EBX			;push offset of struct INT_DATA
    if LCODE
	call	ES:fword ptr funcptr[EBX-TSIZE]
    else
	call	ES:dword ptr funcptr[EBX-TSIZE]
    endif
	add	ESP,SIZEPTR
	pop	DS
    endif
	pop	EBX
	_ifs	<word ptr newstack+4[EBX-TSIZE]> e 0, S5 ;if keep existing stack
	lss	ESP,oldstack[EBX-TSIZE]	;restore interrupted stack
S5:	tst	EAX			;chain?
	jnz	short S1		;no
	mov	AX,word ptr prevvec+4[EBX-TSIZE] ;segment of previous vector
	mov	4+4[EBP],AX
	mov	EAX,dword ptr prevvec[EBX-TSIZE]	;offset of previous vector
	mov	4[EBP],EAX		;so retf at S2 will jump to prev vector
	;Get register return values
	push	_flags[EBX-TSIZE]		;so popfd will pick up this for flags
	jmps	S3

S1:	mov	EAX,_flags[EBX-TSIZE]
	mov	20[EBP],EAX		;so iretd will pick up this for flags

S3:	push	dword ptr _ds[EBX-TSIZE]
	mov	ES,_es[EBX-TSIZE]
	mov	FS,_fs[EBX-TSIZE]
	mov	GS,_gs[EBX-TSIZE]
	mov	EDI,_di[EBX-TSIZE]
	mov	ESI,_si[EBX-TSIZE]
	mov	EDX,_dx[EBX-TSIZE]
	mov	ECX,_cx[EBX-TSIZE]
	mov	EAX,_ax[EBX-TSIZE]
	mov	EBX,_bx[EBX-TSIZE]
	pop	DS

	jz	short S2	;if chain
;popfd
	pop	EBP
	add	ESP,8		;pop off address of far call to us
	iretd

S2:	popfd
	pop	EBP
int_service_ret:
	db	0CBh		;retf
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
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ESI,EDI,ES>
	mov	EAX,size INT_DATA
	push	EAX
	call	_malloc			;allocate thunk
	pop	ECX
    if SPTR
	tst	EAX			;error allocating?
	jz	err1			;yes
	mov	ESI,EAX
	mov	callf[ESI],09Ah			;opcode for far call
	mov	dword ptr faraddr[ESI],offset int_service	;offset of interrupt server
	mov	word ptr faraddr+4[ESI],CS		;segment of interrupt server
    if LCODE
	mov	EAX,P+4+8[EBP]		;get stacksize
    else
	mov	EAX,P+4+4[EBP]		;get stacksize
    endif
	mov	stacksize[ESI],EAX	; RPJ - fix problem with 0-sized stacks
	cdq				;in case EAX is 0, set EDX to 0
	tst	EAX			;special value?
	jz	i4			;no
	_ifs	EAX ae 128, i2
	mov	EAX,128			;128 bytes minimum size
i2:
	mov	stacksize[ESI],EAX	;remember stack size
	push	EAX
	call	_malloc
	pop	ECX
	tst	EAX
	jnz	noe
	jmp	freethunk

err1:	jmp	err			;error allocating for stack

noe:	add	EAX,stacksize[ESI]	;adjust ESP to be past top of stack
	mov	DX,DS			;value for SS
i4:	mov	dword ptr newstack[ESI],EAX	;value for ESP at start of interrupt
	mov	word ptr newstack+4[ESI],DX	;value for SS
	mov	EAX,P+4[EBP]
	mov	funcptr[ESI],EAX
    if LCODE
	mov	AX,P+4+4[EBP]
	mov	funcptr+4[ESI],AX	;get segment of function
    endif

	;Save real and protected mode interrupt vectors into INT_DATA
	mov	CL,P[EBP]		;interrupt number
	mov	AX,2503h
	int	21h			;get real vector in EBX
	mov	dword ptr prevvecr[ESI],EBX
	mov	AX,2502h
	int	21h			;get protected vector in ES:EBX
	mov	dword ptr prevvec[ESI],EBX
	mov	word ptr prevvec+4[ESI],ES	;store in struct

	mov	datasel[ESI],DS		;remember data selector
	
        ;; Lock code & data used by interrupt
        push	ECX
        push	ESI
        push ESI    
		lea  ecx, __x386_memlock
		push ecx
        call inter_mem
        inc intercept_count
        pop	ESI
        pop	ECX

	push	CS
	push	ESI
	push	ECX
	call	_int_setvector
	pop	EAX
	pop	EAX
	pop	EAX
    else ;LPTR
	tst	EDX			;error allocating?
	jz	err1			;yes
	mov	ES,DX
	mov	ESI,EAX
	mov	ES:callf[ESI],09Ah	;opcode for far call
	mov	ES:faraddr[ESI],offset int_service ;offset of interrupt server
	mov	ES:faraddr+4[ESI],CS	;segment of interrupt server
	mov	ES:staticseg[ESI],DS	;segment of static data
    if LCODE
	mov	EAX,P+4+8[EBP]		;get stacksize
    else
	mov	EAX,P+4+4[EBP]		;get stacksize
    endif
	mov	ES:stacksize[ESI],EAX	; RPJ - fix problem with 0-sized stacks
	cdq				;in case EAX is 0, set EDX to 0
	tst	EAX			;use existing stack?
	jz	i4			;yes
	_ifs	EAX ae 128, i2
	mov	EAX,128			;128 bytes minimum size
i2:
	mov	ES:stacksize[ESI],EAX	;remember stack size
	push	ES
	push	EAX
	call	_malloc
	pop	ECX
	pop	ES
	tst	EDX
	jnz     noe
	jmp	freethunk

err1:	jmp	err			;error allocating for stack

noe:	add	EAX,ES:stacksize[ESI]	;adjust ESP to be past top of stack
i4:	mov	dword ptr ES:newstack[ESI],EAX	;value for ESP at start of interrupt
	mov	word ptr ES:newstack+4[ESI],DX	;value for SS
	mov	EAX,P+4[EBP]		;get offset of function
	mov	ES:funcptr[ESI],EAX
    if LCODE
	mov	AX,P+4+4[EBP]
	mov	ES:funcptr+4[ESI],AX	;get segment of function
    endif

	;Save real and protected mode interrupt vectors
	mov	CL,P[EBP]		;interrupt number
	mov	AX,2503h
	int	21h			;get real vector in EBX
	mov	word ptr ES:prevvecr[ESI],EBX
	push	ES
	mov	AX,2502h
	int	21h			;get protected vector in ES:EBX
	mov	AX,ES
	pop	ES
	mov	dword ptr ES:prevvec[ESI],EBX
	mov	word ptr ES:prevvec+4[ESI],AX	;store in struct

    ifdef DOS16RM
	push	AX
	mov	ES:datasel[ESI],ES
	push	ES
	push	ESI			;ES:ESI -> thunk
	call	_D16SegCSAlias		;create CS alias for thunk
	tst	DX
	pop	ESI
	pop	ES
	pop	AX
	jz	dos16err	       
	push	DS
	mov	DS,DX		;interrupt vector must be set to CSAlias
    else
	push	DS
	push	ES
	pop	DS
    endif
	setvec	CL,DS,ESI	;set vector to DS:ESI
	pop	DS
    endif
	clr	EAX		;success

i1:
	unuse <ES,EDI,ESI,EBX>
	pop	EBP
	ret

    ifdef DOS16RM
dos16err:
	mov	AX,ES:stacksize[ESI]
	tst	AX
	jz	freethunk
	sub	AX,ES:newstack[ESI]
	neg	AX			;AX = offset of start of stack
	push	ES:newstack+2[ESI]	;segment of stack
	push	AX
	call	_free		  	;free isr stack
	add	ESP,4
    endif

freethunk:
    if LPTR
	push	ES
    endif
	push	ESI
	call	_free			;free thunk
	add	ESP,SIZEPTR

err:	mov	EAX,-1
	jmp	i1
c_endp	int_intercept


;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Restore previous interrupt vector.
; Use:
;	void int_restore(unsigned vectornumber);

	c_public	int_restore
func	int_restore
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ESI,EDI>

	mov	CL,P[EBP]
	mov	AX,2502h	;get protected mode interrupt vector
	int	21h		;read vector in ES:EBX
    if SPTR
	;Note at this point ES==CS
	push	DS
	pop	ES		;set ES==DS
	mov	EAX,stacksize[EBX]
	tst	EAX
	jz	nostack
	mov	EAX,dword ptr newstack[EBX]
	sub	EAX,stacksize[EBX]
	push	EAX
	call	_free		;free allocated stack
	pop	EAX
nostack:
	;Restore previous real and protected mode interrupt vectors
	push	EBX				;arg for free()
	push	DS
	mov	CL,P[EBP]			;CL = interrupt number
	mov	EAX,dword ptr prevvecr[EBX]	;EAX = real interrupt vector
	lds	EDX,fword ptr prevvec[EBX]	;DS:EDX = protected vector
	mov	EBX,EAX
	mov	AX,2507h
	int	21h				;restore vectors
	pop	DS

        ;; Unlock data and code used by interrupt
        dec intercept_count
        push EBX  ;; INT_DATA
		lea 	ebx, __x386_memunlock
		push	ebx
        call inter_mem

	call	_free				;free INT_DATA (EBX)
	pop	EBX
    else
	mov	EAX,stacksize[EBX]
	tst	EAX
	jz	nostack
	mov	EAX,dword ptr ES:newstack[EBX]
	sub	EAX,ES:stacksize[EBX]
	_push	<ES,EBX>			;save ES,EBX
	push	dword ptr ES:newstack+4[EBX]	;segment of allocated stack
	push	EAX				;offset of allocated stack
	call	_free				;free allocated stack
	add	ESP,SIZEPTR
	_pop	<EBX,ES>
nostack:
	mov	EDX,ES:prevvec[EBX]
	setvec	P[EBP],ES:prevvec+4[EBX],EDX	;restore previous vector
     ifdef DOS16RM
	push	dword ptr ES:datasel[EBX]
     else
	push	ES		;segment of INT_DATA
     endif
	push	EBX		;offset of INT_DATA
	call	_free		;free INT_DATA
	add	ESP,SIZEPTR
    endif
	clr	EAX		;backwards compatibility, old version returned 0
	unuse	<EDI,ESI,EBX>
	pop	EBP
	ret
c_endp	int_restore


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Turn interrupts on/off.

	c_public	int_on
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
; Call previous protected mode interrupt service routine (isr) before
; we intercepted it.
;	long int_prev(struct INT_DATA *p);
; Input:
;	p	Pointer to struct INT_DATA, which gives the address of the
;		routine to call and the values for the registers.
; Output:
;	*p	Is filled in with the values of the registers and flags
;		after the routine returned.
; Returns:
;	values that EDX and EAX had after the isr returned

	c_public	int_prev
func	int_prev
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ESI,EDI,ES>
	push	DS

	;for return from interrupt, we need:
	;	flags
	;	segment
	;	offset

	;Set up so return from interrupt goes to intretx
	pushfd
	pop	EDX
	push	EDX		;flags
	push	CS		;segment
	push	offset intretx	;offset

    if SPTR
	mov	EBX,P[EBP]	;INT_DATA *
    else
	lds	EBX,P[EBP]
    endif

	;Put flags,seg,off on stack so an iretd will call the interrupt
	and	DH,0Ch				;reset T and I flags
	push	EDX				;flags
	push	dword ptr prevvec+4[EBX-TSIZE]	;seg
	push	dword ptr prevvec[EBX-TSIZE]	;offset

	mov	ES,_es[EBX-TSIZE]		;value of ES for isr
	mov	FS,_fs[EBX-TSIZE]		;value of FS for isr
	mov	GS,_gs[EBX-TSIZE]		;value of GS for isr
	push	dword ptr _ds[EBX-TSIZE]	;value of DS for isr
	mov	EAX,_ax[EBX-TSIZE]
	mov	ECX,_cx[EBX-TSIZE]
	mov	EDX,_dx[EBX-TSIZE]
	mov	ESI,_si[EBX-TSIZE]
	mov	EDI,_di[EBX-TSIZE]
	mov	EBX,_bx[EBX-TSIZE]
	pop	DS
	iretd			;call the previous isr

intretx:
	mov	EBP,ESP
	push	EBX
	push	DS
    if SPTR
	mov	DS,[EBP]		;default DS
	mov	EBX,20+P[EBP]		;INTDATA *
    else
	lds	EBX,20+P[EBP]		;INTDATA *
    endif
	pop	dword ptr _ds[EBX-TSIZE]	;value of DS after isr
	mov	_es[EBX-TSIZE],ES		;value of ES after isr
	mov	_fs[EBX-TSIZE],FS		;value of FS after isr
	mov	_gs[EBX-TSIZE],GS		;value of GS after isr

	pushfd
	pop	dword ptr _flags[EBX-TSIZE]
	mov	_ax[EBX-TSIZE],EAX
	mov	_cx[EBX-TSIZE],ECX
	mov	_dx[EBX-TSIZE],EDX
	mov	_si[EBX-TSIZE],ESI
	mov	_di[EBX-TSIZE],EDI
	pop	dword ptr _bx[EBX-TSIZE]
	sbb	ECX,ECX			;note that status of C is maintained
	mov	_cflag[EBX-TSIZE],ECX	;set _cflag to non-zero if carry was set
	cld				;C rules state that direction flag is forward
	pop	DS
	unuse	<ES,EDI,ESI,EBX>
	pop	EBP
int_prev_ret:
	ret
c_endp	int_prev

inter_mem  proc near
       push ESI
       push EDI
       mov ESI,12[ESP]  ;; unlock or lock
       mov EDI,16[ESP]  ;; Used to get data to lock
       
       cmp  intercept_count, 0
       jne   data_only

       mov  EAX, offset int_service_ret[1] ;offset of last instruction in isr
       sub  EAX, offset int_service ;calculate length of isr
       push EAX  
       push CS
       push offset int_service
       call ESI
       lea  ESP,[ESP+12]    ;pop parameters off the stack
       
       mov  EAX, offset int_prev_ret[1] ;offset of last instruction in isr
       sub  EAX, offset int_prev ;calculate length of isr
       push EAX  
       push CS
       push offset int_prev
       call ESI
       lea  ESP,[ESP+12]    ;pop parameters off the stack

data_only:
       mov  EAX, size INT_DATA
       push EAX
       push DS
       push EDI
       call ESI
       lea ESP,[ESP+12]

       pop EDI
       pop ESI
       ret 8
inter_mem  endp


	endcode	int

	end

