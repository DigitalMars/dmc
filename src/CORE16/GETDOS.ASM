;_ getdos.asm	Mon Dec 18 1989   Modified by: Walter Bright
; Windows support added by G. Eric Engstrom Dec 1990	-D_WINDOWS
; Modified by Bob Stout May 1990
; Copyright (C) 1984-1989 by Walter Bright
; All rights reserved.
; Written by Walter Bright
; $Revision: 1.1.1.1 $

include	macros.asm
include stdio.asm

if LCODE
extrn __SET_DOS_ERRNO:far
else
extrn __SET_DOS_ERRNO:near
endif

	begdata
	c_extrn _osmajor,byte
	waiting_ch	db	0
	public ch_is_waiting	
	       ch_is_waiting db	0
    ifdef __OS2__
	public extchar		
	       extchar  db	0	;extended character waiting
	EDOM		equ	1002

	public CharData	
	       CharData label	byte
	CharCode	db	?
	ScanCode	db	?
	public Status		
	       Status   db	?
	NLSShift	db	?
	ShiftState	dw	?
	TimeStamp	dd	?

	FSInfoBuf	label	byte
	FSID		dd	?
	SecsPerCluster	dd	?
	NoOfClusters	dd	?
	AvailClusters	dd	?
	BytesPerSector	dw	?

	FileHandle	dw	?
	Action		dw	?
	NewPos		dd	?
    else ;if DOS
D4CB	STRUC				;DOS 4 control block
	start		dw	?
	fill		dw	0
	number		dw	?
	bufl		dw	?
	bufh		dw	?
	d4cbsz		dw	0
D4CB	ENDS
public kb_callback	
       kb_callback      dd	0
    endif
	enddata

    if LCODE
	c_extrn	_flushterm,far
    else
	c_extrn	_flushterm,near
    endif

    if LCODE
	c_extrn	_LMUL@,far
    else
	c_extrn	_LMUL@,near
    endif

    ifdef __OS2__
	extrn	KBDCHARIN:far
	extrn	KBDPEEK:far
	extrn	VIOWRTTTY:far
	extrn	DOSSETVERIFY:far
	extrn	DOSQVERIFY:far
	extrn	DOSQFSINFO:far
	extrn	DOSOPEN:far
	extrn	DOSCLOSE:far
	extrn	DOSREAD:far
	extrn	DOSWRITE:far
	extrn	DOSCHGFILEPTR:far
	extrn	DOSCHDIR:far
	extrn	DOSMKDIR:far
	extrn	DOSRMDIR:far
	extrn	DOSQCURDISK:far
	extrn	DOSSELECTDISK:far
    endif

	begcode	getdos

	c_public dos_set_verify,dos_get_verify
	c_public dos_set_ctrl_break,dos_get_ctrl_break
	c_public dos_getdiskfreespace
	c_public dos_abs_disk_read,dos_abs_disk_write
	c_public getch,getche,ungetch
	c_public mkdir,rmdir,chdir
	c_public _chdrive,_getdrive
	public __chdir
	c_public __redirect_kb
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Register callback function for kbd input
func	__redirect_kb
	WINENTER
	mov	AX,P[BP]
	mov	DX,P+2[BP]
	mov	WORD PTR kb_callback,AX
	mov	WORD PTR kb_callback+2,DX
	WINLEAVE
	ret
c_endp	__redirect_kb	


;;;;;;;;;;;;;;;;;;;;;;;;;
; Get character from keyboard, don't echo it.

func	getch
    ifdef __OS2__
        cmp	ch_is_waiting,0
        jz	gch2
        mov	AL,waiting_ch
        mov	ch_is_waiting,0
        xor	AH,AH
        jmp short gchret
gch2:	mov	AL,extchar
	test	AL,AL
	jz	gch1
	mov	extchar,0
	xor	AH,AH
	jmp short gchret

gch1:	push	DS
	pushi	AX,<offset DGROUP:CharData>
	xor	AX,AX
	push	AX		;wait for character
	push	AX		;KbdHandle
	call	KBDCHARIN
	test	AX,AX
	mov	AX,-1
	jnz	gchret		;error reading keyboard
	mov	AL,CharCode
	xor	AH,AH
	test	AL,AL
	jz	gchext		;extended character
	cmp	AL,224		;extra cursor pad ?
	jnz	gchret
gchext:
	mov	BH,ScanCode
	mov	extchar,BH	;save extended char for later
gchret:
	ret
    else
	WINENTER_NF
	call	_flushterm
	cmp	ch_is_waiting,0
	jz	gch2
	mov	AL,waiting_ch
	mov	ch_is_waiting,0
	jmp short _G2
gch2:	mov	ax,WORD PTR kb_callback
	or	ax,WORD PTR kb_callback+2
	jz	gch1
	mov	ax,1	;to indicate getch
	jmp	_G3
gch1:	mov	AH,8			;get char from kbd, no echo
	jmp	_G1
    endif
c_endp	getch

;;;;;;;;;;;;;;;;;;;;;;;;;
; Get character from keyboard, echo it.

func	getche
    ifdef __OS2__
		call	_flushterm
        cmp	ch_is_waiting,0
        jz	gce1
        mov	AL,waiting_ch
        mov	ch_is_waiting,0
        jmp short gce2		;best not to echo ungetch character
        
gce1:	call	_getch
	test	AX,AX
	js	gceret		;error reading keyboard
	push	AX		;save character for later
	mov	AX,SP		;pointer to it
	push	SS
	push	AX		;string to print
	pushi	AX,1		;Length to print
	pushi	AX,0		;VioHandle
	call	VIOWRTTTY
	pop	AX		;get back character
gce2:	clr	AH
gceret:	ret
    else
	WINENTER_NF
	call	_flushterm
	cmp	ch_is_waiting,0
	jz	gce1
	mov	AL,waiting_ch
	mov	ch_is_waiting,0
	jmp short _G2		;best not to echo ungetch character
gce1:	mov	ax,WORD PTR kb_callback
	or	ax,WORD PTR kb_callback+2
	jz	_G0
	mov	ax,2	;to indicate getche
_G3:	push	ax
	call	dword ptr kb_callback
	pop	cx
	jmp	_G2
	clr	AH
	ret
_G0:	mov	AH,1
_G1:	bdos			;get char from kbd, echo it
_G2:	clr	AH
	WINLEAVE_NF
	ret
    endif
c_endp	getche

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Put back a character in the keyboard queue
      
func	ungetch
	WINENTER
	cmp	ch_is_waiting,0
	jz	ugc1
	mov	AX,-1
	jmp	ugc2
ugc1:	mov	AX,P[BP]
	cmp	AX,-1
	je	ugc2
	mov	waiting_ch,AL
	mov	ch_is_waiting,1
	clr	AH
ugc2:  	WINLEAVE
	ret
c_endp	ungetch      

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set/reset verify switch for disk writes.
; Use:
;	dos_set_verify(TRUE);	/* turn on verify	*/
;	dos_set_verify(FALSE);	/* turn off verify	*/

func	dos_set_verify
	WINENTER
    ifdef __OS2__
	mov	AX,P[BP]
	push	AX
	call	DOSSETVERIFY
    else
	mov	AL,P[BP]	;get verify flag
	clr	DL		;must be 0 for some reason
	bdos	2Eh
    endif
	WINLEAVE
	ret
c_endp	dos_set_verify

;;;;;;;;;;;;;;;;;;;;;;;;;
; Get disk verify state.
; Returns:
;	TRUE if disk verify is on

func	dos_get_verify
    ifdef __OS2__
	push	AX		;place to put verify setting
	mov	AX,SP		;pointer to it
	push	SS
	push	AX
	call	DOSQVERIFY
	pop	AX		;get back verify setting
    else
	WINENTER_NF
	bdos	54h
	cbw			;clear AH
	WINLEAVE_NF
    endif
	ret
c_endp	dos_get_verify

;;;;;;;;;;;;;;;;;;;;;;;;
; Set/reset Ctrl-Break check.
; Use:
;	dos_set_ctrl_break(TRUE);	/* turn on cntrl-break check	*/
;	dos_set_ctrl_break(FALSE);	/* turn off cntrl-break check	*/

func	dos_set_ctrl_break
	WINENTER
    ifdef __OS2__
	mov	AX,P[BP]
	dec	AX		;can't turn ctrl-break off, so return error
	neg	AX
    else
	mov	DX,P[BP]
	mov	AL,1
	bdos	33h
    endif
	WINLEAVE
	ret
c_endp	dos_set_ctrl_break

;;;;;;;;;;;;;;;;;;;;;;;;;
; Get state of Ctrl-Break checking.
; Returns:
;	TRUE	on
;	FALSE	off

func	dos_get_ctrl_break
    ifdef __OS2__
	mov	AX,1		;always on under OS/2
    else
	WINENTER_NF
	mov	AX,03300h
	bdos
	mov	AL,DL
	cbw			;clear	AH
	WINLEAVE_NF
    endif
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
	WINENTER
    ifdef __OS2__
	mov	AX,P[BP]	;drive
	call	getfsinfo
	test	AX,AX
	jz	gdf1
	mov	AX,-1
	cwd
	WINLEAVE
	ret			;return -1l on error
gdf1:
	mov	AX,word ptr SecsPerCluster
	mov	DX,word ptr (SecsPerCluster + 2)
	mov	BX,word ptr AvailClusters
	mov	CX,word ptr (AvailClusters + 2)
	call	_LMUL@		;multiply to get Available Sectors
	mov	BX,BytesPerSector
	xor	CX,CX
	call	_LMUL@		;multiply to get Available Bytes
    else
	mov	DX,P[BP]	;DL = drive
	bdos	36h
	_if	AX ne -1, L1
	cwd
	WINLEAVE
	ret			;return -1L

L1:
	;Compute DX,AX = AX*BX*CX
	mul	CX
	xor	CX,CX
	call	_LMUL@
    endif
	WINLEAVE
	ret
c_endp	dos_getdiskfreespace

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Absolute disk read.
;	int dos_abs_disk_read(drive,numsec,startsec,buffer)
;	int drive;	/* drive number 0=A, 1=B, ...			*/
;	int numsec;	/* number of sectors to read			*/
;	long startsec;	/* starting logical sector number		*/
;	char *buffer;	/* where the result will be stored		*/
; Returns:
;	0		/* success					*/
;	!= 0		/* failure (the value is what is described in	*/
;			/* the DOS manual)				*/

func	dos_abs_disk_read
    ifdef __OS2__
	WINENTER
	mov	AX,P[BP]		;drive number
	inc	AX			;0 is default drive in getfsinfo
	call	getfsinfo
	test	AX,AX
	jnz	adrret			;return error code
	mov	AL,P[BP]
	mov	BX,01010000011000000b	;DASD,No inherit,Return errors,
					;Deny none,Read Only
	call	opendrive		;open drive for absolute read
	test	AX,AX
	jnz	adrret			;return error code
	mov	AX,P+4[BP]		;startsec
	call	seekdrive
	test	AX,AX
	jnz	adrclr			;close drive and return error code
	mov	AX,P+2[BP]		;numsec
	mov	BX,BytesPerSector
	mul	BX			;AX = no of bytes requested
	push	AX			;save for later
	push	FileHandle
    if SPTR
	push	DS			;segment of buffer
    else
	push	P+8[BP]			;segment of buffer
    endif
	push	P+6[BP]			;offset of buffer
	push	AX			;bytes requested
	push	DS
	pushi	AX,<offset DGROUP:Action> ;pointer to no of bytes read return
	call	DOSREAD
	pop	BX			;restore no of bytes requested
	test	AX,AX
	jnz	adrclr
	cmp	BX,Action		;all bytes read ?
	jz	adrclr
	mov	AX,EDOM			;our own error code
adrclr:
	push	AX			;save return code
	push	FileHandle
	call	DOSCLOSE
	pop	AX			;get back return code

    else ;if MSDOS
	mov	AH,1
A4:	WINENTER
	.save	<SI,DI>
	sub	SP,d4cbsz
	mov	CL,__osmajor
	clr	CH
	mov	SI,CX
	mov	AL,P[BP]		;drive
	mov	CX,P+2[BP]		;numsec
	mov	DX,P+4[BP]		;startsec
	_push	<DS,BP>			;save DS, BP
    if SPTR
	mov	BX,P+8[BP]		;buffer
    else
	lds	BX,P+8[BP]		;buffer
    endif
	cmp	SI,4			;DOS 4+?
	jb	A5			;No, skip it
	mov	-4-d4cbsz+start[BP],DX	;Yes, fill in D4CB structure
	mov     DX,P+6[BP]
	mov	-4-d4cbsz+fill[BP],DX
	mov	-4-d4cbsz+number[BP],CX
	mov	-4-d4cbsz+bufl[BP],BX
	mov	-4-d4cbsz+bufh[BP],DS
	mov	CX,-1
	push	SS				;Point to DCB
	pop	DS
	mov	BX,BP
	sub	BX,4+d4cbsz
A5:
	tst	AH			;do read or write?
	jz	A3
	int	25h			;read
	jmps	A2

A3:	int	26h			;write
A2:	jc	A1			;error (AX has error code)
	clr	AX			;no error
A1:	popf				;clean stack
	_pop	<BP,DS>			;restore DS, BP
	add	SP,d4cbsz		;forget D4CB structure
	.restore <DI,SI>
    endif ;__OS2__
adrret: WINLEAVE
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
    ifdef __OS2__
	push	BP
	mov	BP,SP
	mov	AX,P[BP]	;drive number
	inc	AX		;0 is default drive in getfsinfo
	call	getfsinfo
	test	AX,AX
	jnz	adrret		;return error code
	mov	al,P[BP]
	mov	BX,01010000011000001b	;DASD,No inherit,Return errors,
				;Deny none,Write Only
	call	opendrive	;open drive for absolute write
	test	AX,AX
	jnz	adwret		;return error code
	mov	AX,P+4[BP]	;startsec
	call	seekdrive
	test	AX,AX
	jnz	adwclr		;close drive and return error code
	mov	AX,P+2[BP]	;numsec
	mov	BX,BytesPerSector
	mul	BX		;AX = no of bytes requested
	push	AX		;save for later
	push	FileHandle
    if SPTR
	push	DS		;segment of buffer
    else
	push	P+8[BP]		;segment of buffer
    endif
	push	P+6[BP]		;offset of buffer
	push	AX		;bytes requested
	push	DS
	pushi	AX,<offset DGROUP:Action> ;pointer to no of bytes written return
	call	DOSWRITE
	pop	BX		;restore no of bytes requested
	test	AX,AX
	jnz	adwclr
	cmp	BX,Action	;all bytes written ?
	jz	adwclr
	mov	AX,EDOM		;our own error code
adwclr:
	push	AX		;save return code
	push	FileHandle
	call	DOSCLOSE
	pop	AX		;get back return code
adwret:
	pop	BP
	ret
    else
	clr	AH
	jmp	A4		;use common code
    endif
c_endp	dos_abs_disk_write


    ifdef __OS2__
opendrive	proc	near
	add	AL,'A'		;drive letter
	mov	AH,':'
	xor	BX,BX
	push	BX
	push	AX		;push string "D:\0" onto stack
	mov	AX,SP		;take its address
	push	SS
	push	AX
	push	DS
	pushi	AX,<offset DGROUP:FileHandle>
	push	DS
	pushi	AX,<offset DGROUP:Action>
	xor	AX,AX
	push	AX
	push	AX		;initial allocation
	push	AX		;attribute
	pushi	CX,1		;open if exists, fail otherwise
	push	BX		;Open Mode
	push	AX
	push	AX		;reserved 0l
	call	DOSOPEN
	pop	BX
	pop	BX		;remove file name string from stack
	ret
opendrive	endp

seekdrive	proc	near
	mov	BX,BytesPerSector
	mul	BX		;DX:AX = start position
	push	AX
	push	DX		;save for later
	push	FileHandle
	push	AX
	push	DX		;seek position
	pushi	AX,0		;from start of file
	push	DS		;where to put final position
	pushi	AX,<offset DGROUP:NewPos>
	call	DOSCHGFILEPTR
	pop	CX
	pop	BX		;restore requested position
	test	AX,AX
	jnz	skdret		;return error code
	mov	AX,EDOM		;our error code
	cmp	BX,word ptr NewPos
	jnz	skdret
	cmp	CX,word ptr (NewPos + 2)
	jnz	skdret
	xor	AX,AX		;no error
skdret:
	ret
seekdrive	endp

getfsinfo	proc	near
	push	AX		;drive number
	pushi	AX,1		;Level 1 info required
	push	DS
	pushi	AX,<offset DGROUP:FSInfoBuf>
	pushi	AX,18		;sizeof(FSInfoBuf)
	call	DOSQFSINFO
	ret
getfsinfo	endp
    endif


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Change directory to that specified by pathname.
; Usage:
;	int chdir(char *pathname);
; Returns:
;	0	success
;	-1	failure (errno is set to ENOENT)

__chdir	proc
__chdir	endp

func	chdir
    ifdef __OS2__
	push	BP
	mov	BP,SP
    if SPTR
	push	DS		;segment of pathname
    else
	push	P+2[BP]		;segment of pathname
    endif
	push	P[BP]		;offset of pathname
	xor	AX,AX
	push	AX
	push	AX		;reserved 0l
	call	DOSCHDIR
C1:	test	AX,AX
	jz	chdret
	push    AX
        call    __SET_DOS_ERRNO
	mov	AX,-1
chdret:

    else
	mov	AH,03Bh

_dodir: WINENTER
    if SPTR
	mov	DX,P[BP]
	bdos
    else
	push	DS
	lds	DX,P[BP]
	bdos
	pop	DS
    endif
	jnc	D1		;no error
	push    AX
        call    __SET_DOS_ERRNO
        mov	AX,-1
        jmp	D2
D1:	mov	AX,0
    endif
D2:	WINLEAVE
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
    ifdef __OS2__
	push	BP
	mov	BP,SP
    if SPTR
	push	DS		;segment of pathname
    else
	push	P+2[BP]		;segment of pathname
    endif
	push	P[BP]		;offset of pathname
	xor	AX,AX
	push	AX
	push	AX		;reserved 0l
	call	DOSMKDIR
	jmp	C1
    else
	mov	AH,039h
	jmp	_dodir
    endif
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
    ifdef __OS2__
	push	BP
	mov	BP,SP
    if SPTR
	push	DS		;segment of pathname
    else
	push	P+2[BP]		;segment of pathname
    endif
	push	P[BP]		;offset of pathname
	xor	AX,AX
	push	AX
	push	AX		;reserved 0l
	call	DOSRMDIR
	jmp	C1
    else
	mov	AH,03Ah
	jmp	_dodir
    endif
c_endp	rmdir

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; int _chdrive(int drive);
; Returns:
;	0	success
;	-1	failure

func	_chdrive
    ifdef __OS2__
	push	BP
	mov	BP,SP
	push	P[BP]		;drive number
	call	DOSSELECTDISK
	tst	AX		;error?
	jz	CH1		;no, return 0
	mov	AX,-1
CH1:	pop	BP
	ret
    else
	WINENTER
	mov	DL,P[BP]	;get drive number
	dec	DL		;to 0=A, 1=B, etc.
	bdos	0Eh		;select drive
	bdos	19h		;read current drive
	sub	AL,DL		;see if it matches
	jz	CH1		;yes they do
	mov	AL,0FFh		;error (return -1)
CH1:	cbw
	WINLEAVE
	ret
    endif
c_endp	_chdrive

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; int _getdrive(void);
; Returns:
;	1=A, 2=B, etc.

func	_getdrive
    ifdef __OS2__
	push	BP
	mov	BP,SP
	sub	SP,6

	mov	AX,SP
	push	SS
	push	AX		;ptr to where drive code will go

	inc	AX
	inc	AX
	push	SS
	push	AX		;ptr to where bit map will go

	call	DOSQCURDISK
	pop	AX		;get result
	mov	SP,BP
	pop	BP
	ret
    else
	WINENTER_NF
	bdos	19h		;read current drive
	cbw
	inc	AX
	WINLEAVE_NF
	ret
    endif
c_endp	_getdrive

	endcode	getdos

	end

