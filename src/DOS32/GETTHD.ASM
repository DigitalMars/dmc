;_ getthd.asm

include macros.asm

noerrno	equ	1
include stdio.asm
include flthead.asm

	begdata

	extrn	__thread1:word

    ifdef _MT
	extrn	_calloc:near
	extrn	___faterr:near

    ifdef __OS2__
	extrn	DosGetInfoBlocks:near
    endif

    ifdef __NT__
	extrn	_GetCurrentThreadId:near
    endif

thdtbl	dd	?
thdmsg	db	0Dh,0Ah,'Thread error',0Dh,0Ah,0
    else
	extrn	_errno:dword,__doserrno:dword
	extrn	__fe_cur_env:word
    endif

	enddata

THREADMAX equ	0400h

	begcode	getthd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initialize for multithreaded operation.

    ifdef _MT

	public	__thread_init
__thread_init proc near
	;The idea is to allocate an array of pointers to thread
	;data indexed by thread number. This array is of a fixed
	;size, so we decide in advance the max number of threads
	;to support (THREADMAX).

	;Note that we can call calloc() before the thread data
	;structures exist, because calloc() and sbrk() never
	;touch that stuff (!)

	push	1
	push	(THREADMAX + 1) * SIZEPTR	;extra 1 for sentinal
	call	_calloc
    ife _STDCALL
        add     ESP,8
    endif

	tst	EAX
	jz	__thderr		;out of memory
	mov	thdtbl,EAX

	;Initialize for thread 1
	mov	dword ptr SIZEPTR[EAX],offset FLAT:__thread1
	ret
__thread_init endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Unrecoverable thread errors go here.
; Print a message and abort program.

	public	__thderr
__thderr proc near
	push	offset FLAT:thdmsg
	call	___faterr
__thderr endp

    endif ;_MT

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get pointer to thread data for current thread.
;	struct THREAD *_getthreaddata();
; Returns:
;	NULL	can't get thread data
;	EAX	is only register affected

	public	__getthreaddata

__getthreaddata proc near
    ifdef _MT
      ifdef __NT__

	push	EBX			; save registers
	push	ECX
	push	EDI

	call	_GetCurrentThreadId	; Hope this doesn't affect registers!!!
	mov	ECX, THREADMAX
	mov	EBX, thdtbl

gidlp:
	mov	EDI, [EBX]		; search thread table for this ID
	cmp	EDI, 0
	je	gnext_id

	cmp	EAX, t_id[EDI]
	jne	gnext_id

	mov	EAX, EDI		; found - return pointer to it
gdun:
	pop	EDI
	pop	ECX
	pop	EBX
	ret

gnext_id:
	add	EBX, 4
	loop	gidlp

	clr	EAX
	jmps	gdun

      else		; __NT__

	call	___threadid	;get pointer to thread number
	mov	EAX,[EAX]	;EAX = thread number

	public	__getthreaddata2
__getthreaddata2:		;alternate entry point for EAX = thread number
	_ifs	EAX ae THREADMAX, __thderr	;too many threads
	shl	EAX,2
	add	EAX,thdtbl
	_ifs	<dword ptr [EAX]> e 0, GT1
	mov	EAX,[EAX]
	ret

GT1:	;Allocate instance data for this thread
	push	EBX
	mov	EBX,EAX

	_push	<ECX,EDX,ES>
	push	1
	push	THDSIZE
	call	_calloc
	_pop	<ES,EDX,ECX>
	tst	EAX
	jz	__thderr

	mov	[EBX],EAX
	pop	EBX
	ret
      endif
    else ;single threaded
      if _FLAT
	mov	EAX,offset FLAT:__thread1
      else
	mov	EAX,offset DGROUP:__thread1
      endif
	ret
    endif

__getthreaddata endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get pointer to current thread number.
; Returns:
;	EAX is only register modified

    ifdef __NT__
	public	___threadid
___threadid	proc	near
	push	EBX			; save registers
	push	ECX
	push	EDI

	call	_GetCurrentThreadId	; Hope this doesn't affect registers!!!
	mov	ECX, THREADMAX
	mov	EBX, thdtbl

idlp:
	mov	EDI, [EBX]		; search thread table for this ID
	cmp	EDI, 0
	je	next_id

	cmp	EAX, t_id[EDI]
	jne	next_id

	lea	EAX, t_id[EDI]		; found - return pointer to it
dun:
	pop	EDI
	pop	ECX
	pop	EBX
	ret

next_id:
	add	EBX, 4
	loop	idlp

	clr	EAX
	jmps	dun


___threadid	endp
    endif


    ifdef __OS2__
	public	___threadid
___threadid	proc	near
    if 1
	;This is an undocumented feature of OS/2
	mov	EAX,FS:[0Ch]
	ret
    else
    if 1
	push	EBP
	mov	EBP,ESP
	sub	ESP,8
	lea	EAX,-4[EBP]
	push	EAX
	lea	EAX,-8[EBP]
	push	EAX
	call	DosGetInfoBlocks
	add	ESP,8
	mov	EAX,-8[EBP]
	mov	EAX,0Ch[EAX]
	leave
	ret
    else
	push	EAX
	mov	EAX,ESP
	push	EAX

	push	EAX
	sub	EAX,4
	push	EAX
	call	DosGetInfoBlocks

	mov	EAX,8[ESP]
	mov	EAX,12[EAX]
	add	ESP,16
	ret
    endif
    endif
___threadid	endp
    endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set errno based on value in EAX.
; Preserve contents of all registers and flags.

	public __errno_set
__errno_set proc near

    ifdef _MT
	pushf
	push	EAX
	push	EAX
	call	__getthreaddata
	pop	dword ptr t_errno[EAX]
	pop	EAX
	popf
    else
	mov	_errno,EAX	;for link compatibility with non-_MT code
    endif
	ret
__errno_set endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Retrieve pointer to where errno is stored.
;	int *_errno();

	public __errno

__errno	proc	near

    ifdef _MT
	call	__getthreaddata
	add	EAX,t_errno
    else
      if _FLAT
	mov	EAX,offset FLAT:_errno
      else
	mov	EAX,offset DGROUP:_errno
      endif
    endif
	ret

__errno	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Retrieve pointer to where _doserrno is stored.
;	int *__doserrno();

	public ___doserrno

___doserrno	proc	near

    ifdef _MT
	call	__getthreaddata
	add	EAX,t_doserrno
    else
      if _FLAT
	mov	EAX,offset FLAT:__doserrno
      else
	mov	EAX,offset DGROUP:__doserrno
      endif
    endif
	ret

___doserrno	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set exception bits.
; No registers modified except AX.

	public __FEEXCEPT
__FEEXCEPT proc near
    ifdef _MT
	push	EBX
	mov	EBX,PS+4[ESP]		;get exception bits
	call	__getthreaddata
	xchg	EAX,EBX
	or	t_fe_status[EBX],AX
	pop	EBX
	ret	4
    else
	mov	EAX,PS[ESP]
	or	__fe_cur_env.status,AX
	ret	4
    endif
__FEEXCEPT endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get rounding mode in AX.
; No registers modified.

	public __FEROUND
__FEROUND proc near
    ifdef _MT
	call	__getthreaddata
	movzx	EAX,t_fe_round[EAX]
    else
	movzx	EAX,__fe_cur_env.round
    endif
	ret
__FEROUND endp

	endcode	getthd

	end
