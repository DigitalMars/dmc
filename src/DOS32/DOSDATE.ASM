;_ dosdate.asm   Tue Oct 10 1989   Modified by: Walter Bright */
; $Header: z:/core/crtl/src/dos32/dosdate.asm,v 1.1.1.1 1997/01/02 19:17:56 smarx Exp $
;Copyright (C) 1989 by Walter Bright
;All rights reserved.
;Written by Dave Mansell

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

	begcode	dosdate

	c_public dos_setdate, dos_getdate, dos_settime, dos_gettime

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; set dos date information
;	int dos_setdate(date)

func	dos_setdate
	push	EBP
	mov	EBP,ESP
	push	EDI
    if SPTR
	mov	EAX,P[EBP]	;where to get the information
    else
	push	DS
	lds	EAX,P[EBP]	;where to get the information
    endif
	mov	DL,day[EAX]
	mov	DH,month[EAX]
	mov	CX,year[EAX]
	bdos	2Bh			;set system date
ex1:	movzx	EAX,AL
    if LPTR
	pop	DS
    endif
	pop	EDI
	mov	ESP,EBP
	pop	EBP
	ret
c_endp	dos_setdate

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get dos date information
;	void dos_getdate(date)

func	dos_getdate
	push	EBP
	mov	EBP,ESP
	push	EDI
	bdos	2Ah		;get system date
    if SPTR
	mov	EDI,P[EBP]	;where to store the information
    else
	push	DS
	lds	EDI,P[EBP]	;where to store the information
    endif
	mov	day[EDI],DL
	mov	month[EDI],DH
	mov	year[EDI],CX
	mov	dofw[EDI],AL
	jmps	ex1
c_endp	dos_getdate

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; set dos time information
;	int dos_settime(time)

func	dos_settime
	push	EBP
	mov	EBP,ESP
	push	EDI
    if SPTR
	mov	EDI,P[EBP]	;where to get the information
    else
	push	DS
	lds	EDI,P[EBP]	;where to get the information
    endif
	mov	CH,hour[EDI]
	mov	CL,minutes[EDI]
	mov	DH,seconds[EDI]
	mov	DL,hseconds[EDI]
	bdos	2Dh			;set system time
	jmps	ex1
c_endp	dos_settime

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get dos time information
;	void dos_gettime(time)

func	dos_gettime
	push	EBP
	mov	EBP,ESP
	push	EDI
	bdos	2Ch		;get system time
    if SPTR
	mov	EDI,P[EBP]	;where to store the information
    else
	push	DS
	lds	EDI,P[EBP]	;where to store the information
    endif
	mov	hour[EDI],CH
	mov	minutes[EDI],CL
	mov	seconds[EDI],DH
	mov	hseconds[EDI],DL
	jmp	ex1
c_endp	dos_gettime

	endcode	dosdate

	end

