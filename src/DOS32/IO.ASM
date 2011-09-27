;_ io.asm
; C compiler system calls for Phar Lap DOS386.
;Copyright (C) 1985-1991 by Walter Bright
;All Rights Reserved
;
; Modified by Richard S. Sadowsky July 28,1993\
; $Revision: 1.1.1.1 $

include macros.asm
include stdio.asm

extrn __SET_DOS_ERRNO:near
extrn __SET_ERRNO:near

;Open flags
O_RDONLY	equ	0
O_WRONLY	equ	1
O_RDWR		equ	2
O_APPEND	equ	8
O_NOINHERIT	equ	80h
O_CREAT		equ	100h
O_TRUNC		equ	200h
O_EXCL		equ	400h
O_BINARY        equ     8000h
O_TEXT          equ     4000h

;Permission modes
S_IWRITE	equ	80h
S_IREAD		equ	0100h

;File handle flags
FHND_APPEND	equ	04h
FHND_DEVICE	equ	08h
FHND_TEXT	equ	10h

extrn   ___umask_pmode:word

	begdata
	c_extrn _osmajor,byte
	c_extrn _osminor,byte
	
	public __fmode,___fhnd_info
    
__fmode     dw  4000h   ; _O_TEXT
___fhnd_info    db	FHND_DEVICE+FHND_TEXT   ; STDIN
		db	FHND_DEVICE+FHND_TEXT   ; STDOUT
		db	FHND_DEVICE+FHND_TEXT   ; STDERR
		db	FHND_DEVICE             ; STDAUX
		db	FHND_DEVICE             ; STDPRN
		db	(_NFILE - 5) dup (0)
	enddata

	begcode io

	c_public open,sopen,close,creat
	c_public lseek,filesize,rename,unlink

	c_public isatty,remove,dos_open,dos_creat,dos_close

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Determine if handle is a tty.
; Use:
;	int isatty(fd)
;	int fd;
; Returns:
;	!=0	character device
;	0	not a character device or error

func	isatty
	push	EBX
	mov	EBX,P[ESP]	;get fd (file handle)
	mov	AX,04400h	;get device information
	bdos			;IOCTL
	pop	EBX
	;If sign bit is set in DL, it is a character device.
	movsx	EAX,DL		;AH = 0FFh if char dev, else 0
	mov	AL,AH
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
	push	EBP
	mov	EBP,ESP
	push	EBX
	mov	AH,3Dh			;open file
	mov	AL,P+SIZEPTR[EBP]	;AL = rwmode (0,1,2)
OPEN1:
	mov	EDX,P[EBP]		 ;EDX -> name
	bdos
	jnc	OPEN2
	movsx	EAX,AX
	push    EAX
        call    __SET_DOS_ERRNO
	mov	EAX,-1
	jmp	OPEN3
OPEN2:		
	mov	EBX,EAX
	mov	___fhnd_info[EBX],0
	push	EBX
	push	EBX
	call	_isatty
	pop	EBX
	pop	EBX
	or	EAX,EAX
	jz	OPEN4
	or	___fhnd_info[EBX],FHND_DEVICE
OPEN4:	mov	EAX,EBX
OPEN3:	pop	EBX
	pop	EBP
	ret
c_endp	dos_open

;;;;;;;;;;;;;;;;;;;;;;;;;
; Closes a file (DOS mode)
; Use:
;	int dos_close(fd)
; Returns:
;	0	no error
;	!=0	DOS error code

func	dos_close
	push	EBX
	mov	EBX,PS+4[ESP]		;file handle
	mov	___fhnd_info[EBX],0
	xor	EAX,EAX			;0 high word
	bdos	3Eh			;close file
	jnc	CLOSE_SUCCESS
	push    EAX
        call    __SET_DOS_ERRNO
        mov     EAX,-1
        jmp     CLOSE_DONE
CLOSE_SUCCESS:
        mov     EAX,0
CLOSE_DONE:
        pop	EBX
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

rwmode_data	equ	-4[EBP]
pmode_data	equ	-8[EBP]

func	open
	push	EBP
	mov	EBP,ESP
	sub	ESP,8
	uses	<EBX>
	mov	EAX,P+SIZEPTR[EBP]	;EAX = rwmode (0,1,2)
	and	AL,8Fh			;Mask off sharing bits...
;	or	AL,40h			; add sharing (deny none)...
	_if	_osmajor ge 3, DO4	;if DOS 3+?
	and	AL,7			;only these bits have meaning
DO4:	mov	rwmode_data,EAX		;  and save it
	mov	EAX,P+4+SIZEPTR[EBP]	;EAX = pmode
	mov	EBX,___umask_pmode	;mask off umask bits and save
	not	EBX
	and	EAX,EBX
	mov	pmode_data,EAX

opens:
    if 1
	mov	AL,rwmode_data		;AL = rwmode (0,1,2)
	and	AL,0F3h			;clear reserved bits
	mov	AH,3Dh			;open file
    else
	mov	AX,3D00h		;see if file exists
    endif
	mov	EDX,P[EBP]		;filename offset
	bdos
	jc	nofile			;maybe doesn't exist

	movzx	EBX,AX			;opened file checking existance,
	bdos	3Eh			; now close it
	mov	EBX,rwmode_data		;check for O_EXCL && O_CREAT
	and	EBX,O_EXCL+O_CREAT
	cmp	EBX,O_EXCL+O_CREAT

	jne	gotfile			;all's OK - continue
	mov	EAX,EEXIST		;return EEXIST error
        jmp     DOX
toohand:                                ; We have reached handle limit, return error
        bdos   3Eh                      ;; Close File 
        mov    EAX,EMFILE
DOX:    push  EAX
        call __SET_ERRNO
        jmp  DOXXX
DOXX:	movsx	EAX,AX
	push    EAX
        call    __SET_DOS_ERRNO
DOXXX:
	mov     EAX,-1	
	jmp	O2

nofile:
	_ifs	AX ne 2, DOXX	        ;if not "file not found" error
	test	word ptr rwmode_data,O_CREAT ;check for O_CREAT
	jz	DOXX			;no, return ENOENT
	clr	ECX
	test	byte ptr pmode_data,S_IWRITE ;Write permission?
	jnz	DO2			;Yes, continue
	inc	ECX			;No, set FA_RDONLY
DO2:
	mov	EDX,P[EBP]		;EDX -> name
	bdos	3ch			;create file
	jc	DOXX			;error

	movzx	EBX,AX

	test	byte ptr pmode_data,S_IWRITE ;Write permission?
	jz	notrunc			;no, return as it is

	bdos	3Eh			;file is created, now close it
	jc	DOXX
gotfile:
	mov	EAX,rwmode_data		;EAX = rwmode (0,1,2)
	and	AL,0F3h
	mov	AH,3Dh			;open file

	mov	EDX,P[EBP]		;EDX -> name
	bdos
	jc	DOXX

	movzx	EBX,AX			;fd
        cmp     EBX,_nfile              ;; Lets not go over our handle limit
        jnb     toohand
	mov	___fhnd_info[EBX],0
	test	word ptr rwmode_data,O_TRUNC ;Truncate?
	jz	notrunc			;No, skip it

	;Truncate it by writing 0 bytes to file
	clr	ECX
	bdos	40h			;write
	jc	DOXX
notrunc:
	push	EBX
	push	EBX
	call    _isatty
	pop	EBX
	pop	EBX
	or	EAX,EAX
	jz	notatty
	or	___fhnd_info[EBX],FHND_DEVICE
notatty:        
	test	word ptr rwmode_data,O_APPEND ;Append?
	jz	noappend		;No, skip it
	or	___fhnd_info[EBX],FHND_APPEND
noappend: 
	test	word ptr rwmode_data,O_TEXT
	jz	notext
	or	___fhnd_info[EBX],FHND_TEXT
	jmp	short alldone
notext:	test	word ptr rwmode_data,O_BINARY
	jnz	alldone
	test	__fmode,O_BINARY
	jnz	alldone
	or	___fhnd_info[EBX],FHND_TEXT
alldone:    
	mov	EAX,EBX			;return fd
O2:	unuse	<EBX>
	mov	ESP,EBP
	pop	EBP
	ret
c_endp	open

func	sopen
	push	EBP
	mov	EBP,ESP
	sub	ESP,8
	uses	<EBX>

	_if	_osmajor ge 3, SO1	;DOS 3+?
	mov	EAX,EINVAL
	jmp	DOX
SO1:
	mov	EAX,P+SIZEPTR[EBP]	;EAX = rwmode (0,1,2)
	and	AL,8fh
	mov	ECX,P+4+SIZEPTR[EBP]	;Add sharing bits...
	and	CL,70h
	or	AL,CL
	mov	-4[EBP],AX		; and save it
	mov	EAX,P+8+SIZEPTR[EBP]	;EAX = pmode
	mov	EBX,___umask_pmode	;mask off umask bits and save
	not	EBX
	and	EAX,EBX
	mov	-8[EBP],EAX
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
	push	EBP
	mov	EBP,ESP
	push	EBX
	movzx	ECX,byte ptr P+SIZEPTR[EBP]
	mov	AL,___umask_pmode	;mask off umask low byte bits
	not	AL
	and	CL,AL
	rol	CL,1			;put S_IWRITE in bit 0
	inc	CL			;toggle it
	and	ECX,1			;turning it into read-only bit
	jmps	creat1
c_endp	creat

func	dos_creat
	push	EBP
	mov	EBP,ESP
	push	EBX
	mov	ECX,P+SIZEPTR[EBP]	;ECX = file attribute
creat1:
	mov	AH,3Ch			;create file
	mov	EDX,P[EBP]		;EDX -> name
	bdos
	movzx	EAX,AX

	jnc	creat5		;no error
        push    EAX
        call    __SET_DOS_ERRNO
        jmp     creat9
creat8:
        bdos    3Eh           ;; Close File
        mov     EDX,P[EBP]    ;; Name of File
	bdos    41h           ;; Delete File
        mov     EAX,EMFILE    ;; Too many files
        push    EAX
        call    __SET_ERRNO
creat9:
	mov     EAX,-1			;error
	jmp	short creat4
creat5:	
	mov	EBX,EAX
        cmp     EBX,_nfile
        jnb     creat8
	mov	___fhnd_info[EBX],0
	test	__fmode,O_BINARY
	jnz	creat6
	or	___fhnd_info[EBX],FHND_TEXT
creat6:	push	EBX
	push	EBX
	call	_isatty
	pop	EBX
	pop	EBX
	or	EAX,EAX
	jz	creat7
	or	___fhnd_info[EBX],FHND_DEVICE
creat7:	mov	EAX,EBX
creat4: 
	pop	EBX
	pop	EBP
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
	push	EBP
	mov	EBP,ESP
	uses	<EBX>
	mov	EBX,P[EBP]		;file handle
	mov	___fhnd_info[EBX],0
	bdos	3Eh			;write to file or device
	unuse	<EBX>
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
	push	EBP
	mov	EBP,ESP
	uses	<EDI>
      ife ESeqDS
	mov	AX,DS
	mov	ES,AX
      endif
	mov	AH,56h			;DOS rename function
	mov	EDI,P+SIZEPTR[EBP]	;The new name.
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
	push	EBP
	mov	EBP,ESP
	uses	<EDI>
	mov	AH,41h			;delete file function
U1:
	mov	EDX,P[EBP]		;EDX -> name
	bdos
	unuse	<EDI>
U2:	pop	EBP
U3:	jnc	short L7		;no error
L8:	movsx	EAX,AX
	push    EAX
        call    __SET_DOS_ERRNO
        mov     EAX,-1
        jmp     Lz
L7:	mov     EAX,0
Lz:	ret
c_endp	unlink

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set handle count.
;	int _dos_sethandlecount(unsigned nhandles);
; Returns:
;	0	success
;	!=0	failure, errno is set

	public	__dos_sethandlecount
__dos_sethandlecount proc near
	mov	EAX,1			;invalid DOS function number
	_ifs	_osmajor b 3, U3
	ja	short SH1
	_ifs	_osminor b 3, U3	;must be at least DOS 3.3
SH1:	push	EBX
	mov	EBX,PS+4[ESP]
	or	BL,1
	bdos	67h
	pop	EBX
	jmp	U3
__dos_sethandlecount endp

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
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX>
	mov	EBX,P[EBP]		;file handle
	mov	DX,P+4[EBP]		;lsw of offset
	mov	CX,P+6[EBP]		;msw of offset
	mov	AL,P+8[EBP]		;mode (0,1,2)
	bdos	42h			;write to file or device
	movzx	EAX,AX
	jnc	short L9		;no error
	push    EAX
        call    __SET_DOS_ERRNO
	mov     EAX,-1			;EAX = -1
L9:	shl	EDX,16			;mov msw to top half of reg.
	or	EAX,EDX			;combine into one reg.
	unuse	<EDX,ECX,EBX>
	pop	EBP
	ret
c_endp	lseek

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get and return the size of a file.
; Use:
;	long filesize(filename)
;	char *filename;
; Returns:
;	-1L	error

func	filesize
	push	EBP
	mov	EBP,ESP
	sub	ESP,44		;44 bytes for DTA

	;Set DTA to the 44 bytes on the stack (SS:EBP)
	mov	EDX,ESP		;DS:EDX is DTA
	bdos	1Ah		;set DTA

	;Find first matching file
	mov	EDX,P[EBP]
	mov	CX,6		;find all normal files, plus system and hidden
	bdos	4Eh		;findfirst (DS:EDX -> filename)
	jc	L11		;no error

	;Load file size from DTA
	mov	EAX,26[ESP]

L12:	leave
	ret

L11:
	movsx	EAX,AX
	push    EAX
        call    __SET_DOS_ERRNO
	mov     EAX,-1
	cwd			;return -1L on error
	leave
	ret
c_endp	filesize

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	#include <io.h>
;	int access(char *path,int mode);
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
	mov	AX,4300h	;get file's attribute
	mov	EDX,PS[ESP]	;DS:DX = path
	bdos
	jc	I4
	test	byte ptr PS+SIZEPTR[ESP],2	;is W_OK set?
	jz	I5				;no
	shr	CL,1		;is file read-only?
	jnc	I5		;no
	mov	EAX,EACCES
	push	EAX
	call 	__SET_ERRNO
	mov  EAX, -1
	jmp  I6
I4:	
	push    EAX
        call    __SET_DOS_ERRNO
        mov     EAX,-1
	jmp I6
I5:
	mov EAX,0
I6:
	ret
c_endp	access

	endcode io

	end

