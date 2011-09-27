;_ flt87.asm   Apr 7, 1991
;$Header: z:/core/crtl/src/core16/flt87.asm,v 1.1.1.1 1997/01/02 19:17:35 smarx Exp $
;Copyright (C) 1991 by Walter Bright
;All Rights Reserved, written by Walter Bright
;Floating point package

include macros.asm
include flthead.asm

	.8087

    ifdef _MT
      if LCODE
	extrn	__FEEXCEPT:far
      else
	extrn	__FEEXCEPT:near
      endif
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
	WINENTER
	sub	SP,2
	push	AX

	;Save any existing exception flags
	fstsw	-2[BP]
	fcompp
    ifdef _MT
	mov	AX,-2[BP]
	and	AX,FE_ALL_EXCEPT
	jz	FC2			;no existing exceptions
	push	AX
	call	__FEEXCEPT
FC2:
    else
	and	__fe_cur_env.status,FE_ALL_EXCEPT
	mov	AX,-2[BP]
	or	word ptr __fe_cur_env.status,AX
    endif

	fstsw	-2[BP]
FC1:	fclex				;clear any new exceptions
	mov	AX,-2[BP]
	sahf

	pop	AX
	mov	SP,BP
	pop	BP
    ifdef _WINDOWS
    ifndef I8086S
    ifndef I8086C
        lea	BP,-1[BP]		;can't use WINLEAVE, because
					;the DEC BP will step on flags
    endif
    endif
    endif
	ret
c_endp	_FCOMPP@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Do an FTST, but do not raise exceptions.
; Returns:
;	result in flags
;	no CPU registers changed

	c_public _FTEST0@
func	_FTEST0@
	WINENTER
	sub	SP,2
	push	AX

	;Save any existing exception flags
	fstsw	-2[BP]
    ifdef _MT
	fwait
	mov	AX,-2[BP]
	and	AX,FE_ALL_EXCEPT
	jz	FT1			;no existing exceptions
	push	AX
	call	__FEEXCEPT
FT1:
    else
	and	__fe_cur_env.status,FE_ALL_EXCEPT
	fwait
	mov	AX,-2[BP]
	or	word ptr __fe_cur_env.status,AX
    endif

	ftst
	fstsw	-2[BP]
	fstp	ST(0)
	jmp	FC1
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
	push	BP
	mov	BP,SP
	sub	SP,8
	fst	qword ptr -8[BP]
	push	AX
	fwait
	mov	AX,-2[BP]
	shl	AX,1		;no -0 bugs
	or	AX,-4[BP]
	or	AX,-6[BP]
	or	AX,-8[BP]
	pop	AX
	mov	SP,BP
	pop	BP
	ret
c_endp	_FTEST@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Test ST0, pop it, and set results in PSW.

;**** NO LONGER USED ****
c_public	_DTST87@

func	_DTST87@
	push	BP
	mov	BP,SP
	FTST
	FSTP	ST(0)
;	callm	_87TOPSW@
;	ret
c_endp	_DTST87@


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert 8087 flags into 8088 flags.

;**** NO LONGER USED ****

c_public _87TOPSW@

func	_87TOPSW@
	push	BP
	mov	BP,SP
	sub	SP,2			;can only transfer through memory
	fstsw	word ptr -2[BP]		;transfer 8087 status word
	push	AX			;save AX, allow 8087 time to finish
	fwait				;make sure 8087 is finished
	mov	AH,byte ptr -1[BP]	;interested in second byte
	sahf				;transfer to 8088 flags
	pop	AX
	mov	SP,BP
	pop	BP
	ret
c_endp	_87TOPSW@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert AX,BX,CX,DX into ST0.


c_public _DBLTO87@

func	_DBLTO87@
	push	BP
	mov	BP,SP
	_push	<AX,BX,CX,DX>
	fld	qword ptr -8[BP]
N1:	fwait			;make sure it's done before popping stack
	mov	SP,BP
	pop	BP
	ret
c_endp	_DBLTO87@


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert ST0 into an int in AX.
; ANSI says that rounding to int truncates towards 0.

c_public _DBLINT87@

func	_DBLINT87@
	push	BP
	mov	BP,SP
	sub	SP,4
	fstcw	-2[BP]		;save original control word
	fldcw	roundto0
	fistp	word ptr -4[BP]
	fwait
	pop	AX
N3:	;fnldcw	-2[BP]		;restore original control word
	db	0D8h+1,06Eh,-2
	fwait
	mov	SP,BP
	pop	BP
	ret
c_endp	_DBLINT87@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert ST0 into a long in DX,AX.
; ANSI says that rounding to long truncates towards 0.

c_public  _DBLLNG87@

func	_DBLLNG87@
	push	BP
	mov	BP,SP
	sub	SP,6
	fstcw	-2[BP]		;save original control word
	fldcw	roundto0
	fistp	dword ptr -6[BP]
	fwait
	pop	AX
	pop	DX
	jmps	N3
c_endp	_DBLLNG87@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert DX,AX (float) into ST0.

c_public	_FLTTO87@

func	_FLTTO87@
	push	BP
	mov	BP,SP
	_push	<DX,AX>
	fld	dword ptr -4[BP]
	fwait			;make sure it's done before popping stack
	mov	SP,BP
	pop	BP
	ret
c_endp	_FLTTO87@


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get and return the 8087 status word.
; This routine will hang if there is no 8087, therefore
; _8087 should be consulted before calling it.

	c_public	_status87
func	_status87
	WINENTER
	push	AX		;make room on stack
	fstsw	word ptr -2[BP]	;store 8087 status word
S1:	fwait
	pop	AX
	WINLEAVE
	ret
c_endp	_status87

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get and clear the 8087 status word.
; This routine will hang if there is no 8087, therefore
; _8087 should be consulted before calling it.
; Returns:
;	8087 status word prior to its being cleared

	c_public	_clear87
func	_clear87
	WINENTER
	push	AX		;make room on stack
	fstsw	word ptr -2[BP]	;store 8087 clear word
	fclex			;clear exceptions
	jmp	S1
c_endp	_clear87

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get and set the 8087 control word.
; This routine will hang if there is no 8087, therefore
; _8087 should be consulted before calling it.
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
	WINENTER
	push	AX		;make room on stack
	fstcw	word ptr -2[BP]	;store 8087 control word
	mov	CX,P+2[BP]
	mov	BX,CX
	not	BX		;BX = ~mask
	and	CX,P[BP]	;CX = newvalue & mask
	fwait			;make sure the control word is there
	pop	AX		;AX = control word
	and	BX,AX		;BX = control & ~mask
	or	BX,CX		;BX = (control & ~mask) | (newvalue & mask)
	push	BX
	;fnldcw	-2[BP]		;load new control word
	db	0D8h+1,06Eh,-2
	fwait
	pop	BX
	WINLEAVE
	ret
c_endp	_control87

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Reset 8087
; This routine will hang if there is no 8087, therefore
; _8087 should be consulted before calling it.

	c_public	_fpreset
func	_fpreset
	finit
	ret
c_endp	_fpreset

	endcode	double

	end
