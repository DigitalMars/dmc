;_ getenv.asm   Modified by: Walter Bright */
; DOS386 supported added by G. Eric Engstrom
; Copyright (C) 1985-1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

	include	MACROS.ASM

	begdata
	extrn	__envptr:dword

    ifdef _MT
	public ___env_semaphore
___env_semaphore dd	0		;getenv/putenv semaphore
    endif

	enddata

    ifdef _MT
	extrn	DOSCREATEMUTEXSEM:near
	extrn	__semerr:near
    endif

	begcode	getenv

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get environment string
; Return pointer to it.

	c_public getenv
func	getenv
	push	EBP
	mov	EBP,ESP
	uses	<ES,EBX,ESI,EDI>
   	mov	EDI,dword ptr __envptr

	;Now look for the environment string
	;EDI -> environment
    ife ESeqDS
	mov	AX,DS
        mov	ES,AX
    endif
	clr	EAX			;0 used for the .ifs, scasb and error
					; return value!
	cld
	mov	ECX,-1			;arbitrarilly large count
	mov	EBX,ECX			;BX = -1 (save cycles on indexing)
L10:	  mov	  ESI,P[EBP]		;SI -> name we're looking for
	  repe	  cmpsb			;scan till first mismatch
	  _ifs	  <[EBX + EDI]> e AL, L3	;end of environment string, no match
	  _ifs	  <[EBX + ESI]> e AL, L7	;possible match
L8:	  repne	  scasb			;scan for terminating 0
	  jmp	  L10
L7:	_ifs	<byte ptr [EBX + EDI]> ne '=', L8
	  mov     EAX,EDI                 ;point to value of environment var
L3:	unuse	<EDI,ESI,EBX,ES>
	pop	EBP
	ret
c_endp	getenv

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initialize

    ifdef _MT

__getenv_init proc near
	clr	EAX
	push	EAX				;semaphore as reset
	push	EAX				;attr of semaphore
	push	offset FLAT:___env_semaphore	;address of handle
	push	EAX				;create anonymous semaphore
	call	DOSCREATEMUTEXSEM
	add	ESP,5*4
	tst	EAX
	jnz	IN1
	ret

IN1:	jmp	__semerr
__getenv_init endp

static_ctor	__getenv_init

    endif

	endcode	getenv

	end

