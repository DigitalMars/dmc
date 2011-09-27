;_ dosdate.asm	 Tue Oct 10 1989   Modified by: Walter Bright */
; Windows support added by G. Eric Engstrom Dec 1990	-D_WINDOWS
; Modified by G. Eric Engstrom Jan 1991
; Copyright (C) 1989 by Walter Bright
; All rights reserved.
; Written by Dave Mansell

include	macros.asm

DATE STRUC
	day		db	?
	month		db	?
	year		dw	?
	dofw		db	?
DATE ENDS

TIME STRUC
	hour		db	?
	minutes		db	?
	seconds		db	?
	hseconds	db	?
TIME ENDS

DATETIME STRUC
	Ohour		db	?
	Ominutes	db	?
	Oseconds	db	?
	Ohseconds	db	?
	Oday		db	?
	Omonth		db	?
	Oyear		dw	?
	Ozone		dw	?
	Odofw		db	?
DATETIME ENDS

DATETIMESIZE equ 11

    ifdef __OS2__
	extrn	DOSGETDATETIME:far
	extrn	DOSSETDATETIME:far
    endif

	begcode	dosdate

	c_public dos_setdate, dos_getdate, dos_settime, dos_gettime

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set dos date information.
;	int dos_setdate(date)

func	dos_setdate
	WINENTER
    ifdef __OS2__
	sub	SP,DATETIMESIZE
	mov	BX,SP		;local time and date structure
	push	SS
	push	BX
	call	DOSGETDATETIME
	push	DI
    if SPTR
	mov	DI,P[BP]	;where to get the information
    else
	push	DS
	lds	DI,P[BP]	;where to get the information
    endif
;	mov	AL,day[DI]
;	mov	SS:Oday[BX],AL
;	mov	AL,month[DI]
;	mov	SS:Omonth[BX],AL
	mov	AX,word ptr day[DI]
	mov	word ptr SS:Oday[BX],AX	;load day & month as a word, gee
	mov	AX,year[DI]
	mov	SS:Oyear[BX],AX
	push	SS
	push	BX
	call	DOSSETDATETIME
    else ;__OS2__
	push	DI
    if SPTR
	mov	DI,P[BP]	;where to get the information
    else
	push	DS
	lds	DI,P[BP]	;where to get the information
    endif
;	mov	DL,day[DI]
;	mov	DH,month[DI]
	mov	DX,word ptr day[DI]	;load day & month as a word, gee
	mov	CX,year[DI]
	bdos	02Bh			;set system date
    endif ;__OS2__
ex1:	clr	AH
    if LPTR
	pop	DS
    endif
	pop	DI
	mov	SP,BP
	WINLEAVE
	ret
c_endp	dos_setdate

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get dos date information
;	void dos_getdate(date)

func	dos_getdate
	WINENTER
ifdef __OS2__
	sub	SP,DATETIMESIZE
	mov	BX,SP		;local time and date structure
	push	SS
	push	BX
	call	DOSGETDATETIME
	push	DI
    if SPTR
	mov	DI,P[BP]	;where to get the information
    else
	push	DS
	lds	DI,P[BP]	;where to get the information
    endif
;	mov	AL,SS:Oday[BX]
;	mov	day[DI],AL
;	mov	AL,SS:Omonth[BX]
;	mov	month[DI],AL
	mov	AX,word ptr SS:Oday[BX]
	mov	word ptr day[DI],AX	;load day & month as a word, gee
	mov	AX,SS:Oyear[BX]
	mov	year[DI],AX
	mov	AL,SS:Odofw[BX]
	mov	dofw[DI],AL
else
	push	DI
	bdos	02Ah		;get system date
    if SPTR
	mov	DI,P[BP]	;where to store the information
    else
	push	DS
	lds	DI,P[BP]	;where to store the information
    endif
;	mov	day[DI],DL
;	mov	month[DI],DH
	mov	word ptr day[DI],DX	;save day & month as a word, gee
	mov	year[DI],CX
	mov	dofw[DI],AL
endif
	jmps	ex1
c_endp	dos_getdate

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; set dos time information
;	int dos_settime(time)

func	dos_settime
	WINENTER
    ifdef __OS2__
	sub	SP,DATETIMESIZE
	mov	BX,SP		;local time and date structure
	push	SS
	push	BX
	call	DOSGETDATETIME
	push	DI
    if SPTR
	mov	DI,P[BP]	;where to get the information
    else
	push	DS
	lds	DI,P[BP]	;where to get the information
    endif
	mov	AX,word ptr hour[DI]
	mov	word ptr SS:Ohour[BX],AX	;move hour & minutes as a word, gee
	mov	AX,word ptr seconds[DI]
	mov	word ptr SS:Oseconds[BX],AX	;move seconds & hseconds as a word, gee
	push	SS
	push	BX
	call	DOSSETDATETIME
    else ;__OS2__
	push	DI
    if SPTR
	mov	DI,P[BP]	;where to get the information
    else
	push	DS
	lds	DI,P[BP]	;where to get the information
    endif
	mov	CX,word ptr hour[DI]	; load hours & minutes as a word, gee
	xchg	CH,CL			; swap because they are in the wrong order, gee
	mov	DX,word ptr seconds[DI] ; load seconds & hseconds as a word, gee
	xchg	DH,DL			; swap because they are in the wrong order, gee
	bdos	02Dh			;set system time
    endif ;__OS2__
	jmps	ex1
c_endp	dos_settime

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get dos time information
;	void dos_gettime(time)

func	dos_gettime
	WINENTER
ifdef __OS2__
	sub	SP,DATETIMESIZE
	mov	BX,SP		;local time and date structure
	push	SS
	push	BX
	call	DOSGETDATETIME
	push	DI
    if SPTR
	mov	DI,P[BP]	;where to get the information
    else
	push	DS
	lds	DI,P[BP]	;where to get the information
    endif
	mov	AX,word ptr SS:Ohour[BX]
	mov	word ptr hour[DI],AX	;move hour & minutes as a word, gee
	mov	AX,word ptr SS:Oseconds[BX]
	mov	word ptr seconds[DI],AX ;move seconds & hseconds as a word, gee
else
	push	DI
	bdos	02Ch		;get system time
    if SPTR
	mov	DI,P[BP]	;where to store the information
    else
	push	DS
	lds	DI,P[BP]	;where to store the information
    endif
		xchg	CH,CL			;swap cause they are in the wrong order,gee
	mov	word ptr hour[DI],CX	;save hours&minutes as a word,gee
	xchg	DH,DL			;swap cause they are in the wrong order,gee
	mov	word ptr seconds[DI],DX ;save seconds&hseconds as a word,gee
endif
	jmp	ex1
c_endp	dos_gettime

	endcode	dosdate

	end

