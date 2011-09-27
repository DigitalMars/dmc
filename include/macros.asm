;_ macros.asm
; Copyright (C) 1985-1991 by Walter Bright
; All Rights Reserved
; Modified for COFF and ELF support by David Bustin
; $Revision: 1.1.1.1 $ 

ifdef _WIN32
ifndef __NT__
__NT__ equ 1
endif
endif

ifndef M_I386

ifdef _WINDOWS
    extrn  MESSAGEFATALERROR : far
endif

; modified for RATIONAL support by P Murray, April 1990
; define DOS16RM for protected mode programs

; Determine which memory model we are assembling for. For .COM files,
; force S model.

ifdef DOS16RM
ifndef I8086L
%out	Only L model supported with RATIONAL DOS Extender.
end
endif
else
    ifdef I8086T
I8086S equ	1
    else
    ifndef I8086S
    ifndef I8086M
    ifndef I8086C
    ifndef I8086L		;if none of the memory models are defined
    ifndef I8086V
I8086S equ	1		;default to S model
    endif ;I8086V
    endif ;I8086L
    endif ;I8086C
    endif ;I8086M
    endif ;I8086S
    endif ;I8086T
endif ;DOS16RM

ifdef __OS2__
	.286C
endif


ifdef DOS16RM
	.286P
endif

ifndef I386
I386	equ	0
endif

;Decide if SI and DI are saved across function calls
SAVESIDI equ	1		;1 means SI and DI are saved across functions

if 0	;Lattice conventions no longer supported
MSC	equ	1		;ifdef means use Microsoft C calling conventions
				;ifndef means use Lattice
endif

; Macros to bracket data segment stuff.

ifndef STARTUP
begdata macro

ifdef DOS16RM
;Segment so we can find the start of DGROUP

NULL	segment para public 'BEGDATA'		;Note PARAGRAPH alignment
NULL	ends
endif

_DATA	segment word public 'DATA'
_DATA	ends
CONST	segment word public 'CONST'
CONST	ends
_BSS	segment word public 'BSS'
_BSS	ends
ifdef DOS16RM
DGROUP	group	NULL,_DATA,CONST,_BSS
else
DGROUP	group	_DATA,CONST,_BSS
endif
_DATA	segment
	assume ds:DGROUP
	endm
endif ; STARTUP

enddata macro
_DATA	ends
	endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Macros specific to each memory model in an attempt to make it easier
; to write memory model independent code.
;	begcode,endcode		Use to bracket code sections
;	P			Offset on BP to first argument on stack
;				(excluding any local variables)
;	SPTR			1 if small data model
;	LPTR			1 if large pointers (large data)
;	LCODE			1 if large code model
;	ESeqDS			1 if ES == DS at all times
;	SSeqDS			1 if SS == DS at all times
;	SIZEPTR			# of bytes in a pointer
;	func			Declare a function as NEAR or FAR
;	callm			Call function as NEAR or FAR

;;;;;;;;;;;;;; SMALL MEMORY MODEL ;;;;;;;;;;;;;;;;;
ifdef I8086S
begcode macro	module
_TEXT	segment word public 'CODE'
	assume	cs:_TEXT
	endm

endcode macro	module
_TEXT	ENDS
	endm

P	equ	4	; Offset of start of parameters on the stack frame
SPTR	equ	1
LPTR	equ	0
LCODE	equ	0
ESeqDS	equ	0
SSeqDS	equ	1
SIZEPTR equ	2	; Size of a pointer

p_func	macro	name
name	proc	near
	endm

p_endp	macro	name
name	endp
	endm

callp	macro	name
	call	near ptr name
	endm

func	macro	name
_&name	proc	near
    ifndef name
name	equ	_&name
    endif
	endm

callm	macro	name
	call	near ptr _&name
	endm
endif

;;;;;;;;;;;;;;;;; MEDIUM MEMORY MODEL ;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ifdef I8086M
begcode macro	module
module&_TEXT	segment para public 'CODE'
	assume	cs:module&_TEXT
	endm

endcode macro	module
module&_TEXT	ends
	endm

P	equ	6	; Offset of start of parameters on the stack frame
SPTR	equ	1
LPTR	equ	0
LCODE	equ	1
ESeqDS	equ	0
SSeqDS	equ	1
SIZEPTR equ	2

p_func	macro	name
name	proc	far
	endm

p_endp	macro	name
name	endp
	endm

callp	macro	name
	call	far ptr name
	endm

func	macro	name
_&name	proc	far
    ifndef name
name	equ	_&name
    endif
	endm

callm	macro	name
	call	far ptr _&name
	endm
endif

;;;;;;;;;;;;;;;;; COMPACT MEMORY MODEL ;;;;;;;;;;;;;;

ifdef I8086C
begcode macro	module
_TEXT	segment word public 'CODE'
	assume	cs:_TEXT
	endm

endcode macro	module
_TEXT	ends
	endm

P	equ	4	; Offset of start of parameters on the stack frame
SPTR	equ	0
LPTR	equ	1
LCODE	equ	0
ESeqDS	equ	0
SSeqDS	equ	0
SIZEPTR equ	4

p_func	macro	name
name	proc	near
	endm

p_endp	macro	name
name	endp
	endm

callp	macro	name
	call	near ptr name
	endm

func	macro	name
_&name	proc	near
    ifndef name
name	equ	_&name
    endif
	endm

callm	macro	name
	call	near ptr _&name
	endm
endif

;;;;;;;;;;;;;;;; LARGE MEMORY MODEL ;;;;;;;;;;;;;;;;;;;

ifdef I8086L
begcode macro	module
module&_TEXT	segment para public 'CODE'
	assume	cs:module&_TEXT
	endm

endcode macro	module
module&_TEXT	ends
	endm

P	equ	6	; Offset of start of parameters on the stack frame
SPTR	equ	0
LPTR	equ	1
LCODE	equ	1
ESeqDS	equ	0
SSeqDS	equ	0
SIZEPTR equ	4

p_func	macro	name
name	proc	far
	endm

p_endp	macro	name
name	endp
	endm

callp	macro	name
	call	far ptr name
	endm

func	macro	name
_&name	proc	far
    ifndef name
name	equ	_&name
    endif
	endm

callm	macro	name
	call	far ptr _&name
	endm
endif

;Macros to replace public, extrn, and endp for C-callable assembly routines,
; and to define labels: c_label defines labels,
; c_public replaces public, c_extrn replaces extrn, and c_endp replaces endp

c_name	macro	name
	name equ _&name
	endm

c_label macro	name
_&name:
	endm

c_public macro	a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
    ifnb <a>			;;Check for blank argument
	public	_&a
	a equ _&a
      ifnb <b>
	c_public b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
      endif
    endif
	endm

c_extrn macro	a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
    ifnb <a>			;;Check for blank argument
	extrn	_&a:b
	a equ _&a
      ifnb <c>
	c_extrn c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
      endif
    endif
	endm

c_endp	macro	name
_&name	ENDP
	endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Define function ctor as a static constructor
static_ctor macro	ctor
    if LCODE
XIFB	segment word public 'DATA'
XIFB	ends
XIF	segment word public 'DATA'
	dd	ctor
XIF	ends
XIFE	segment word public 'DATA'
XIFE	ends
    else
XIB	segment word public 'DATA'
XIB	ends
XI	segment word public 'DATA'
	dw	ctor
XI	ends
XIE	segment word public 'DATA'
XIE	ends
    endif
	endm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Define function ctor as a static destructor
static_dtor macro	dtor
    if LCODE
XCFB	segment word public 'DATA'
XCFB	ends
XCF	segment word public 'DATA'
	dd	dtor
XCF	ends
XCFE	segment word public 'DATA'
XCFE	ends
    else
XCB	segment word public 'DATA'
XCB	ends
XC	segment word public 'DATA'
	dw	dtor
XC	ends
XCE	segment word public 'DATA'
XCE	ends
    endif
	endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Other more or less useful macros
; Commented out ones are obsolete

;setESeqDS macro		;set ES == DS, if not already true
;    ife ESeqDS
;	push	DS
;	pop	ES
;    endif
;	endm

.push	macro	list
	irp	arg,<list>
	 push	arg
	endm
	endm

.pop	macro	list
	irp	arg,<list>
	 pop	arg
	endm
	endm

_push	macro	list
	irp	arg,<list>
	 push	arg
	endm
	endm

_pop	macro	list
	irp	arg,<list>
	 pop	arg
	endm
	endm

; Macros to save and restore regs destroyed by a function

.save	macro	list
    if SAVESIDI
	irp	arg,<list>
	 push	arg
	endm
    endif
	endm

.restore macro	list
    if SAVESIDI
	irp	arg,<list>
	 pop	arg
	endm
    endif
	endm

_save	macro	list
    if SAVESIDI
	irp	arg,<list>
	 push	arg
	endm
    endif
	endm

_restore macro	list
    if SAVESIDI
	irp	arg,<list>
	 pop	arg
	endm
    endif
	endm

; Macros to save and restore ES, but only if ESeqDS is 1.
;pushES macro
;    if ESeqDS
;	push	ES
;    endif
;	endm
;
;popES	macro
;    if ESeqDS
;	pop	ES
;    endif
;	endm

clr	macro	list		;clear a register
	irp	reg,<list>
	 xor	reg,reg
	endm
	endm

tst	macro	reg
	test	reg,reg
	endm

jmps	macro	lbl
	jmp	short	lbl
	endm

    if @Version lt 600
	;For compatibility with MASM 5.10
.if	macro	arg1,cond,arg2,lbl
	cmp	arg1,arg2
	j&cond	lbl
	endm

    endif

_if	macro	arg1,cond,arg2,lbl
	cmp	arg1,arg2
	j&cond	lbl
	endm

;sob	macro	arg,lbl
;	ifidn	<arg>,<CX>
;	 loop	lbl
;	else
;	 dec	arg
;	 jnz	lbl
;	endif
;	endm


    ifdef _WINDOWS
    extrn  DOS3CALL : far
    endif

ifndef nobdos
bdos	macro	func
	ifnb	<func>
	mov	AH,func
	endif
    ifndef _WINDOWS
	int	21h
    else
        call	DOS3CALL
    endif
	endm

dpmi	macro	func
	ifnb	<func>
        mov	AX,func
        endif
	int	31h
        endm
else
__bdos	macro	func
	ifnb	<func>
	mov	AH,func
	endif
    ifndef _WINDOWS
	int	21h
    else
        call	DOS3CALL
    endif
	endm

__dpmi	macro	func
	ifnb	<func>
        mov	AX,func
        endif
	int	31h
        endm
endif

.retf	macro	val		;force assembler to build a far return
	ifnb	<val>
	 db	0CAh
	 dw	val
	else
	 db	0CBh
	endif
	endm

_retf	macro	val		;force assembler to build a far return
	ifnb	<val>
	 db	0CAh
	 dw	val
	else
	 db	0CBh
	endif
	endm

; Sometimes MASM ignores my segment overrides.
segES	macro
	db	26h
	endm

; 32 bit negate
neg32	macro	reg1,reg2
	 neg	reg1
	 neg	reg2
	 sbb	reg1,0
	endm

; Push immediate (reg is for scratch)
pushi	macro	reg,value
	if 0
	 push	value		;for 286 code generation only
	else
	 mov	reg,value
	 push	reg
	endif
	endm

; Inc/dec BP if I8086V memory model
incBP	macro
	ifdef I8086V
	  inc	BP
	endif
	endm

decBP	macro
	ifdef I8086V
	  dec	BP
	endif
	endm

WINENTER macro
    ifdef _WINDOWS
    ifndef I8086S
    ifndef I8086C
	inc	BP
    endif
    endif
    endif
        push	BP
        mov	BP,SP
	endm

WINLEAVE macro
	pop	BP
    ifdef _WINDOWS
    ifndef I8086S
    ifndef I8086C
        dec	BP
    endif
    endif
    endif
	endm

WINENTER_NF macro
    ifdef _WINDOWS
    ifndef I8086S
    ifndef I8086C
    	inc	BP
        push	BP
        mov	BP,SP
    endif
    endif
    endif
	endm

WINLEAVE_NF macro
    ifdef _WINDOWS
    ifndef I8086S
    ifndef I8086C
        pop	BP
        dec	BP
    endif
    endif
    endif
	endm

else	;M_I386

	.386P
	.387

ifdef DOS386
SEG_1ST_MEG	equ	034h	;Selector of 1st Mbyte used by MSDOS
SEG_SCREEN	equ	01ch	;Selector for the video memory.
SEG_PSP		equ	024H	;PSP selector.
SEG_DGROUP	equ	014H	;DGROUP selector.
SEG_ENV		equ	02cH	;Selector of environment string.

OMF	equ	1
COFF	equ	0
ELF	equ	0
_FLAT	equ	0		;FLAT memory model
_STDCALL equ	0		;default to _stdcall
_RETST0	equ	0		;return floating point results in ST0
endif	;DOS386

ifdef X386
_FLAT	equ	0
_STDCALL equ	0
_RETST0	equ	0
endif

ifdef __OS2__
OMF	equ	1
COFF	equ	0
ELF	equ	0
_FLAT	equ	1	;FLAT memory model
_STDCALL equ	1
_RETST0	equ	1
_INLINE_8087 equ 1	;defined if we always generate inline 8087 code
endif

ifdef __NT__
OMF	equ	1
COFF	equ	0
ELF	equ	0
_FLAT	equ	1
_STDCALL equ	0
_RETST0	equ	1
;_NOCTOR equ	1	;defined if no static constructors/destructors supported
_INLINE_8087 equ 1	;defined if we always generate inline 8087 code
LONGDOUBLE equ 1	;defined if memory model supports 80 bit long doubles
endif

ifdef M_XENIX
OMF	equ	1	;Set to non-zero if OMF object format.
COFF	equ	0	;Set to non-zero if COFF object format.
ELF	equ	0
_FLAT	equ	0
_STDCALL equ	0
_RETST0	equ	0
endif

ifdef M_UNIX
ifndef OMF
OMF     equ     0       ;Set to non-zero if OMF object format.
endif
ifdef M_ELF
ifndef ELF
ELF     equ     1       ;Set to non-zero if ELF object format.
endif
ifndef COFF
COFF    equ     0       ;Set to non-zero if COFF object format.
endif
else	;M_ELF
ifndef ELF
ELF     equ     0       ;Set to non-zero if ELF object format.
endif
ifndef COFF
COFF    equ     1       ;Set to non-zero if COFF object format.
endif
endif	;M_ELF
_FLAT	equ	0
_STDCALL equ	0
_RETST0	equ	0
endif

.ERRNZ	OMF AND COFF AND ELF
.ERRE	OMF OR COFF OR ELF

I386	equ	1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Macros specific to each memory model in an attempt to make it easier
; to write memory model independent code.
;	begcode,endcode		Use to bracket code sections
;	begdata,enddata		Use to bracket data sections
;	begrcode,endrcode       Use to bracket real mode code sections 
;	begrdata,endrdata       Use to bracket real mode data sections 
;                               (Pharlap DOS386 only)
;	P			Offset on EBP to first argument on stack
;				Assuming EBP was pushed.
;	PS			Offset on ESP to first argument on stack
;				Assuming nothing was pushed on the stack.
;	ESeqDS			1 if ES == DS at all times
;	FSeqDS			1 if FS == DS at all times
;	GSeqDS			1 if GS == DS at all times
;	SSeqDS			1 if SS == DS at all times
;	SIZEPTR			# of bytes in a pointer
;	func			Declare a function as NEAR or FAR
;	callm			Call function as NEAR or FAR
;	LPTR			Large data model?
;	SPTR			Small data model?


;Macro for start and end of real mode code segment.

begcode_16 macro     
__X386_CODESEG_16       segment para use16 public 'CODE'
        assume ds:__X386_GROUP_16
        assume cs:__X386_CODESEG_16
endm

endcode_16 macro
__X386_CODESEG_16       ends
endm

begcode macro	module
    if _FLAT
_TEXT	segment para use32 public 'CODE'
	  assume	CS:FLAT,DS:FLAT,SS:FLAT
    else
_TEXT	segment dword public 'CODE'
	  assume	CS:_TEXT
    endif
	endm

endcode macro	module
_TEXT	ENDS
	endm

begdata macro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set up segments for data
; Regular initialized data goes in _DATA

_DATA	segment dword public 'DATA'
_DATA   ends

	ifndef _NOCTOR
;Function pointers to constructors
if OMF
XIFCB	segment dword public 'DATA'
XIFCB	ends
XIFU	segment dword public 'DATA'
XIFU	ends
XIFL	segment dword public 'DATA'
XIFL	ends
XIFM	segment dword public 'DATA'
XIFM	ends
XIFCE	segment dword public 'DATA'
XIFCE	ends
XIB     segment dword public 'DATA'
XIB	ends
XI	segment dword public 'DATA'
XI	ends
XIE	segment dword public 'DATA'
XIE	ends

;Function pointers to destructors
XCB	segment dword public 'DATA'
XCB	ends
XC	segment dword public 'DATA'
XC	ends
XCE	segment dword public 'DATA'
XCE	ends

;Constant data, such as switch tables, go here.

CONST	segment dword public 'CONST'
CONST	ends
endif
;Segment for uninitialized data. This is set to 0 by the startup code/OS,
;so it does not consume room in the executable file.

_BSS	segment dword public 'BSS'
_BSS	ends

if OMF
;Function pointers to destructors
XOB	segment dword public 'BSS'
XOB	ends
XO	segment dword public 'BSS'
XO	ends
XOE	segment dword public 'BSS'
XOE	ends
endif

if OMF
ifdef DOS386
HUGE_BSS	segment dword public 'HUGE_BSS'
HUGE_BSS	ends
endif

EEND	segment dword public 'ENDBSS'
EEND	ends

ifdef DOS386
STACK	segment para stack 'STACK'
STACK	ends

DGROUP	group	_DATA,XIFCB,XIFU,XIFL,XIFM,XIFCE,XOB,XO,XOE,XIB,XI,XIE,XCB,XC,XCE,CONST,_BSS,EEND,STACK
else
ifdef __OS2__
STACK	segment para stack 'STACK'
STACK	ends

DGROUP	group	_DATA,XIFCB,XIFU,XIFL,XIFM,XIFCE,XOB,XO,XOE,XIB,XI,XIE,XCB,XC,XCE,CONST,_BSS,EEND,STACK
else
ifdef __NT__
STACK	segment para stack 'STACK'
STACK	ends

DGROUP	group	_DATA,XIFCB,XIFU,XIFL,XIFM,XIFCE,XOB,XO,XOE,XIB,XI,XIE,XCB,XC,XCE,CONST,_BSS,EEND,STACK
else
DGROUP	group	_DATA,XIB,XI,XIE,XCB,XC,XCE,CONST,_BSS,EEND
endif	;__NT__

endif	;__OS2__
endif	;DOS386

else	;OMF
if OMF 
DGROUP	group	_DATA,XIFCB,XIFU,XIFL,XIFM,XIFCE,XOB,XO,XOE,XIB,XI,XIE,XCB,XC,XCE,CONST,_BSS
else
DGROUP	group	_DATA,_BSS
endif
endif
	endif

_DATA	segment
	if _FLAT
	  assume DS:FLAT
	else
	  assume DS:DGROUP
	endif
	endm

enddata macro
_DATA	ends
	endm

P	equ	8	; Offset of start of parameters on the stack frame
			; From EBP assuming EBP was pushed.
PS	equ	4	; Offset of start of parameters on the stack frame

			; From ESP assuming EBP was NOT pushed.
;; For Win32, ESeqDS should be TRUE
ifdef _WIN32
ESeqDS  equ     1
else
ESeqDS  equ     0
endif

FSeqDS	equ	0
GSeqDS	equ	0
SSeqDS	equ	1
SIZEPTR equ	4	; Size of a pointer
LPTR	equ	0
SPTR	equ	1
LCODE	equ	0

func	macro	name
_&name	proc	near
    ifndef name
name	equ	_&name
    endif
	endm

callm	macro	name
	call	_&name
	endm

;Macros to replace public, extrn, and endp for C-callable assembly routines,
; and to define labels: c_label defines labels,
; c_public replaces public, c_extrn replaces extrn, and c_endp replaces endp

c_name	macro	name
	name equ _&name
	endm

c_label macro	name
_&name:
	endm

c_public macro	a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
    ifnb <a>			;;Check for blank argument
	public	_&a
	a equ _&a
      ifnb <b>
	c_public b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
      endif
    endif
	endm

c_extrn macro	a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
    ifnb <a>			;;Check for blank argument
	extrn	_&a:b
	a equ _&a
      ifnb <c>
	c_extrn c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
      endif
    endif
	endm


c_endp	macro	name
_&name	ENDP
	endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Define function ctor as a static constructor
static_ctor macro	ctor
  ifndef _NOCTOR
  if 1
XIFCB	segment dword public 'DATA'
XIFCB	ends
XIFU	segment dword public 'DATA'
	;dd	ctor
XIFU	ends
XIFL	segment dword public 'DATA'
	;dd	ctor
XIFL	ends
XIFM	segment dword public 'DATA'
    if _FLAT
	dd	offset FLAT:ctor
    else
	dd	ctor
    endif
XIFM	ends
XIFCE	segment dword public 'DATA'
XIFCE	ends
    else
XIB	segment dword public 'DATA'
XIB	ends
XI	segment dword public 'DATA'
    if _FLAT
	dd	offset FLAT:ctor
    else
	dd	ctor
    endif
XI	ends
XIE	segment dword public 'DATA'
XIE	ends
  endif
  endif
	endm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Define function dtor as a static destructor
static_dtor macro	dtor
  ifndef _NOCTOR
  if 1
XOB	segment dword public 'DATA'
XOB	ends
XO	segment dword public 'DATA'
    if _FLAT
	dd	offset FLAT:dtor
    else
	dd	dtor
    endif
XO	ends
XOE	segment dword public 'DATA'
XOE	ends
  else
XCB	segment dword public 'DATA'
XCB	ends
XC	segment dword public 'DATA'
    if _FLAT
	dd	offset FLAT:dtor
    else
	dd	dtor
    endif

XC	ends
XCE	segment dword public 'DATA'
XCE	ends
  endif
  endif
	endm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Other more or less useful macros

;Aligns the code on dword boundary for max speed.
_align	macro
	;MASM 6.0 has a bug where ALIGN 4 produces bogus code when
	;2 or 3 bytes of filler are necessary.
	ifdef @Version
	  if @Version eq 600
	    if 0
	    if	(($ - _TEXT) and 3) eq 1
		db	08Bh,0C0h		;MOV EAX,EAX
		nop
	    else
	    if	(($ - _TEXT) and 3) eq 2
		db	08Bh,0C0h		;MOV EAX,EAX
	    else
	    if	(($ - _TEXT) and 3) eq 3
		nop
	    endif
	    endif
	    endif
	    endif
	  else
		align	4			;for MASM 5.1
	  endif
	else
		align	4			;for 386ASM
	endif
	endm

_push	macro	list
	irp	arg,<list>
	 push	arg
	endm
	endm

_pop	macro	list
	irp	arg,<list>
	 pop	arg
	endm
	endm

_if	macro	arg1,cond,arg2,lbl
	cmp	arg1,arg2
	j&cond	lbl
	endm

_ifs	macro	arg1,cond,arg2,lbl
	cmp	arg1,arg2
	j&cond	short lbl
	endm

clr	macro	list		;clear a register
	irp	reg,<list>
	 xor	reg,reg
	endm
	endm

jmps	macro	lbl
	jmp	short	lbl
	endm

tst	macro	reg
	test	reg,reg
	endm

ifndef nobdos

ifdef DOS386
bdos	macro	func		;DOS system call.
	ifnb	<func>
	 mov	AH,func
	endif
	int	21h
	endm
endif ;DOS386

ifdef X386
bdos	macro	func		;DOS system call.
	ifnb	<func>
	 mov	AH,func
	endif
	int	21h
	endm
endif ;DOS386

ifdef M_XENIX
bdos	macro	func		; 386 XENIX system call.
	ifnb	<func>
	 mov	EAX,func
	endif

	db 9ah			; call far 0x7:0
	dd 0
	dw 07h
	endm
endif ;M_XENIX

ifdef M_UNIX
bdos	macro	func		; UNIX system call.
	ifnb	<func>
	 mov	EAX,func
	endif

	db 9ah			; call far 0x7:0
	dd 0
	dw 07h
	endm
endif ;M_UNIX

endif ;nobdos

_retf	macro	val		;force assembler to build a far return
	ifnb	<val>
	 db	0CAh
	 dw	val
	else
	 db	0CBh
	endif
	endm

_ret	macro	val		;decide whether caller or callee cleans stack
	if _STDCALL
	  ret	val
	else
	  ret
	endif
	endm

; Macros to save and restore regs destroyed by a function
; Give the macro the list of registers used by the function:
;	uses	<AX,BX,SI,DI>
;
; At exit to function use 'unuse':
;	unuse	<DI,SI,BX,AX>

uses	macro	list
	irp	reg,<list>
	 ifidn	<reg>,<ebx>	;Save ebx.
	 push	reg
	 endif

	 ifidn	<reg>,<EBX>	;Save EBX (bug in ifidni).
	 push	reg
	 endif

	 ifidn	<reg>,<esi>	;Save esi.
	 push	reg
	 endif

	 ifidn	<reg>,<ESI>
	 push	reg
	 endif

	 ifidn	<reg>,<edi>	;Save edi.
	 push	reg
	 endif

	 ifidn	<reg>,<EDI>
	 push	reg
	 endif

	 ifidn	<reg>,<bx>	;Save bx.
	 push	reg
	 endif

	 ifidn	<reg>,<BX>
	 push	reg
	 endif

	 ifidn	<reg>,<si>	;Save si.
	 push	reg
	 endif

	 ifidn	<reg>,<SI>
	 push	reg
	 endif

	 ifidn	<reg>,<di>	;Save di.
	 push	reg
	 endif

	 ifidn	<reg>,<DI>
	 push	reg
	 endif

	 ifidn	<reg>,<ds>	;Save ds.
	 push	reg
	 endif

	 ifidn	<reg>,<DS>
	 push	reg
	 endif

	 ifidn	<reg>,<es>	;Save es.
	 push	reg
	 endif

	 ifidn	<reg>,<ES>
	 push	reg
	 endif
	endm
	endm

unuse	macro	list
	irp	reg,<list>
	 ifidn	<reg>,<ebx>	;Restore ebx.
	 pop	reg
	 endif

	 ifidn	<reg>,<EBX>
	 pop	reg
	 endif

	 ifidn	<reg>,<esi>	;Restore esi.
	 pop	reg
	 endif

	 ifidn	<reg>,<ESI>
	 pop	reg
	 endif

	 ifidn	<reg>,<edi>	;Restore edi.
	 pop	reg
	 endif

	 ifidn	<reg>,<EDI>
	 pop	reg
	 endif

	 ifidn	<reg>,<bx>	;Restore bx.
	 pop	reg
	 endif

	 ifidn	<reg>,<BX>
	 pop	reg
	 endif

	 ifidn	<reg>,<si>	;Restore si.
	 pop	reg
	 endif

	 ifidn	<reg>,<SI>
	 pop	reg
	 endif

	 ifidn	<reg>,<di>	;Restore di.
	 pop	reg
	 endif

	 ifidn	<reg>,<DI>
	 pop	reg
	 endif

	 ifidn	<reg>,<ds>	;Restore ds.
	 pop	reg
	 endif

	 ifidn	<reg>,<DS>
	 pop	reg
	 endif

	 ifidn	<reg>,<es>	;Restore es.
	 pop	reg
	 endif

	 ifidn	<reg>,<ES>
	 pop	reg
	 endif

	endm
	endm


endif	;M_I386

; Executable type
EXE_DOS		equ	1		; MSDOS
EXE_DOS16RM	equ	2		; Rational 286 DOS Extender
EXE_ZPM		equ	4		; ZPM 286 DOS Extender
EXE_PHAR386	equ	8		; Pharlap 386 DOS Extender
EXE_DOSX	equ	010h		; DOSX 386 DOS Extender
EXE_WINDOWS	equ	020h		; Windows 3
EXE_OS2		equ	040h		; OS/2 1.x
EXE_SCOUNIX	equ	080h		; SCO Unix
EXE_OS2_2	equ	100h		; OS/2 2.0
EXE_WINDOWSNT	equ	200h		; Windows NT

