; This is a part of the Microsoft Foundation Classes C++ library.
; Copyright (C) 1992-1993 Microsoft Corporation
; All rights reserved.
;
; This source code is only intended as a supplement to the
; Microsoft Foundation Classes Reference and Microsoft
; QuickHelp and/or WinHelp documentation provided with the library.
; See these sources for detailed information regarding the
; Microsoft Foundation Classes product.

;////////////////////////////////////////////////////////////////////////////
;* OLECALL_.ASM
;*
;* Intel 8086 implementation of _AfxDispatchCall
;////////////////////////////////////////////////////////////////////////////


_DATA       SEGMENT WORD PUBLIC 'DATA'
	extrn       _STKHQQ:WORD         ; stack bottom check value
_DATA       ENDS
DGROUP      GROUP   _DATA

; Do NOT pop r1 unless LARGE/MEDIUM model (opposite order from push2)
pop2        MACRO   r1, r2
			pop     r2
ifdef _LARGE_CODE
			pop     r1
endif
			ENDM

; Do NOT push r1 unless LARGE/MEDIUM model
push2       MACRO   r1, r2
ifdef _LARGE_CODE
			push    r1
endif
			push    r2
			ENDM

; Return to register (may be register pair)
retr2       MACRO   r1, r2
ifdef _LARGE_CODE
			push    r1
			push    r2
			retf
else
			jmp     r2
endif
			ENDM

_TEXT   SEGMENT BYTE PUBLIC 'CODE'
		ASSUME  CS:_TEXT, DS:DGROUP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; DWORD PASCAL
; _AfxDispatchCall(AFX_PMSG pfn, void BASED_STACK* pArgs, UINT nSizeArgs);

_AFXDISPATCHCALL:
		PUBLIC      _AFXDISPATCHCALL

		pop2    dx,cx           ;pop return address (in [dx]:cx)
		add     sp,4            ;pop 'nSizeArgs' and 'pArgs', don't need them
		pop2    bx,ax           ;pop 'pfn' (in [bx]:ax)

		push2   dx,cx           ;push this function's return address
		retr2   bx,ax           ;simulate return to [bx]:ax (pfn)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; BYTE BASED_STACK* PASCAL
; _AfxStackAlloc(size_t nSize, BYTE BASED_STACK* BASED_STACK* ppOldStack)

_AFXSTACKALLOC:
		PUBLIC  _AFXSTACKALLOC

		pop2    dx,cx           ;pop return address (in [dx]:cx)

		pop     bx              ;pop ppOldStack
		pop	ax		;pop nSize
		or	bx,bx
		jz	short skip_store
		mov	WORD PTR ss:[bx],sp	;save current stack pointer

skip_store:
		mov     bx,sp           ;adjust stack pointer
		sub     bx,ax
		mov     ax,0
		jb	short alloc_exit    ;underflow?
		cmp     bx,[_STKHQQ]
		jb	short alloc_exit    ;less than stack low?

		mov     ax,bx           ;new stack pointer is in bx
		xchg    bx,sp

alloc_exit:
		retr2   dx,cx           ;return to caller

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; void PASCAL
;; _AfxStackRestore(BYTE BASED_STACK pOldStack)

_AFXSTACKRESTORE:
		PUBLIC      _AFXSTACKRESTORE

		pop2    dx,cx       ;pop return address (in [dx]:cx)

		pop     ax          ;pop pOldStack
		mov     sp,ax       ; and restore stack pointer

		retr2   dx,cx       ;return to caller

_TEXT   ENDS
		END

;////////////////////////////////////////////////////////////////////////////
