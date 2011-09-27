;_ flt87.asm   Apr 7, 1991
;$Header: z:/core/crtl/src/core32/flt87.asm,v 1.1.1.1 1997/01/02 19:17:47 smarx Exp $
;Copyright (C) 1991 by Walter Bright
;All Rights Reserved, written by Walter Bright
;Floating point package

include macros.asm
include flthead.asm

	.287

    ifdef _MT
	extrn	__FEEXCEPT:near
    endif

	begdata

    ifndef _MT
	extrn	__fe_cur_env:word
    endif

	public roundto0
roundto0	dw	0FBFh		;8087 control word to round to 0

	enddata

	begcode	double

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Do an FCOMPP, but do not raise exceptions.
; Returns:
;	result in flags
;	no CPU registers changed

	c_public _FCOMPP@
func	_FCOMPP@
	push	EAX

	;Save any existing exception flags
	fstsw	AX
    ifdef _MT
	and	EAX,FE_ALL_EXCEPT
	jz	FC2			;no existing exceptions
	push	EAX
	call	__FEEXCEPT
FC2:
    else
	and	__fe_cur_env.status,FE_ALL_EXCEPT
	or	word ptr __fe_cur_env.status,AX
    endif

	fcompp
	fstsw	AX
	fclex				;clear any new exceptions
	sahf

	pop	EAX
	ret
c_endp	_FCOMPP@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Do an FTST, but do not raise exceptions.
; Returns:
;	result in flags
;	no CPU registers changed

	c_public _FTEST0@
func	_FTEST0@
	push	EAX

	;Save any existing exception flags
	fstsw	AX
    ifdef _MT
	and	EAX,FE_ALL_EXCEPT
	jz	FT1			;no existing exceptions
	push	EAX
	call	__FEEXCEPT
FT1:
    else
	and	__fe_cur_env.status,FE_ALL_EXCEPT
	or	word ptr __fe_cur_env.status,AX
    endif

	ftst
	fstp	ST(0)
	fstsw	AX
	fclex				;clear any new exceptions
	sahf

	pop	EAX
	ret
c_endp	_FTEST0@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set Z flag if ST0 is 0.
; Do not raise any exceptions.
; Do not pop ST0.
; Returns:
;	result in Z flag
;	no CPU registers changed

	c_public _FTEST@
func	_FTEST@
	push	EAX
	sub	ESP,8
	fst	qword ptr [ESP]
	fwait
	mov	EAX,4[ESP]
	shl	EAX,1		;no -0 bugs
	or	EAX,[ESP]
	add	ESP,8
	tst	EAX
	pop	EAX
	ret
c_endp	_FTEST@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Test ST0, pop it, and set results in PSW.

c_public _DTST87@

func	_DTST87@
	FTST
	FSTP	ST(0)
;	callm	_87TOPSW@
;	ret
c_endp	_DTST87@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert 8087 flags into 8088 flags.

c_public _87TOPSW@

func	_87TOPSW@
	clr	EAX
;	fnstsw	AX		;transfer 80387 status word
	db	0DFh,0E0h	;386asm doesn't like the ax destination.
	ret
c_endp	_87TOPSW@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert EDX,EAX into ST0.

c_public _DBLTO87@
func	_DBLTO87@
	push	EDX
	push	EAX
	fld	qword ptr [ESP]
N1:	fwait			;make sure it's done before popping stack
	lea	ESP,8[ESP]	;do not affect flags
	ret
c_endp	_DBLTO87@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert ST0 into a short in AX.
; ANSI says that rounding to int truncates towards 0.

c_public _DBLINT87@

func	_DBLINT87@
	sub	ESP,8
	fstcw	4[ESP]			;save original control word
	fldcw	roundto0
	fistp	word ptr [ESP]
	fwait
	movzx	EAX,word ptr [ESP]	;Clear upper word and put in EAX.
	;fnldcw (don't need fwait here)
	fldcw	4[ESP]			;restore original control word
	fwait
	add	ESP,8
	ret
c_endp	_DBLINT87@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert ST0 into a long in EAX.
; ANSI says that rounding to long truncates towards 0.

c_public _DBLLNG87@
func	_DBLLNG87@
	sub	ESP,8
	fstcw	4[ESP]		;save original control word
	fldcw	roundto0
	fistp	dword ptr [ESP]
	fwait
	pop	EAX
	fldcw	[ESP]
	add	ESP,4
	ret
c_endp	_DBLLNG87@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert EAX (float) into ST0.

c_public _FLTTO87@
func	_FLTTO87@
	push	EAX
	fld	dword ptr [ESP]
	fwait
	pop	EAX
	ret
c_endp	_FLTTO87@


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get and return the 8087 status word.
; This routine will hang if there is no 8087, therefore
; __8087 should be consulted before calling it.

	c_public	_status87
func	_status87
	clr	EAX
;	fnstsw	AX		;transfer 80387 status word
	db	0DFh,0E0h	;386asm doesn't like the ax destination.
	ret
c_endp	_status87

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get and clear the 8087 status word.
; This routine will hang if there is no 8087, therefore
; __8087 should be consulted before calling it.
; Returns:
;	8087 status word prior to its being cleared

	c_public	_clear87
func	_clear87
	clr	EAX
;	fnstsw	AX		;transfer 80387 status word
	db	0DFh,0E0h	;386asm doesn't like the ax destination.
	fclex			;clear exceptions
	fwait
	ret
c_endp	_clear87

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get and set the 8087 control word.
; This routine will hang if there is no 8087, therefore
; __8087 should be consulted before calling it.
; Use:
;	unsigned _control87(newvalue,mask)
;	unsigned newvalue;	/* new value for control word		*/
;	unsigned mask;		/* mask for which bits to modify	*/
; The control word will be set to:
;	(oldcontrolword & ~mask) | (newvalue & mask)
; Returns:
;	8087 control word prior to its being set

	c_public	_control87
func	_control87
	push	EBP
	mov	EBP,ESP
	push	EAX		;make room on stack
	uses	EBX

	fstcw	-4[EBP]		;store 8087 control word
	mov	ECX,P+4[EBP]
	mov	EBX,ECX
	not	EBX		;EBX = ~mask
	and	ECX,P[EBP]	;ECX = newvalue & mask
	fwait			;make sure the control word is there
	mov	EAX,-4[EBP]	;EAX = control word
	and	EBX,EAX		;EBX = control & ~mask
	or	EBX,ECX		;EBX = (control & ~mask) | (newvalue & mask)
	mov	-4[EBP],EBX
	;fnldcw	-4[EBP]		;load new control word
	db	0D8h+1,06Dh,-4
	fwait

	unuse	EBX
	mov	ESP,EBP
	pop	EBP
	_ret	8
c_endp	_control87

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Reset 8087
; This routine will hang if there is no 8087, therefore
; __8087 should be consulted before calling it.

	c_public	_fpreset
func	_fpreset
	finit
	ret
c_endp	_fpreset

	endcode	double

	end
