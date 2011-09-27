;_ stdio.asm   
; Copyright (C) 1985-1990 by Walter Bright	*/
; All rights reserved.				*/
; $Revision: 1.1.1.1 $ 
; Standard I/O header file	*/

EOF	equ	-1

ifdef _WIN32
ifndef __NT__
__NT__ equ 1
endif
endif

ifdef __NT__
_NFILE	equ	60
else
ifdef M_UNIX
_NFILE	equ	60
else
ifdef M_XENIX
_NFILE	equ	60
else
_NFILE	equ	40
endif
endif
endif

ifdef	M_I386

BUFSIZ		equ	4096

ifdef DOS386
_ptr	equ	0
_cnt	equ	_ptr + SIZEPTR
_base	equ	_cnt + 4
_flag	equ	_base + SIZEPTR
_file	equ	_flag + 4
_bufsiz equ	_file + 4
else
ifdef __OS2__
_ptr		equ	0
_cnt		equ	4
_base		equ	8
_flag		equ	12
_file		equ	16
_charbuf	equ	20
_bufsiz		equ	24
__tmpnum	equ	28
else
ifdef __NT__
_ptr		equ	0
_cnt		equ	4
_base		equ	8
_flag		equ	12
_file		equ	16
_bufsiz		equ	20
__tmpnum	equ	24
else	; M_UNIX and M_XENIX
_cnt	equ	0
_ptr	equ	_cnt + 4
_base	equ	_ptr + SIZEPTR
_flag	equ	_base + SIZEPTR
_file	equ	_flag + 1
endif
endif
endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Thread data structure (one is allocated per thread)
;Must match __thread1 in cinit.asm and struct THREAD in mt.h

    ifdef __NT__
thread_t struc
	t_stackoff	dd	0
;	t_stackseg	dw	0
	t_parameter	dd	0
	t_func		dd	0
	t_stacktop	dd	0
	t_errno		dd	0
;   if SPTR
	t_strtok	dd	0
;   else
;	t_strtok	df	0
;   endif
;   ifdef __NT__
	t_id		dd	0	;thread id
	t_hndl		dd	0	;thread handle
	t_envptr	dd	0	;pointer to malloced memory for getenv return
;   endif
	t_tm		dd	9 dup (0)	;struct tm

	;Actually t_fenv
	t_fe_status	dw	0
	t_fe_control	dw	0
	t_fe_round	dw	0
	t_fe_res1	dw	0
	t_fe_res2	dw	0

	t_asctime	db	26 dup (0)
	t_digstr	db	32 dup (0)	;DBL_DIG*2+2
	t_doserrno      dd      0

; see to MT.H, STDIO.ASM(This file), CINIT.ASM.
	t_wcstok	dd	0		;wcstok()
	t_wenvptr	dd	0		;wgetenv()
	t_wasctime	dw	26 dup (0)	;wasctime()
	t_cppeh_sv  dd      114 dup (0)       ;global statics for cplusplus exceptions..

thread_t ends
    else
thread_t struc
	t_stackoff	dd	0
;	t_stackseg	dw	0
	t_parameter	dd	0
	t_func		dd	0
	t_stacktop	dd	0
	t_errno		dd	0
;   if SPTR
	t_strtok	dd	0
;   else
;	t_strtok	df	0
;   endif
;   ifdef __NT__
;	t_id		dd	0	;thread id
;	t_hndl		dd	0	;thread handle
;	t_envptr	dd	0	;pointer to malloced memory for getenv return
;   endif
	t_tm		dd	9 dup (0)	;struct tm

	;Actually t_fenv
	t_fe_status	dw	0
	t_fe_control	dw	0
	t_fe_round	dw	0
	t_fe_res1	dw	0
	t_fe_res2	dw	0

	t_asctime	db	26 dup (0)
	t_digstr	db	32 dup (0)	;DBL_DIG*2+2
	t_doserrno      dd      0
	
	ifdef DOS386
	t_cppeh_sv      dd      110 dup (0) 
	else
	
	if LCODE
    ;; Large or Medium
    if LPTR
    t_cppeh_sv    db      350 dup (0)     ; globals for CPP exception handling.
    else
    t_cppeh_sv     db      268 dup (0)
    endif

    else
    ;; Compact or Small
    if LPTR
    t_cppeh_sv     db      330 dup (0)
    else
    t_cppeh_sv     db      248 dup (0)
    endif
    endif ;; LCODE
    
    endif ;; DOS386

thread_t ends
    endif

THDSIZE	equ	(size thread_t)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ifdef _MT

  ifndef noerrno
    if LCODE
	extrn	__errno_set:far
    else
	extrn	__errno_set:near
    endif
  endif

errno_set macro	value
    ifnb <value>
	mov	EAX,value
    endif
	call	__errno_set
	endm

else

 ifndef noerrno
	begdata
	extrn	_errno:dword
	enddata
 endif

errno_set macro	value
    ifnb <value>
	mov	_errno,value
    else
	mov	_errno,EAX
    endif
	endm

endif

else	;M_I386

BUFSIZ		equ	1024
    IF SPTR
BIGBUF		equ	20 * 1024
    ENDIF

_ptr	equ	0
_cnt	equ	_ptr + SIZEPTR
_base	equ	_cnt + 2
_flag	equ	_base + SIZEPTR
_file	equ	_flag + 2
_bufsiz equ	_file + 2
    IFDEF	BIGBUF
_seg	equ	_bufsiz + 2
    ENDIF

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Thread data structure (one is allocated per thread)
;Must match __thread1 in cinit.asm and struct THREAD in mt.h

thread_t struc
	t_stackoff	dw	0
	t_stackseg	dw	0
	t_stacktop	dw	0
	t_errno		dw	0
	t_strtok	dd	0
	t_tm		dw	9 dup (0)	;struct tm

	;Actually fenv_t
	t_fe_status	dw	0
	t_fe_control	dw	0
	t_fe_round	dw	0
	t_fe_res1	dw	0
	t_fe_res2	dw	0

	t_asctime	db	26 dup (0)
	t_digstr	db	32 dup (0)	;DBL_DIG*2+2
	t_doserrno      dw      0
thread_t ends

THDSIZE	equ	(size thread_t)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ifdef _MT

  ifndef noerrno
    if LCODE
	extrn	__errno_set:far
    else
	extrn	__errno_set:near
    endif
  endif

errno_set macro	value
    ifnb <value>
	mov	AX,value
    endif
	call	__errno_set
	endm

else

 ifndef noerrno
	begdata
	extrn	_errno:word
	enddata
 endif

errno_set macro	value
    ifnb <value>
	mov	_errno,value
    else
	mov	_errno,AX
    endif
	endm

endif

endif

_IOREAD		equ	1
_IOWRT		equ	2
_IONBF		equ	4
_IOMYBUF	equ	8
_IOEOF		equ	10h
_IOERR		equ	20h
_IOLBF		equ	40h
_IORW		equ	80h
_IOTRAN		equ	100h
_IOAPP		equ	200h
    ifdef	BIGBUF
_IOBIGBUF	equ	400h
    endif


;Values for errno
ifdef M_UNIX 
ENOENT		equ	2
EACCES		equ	13
EINVAL		equ	22
ENOMEM		equ	12
EDEADLOCK	equ	56
EEXIST		equ	17
ERANGE		equ	34
else
ifdef M_XENIX
ENOENT		equ	2
EACCES		equ	13
EINVAL		equ	22
ENOMEM		equ	12
EDEADLOCK	equ	56
EEXIST		equ	17
ERANGE		equ	34
else
EZERO           equ     0
EPERM           equ     1
ENOENT          equ     2
ESRCH           equ     3
EINTR           equ     4
EIO             equ     5
ENXIO           equ     6
E2BIG           equ     7
ENOEXEC         equ     8
EBADF           equ     9
ECHILD          equ     10
EAGAIN          equ     11
ENOMEM          equ     12
EACCES          equ     13
EFAULT          equ     14
ENOTBLK         equ     15
EBUSY           equ     16
EEXIST          equ     17
EXDEV           equ     18
ENODEV          equ     19
ENOTDIR         equ     20
EISDIR          equ     21
EINVAL          equ     22
ENFILE          equ     23
EMFILE          equ     24
ENOTTY          equ     25
ETXTBSY         equ     26
EFBIG           equ     27
ENOSPC          equ     28
ESPIPE          equ     29
EROFS           equ     30
EMLINK          equ     31
EPIPE           equ     32
EDOM            equ     33
ERANGE          equ     34
EUCLEAN         equ     25
EDEADLOCK       equ     36
EDEADLK         equ     36
ENAMETOOLONG    equ     38
ENOLCK          equ     39
ENOSYS          equ     40
ENOTEMPTY       equ     41
EILSEQ          equ     42
endif
endif

