;_ io.asm   Tue Dec 19 1989   Modified by: Walter Bright
; Windows support added by G. Eric Engstrom Dec 1990	-D_WINDOWS
; Modified by Bob Stout May 1990
; Modified by Richard S. Sadowsky July 28,1993
; OS2 support added by Nikki Locke May 1989		-D__OS2__
; Copyright (C) 1985-1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright
; $Revision: 1.1.1.1 $

	include macros.asm
	include stdio.asm

ifdef _WINDOWS
extrn SETHANDLECOUNT:far
endif

if LCODE
extrn __SET_DOS_ERRNO:far
extrn __SET_ERRNO:far
else
extrn __SET_DOS_ERRNO:near
extrn __SET_ERRNO:near
endif

;Open flags
O_RDONLY	equ	0
O_WRONLY	equ	1
O_RDWR		equ	2
O_APPEND	equ	8
O_NOINHERIT	equ	80h
O_CREAT		equ	100h
O_TRUNC		equ	200h
O_EXCL		equ	400h
O_BINARY	equ	8000h
O_TEXT		equ	4000h

;Permission modes
S_IWRITE	equ	80h
S_IREAD		equ	0100h

;File handle flags
FHND_APPEND	equ	04h
FHND_DEVICE	equ	08h
FHND_TEXT	equ	10h

    ifdef __OS2__
extrn	DOSREAD:far
extrn	DOSWRITE:far
extrn	DOSOPEN:far
extrn	DOSCLOSE:far
extrn	DOSMOVE:far
extrn	DOSDELETE:far
extrn	DOSCHGFILEPTR:far
extrn	DOSFINDFIRST:far
extrn	DOSFINDCLOSE:far
extrn	DOSQHANDTYPE:far
extrn	DOSQFILEMODE:far
extrn	DOSSETMAXFH:far
    endif

extrn	___umask_pmode:word

	begdata
;	c_extrn errno,word
	c_extrn _osmajor,byte
	c_extrn _osminor,byte
   ifdef _WINDOWS
	public	__pwincio_rw
    if LCODE
__pwincio_rw	dd	0
    else
__pwincio_rw	dw	0
    endif
    if SPTR
	public	__pwincio_rwx
    if LCODE
__pwincio_rwx	dd	0
    else
__pwincio_rwx	dw	0
    endif
    endif
   endif
   	public __fmode,___fhnd_info
__fmode		dw	4000h  	; _O_TEXT

___fhnd_info	db	FHND_DEVICE+FHND_TEXT	; STDIN
		db	FHND_DEVICE+FHND_TEXT	; STDOUT
		db	FHND_DEVICE+FHND_TEXT	; STDERR
		db	FHND_DEVICE		; STDAUX
		db	FHND_DEVICE		; STDPRN
		db	(_NFILE - 5) dup (0)
	enddata

	begcode io

	c_public open,sopen,close,creat
	c_public filesize,rename,unlink

    ifndef __OS2__
	c_public dos_creat,dos_open,dos_close
    endif
	c_public isatty,remove

    ifdef __OS2__
;;;;;;;;;;;;;;;;;;;;;;;;;
; set up arguments fd,buffer,length for read(), write(), open() etc.
;
setargs proc	near
	pop	DX			;return address
	push	BP
	mov	BP,SP
	sub	SP,AX			;reserve space for arguments
	push	DX			;put return address back
	mov	AX,P[BP]		;1st argument
	mov	BX,P+2[BP]
	mov	CX,P+4[BP]
	mov	DX,P+6[BP]		;4th argument
	ret
setargs endp

;;;;;;;;;;;;;;;;;;;;;;;;;
; Open a file
; Use:
;	int open(name,rwmode,pmode)
; Returns:
;	-1	error
;	fd	file handle
;

func	open
	mov	AX,4
	call	setargs
O8:
    if SPTR
	and	BL,8Fh			;sharing mode: deny none
	or	BL,40h
    else
	and	CL,8Fh			;sharing mode: deny none
	or	CL,40h
    endif
O1:
if SPTR
	push	BX			;save rwmode
	push	DS			;name segment
	push	AX			;name offset
else
	push	CX			;save rwmode
	push	BX			;name segment
	push	AX			;name offset
	mov	BX,CX			;rwmode in BX
	mov	CX,DX			;pmode in CX
endif
	;BX = rwmode
	;CX = pmode (file attribute)

	test	BX,O_CREAT
	jz	O2			;no

	mov	DX,10001b		;open if exist, create if not exist
	test	BX,O_EXCL
	jz	O5
	test	BX,O_TRUNC		;both O_EXCL and O_TRUNC
	jnz	Oerror			;is an invalid rwmode
	dec	DX			;fail if exist, create if not exist
	jmp	O6

Oerror: mov	AX,EINVAL
	jmp	OX

O5:	test	BX,O_TRUNC
	jz	O6
	inc	DX			;replace if exist, create if not exist

O6:
; following lines are commented out so that PMODE argument is ignored (0)
;	test	CX,092h			;any WRITE permissions set ?
	mov	CX,0			;normal attribute
;	jnz	O4
;	inc	CX			;read only attribute
	jmp short O4

O2:
	clr	CX			;attribute = 0
	mov	DX,1			;open if exists, fail if not
	test	BX,O_TRUNC
	jz	O4
	inc	DX			;replace if exists, fail if not

O4:	;Dump unused bits in open mode
	and	BX,0f3h			;mask unused bits

	;ptr to file name is already on stack
	;BX = open mode
	;CX = attribute
	;DX = open flag

	lea	AX,[BP-2]
	push	SS
	push	AX			;address of file handle

	dec	AX
	dec	AX
	push	SS
	push	AX			;address of action taken

	xor	AX,AX
	push	AX
	push	AX			;primary allocation = 0
	push	CX			;attribute
	push	DX			;open flag
	push	BX			;open mode
	push	AX
	push	AX			;Reserved 0l
	call	DOSOPEN

	pop	BX			;restore rwmode
	tst	AX			;error?
	jnz	OX			;yes, nothing more to do
	mov	AX,[BP-2]		;error?
	and	BX,O_APPEND		;No, append?
	jz	O7			;No, all done
	push	AX			;Save handle
	pushi	BX,2			;Use SEEK_END
	clr	BX			;Offset = 0L
	push	BX
	push	BX
	push	AX			;fd
	callm	lseek
	add	SP,8
	pop	AX			;Restore handle...
	jmp	O7			; and all done

OX:	errno_set			;save DOS error number
	mov	AX,-1			;error
O7:	mov	SP,BP
	pop	BP
	ret
c_endp	open

func	sopen
	push	BP
	mov	BP,SP
	sub	SP,4			;reserve space for arguments
	mov	AX,P[BP]		;1st argument
	mov	BX,P+2[BP]
    if SPTR
	and	BL,8Fh			;mask rwmode
	mov	CX,P+4[BP]		;get shmode...
	and	CL,70h			; and mask it
	or	BL,CL			;merge rwmode & shmode
	mov	CX,P+6[BP]		;pmode
    else
	mov	CX,P+4[BP]		;get rwmode...
	and	CL,8Fh			; and mask it
	mov	DX,P+6[BP]		;get shmode...
	and	DL,70h			; and mask it
	or	CL,DL			;merge rwmode & shmode
	mov	DX,P+8[BP]		;pmode
    endif
	jmp	O1
c_endp	sopen
;;;;;;;;;;;;;;;;;;;;;;;;;
; Create a file
; Use:
;	int creat(name,pmode)
; Returns:
;	-1	error
;	fd	file handle
;

func	creat
	mov	AX,4
	call	setargs
    if	SPTR
	mov	CX,BX				;attribute
	mov	BX,O_CREAT or O_TRUNC or O_RDWR ;replace if exists, create if not
    else
	mov	DX,CX				;attribute
	mov	CX,O_CREAT or O_TRUNC or O_RDWR ;replace if exists, create if not
    endif
	jmp	O8
c_endp	creat

;;;;;;;;;;;;;;;;;;;;;;;;;
; Close a file
; Use:
;	int close(fd)
; Returns:
;	-1	error
;	0	successful
;

func	close
	push	BP
	mov	BP,SP
	push	P[BP]			;file handle
	call	DOSCLOSE
C1:	test	AX,AX
	jz	C2
	errno_set
	mov	AX,-1
C2:
	pop	BP
	ret
c_endp	close

;;;;;;;;;;;;;;;;;;;;;;;;
; Rename a file.		J.K.H. 2/15/86
; Use:
;	int rename (from, to)
; Returns:
;	-1	error
;	0	successful
;

func	rename
	push	BP
	mov	BP,SP
if SPTR
	push	DS			;old path segment
	push	P[BP]			;old path offset
	push	DS			;new path segment
	push	P+2[BP]			;new path offset
else
	push	P+2[BP]			;old path segment
	push	P[BP]			;old path offset
	push	P+6[BP]			;new path segment
	push	P+4[BP]			;new path offset
endif
	xor	AX,AX
	push	AX
	push	AX			;Reserved 0l
	call	DOSMOVE
	jmp	C1
c_endp	rename

;;;;;;;;;;;;;;;;;;;;;;;;;
; Delete a file
; Use:
;	int unlink(name)
; Returns:
;	-1	error
;	0	successful
;

func	remove
c_endp	remove

func	unlink
	push	BP
	mov	BP,SP
if SPTR
	push	DS			;name segment
else
	push	P+2[BP]			;name segment
endif
	push	P[BP]			;name offset
	xor	AX,AX
	push	AX
	push	AX			;Reserved 0l
	call	DOSDELETE
	jmp	C1
c_endp	unlink

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set handle count.
;	int _dos_sethandlecount(unsigned nhandles);
; Returns:
;	0	success
;	!=0	failure, errno is set

	public	__dos_sethandlecount
func	_dos_sethandlecount
	push	BP
	mov	BP,SP
	push	P[BP]
	call	DOSSETMAXFH
	jmp	C1
c_endp	_dos_sethandlecount

;;;;;;;;;;;;;;;;;;;;;;;;;
; Seek to specified file position.
; Use:
;	long lseek(int fd,long offset,int mode)
; Input:
;	mode =	SEEK_SET or SEEK_CUR or SEEK_END
; Returns:
;	-1L	error
;	n	new file position
;

func	lseek
	mov	AX,4
	call	setargs
	push	AX			;file handle
	push	CX
	push	BX			;new position
	push	DX			;mode
	lea	AX,[BP-4]
	push	SS
	push	AX			;where to put new position
	call	DOSCHGFILEPTR
	tst	AX			;error?
	jnz	L2			;yes
	mov	AX,[BP-4]
	mov	DX,[BP-2]		;new position
L1:	mov	SP,BP
	pop	BP
	ret

L2:	errno_set			;save DOS error number
	mov	AX,-1
	cwd
	jmps	L1
c_endp	lseek

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get and return the size of a file.
; Use:
;	long filesize(filename)
;	char *filename;
; Returns:
;	-1L	error

func	filesize
	push	BP
	mov	BP,SP
	sub	SP,40		;36 bytes for FileFindBuf
	mov	word ptr [BP-4],1	;no of entries to find
	mov	[BP-2],0FFFFh	;dir handle
if SPTR
	push	DS		;name segment
else
	push	P+2[BP]		;name segment
endif
	push	P[BP]		;name offset
	lea	AX,[BP-2]
	push	SS
	push	AX		;address of dir handle
	pushi	BX,6		;find all normal files, plus system and hidden
	lea	BX,[BP-40]	;FileFindBuf
	push	SS
	push	BX
	pushi	BX,36		;buffer length
	push	SS
	dec	AX
	dec	AX
	push	AX		;address of search count
	xor	AX,AX
	push	AX
	push	AX		;reserved 0l
	call	DOSFINDFIRST
	push	AX		;save error code
	push	word ptr [BP-2] ;directory handle
	call	DOSFINDCLOSE
	pop	BX		;get back previous error code
	mov	AX,[BP-40+12]
	mov	DX,[BP-40+14]	;file size
	test	BX,BX
	jz	S1		;no error
	mov	AX,BX
	errno_set
	mov	AX,-1
	cwd			;error return = -1L
S1:	mov	SP,BP
	pop	BP
	ret

c_endp	filesize

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Determine if handle is a tty.
; Use:
;	int isatty(int fd)
; Returns:
;	!=0	character device
;	0	not a character device or error

func	isatty

	push	BP
	mov	BP,SP
	sub	SP,4
	push	P[BP]		;file handle
	lea	AX,[BP-4]
	push	SS
	push	AX		;address of handle type
	inc	AX
	inc	AX
	push	SS
	push	AX		;address of FlagWord
	call	DOSQHANDTYPE
	test	AX,AX
	jz	F9		;error
	errno_set
	xor	AX,AX
	jmps	F8

F9:	mov	BX,[BP-4]
	cmp	BL,1
	jnz	F8		;not a tty
;	test	AX,00000011b
;	jnz	F8		;not screen or keyboard
	inc	AX		;now AX = 1
F8:	mov	SP,BP
	pop	BP
	ret
c_endp	isatty


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	#include <io.h>
;	int access(char *filename,int mode);
; Synopsis:
;	Test file for existence and it's access bits. The access bits are:
;	#define F_OK	0	/* does file exist?	*/
;	#define X_OK	1	/* execute permission?	*/
;	#define W_OK	2	/* write permission?	*/
;	#define R_OK	4	/* read permission?	*/
;	OR these values together to test a combination.
; Bugs:
;	Since MS-DOS only records a write permission bit, the X_OK and
;	R_OK access permissions always succeed. They are included here
;	for unix compatibility.
; Returns:
;	0	file exists and access mode is granted
;	-1	otherwise and errno is set

	c_public access
func	access
	push	BP
	mov	BP,SP
	sub	SP,2			;make room for attribute
	mov	AX,SP

    if LPTR
	push	P+2[BP]			;filename segment
    else
	push	DS
    endif
	push	P[BP]			;filename offset

	push	SS			;push &attribute
	push	AX

	clr	AX			;push 0L
	push	AX
	push	AX

	call	DOSQFILEMODE
	tst	AX
	stc				;assume error
	jnz	A1			;error
	test	byte ptr P+SIZEPTR[BP],2 ;is W_OK set?
	jz	A2			;no
	shr	byte ptr -2[BP],1	;is file read-only?
	jnc	A2			;no
	mov	AX,EACCES
A1:	errno_set
A2:	sbb	AX,AX
	mov	SP,BP
	pop	BP
	ret
c_endp	access

    else ;__OS2__

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Determine if handle is a tty.
; Use:
;	int isatty(fd)
;	int fd;
; Returns:
;	!=0	character device
;	0	not a character device or error

func	isatty
	WINENTER
	mov	BX,P[BP]
	if 0
    ifdef _WINDOWS
	_if	BX ae 3, _I0	;stdin,stdout,stderr are handled by WINC spec
	  mov	  AH,0FFh	;force character device
	  jmp	  short _I1
_I0:
    endif
    endif
	mov	AX,04400h	;get device information
	bdos			;IOCTL

	;If sign bit is set in DL, it is a character device.
	mov	AL,DL
	cbw			;AH = 0FFh if char dev, else 0
_I1:	mov	AL,AH
	WINLEAVE
	ret
c_endp	isatty

;;;;;;;;;;;;;;;;;;;;;;;;;
; Open a file (DOS mode)
; Use:
;	int dos_open(name,rwmode)
; Returns:
;	-1	error
;	fd	file handle
;

func	dos_open
	WINENTER
	mov	AH,3Dh			;open file
	mov	AL,P+SIZEPTR[BP]	;AL = rwmode (0,1,2)
OPEN1:
    if SPTR
	mov	DX,P[BP]		;DX -> name
	bdos
    else
	push	DS
	lds	DX,P[BP]		;DS:DX -> name
	bdos
	pop	DS
    endif
	jnc	OPEN2
	push    AX
        call    __SET_DOS_ERRNO
	mov     AX,-1
	jmp	short OPEN3
OPEN2:
	mov	BX,AX
	mov	___fhnd_info[BX],0
	push	BX
	push	BX
	call	_isatty
	pop	BX
	pop	BX
	or	AX,AX
	jz	OPEN4
	or	___fhnd_info[BX],FHND_DEVICE
OPEN4:	mov	AX,BX
OPEN3:	WINLEAVE
	ret	
c_endp	dos_open

;;;;;;;;;;;;;;;;;;;;;;;;;
; Closes a file (DOS mode)
; Use:
;	int dos_close(fd)
; Returns:
;	DOS	error
;

func	dos_close
	WINENTER
	mov	BX,P[BP]
	mov	___fhnd_info[BX],0
	mov	AH,3Eh			;close file
	bdos
	jnc	CLOSE_SUCCESS
	push    AX
        call    __SET_DOS_ERRNO
        jmp     CLOSE_DONE
CLOSE_SUCCESS:
	mov     AX,0
CLOSE_DONE:
        WINLEAVE
	ret
c_endp	dos_close


;;;;;;;;;;;;;;;;;;;;;;;;;
; Open a file
; Use:
;	int open(name,rwmode,pmode)
; Returns:
;	-1	error
;	fd	file handle
;

_bdos	proc	near
    if LPTR
	push	DS
	lds	DX,P[BP]		;filename
	bdos
	pop	DS
    else
	mov	DX,P[BP]		;filename
	bdos
    endif
	ret
_bdos	endp

func	open
	WINENTER
	sub	SP,4
	mov	AX,P+SIZEPTR[BP]	;AX = rwmode (0,1,2)
	and	AL,8Fh			;Mask off sharing bits...
;	or	AL,40h			; add sharing (deny none)...
	_if	_osmajor ge 3, DO4	;if DOS 3+?
	and	AL,7			;only these bits have meaning
DO4:	mov	-2[BP],AX		;  and save it
	mov	AX,P+2+SIZEPTR[BP]	;AX = pmode
	mov	BX,___umask_pmode	;mask off umask bits and save
	not	BX
	and	AX,BX
	mov	-4[BP],AX

opens:
    if 1
	mov	AL,-2[BP]		;AL = rwmode (0,1,2)
	and	AL,0F3h			;clear reserved bits
	mov	AH,3Dh			;open file
    else
	mov	AX,3D00h		;see if file exists
    endif
	call	_bdos
	jc	nofile			;maybe doesn't exist
	mov	BX,AX			;opened file checking existance,
	bdos	3Eh			; now close it
	mov	BX,-2[BP]		;check for O_EXCL && O_CREAT
	test	BX,O_CREAT
	jz	gotfile			;all's OK - continue
	test	BX,O_EXCL
	jz	gotfile			;all's OK - continue
exist:	mov     AX,EEXIST		;return EEXIST error
        jmp     DOX
toohand:                                ; We have reached handle limit, return error
        bdos   3Eh                      ;; Close File 
        mov    AX,EMFILE
DOX:    push  AX
        call __SET_ERRNO
        jmp  DOXXX
DOXX:	push    AX
        call    __SET_DOS_ERRNO
DOXXX:
	mov     AX,-1
	jmp	O2

nofile:
	_if	AX ne 2, DOXX     	;if not "file not found" error
	test	word ptr -2[BP],O_CREAT ;check for O_CREAT
	jz	DOXX			;no, return ENOENT
	clr	CX
	test	byte ptr -4[BP],S_IWRITE ;Write permission?
	jnz	DO2			;Yes, continue
	inc	CX			;No, set FA_RDONLY
DO2:
	mov	AH,3Ch			;create file
	call	_bdos
	jc	DOXX			;error

	mov	BX,AX			;BX = fd
	test	byte ptr -4[BP],S_IWRITE ;Write permission?
	jz	notrunc			;no, return as it is

	bdos	3Eh			;file is created, now close it
	jc	DOXX
gotfile:
	mov	AL,-2[BP]		;AL = rwmode (0,1,2)
	and	AL,0F3h			;clear reserved bits
	mov	AH,3Dh			;open file
	call	_bdos
	jc	DOXX

	mov	BX,AX			;BX = fd
	;; Make sure we are not over our handle limit
	cmp BX,_nfile
	jnb  toohand
	mov	___fhnd_info[BX],0
	test	word ptr -2[BP],O_TRUNC ;Truncate?
	jz	notrunc			;No, skip it

	;Truncate it by writing 0 bytes to file
	clr	CX
	bdos	40h			;write
	jc	DOXX
notrunc:
	push	BX
	push	BX
	call	_isatty
	pop	BX
	pop	BX
	or	AX,AX
	jz	notatty
	or	___fhnd_info[BX],FHND_DEVICE
	jmp	noappend			; ignore APPEND for DEVICEs
notatty:		
	test	word ptr -2[BP],O_APPEND ;Append?
	jz	noappend		;No, skip it
	or	___fhnd_info[BX],FHND_APPEND
noappend:
	test	word ptr -2[BP],O_TEXT
	jz	notext
	or	___fhnd_info[BX],FHND_TEXT
	jmp	short alldone
notext:
	test	word ptr -2[BP],O_BINARY
	jnz	alldone
	
	test	__fmode,O_BINARY
	jnz	alldone
	or	___fhnd_info[BX],FHND_TEXT
alldone:	
	mov	AX,BX			;return fd
O2:	mov	SP,BP
	WINLEAVE
	ret
c_endp	open

func	sopen
	WINENTER
	sub	SP,4
	_if	_osmajor ge 3, SO1	;DOS 3+?
	mov	AX,EINVAL		;invalid operation for DOS 2
	jmp	DOX
SO1:
	mov	AX,P+SIZEPTR[BP]	;AX = rwmode (0,1,2)
	and	AL,8Fh
	mov	BX,P+2+SIZEPTR[BP]	;Add sharing bits...
	and	BL,70h
	or	AL,BL
	mov	-2[BP],AX		; and save it
	mov	AX,P+4+SIZEPTR[BP]	;AX = pmode
	mov	BX,___umask_pmode	;mask off umask bits and save
	not	BX
	and	AX,BX
	mov	-4[BP],AX
	jmp	opens
c_endp	sopen

;;;;;;;;;;;;;;;;;;;;;;;;;
; Create a file
; Use:
;	int creat(name,pmode)
; Returns:
;	-1	error
;	fd	file handle
;

func	creat
	WINENTER
	mov	CL,P+SIZEPTR[BP]	;get pmode
	mov	AL,byte ptr ___umask_pmode	;mask off umask low byte bits
	not	AL
	and	CL,AL
	rol	CL,1			;put S_IWRITE in bit 0
	inc	CL			;toggle it
	and	CX,1			;turning it into read-only bit
	jmps	creat1
c_endp	creat

func	dos_creat
	WINENTER
	mov	CX,P+SIZEPTR[BP]	;CX = file attribute
creat1:
	mov	AH,3Ch			;create file
    if SPTR
	mov	DX,P[BP]		;DX -> name
	bdos
    else
	push	DS
	lds	DX,P[BP]		;DS:DX -> name
	bdos
	pop	DS
    endif
	jnc	creat5			;no error
        push    AX
        call    __SET_DOS_ERRNO
	jmp     creat9
creat8:                                ;; File handle is beyond end of array, return EMFILE
        bdos 3Eh       			;; Close it and set errno to EMFILE
        ;; Now delete the file
     if SPTR
        mov  DX,P[BP]
        bdos 41h
     else
        push DS
        lds  DX,P[BP]
        bdos 41h
        pop DS
     endif
        mov  AX,EMFILE
        push AX
        call __SET_ERRNO
creat9:
	mov     AX,-1			;error
	jmp	short creat4
creat5:	
	mov	BX,AX
	cmp     BX,_nfile      ;; Make sure that we are not over handle limit
	jnb     creat8
	mov	___fhnd_info[BX],0
	test	__fmode,O_BINARY
	jnz	creat6
	or	___fhnd_info[BX],FHND_TEXT
creat6:	push	BX
	push	BX
	call	_isatty
	pop	BX
	pop	BX
	or	AX,AX
	jz	creat7
	or	___fhnd_info[BX],FHND_DEVICE
creat7:	mov	AX,BX
creat4:	
        WINLEAVE
	ret
c_endp	dos_creat

;;;;;;;;;;;;;;;;;;;;;;;;;
; Close a file
; Use:
;	int close(fd)
; Returns:
;	-1	error
;	0	successful
;

func	close
	WINENTER
	mov	BX,P[BP]		;file handle
	mov	___fhnd_info[BX],0
	bdos	3Eh			;write to file or device
	jmp	U2
c_endp	close

;;;;;;;;;;;;;;;;;;;;;;;;
; Rename a file.		J.K.H. 2/15/86
; Use:
;	int rename (from, to)
; Returns:
;	-1	error
;	0	successful
;

func	rename
	WINENTER
	.save	DI
	mov	AH,56h			;DOS rename function
    if SPTR
      ife ESeqDS
	mov	BX,DS
	mov	ES,BX
      endif
	mov	DI,P+SIZEPTR[BP]	;The new name.
    else    ;LPTR
	les	DI,P+SIZEPTR[BP]	;The new name.
    endif
	jmps	U1
c_endp	rename

;;;;;;;;;;;;;;;;;;;;;;;;;
; Delete a file
; Use:
;	int remove(char *name)
; Returns:
;	-1	error
;	0	successful
;

func	remove
c_endp	remove

func	unlink
	WINENTER
	.save	DI
	mov	AH,41h			;delete file function
U1:
    if SPTR
	mov	DX,P[BP]		;DX -> name
	bdos
    else
	push	DS
	lds	DX,P[BP]		;DS:DX -> name
	bdos
	pop	DS
    endif
	.restore DI
U2:	WINLEAVE
	jnc	L7			;no error
	push    AX
        call    __SET_DOS_ERRNO
        mov     AX,-1
        jmp     Lz
L7:	mov     AX,0
Lz:	ret
c_endp	unlink

ifdef _WINDOWS

	public	__dos_sethandlecount
func	_dos_sethandlecount
	WINENTER
	mov		BX, P[BP]
	push	BX
	call	SETHANDLECOUNT
	WINLEAVE
	mov		AX,0
	ret
c_endp	_dos_sethandlecount

else

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set handle count.
;	int _dos_sethandlecount(unsigned nhandles);
; Returns:
;	0	success
;	!=0	failure, errno is set

	public	__dos_sethandlecount
func	_dos_sethandlecount
	WINENTER
	mov	AX,1			;invalid function number
	_if	_osmajor b 3, U2
	ja	SH1
	_if	_osminor b 3, U2	;must be at least DOS 3.3
SH1:	mov	BX,P[BP]
	or	BL,1
	bdos	67h
	jmp	U2
c_endp	_dos_sethandlecount

endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get and return the size of a file.
; Use:
;	long filesize(filename)
;	char *filename;
; Returns:
;	-1L	error

func	filesize
	WINENTER
	sub	SP,44		;44 bytes for DTA

	push	DS

	bdos	2Fh		;EX:BX = DTA
	push	BX
	push	ES


	;Set DTA to the 44 bytes on the stack (SS:BP)
    if LPTR
	push	SS
	pop	DS
    endif
	lea	DX,-44[BP]	;DS:DX is DTA
	bdos	1Ah		;set DTA

	;Find first matching file
    if SPTR
	mov	DX,P[BP]
    else
	lds	DX,P[BP]
    endif
	mov	CX,6		;find all normal files, plus system and hidden
	bdos	4Eh		;findfirst (DS:DX -> filename)

	pop	DS		;DS:DX = old DTA
	pop	DX
	pushf			;save carry flag
	push	AX
	bdos	1Ah		;restore DTA
	pop	AX
	popf			;restore carry flag
	pop	DS
	jc	L11		;no error

	;Load file size from DTA
	mov	DX,-44+28[BP]
	mov	AX,-44+26[BP]

L12:	add	SP,44
	WINLEAVE
	ret

L11:
	push    AX
        call    __SET_DOS_ERRNO
	mov     AX,-1
	cwd			;return -1L on error
	jmp	L12
c_endp	filesize

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	#include <io.h>
;	int access(char *filename,int mode);
; Synopsis:
;	Test file for existence and it's access bits. The access bits are:
;	#define F_OK	0	/* does file exist?	*/
;	#define X_OK	1	/* execute permission?	*/
;	#define W_OK	2	/* write permission?	*/
;	#define R_OK	4	/* read permission?	*/
;	OR these values together to test a combination.
; Bugs:
;	Since MS-DOS only records a write permission bit, the X_OK and
;	R_OK access permissions always succeed. They are included here
;	for unix compatibility.
; Returns:
;	0	file exists and access mode is granted
;	-1	otherwise and errno is set

	c_public access
func	access
	WINENTER
	mov	AX,4300h	;get file's attribute
    if LPTR
	push	DS
	lds	DX,P[BP]			;DS:DX = path
	bdos
	pop	DS
	jc	I4
	test	byte ptr P+SIZEPTR[BP],2	;is W_OK set?
    else
	mov	DX,P[BP]			;DS:DX = path
	bdos
	jc	I4
	test	byte ptr P+SIZEPTR[BP],2	;is W_OK set?
    endif
	jz	I7				;no
	shr	CL,1				;is file read-only?
	jnc	I7				;no
	mov	AX,EACCES
	push 	AX
	call __SET_ERRNO
	mov    AX,-1
	jmp Iz
I4:	push    AX
        call    __SET_DOS_ERRNO
        mov     AX,-1
        jmp     Iz
I7:	mov     AX,0
Iz:	WINLEAVE
	ret
c_endp	access

    endif ;__OS2__

	endcode io

	end
