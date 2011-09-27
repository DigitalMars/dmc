;_ dllstart.asm
; equivalent of winstart.asm except for DLL's instead of regular Windows apps
; Copyright (C) 1991 Walter Bright
; All Rights Reserved
; Written by: G. Eric Engstrom

includelib libw.lib

	DOSSEG		;have linker fix ordering of segments

      ;; Define LCODE
      ifdef I8086S
        LCODE equ 0
      endif
      ifdef I8086M
        LCODE equ 1
      endif
      ifdef I8086L
        LCODE equ 1
      endif
      ifdef I8086C
        LCODE equ 0
      endif
        

	extrn	LIBMAIN		:far
        extrn	LOCALINIT	:far
	extrn	__cinit_f	:far
	extrn   __CALL_RAWLIBMAIN :far
	extrn	__alloc_flags : word
;	extrn   ___dll_started  :byte
       extrn __osmode      : byte
       extrn __cpumode     : byte
      if LCODE
        extrn _getpsp : far
      else
        extrn _getpsp : near
      endif
      

GMEM_DDESHARE	equ 2000h
GMEM_MOVEABLE	equ 0002h

	public	rsrvptrs

NULL	segment para public 'BEGDATA'
	;Windows seems to require this
	dw	0,0
rsrvptrs dw	5
	dw	0,0,0,0,0
NULL	ends

_DATA	segment para public 'DATA'

         extrn   __psp       : word
		db	0,1
__module_handle	dw	?		; Module handle



	public __hModule
__hModule dw	0
        public ___dll_started
___dll_started db 0

_DATA	ends

CDATA	segment word common 'DATA'
CDATA	ends
CONST	segment word public 'CONST'
CONST	ends
_BSS	segment word public 'BSS'
_BSS	ends
XIFB	segment word public 'DATA'
XIFB	ends
XIF	segment word public 'DATA'
XIF	ends
XIFE	segment word public 'DATA'
XIFE	ends
XIB	segment word public 'DATA'
XIB	ends
XI	segment word public 'DATA'
XI	ends
XIE	segment word public 'DATA'
XIE	ends
XOB	segment word public 'BSS'
XOB	ends
XO	segment word public 'BSS'
XO	ends
XOE	segment word public 'BSS'
XOE	ends
XPB	segment word public 'DATA'
XPB	ends
XP	segment word public 'DATA'
XP	ends
XPE	segment word public 'DATA'
XPE	ends
XCB	segment word public 'DATA'
XCB	ends
XC	segment word public 'DATA'
XC	ends
XCE	segment word public 'DATA'
XCE	ends
XCFB	segment word public 'DATA'
XCFB	ends
XCF	segment word public 'DATA'
XCF	ends
XCFE	segment word public 'DATA'
XCFE	ends
DBDATA	segment word common 'DATA'
DBDATA	ends

DGROUP	group	NULL,_DATA,CDATA,CONST,_BSS,XIFB,XIF,XIFE,XIB,XI,XIE,XOB,XO,XOE,XPB,XP,XPE,XCB,XC,XCE,XCFB,XCF,XCFE

_TEXT	segment para public 'CODE'
	assume	CS:_TEXT
        assume  DS:DGROUP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; On input:
;	SS:SP	Current user stack
;	DS	Selector for DLL's DGROUP, if any, otherwise, selector
;		for DGROUP of process attaching to DLL.
;		C and C++ DLLs always have a DGROUP.
; For Windows:
;	DI	instance handle
;	CX	heap size
;	ES:SI	command line
; Returns:
;	AX	0 error, !=0 success
	

	public	_CRTLibEntry

_CRTLibEntry proc	far
	mov	[__alloc_flags], GMEM_DDESHARE + GMEM_MOVEABLE
        mov     __osmode,2      ;indicates windows
        mov     __cpumode,1     ;we're in protected mode
	cld			;no direction flag bugs

        ;; Setup _psp
        call _getpsp
        mov __psp, AX

	;Arguments to __far __pascal __CALL_RAWLIBMAIN(HANDLE,WORD,WORD,LPSTR)
	push	DI		;module instance handle
        push	DS		;data segment
        push	CX		;size of heap
        push	ES
        push	SI		;command line
        jcxz	L1			;if heap size is non-zero

	    ;LocalInit sets up the Windows heap
        push	  DS
        xor	  AX,AX
        push	  AX
        push	  CX
        call	  LOCALINIT		;(DS:0,CX)
        or	  AX,AX
        jz	  L8	 ;error
        call  __CALL_RAWLIBMAIN ;; Check user defined function ptr.
        or    AX,AX
        jz    L9    ; error, but stack is cleaned up
        ;;Arguments to __far __pascal LibMain(HANDLE,WORD,WORD,LPSTR)
        push	DI		;module instance handle
        push	DS		;data segment
        push	CX		;size of heap
        push	ES
        push	SI		;command line
L1:	
        mov	__module_handle,DI
        mov     __hModule, DI
	call   	__cinit_f		; call static constructors
	inc     ___dll_started
        call	LIBMAIN			; invoke the 'C' routine for initialization
        ret				; LibMain is responsible for stack clean up

L8:	add	SP,10			; clean up stack if LibMain not called
L9:
        ret
_CRTLibEntry endp

_TEXT	ends

	end	
