;_ getdos.asm   Mon Dec 18 1989   Modified by: Walter Bright */
; $Header: z:/core/crtl/src/dos32/getdos.asm,v 1.1.1.1 1997/01/02 19:17:57 smarx Exp $
;Copyright (C) 1984-1991 by Walter Bright
;All rights reserved.
;Written by Walter Bright
;Modified 22 May 90 by Bob Stout

include	macros.asm
include stdio.asm

extrn __SET_DOS_ERRNO:near

	begdata
	c_extrn _osmajor,byte
	waiting_ch	db	0
	ch_is_waiting	db	0

D4CB	STRUC				;DOS 4 control block
    	start   	dw      ?
	fill    	dw      0
	number  	dw      ?
	bufl     	dw      ?
	bufh     	dw      ?
	d4cbsz		dw	0
D4CB	ENDS

	enddata

	c_extrn	_flushterm,near

	begcode	getdos

	c_public dos_set_verify,dos_get_verify
	c_public dos_set_ctrl_break,dos_get_ctrl_break
	c_public dos_getdiskfreespace
if 0  ;not currently supported
	c_public dos_abs_disk_read,dos_abs_disk_write
endif
	c_public kbhit,getch,getche,ungetch
	c_public mkdir,rmdir,chdir
	c_public _chdrive,_getdrive
	public	__chdir

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Return 1 if key is available from console, else 0

func	kbhit
        cmp	ch_is_waiting,0
        jz	kbh1
        mov	EAX,1
        jmp	kbhret
kbh1:	clr	EAX
	bdos	0Bh	;check standard input status
	movsx	EAX,AL	;EAX is now 0 or -1
	neg	EAX	;0 or 1
kbhret:
	ret
c_endp	kbhit

;;;;;;;;;;;;;;;;;;;;;;;;;
; Get character from keyboard, don't echo it.

func	getch
	call	_flushterm
	cmp	ch_is_waiting,0
	jz	gch1
	mov	AL,waiting_ch
	mov	ch_is_waiting,0
	jmp	G2
gch1:	mov	AH,8			;get char from kbd, no echo
	jmp short G1
c_endp	getch

;;;;;;;;;;;;;;;;;;;;;;;;;
; Get character from keyboard, echo it.

func	getche
	call	_flushterm
	cmp	ch_is_waiting,0
	jz	gce1
	mov	AL,waiting_ch
	mov	ch_is_waiting,0
	jmp	G2
gce1:	mov	AH,1
G1:	bdos			;get char from kbd, echo it
G2:	movzx	EAX,AL
gceret:
	ret
c_endp	getche

;;;;;;;;;;;;;;;;;;;;;;;;;
; Put one character back in keyboard queue

func	ungetch
	push	EBP
	mov	EBP,ESP
	cmp	ch_is_waiting,0
	jz	ugc1
	mov	EAX,-1
	jmp	ugc2
ugc1:	mov	EAX,P[EBP]
	cmp	EAX,-1
	jz	ugc2
	mov	waiting_ch,AL
	mov	ch_is_waiting,1
	movzx	EAX,AL
ugc2:	pop	EBP
	ret	
c_endp	ungetch

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set/reset verify switch for disk writes.
; Use:
;	dos_set_verify(TRUE);	/* turn on verify	*/
;	dos_set_verify(FALSE);	/* turn off verify	*/

func	dos_set_verify
	push	EBP
	mov	EBP,ESP
	mov	AL,P[EBP]	;get verify flag
	clr	DL		;must be 0 for some reason
	bdos	2Eh
	pop	EBP
	ret
c_endp	dos_set_verify

;;;;;;;;;;;;;;;;;;;;;;;;;
; Get disk verify state.
; Returns:
;	TRUE if disk verify is on

func	dos_get_verify
	bdos	54h
	cbw			;clear AH
	ret
c_endp	dos_get_verify

;;;;;;;;;;;;;;;;;;;;;;;;
; Set/reset Ctrl-Break check.
; Use:
;	dos_set_ctrl_break(TRUE);	/* turn on cntrl-break check	*/
;	dos_set_ctrl_break(FALSE);	/* turn off cntrl-break check	*/

func	dos_set_ctrl_break
	push	EBP
	mov	EBP,ESP
	mov	DX,P[EBP]
	mov	AL,1
	bdos	33h
	pop	EBP
	ret
c_endp	dos_set_ctrl_break

;;;;;;;;;;;;;;;;;;;;;;;;;
; Get state of Ctrl-Break checking.
; Returns:
;	TRUE	on
;	FALSE	off

func	dos_get_ctrl_break
	mov	AX,03300h
	bdos
	mov	AL,DL
	cbw			;clear	AH
	ret
c_endp	dos_get_ctrl_break

;;;;;;;;;;;;;;;;;;;;;;;;;
; Get disk free space in bytes.
; Use:
;	long = dos_getdiskfreespace(drive);
; Input:
;	drive == 0:	default drive
;	drive == 1:	drive = A:, etc.
; Returns:
;	a long
;	-1L if drive was invalid

func	dos_getdiskfreespace
	push	EBP
	mov	EBP,ESP
	uses	EBX
	mov	DX,P[EBP]	;DL = drive
	bdos	36h
	_ifs	AX ne -1, L1
	  clr     EAX
	  dec     EAX
	  unuse   EBX
	  pop     EBP
	  ret                     ;return -1L
	;Compute EAX = EAX*EBX*ECX
L1:	movzx	EAX,AX
	movzx	EBX,BX
	movzx	ECX,CX
	mul	EBX
	mul	ECX
	unuse	EBX
	pop	EBP
	ret
c_endp	dos_getdiskfreespace

if 0     ;abs_disk_read and abs_disk_write are not currently supported
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Absolute disk read.
;	int dos_abs_disk_read(drive,numsec,startsec,buffer)
;	int drive;	/* drive number 0=A, 1=B, ...			*/
;	int numsec;	/* number of sectors to read			*/
;	int startsec;	/* starting logical sector number		*/
;	char *buffer;	/* where the result will be stored		*/
; Returns:
;	0		/* success					*/
;	!= 0		/* failure (the value is what is described in	*/
;			/* the DOS manual)				*/

func	dos_abs_disk_read
	mov	AH,1
A4:
	push	EBP
	mov	EBP,ESP
	uses	<ESI,EDI>
	sub	ESP,d4cbsz
	movzx	ECX,byte ptr __osmajor
	mov	ESI,ECX
	mov	AL,P[EBP]		;drive
	mov	CX,P+2[EBP]		;numsec
	mov	DX,P+4[EBP]		;startsec
	push	DS
	push	EBP
	mov	EBX,P+6[EBP]		;buffer
    	cmp	ESI,4			;DOS 4+?
	jb	A5			;No, skip it
	  mov     -4-d4cbsz+start[EBP],DX  ;Yes, fill in D4CB structure
	  mov     -4-d4cbsz+fill[EBP],0
	  mov     -4-d4cbsz+number[EBP],CX
	  mov     -4-d4cbsz+bufl[EBP],BX
	  mov     -4-d4cbsz+bufh[EBP],DS
	  mov     CX,-1
	  push    SS                      ;Point to DCB
	  pop     DS
	  mov     EBX,EBP
	  sub     EBX,4+d4cbsz
A5:	tst	AH			;do read or write?
	jz	A3
	int	25h			;read
	jmps	A2

A3:	int	26h			;write
A2:	jc	A1			;error (AX has error code)
	clr	EAX			;no error
A1:	popf				;clean stack
	pop	EBP
	pop	DS
    	add	ESP,d4cbsz		;forget D4CB structure
	unuse	<EDI,ESI>
adrret:
	pop	EBP
	ret
c_endp	dos_abs_disk_read

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Absolute disk write.
;	int dos_abs_disk_write(drive,numsec,startsec,buffer)
;	int drive;	/* drive number 0=A, 1=B, ...			*/
;	int numsec;	/* number of sectors to write			*/
;	int startsec;	/* starting logical sector number		*/
;	char *buffer;	/* where the result will be read from		*/
; Returns:
;	0		/* success					*/
;	!= 0		/* failure (the value is what is described in	*/
;			/* the DOS manual)				*/

func	dos_abs_disk_write
	clr	AH
	jmp	A4		;use common code
c_endp	dos_abs_disk_write
endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Change directory to that specified by pathname.
; Usage:
;	int chdir(char *pathname);
; Returns:
;	0	success
;	-1	failure (errno is set to ENOENT)

__chdir proc
__chdir endp

func	chdir
	mov	AH,03Bh

_dodir:	mov	EDX,PS[ESP]
	bdos
	jnc	D1		;no error
	push    EAX
        call    __SET_DOS_ERRNO
        mov     EAX,-1
        ret
D1:	mov	EAX,0
	ret
c_endp	chdir

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Create subdirectory given by pathname.
; Usage:
;	int mkdir(char *pathname);
; Returns:
;	0	success
;	-1	failure, in which case errno is set to:
;	    ENOENT:	path not found
;	    EACCES:	access denied

func	mkdir
	mov	AH,039h
	jmp	_dodir
c_endp	mkdir

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Remove subdirectory given by pathname.
; Usage:
;	int rmdir(char *pathname);
; Returns:
;	0	success
;	-1	failure, in which case errno is set to:
;	    ENOENT:	path not found
;	    EACCES:	access denied or directory is not empty

func	rmdir
	mov	AH,03Ah
	jmp	_dodir
c_endp	rmdir

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; int _chdrive(int drive);
; Returns:
;	0	success
;	-1	failure

func	_chdrive
	mov	DL,PS[ESP]	;get drive number
	dec	DL		;to 0=A, 1=B, etc.
	bdos	0Eh		;select drive
	bdos	19h		;read current drive
	sub	AL,DL		;see if it matches
	jz	short CH1	;yes they do
	  mov     AL,0FFh         ;error (return -1)
CH1:	movsx	EAX,AL
	ret
c_endp	_chdrive

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; int _getdrive(void);
; Returns:
;	1=A, 2=B, etc.

func	_getdrive
	bdos	19h		;read current drive
	movzx	EAX,AL
	inc	EAX
	ret
c_endp	_getdrive

	endcode	getdos

	end

