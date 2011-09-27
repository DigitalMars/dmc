;_ getenv.asm   Sun Dec 10 1989   Modified by: Walter Bright
; Windows support added by G. Eric Engstrom Jan 1991	-D_WINDOWS
; Copyright (C) 1985-1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

	include	MACROS.ASM

	begdata
	c_extrn	aenvseg,word


	public	__envptr,__envbuf
    if SPTR
__envptr	dw	0		;pointer to start of environment string
__envbuf	dw	0		;pointer to malloced buffer
    else
__envptr	dd	0		;pointer to start of environment string
__envbuf	dd	0		;pointer to malloced buffer
    endif
dummy		db	'A',0		;dummy string for initialization

    ifdef _MT
	public ___env_semaphore
___env_semaphore dd	0		;getenv/putenv semaphore
    endif

	enddata

    if SPTR
    if LCODE
	c_extrn	malloc,far
    else
	c_extrn	malloc,near
    endif
    endif

    ifdef _MT
	extrn	DOSSEMREQUEST:far
	extrn	DOSSEMCLEAR:far
    if LCODE
	c_extrn	_semerr,far
    else
	c_extrn	_semerr,near
    endif
    endif

    ifdef _WINDOWS
	extrn	GETDOSENVIRONMENT:far
    endif

	begcode	getenv

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get environment string
; Return pointer to it.

	c_public getenv
func	getenv
	WINENTER
	.save	<SI,DI>

    ifdef _MT
	mov	AX,offset DGROUP:___env_semaphore
	push	DS
	push	AX
	mov	AX,-1
	push	AX
	push	AX		;wait until semaphore is unowned
	call	DOSSEMREQUEST
	tst	AX
	jz	L12
L13:	jmp	_semerr
L12:
    endif

    if LPTR
	_if	<word ptr __envptr+2> ne 0, L1
	mov	AX,aenvseg
      ifdef _WINDOWS
	tst	AX
	jnz	L14
	call	GETDOSENVIRONMENT
	tst	DX
	jz	L4			;no environment available
	mov	word ptr __envptr,AX
	mov	word ptr __envptr+2,DX
	jmps	L1
L14:
      endif
	mov	word ptr __envptr+2,AX	;initialize
L1:	les	DI,__envptr
    else
	push	DS
	pop	ES
	mov	DI,word ptr __envptr	;pointer to environment string
	tst	DI			;initialized?
	jnz	L1			;yes

	;Copy the environment string into the data segment so we can
	;access it without far pointers.

	;First figure out how long it is.

	clr	SI			;offset of environment string
	mov	AX,aenvseg		;get segment of environment string
	tst	AX
      ifdef _WINDOWS
	jnz	L15
	call	GETDOSENVIRONMENT
	mov	aenvseg,DX
	mov	SI,AX
	mov	AX,DX
	tst	AX
      endif
	jz	L4			;no environment string
L15:	mov	ES,AX
	mov	DI,SI
	clr	AL			;look for 0
	mov	CX,0FFFFh		;largest possible count
L2:	repne	scasb			;scan for 0
	scasb				;look at next byte
	jnz	L2			;read next environment string
	push	DI
	push	DI			;DI = # of bytes in environment string
	callm	malloc			;allocate that many bytes
	add	SP,2
	pop	CX			;CX = # of bytes in string
	tst	AX			;error?
	jz	L3			;yes
	mov	DI,AX
	mov	word ptr __envptr,AX	;save environment pointer
    ifndef _WINDOWS
	mov	word ptr __envbuf,AX	;save environment pointer
    endif
	push	DS
	pop	ES			;ES:DI -> new environment
	push	DS			;save old segment
	mov	DS,aenvseg		;DS:SI -> original environment string
	rep	movsb			;transfer the bytes
	pop	DS			;restore original segment
	mov	DI,AX			;ES:DI -> new environment string
L1:
    endif

	;Now look for the environment string
	;ES:DI -> environment

	clr	AX			;0 used for the _ifs, scasb and error
					; return value!
	mov	CX,-1			;arbitrarilly large count
	mov	BX,CX			;BX = -1 (save cycles on indexing)
    if SPTR
L10:	mov	SI,P[BP]		;SI -> name we're looking for
	repe	cmpsb			;scan till first mismatch
	_if	<[BX + DI]> e AL, L3	;end of environment string, no match
    else
	push	DS
L10:	lds	SI,P[BP]		;SI -> name we're looking for
	repe	cmpsb			;scan till first mismatch
	_if	<ES:[BX + DI]> e AL, L6	;end of environment string, no match
    endif
	_if	<[BX + SI]> e AL, L7	;possible match
L8:	repne	scasb			;scan for terminating 0
	jmp	L10

L7:
    if SPTR
	_if	<byte ptr [BX + DI]> ne '=', L8
	mov	AX,DI			;point to value of environment var
L3:
    else
	_if	<byte ptr ES:[BX + DI]> ne '=', L8
	mov	AX,DI			;point to value of environment var
	mov	DX,ES
L11:	pop	DS
    endif
L4:
    ifdef _MT
	push	AX
	mov	AX,offset DGROUP:___env_semaphore
	push	DS
	push	AX
	call	DOSSEMCLEAR
	tst	AX
	jnz	L13			;fatal error
	pop	AX
    endif
	.restore <DI,SI>
        WINLEAVE
	ret

    if LPTR
L6:
	cwd				;DX,AX = 0
	jmp	L11
    endif
c_endp	getenv

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initialize _envptr

func	_getenv_init
	WINENTER_NF
    if LPTR
	push	DS
    endif
	mov	AX,offset DGROUP:dummy
	push	AX
	callm	getenv
        add	SP,SIZEPTR
        WINLEAVE_NF
	ret
c_endp	_getenv_init

	endcode	getenv

static_ctor	_getenv_init

	end
