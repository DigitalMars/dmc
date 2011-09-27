; Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $
	include macros.asm
	include stdio.asm

if LCODE
extrn __SET_DOS_ERRNO:far
else
extrn __SET_DOS_ERRNO:near
endif

	begcode io

	c_public lseek

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
	WINENTER
	mov	BX,P[BP]		;file handle
	mov	DX,P+2[BP]		;lsw of offset
	mov	CX,P+4[BP]		;msw of offset
	mov	AL,P+6[BP]		;mode (0,1,2)
	bdos	42h			;write to file or device
	jnc	L9			;no error
	push    AX
        call    __SET_DOS_ERRNO
	mov	AX,-1			;AX = -1
	cwd				;DX = -1
L9:	WINLEAVE
	ret
c_endp	lseek

	endcode io

	end

