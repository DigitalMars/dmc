;_ write.asm Nov 9, 1993
; Copyright 1993 by Digital Mars
; All Rights Reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	include stdio.asm

        extrn   __SET_ERRNO:near
        
    	extrn	__chkstack:near
    
    ifdef _WIN32
	extrn   _GlobalAlloc@8:near
	extrn   _GlobalFree@4:near
	extrn   _SetFilePointer@16:near
	extrn   _WriteFile@20:near
	comm    near __osfhnd:byte:0f0h
	comm	near ___fhnd_info:byte:03Ch
    else	
    	extrn	___stk_ovfl:near
        extrn   ___fhnd_info:byte
    endif        
    
;----------------------------------------------------------------------------
;------ The __write_fp vector is used by various utilities to re-direct -----
;-------- stdout and stderr.  When it is non-zero, the write routine calls --
;-------- it for these two streams.  It has the same prototype as write, ----
;-------- and must have the same behavior; except that write will perform ---
;-------- any necessary LF -> CR/LF translation. ----------------------------
;----------------------------------------------------------------------------

    	c_public __write_fp
    	begdata
___write_fp	dd	0    	
    	enddata

	c_public write
	begcode write
        
;****************************************************************************
; int __clib write (int fd, void *buffer, unsigned len)
; 
; The write function writes len bytes from buffer to the file specified by
; the file descriptor fd.  In text mode, each single line-feed character is 
; replaced with a carriage-return-line-feed pair.  Only the single line-feed
; is counted in the return value.
;
; Data is written, starting at the current position of the file pointer
; associated with the given file.  If the file is open for appending, data
; is written at the end of the file.  After the write operation, the file
; pointer is increased by the number of bytes written.
;
; This function returns the number of bytes written (not counting carriage
; return characters added in text mode).  If an error occurs, -1 is returned
; and errno is set to EBADF (invalid file handle) or ENOSPC (no space left
; on device).
;****************************************************************************

;----------------------------------------------------------------------------
;------	stack frame variables -----------------------------------------------
;----------------------------------------------------------------------------

user_rem   = dword ptr -4
work_limit = dword ptr -8
work_buf   = dword ptr -12
lf_ct      = dword ptr -16
no_lf	   = byte ptr -20

    ifdef _WIN32
actual_bytes_written = dword ptr -24
localbytes = 24
    else    
localbytes = 20
    endif

;----------------------------------------------------------------------------
;------	parameters ----------------------------------------------------------
;----------------------------------------------------------------------------

fd         = 8
buffer     = 12
len        = 16

;----------------------------------------------------------------------------
;------	some miscellaneous manifest constants -------------------------------
;----------------------------------------------------------------------------

TRUE	     = 01H
FALSE	     = 00H
LF	     = 0AH
CRLF         = 0A0DH
DOS_SEEK_END = 4202H
FILE_END     = 02H
DOS_WRITE    = 40H
FHND_APPEND  = 04H
FHND_TEXT    = 10H

MINSTKFREE   = 320

;----------------------------------------------------------------------------
;------ set up the stack frame ----------------------------------------------
;----------------------------------------------------------------------------

func	write
	push	EBP
	mov	EBP,ESP
	sub	ESP,localbytes
	push	EBX
	
;----------------------------------------------------------------------------
;------ load and validate the file handle to EBX ----------------------------
;----------------------------------------------------------------------------

WJ0:	mov	EBX,fd[EBP]
	cmp	EBX,_NFILE	; number of handles supported
	jnb	short WE1
	
;----------------------------------------------------------------------------
;------ ECX will hold the number of bytes to write -- if it is zero, we're --
;-------- done --------------------------------------------------------------
;----------------------------------------------------------------------------
	
	mov	ECX,len[EBP]
	or	ECX,ECX
	jz	short WS14
	
;----------------------------------------------------------------------------
;------ if either the _O_APPEND or _O_TEXT options were set on the open, ----
;--------- we take the time to process these now ----------------------------
;----------------------------------------------------------------------------

	test	___fhnd_info[EBX],FHND_APPEND+FHND_TEXT
	jnz	short WR9
WJ9:	
;----------------------------------------------------------------------------
;------ set up the buffer pointer for the write -----------------------------
;----------------------------------------------------------------------------

	mov	EDX,buffer[EBP]
	
;----------------------------------------------------------------------------
;------ For stdout, stderr, check the __write_fp vector.  If it has been ----
;-------- set, jump to call it below. ---------------------------------------
;----------------------------------------------------------------------------

	cmp	EBX,3
	jae	WS33
	mov	EAX,___write_fp
	or	EAX,EAX
	jnz	short WS29
WS33:
;----------------------------------------------------------------------------
;------ call the operating system to do the write -- check for error --------
;----------------------------------------------------------------------------

        push    ECX                     ; It would be too bad if something
        push    EDX                     ;   happened to ECX and EDX.
    ifdef _WIN32
	push	0			; lpOverlapped
	lea	EAX,actual_bytes_written[EBP]
	push	EAX			; lpNumberOfBytesWritten
	push	ECX			; nNumberOfBytesToWrite
	push	EDX			; lpBuffer
	push	dword ptr __osfhnd[4*EBX] ; hFile
	call	_WriteFile@20
	pop     EDX
	pop     ECX
	or	EAX,EAX
	jz	short WE1
	mov	EAX,actual_bytes_written[EBP]
    else
	bdos	DOS_WRITE
	pop	EDX
	pop	ECX
    	jc	short WE1
    endif    	
    	
;----------------------------------------------------------------------------
;------ if no bytes were written, we are out of disk space -- return an -----
;-------- error -------------------------------------------------------------
;----------------------------------------------------------------------------
WS30:    	
    	or	EAX,EAX
    	jz	short WE2
    	
;----------------------------------------------------------------------------
;------ and return ----------------------------------------------------------
;----------------------------------------------------------------------------

WS13:	pop	EBX
	mov	ESP,EBP
	pop	EBP
	ret

;----------------------------------------------------------------------------
;------ call the __write_fp vector to do the actual write -------------------
;----------------------------------------------------------------------------
WS29:
	push	ECX
	push	EDX
	push	EBX
	call	EAX
	add	ESP,12
	jmp	short WS30

;----------------------------------------------------------------------------
;------ we come here if ECX is zero:  just return a zero --------------------
;----------------------------------------------------------------------------
	
WS14:	xor	EAX,EAX
	jmp	short WS13
	
;----------------------------------------------------------------------------
;------ set the error code and return a -1 ----------------------------------
;----------------------------------------------------------------------------

WE1:    push    dword ptr EBADF
        call    __SET_ERRNO
	jmp	short WS22
	
WE2:    push    dword ptr ENOSPC
        call    __SET_ERRNO
WS22:	mov	EAX,-1
	jmp	short WS13

;----------------------------------------------------------------------------
;------ we come here when either _O_APPEND or _O_TEXT was specified -- ------
;-------- first, check for _O_APPEND -- if it isn't set, _O_TEXT must have --
;-------- been --------------------------------------------------------------
;----------------------------------------------------------------------------

WR9:	test	___fhnd_info[EBX],FHND_APPEND
	jz	short WS11		; jump to _O_TEXT processing
	
;----------------------------------------------------------------------------
;------ if _O_APPEND is specified, call the operating system to position ----
;-------- to end-of-file -- we use ECX here, so we need to restore it -------
;----------------------------------------------------------------------------
	
    ifdef _WIN32
	push	ecx
	push	edx
	push	FILE_END		; dwMoveMethod
	push	0			; lpDistanceToMoveHigh
        push	0			; lDistanceToMove
        push	dword ptr __osfhnd[4*EBX] ; hFile
	call	_SetFilePointer@16
	pop		edx
	pop		ecx
	cmp	AX,-1
	je	short WE1
    else
	xor	ECX,ECX			; DX:CX = offset from end
	xor	EDX,EDX
	mov	EAX,DOS_SEEK_END
	push	ECX
	push	EDX
	bdos
	pop	EDX
	pop	ECX
	jb	short WE1
	mov	ECX,len[EBP]		; restore ECX
    endif	
    
	
;----------------------------------------------------------------------------
;------ if the file is not open in text mode, write the user's buffer -------
;-------- directly to disk --------------------------------------------------
;----------------------------------------------------------------------------

	test	___fhnd_info[EBX],FHND_TEXT
	jz	WJ9
WS11:	
	
;----------------------------------------------------------------------------
;------ we will do all string operations in the UP direction -- set ES ------
;-------- equal to DS for string operations ---------------------------------
;----------------------------------------------------------------------------

    	cld
    	
    	push	DS
    	pop	ES
    	
;----------------------------------------------------------------------------
;------ count the number of LFs in the user's buffer -- address the buffer --
;-------- using EDI ---------------------------------------------------------
;----------------------------------------------------------------------------

	push	EDI
        mov	EDI,buffer[EBP]
    
;----------------------------------------------------------------------------
;------ now scan and count LFs into EDX  -- note that the REP instruction ---
;-------- will not affect the condition code if ECX == 0 -- but ECX is ------
;-------- never zero the first time through, and on subsequent iterations ---
;-------- of the loop the INC EDX instruction will have set NZ, so the loop -
;-------- will always terminate ---------------------------------------------
;----------------------------------------------------------------------------

	mov	AL,LF
	mov	EDX,0
WL1:	repnz	scasb
	jnz	short WX1		; jump if no more LFs
	inc	EDX
	jmp	short WL1

WX1:	pop	EDI

;----------------------------------------------------------------------------
;------ if there are no LFs, then we don't need to do any translation -- ----
;-------- set ECX back to the length to read, and re-join the processing ----
;-------- above -------------------------------------------------------------
;----------------------------------------------------------------------------

	cmp	EDX,0
	jne	short WS25			; jump if no tranlation needed
	mov	ECX,len[EBP]
	jmp	WJ9
WS25:	
;----------------------------------------------------------------------------
;------ save the calculated number of line feeds ----------------------------
;----------------------------------------------------------------------------

	mov	lf_ct[EBP],EDX
	
;----------------------------------------------------------------------------
;------ calculate the size of the working buffer -- optimally equal to the --
;-------- size of the user's buffer plus one for each CR we will add (which -
;-------- is equal to the number of LFs in the buffer -- one CR per LF) -- --
;-------- round this number up to a multiple of two, since we will allocate -
;-------- the working buffer on the stack -- if there is not enough space ---
;-------- on the stack, use what is available -------------------------------
;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
;------ call chkstack to find out how much room we have on the stack -- -----
;-------- chkstack returns all of the free stack space, but we must leave ---
;-------- room to call the operating system, so subtract MINSTKFREE from ----
;-------- the amount returned by chkstack before proceeding -- if there's ---
;-------- not even that much, or if we are left with less than four bytes ---
;-------- after giving the OS its due, trigger a stack overflow right now. --
;----------------------------------------------------------------------------
WS10:
    ifndef _WIN32
	call	__chkstack
	sub	EAX,MINSTKFREE
	jb	___stk_ovfl		; jump if out of stack space
	and	EAX,0FFFFFFFCH
	cmp	EAX,4
	jbe	___stk_ovfl		; jump if out of stack space
    endif	

;----------------------------------------------------------------------------
;------ calculate maximum buffer size needed -- the size of the user's ------
;-------- buffer plus one for each CR we will add (which is the same as -----
;-------- the number of LFs in the buffer) -- plus one to allow an extra ----
;-------- CR to be thrown in at any time -- round this up to a multiple of --
;-------- of four, since the space will be allocated on the stack -- if -----
;-------- there are any overflows in this calculation, the required buffer --
;-------- size is too large to process -- return an EBADF error -------------
;----------------------------------------------------------------------------

	mov	EDX,len[EBP]
	add	EDX,lf_ct[EBP]
	jc	WE1
	add	EDX,4			; 1 for buffer pad + 3 for rounding
	jc	WE1
	and	EDX,0FFFFFFFCH
	
;----------------------------------------------------------------------------
;------ if there is not enough space to allocate the full working buffer, ---
;-------- use what is available ---------------------------------------------
;----------------------------------------------------------------------------

    ifndef _WIN32	
	cmp	EAX,EDX
	jae	short WS3
	mov	EDX,EAX
WS3:	
    endif

;----------------------------------------------------------------------------
;------ allocate the working buffer on the stack and save its address at ----
;-------- work_buf -- we need to move the saved value of EBX from the top ---
;-------- of the stack to the new top of the stack -- we will use EAX to ----
;-------- do this so that we preserve the file handle in EBX ----------------
;----------------------------------------------------------------------------

    ifdef _WIN32
        push	EDX		; save EDX:  GlobalAlloc does not preserve it
        push	EDX		; dwBytes
        push	0		; dwFlags = GMEM_FIXED
        call	_GlobalAlloc@8
        or	EAX,EAX
        jz	WE1		; return EBADF error
        mov	work_buf[EBP],EAX
        pop	EDX		; restore EDX
    else
	pop	EAX
	
	sub	ESP,EDX
	mov	work_buf[EBP],ESP
	
	push	EAX
    endif	

;----------------------------------------------------------------------------
;------ save additional registers for text-mode conversion ------------------
;----------------------------------------------------------------------------
	
	push	ESI
	push	EDI
	
;----------------------------------------------------------------------------
;-------- EDI points to the work buffer -------------------------------------
;----------------------------------------------------------------------------

	mov	EDI,work_buf[EBP]

;----------------------------------------------------------------------------
;------ the work_limit variable is set to the last byte of the working ------
;-------- buffer -- by subtracting one from EDX here, we avoid having to ----
;-------- make an extra check to see if a CR-LF pair will fit in the work ---
;-------- buffer where only a LF appeared in the users buffer ---------------
;----------------------------------------------------------------------------
	
	dec	EDX
	add	EDX,EDI
	mov	work_limit[EBP],EDX

;----------------------------------------------------------------------------
;------ here are the principal variables in the main loop below: ------------
;-------- ESI -- points to the next byte to be examined in the user's -------
;--------          buffer ---------------------------------------------------
;-------- ECX -- the number of bytes remaining to be processed in the -------
;--------        user's buffer ----------------------------------------------
;-------- EDI -- the next available byte in the working buffer --------------
;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
;------ initialize ESI and ECX for the loop below (EDI and EDX already have -
;-------- their proper initial values) --------------------------------------
;----------------------------------------------------------------------------

        mov	ESI,buffer[EBP]
	mov	ECX,len[EBP]
	
;----------------------------------------------------------------------------
;------ set the no_lf flag to FALSE (it will be set if it is discovered -----
;-------- that the user's buffer does not end in a line feed) ---------------
;----------------------------------------------------------------------------

	mov	no_lf[EBP],FALSE
   	
;----------------------------------------------------------------------------
;------ each time around this loop we will move a string of bytes -----------
;-------- terminated by a line-feed character to the working buffer, --------
;-------- replacing the LF by a CR-LF pair ----------------------------------
;----------------------------------------------------------------------------
WL5:
;----------------------------------------------------------------------------
;------ first, scan the user's buffer for a line-feed character -- we will --
;-------- need to save EDI temporarily and set it equal to ESI, the source --
;-------- buffer pointer ----------------------------------------------------
;----------------------------------------------------------------------------
	
	mov	EDX,EDI
	
	mov	EDI,ESI
	mov	al,LF	
	repnz	scasb
	
;----------------------------------------------------------------------------
;------ if we reach the end of the string without finding a LF character ----
;-------- we jump below and set the no_lf flag, which will eventually lead --
;-------- to the termination of this main loop ------------------------------
;----------------------------------------------------------------------------
	
	jnz	short WR18		; jump if no more LFs
WJ18:	
;----------------------------------------------------------------------------
;------ we have identified a string terminated by a line-feed -- calculate --
;-------- the length of the string for the MOVSD below, after first saving --
;-------- ECX (which has the number of user buffer bytes left to scan) ------
;----------------------------------------------------------------------------

	mov	user_rem[EBP],ECX
	
	mov	ECX,EDI
	sub	ECX,ESI
	
;----------------------------------------------------------------------------
;------ restore EDI (working buffer pointer) --------------------------------
;----------------------------------------------------------------------------

	mov	EDI,EDX
	
;----------------------------------------------------------------------------
;------ see if there is room in the buffer to move the bytes ----------------
;----------------------------------------------------------------------------

	add	EDX,ECX
	cmp	EDX,work_limit[EBP]
	ja	short WS6
	
;----------------------------------------------------------------------------
;------ if the no_lf flag is set, then this piece does not end in a LF byte -
;-------- because it is the last piece to move ------------------------------
;----------------------------------------------------------------------------

	cmp	no_lf[EBP],TRUE
	je	short WS19		; jump if this is the last piece
	
;----------------------------------------------------------------------------
;------ move the string, including the LF, to the working buffer -- we will -
;-------- overwrite the LF with a CR below, but it takes as many cycles to --
;-------- avoid moving the LF (and then to adjust ESI and EDI) as it takes --
;-------- to just go ahead and move it) -------------------------------------
;----------------------------------------------------------------------------

	mov	EAX,ECX
	shr	ECX,2
	rep	movsd
	mov	ECX,EAX
	and	ECX,3
	rep	movsb

;----------------------------------------------------------------------------
;------ insert a CR and a LF into the working buffer -- the CR took the -----
;-------- place of the original LF, but the LF is a new character, and we ---
;-------- will have to add one to EDI after moving it in -- note that there -
;-------- will always be room for the LF, since work_limit is actually one --
;-------- less than the size of the working buffer --------------------------
;----------------------------------------------------------------------------

	mov	word ptr ES:[EDI-1],CRLF
	inc	EDI
	
;----------------------------------------------------------------------------
;------ restore ECX to be the number of bytes remaining in the user's -------
;-------- buffer -- if there are more characters in the buffer, continue ----
;-------- scanning ----------------------------------------------------------
;----------------------------------------------------------------------------
	
	mov	ECX,user_rem[EBP]
	or	ECX,ECX
	jnz	short WL5
	
;----------------------------------------------------------------------------
;------ jump to write out the working buffer return the number of bytes -----
;-------- written -----------------------------------------------------------
;----------------------------------------------------------------------------

	jmp	short WS26
	
;----------------------------------------------------------------------------
;------ when we reach the end of the string without finding a LF byte, we ---
;-------- jump here to set the no_lf flag, which will eventually be ---------
;-------- responsible for terminating the main loop -------------------------
;----------------------------------------------------------------------------
	
WR18:	mov	no_lf[EBP],TRUE
	jmp	short WJ18	
	
;----------------------------------------------------------------------------
;------ when there are more bytes in the string to be moved than are --------
;-------- available in the working buffer, we jump here from the code above -
;-------- we will move in enough of the characters to fill out the work -----
;-------- buffer (since there was not room for the entire string, we will ---
;-------- not have to do any translation) -- the number of bytes still ------
;-------- available in the working buffer is the buffer limit minus EDI -- --
;-------- note that this calculation may yeild zero (if the buffer was ------
;-------- exactly full to the limit) or even negative one, since we have ----
;-------- allocated one buffer space past the limit for the CR added after --
;-------- a LF which fell in the last position ------------------------------
;----------------------------------------------------------------------------

WS6:	mov	ECX,work_limit[EBP]
	sub	ECX,EDI
	jbe	short WS7		; jump if buffer is already full
	
;----------------------------------------------------------------------------
;------ move the characters from the user's buffer to the working buffer -- -
;-------- we only come here when we know that there is no LF in the data ----
;-------- and therefore we don't need to do any translation -----------------
;----------------------------------------------------------------------------

WS19:	mov	EAX,ECX
	shr	ECX,2
	rep	movsd
	mov	ECX,EAX
	and	ECX,3
	rep	movsb
WS7:
;----------------------------------------------------------------------------
;------ we also need to adjust user_rem, which was updated assuming that ----
;-------- all of the string would fit into the working buffer -- EDX still --
;-------- holds the location where EDI would have ended if we had moved the -
;-------- whole string -- the difference is the adjustment factor for -------
;-------- user_rem ----------------------------------------------------------
;----------------------------------------------------------------------------

	sub	EDX,EDI
	add	user_rem[EBP],EDX

;----------------------------------------------------------------------------
;------ set up parameters to perform the write ------------------------------
;----------------------------------------------------------------------------
WS26:
	mov	EDX,work_buf[EBP]	; offset of working buffer
	mov	ECX,EDI
	sub	ECX,work_buf[EBP]	; number of bytes to write

;----------------------------------------------------------------------------
;------ For stdout, stderr, check the __write_fp vector.  If it has been ----
;-------- set, jump to call it below. ---------------------------------------
;----------------------------------------------------------------------------

	cmp	EBX,3
	jae	WS34
	mov	EAX,___write_fp
	or	EAX,EAX
	jnz	short WS31
WS34:
;----------------------------------------------------------------------------
;------ call the operating system to write out the buffer -- check for an ---
;-------- error -------------------------------------------------------------
;----------------------------------------------------------------------------

        push    ECX                     ; It would be too bad if something
        push    EDX                     ;   happened to ECX and EDX.
    ifdef _WIN32
	push	0			; lpOverlapped
	lea	EAX,actual_bytes_written[EBP]
	push	EAX			; lpNumberOfBytesWritten
	push	ECX			; nNumberOfBytesToWrite
	push	EDX			; lpBuffer
	push	dword ptr __osfhnd[4*EBX] ; hFile
	call	_WriteFile@20
	pop     EDX
	pop     ECX
	or	EAX,EAX
	jz	short WE3
	mov	EAX,actual_bytes_written[EBP]
    else
	bdos	DOS_WRITE
	pop	EDX
	pop	ECX
    	jc	short WE3
    endif    	

;----------------------------------------------------------------------------
;------ calculate number of unwritten characters -- if any bytes were -------
;-------- unwritten, the disk is probably full -- we will return an error ---
;----------------------------------------------------------------------------
WS32:
	sub	ECX,EAX
	jnz	short WS27

;----------------------------------------------------------------------------
;------ see if there is anything more to do ---------------------------------
;----------------------------------------------------------------------------

	mov	ECX,user_rem[EBP]
	or	ECX,ECX
	jz	short WS21		; jump if finished

;----------------------------------------------------------------------------
;------ re-initialize the working buffer pointers and jump to fill it up ----
;-------- again -------------------------------------------------------------
;----------------------------------------------------------------------------

	mov	EDI,work_buf[EBP]
	jmp	WL5

;----------------------------------------------------------------------------
;------ call the __write_fp vector to do the actual write -- preserve ECX ---
;-------- and EDX -----------------------------------------------------------
;----------------------------------------------------------------------------
WS31:
	push	ECX
	push	EDX
	
	push	ECX
	push	EDX
	push	EBX
	call	EAX
	add	ESP,12
	
	pop	EDX
	pop	ECX
	jmp	short WS32

;----------------------------------------------------------------------------
;------ some bytes were unwritten, most likely because the disk is full -- --
;-------- we will need to return the number of original user buffer bytes ---
;-------- which were successfully written -- but note that we have added ----
;-------- extra CR characters into the buffer -- we'll need to do a bit of --
;-------- reckoning here -- the proper return value is calculated as: -------
;----------------------------------------------------------------------------
;----------- (1) the size of the user's buffer, minus -----------------------
;----------- (2) the number of unprocessed user buffer bytes, minus ---------
;----------- (3) the number of bytes which did not get written just now; ----
;----------------------------------------------------------------------------
;-------- but this last value may have been too large, since the unwritten --
;-------- data may have contained CR-LF pairs which represent single bytes --
;-------- in the original data -- therefore we reduce total (3) by: ---------
;----------------------------------------------------------------------------
;----------- (4) the number of LFs in the unwritten part of the data; -------
;----------------------------------------------------------------------------
;-------- BUT if the first unwritten byte is a LF, it is the only -----------
;-------- representative of the original LF, therefore we don't count it ----
;-------- in total (4) ------------------------------------------------------
;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
;------ prepare to count LFs again -- address the second unwritten byte, ----
;-------- where we will begin -----------------------------------------------
;----------------------------------------------------------------------------
WS27:
	mov	EBX,EDI			; save EDI

	mov	EDI,work_buf[EBP]
	add	EDI,EAX			; to get addr of first unwritten byte
	inc	EDI			; address of second unwritten byte

;----------------------------------------------------------------------------
;------ right now ECX has the number of unwritten bytes (total (3) above) ---
;-------- move it to EDX, where we will calculate the number of unwritten ---
;-------- original user buffer bytes (total (3) - total (4)) above ----------
;----------------------------------------------------------------------------

	mov	EDX,ECX
	
;----------------------------------------------------------------------------
;------ ECX will control the search loop -- set it to one less than the -----
;-------- number of unwritten bytes (since we will skip the first unwritten -
;-------- byte in our search) -- if there is exactly one unwritten byte -----
;-------- we do not need to search at all -----------------------------------
;----------------------------------------------------------------------------
	
	dec	ECX
	jz	short WX28		; jump if no bytes to search
	
;----------------------------------------------------------------------------
;------ look for LFs in the unwritten data -- decrement EDX for each one ----
;-------- found -------------------------------------------------------------
;----------------------------------------------------------------------------

	mov	AL,LF
WL28:	repnz	scasb
	jnz	short WX28		; jump if no more LFs
	dec	DX
	jmp	short WL28
WX28:
	mov	EDI,EBX			; restore EDI
	
;----------------------------------------------------------------------------
;------ calculate the number of successfully written user bytes, as ---------
;-------- described above -- if non-zero, return this number ----------------
;----------------------------------------------------------------------------

	mov	EAX,len[EBP]		; size of user's buffer
	sub	EAX,user_rem[EBP]	; unprocessed user buffer bytes
	sub	EAX,EDX			; unwritten user buffer bytes
	jnz	short WX0

;----------------------------------------------------------------------------
;------ there is no space left on the disk and none of the user's buffer ----
;-------- was written -- set ENOSPC and return -1 ---------------------------
;----------------------------------------------------------------------------

WE4:    push    dword ptr ENOSPC
        call    __SET_ERRNO
	jmp	short WS8

;----------------------------------------------------------------------------
;------ all other errors (invalid handle, or access denied) -- set EBADF ----
;-------- and return -1 -----------------------------------------------------
;----------------------------------------------------------------------------

WE3:    push    dword ptr EBADF
        call    __SET_ERRNO
WS8:	mov	EAX,-1
	jmp	short WX0

;----------------------------------------------------------------------------
;------ a successful write! -- return the number of bytes the user asked us -
;-------- to write ----------------------------------------------------------
;----------------------------------------------------------------------------
	
WS21:	mov	EAX,len[EBP]
	
;----------------------------------------------------------------------------
;------	free the working buffer (NT only), restore the registers, clear the -
;-------- stack frame and return --------------------------------------------
;----------------------------------------------------------------------------

WX0:	
    ifdef _WIN32
	push	EAX			; save our return value
	push	work_buf[EBP]
	call	_GlobalFree@4
	pop	EAX			; restore return value
    endif

	pop	EDI
	pop	ESI
	pop	EBX
	mov	ESP,EBP
	pop	EBP
	ret
c_endp	write

	endcode write
	end
