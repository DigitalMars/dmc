;_ write.asm Nov 9, 1993
; Copyright 1993 by Digital Mars
; All Rights Reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	include stdio.asm

        extrn   ___fhnd_info:byte
        
    if LCODE
    	extrn	__chkstack:far
    	extrn	___stk_ovfl:far
    	extrn   __SET_ERRNO:far
    else
    	extrn	__chkstack:near
    	extrn	___stk_ovfl:near
    	extrn   __SET_ERRNO:near
    endif    	    		
    
    ifdef _WINDOWS
	extrn   ___winc_bdos:dword	
    endif

	c_public write
    if SPTR
	c_public _writex
    endif

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

user_sel   = word ptr -2	; must be 1st in local variables
user_rem   = word ptr -4
work_limit = word ptr -6
work_buf   = word ptr -8
lf_ct      = word ptr -10
no_lf	   = byte ptr -12
localbytes = 12

;----------------------------------------------------------------------------
;------	parameters ----------------------------------------------------------
;----------------------------------------------------------------------------

fd         = P
buffer     = P+2
len        = P+2+SIZEPTR

;----------------------------------------------------------------------------
;------	some miscellaneous manifest constants -------------------------------
;----------------------------------------------------------------------------

TRUE	     = 01H
FALSE	     = 00H
LF	     = 0AH
CRLF         = 0A0DH
DOS_SEEK_END = 4202H
DOS_WRITE    = 40H
FHND_APPEND  = 04H
FHND_TEXT    = 10H

ifdef _WINDOWS
MINSTKFREE   = 640
else
MINSTKFREE   = 64
endif

;----------------------------------------------------------------------------
;------ set up the stack frame -- in the near data models, we push DS onto --
;-------- the stack as the proper value for user_sel (-2[BP]) -- the --------
;-------- writex routine (below) will set this to a different value and -----
;-------- jump to WJ0 -------------------------------------------------------
;----------------------------------------------------------------------------

func	write
	WINENTER
	
    if SPTR
    	push	DS
    endif
	
;----------------------------------------------------------------------------
;------ load and validate the file handle to BX -----------------------------
;----------------------------------------------------------------------------

WJ0:	mov	BX,fd[BP]
	cmp	BX,_nfile	; number of handles supported
	jnb	WE1
	
;----------------------------------------------------------------------------
;------ CX will hold the number of bytes to write -- if it is zero, we're ---
;-------- done --------------------------------------------------------------
;----------------------------------------------------------------------------
	
	mov	CX,len[BP]
	cmp	CX,0
	je	WS14
	
;----------------------------------------------------------------------------
;------ if either the _O_APPEND or _O_TEXT options were set on the open, ----
;--------- we take the time to process these now ----------------------------
;----------------------------------------------------------------------------

	test	___fhnd_info[BX],FHND_APPEND+FHND_TEXT
	jnz	WR9
WJ9:	
;----------------------------------------------------------------------------
;------ set up the buffer pointer for the write -- be sure to save the ------
;-------- caller's DS -------------------------------------------------------
;----------------------------------------------------------------------------

	push	DS

    if LPTR
	lds	DX,buffer[BP]
    else	
	mov	DS,user_sel[BP]
	mov	DX,buffer[BP]
    endif

;----------------------------------------------------------------------------
;------ under WINDOWS, console writes (fd < 5) must be filtered by WINC -- --
;-------- jump if appropriate and call WINC ---------------------------------
;----------------------------------------------------------------------------

    ifdef _WINDOWS
	cmp	BX,5
	jb	WR21
    endif
    
;----------------------------------------------------------------------------
;------ call the operating system to do the write -- restore DS and check ---
;-------- for error ---------------------------------------------------------
;----------------------------------------------------------------------------

	bdos	DOS_WRITE
WJ21:	
	pop	DS
    	jc	WE1
    	
;----------------------------------------------------------------------------
;------ if no bytes were written, we are out of disk space -- return an -----
;-------- error -------------------------------------------------------------
;----------------------------------------------------------------------------
    	
    	cmp	AX,0
    	je	WE2
    	
;----------------------------------------------------------------------------
;------ and return ----------------------------------------------------------
;----------------------------------------------------------------------------
WS13:
    if SPTR
    	mov	SP,BP
    endif

	WINLEAVE
	ret

;----------------------------------------------------------------------------
;------ we come here if CX is zero:  just return a zero ---------------------
;----------------------------------------------------------------------------
	
WS14:	xor	AX,AX
	jmp	short WS13
	
;----------------------------------------------------------------------------
;------ set the error code and return a -1 ----------------------------------
;----------------------------------------------------------------------------

WE1:	mov     AX,EBADF
        push    AX
        call    __SET_ERRNO
	jmp	WS22
	
WE2:	mov     AX,ENOSPC
        push    AX
        call    __SET_ERRNO
WS22:	mov	AX,-1
	jmp	WS13

;----------------------------------------------------------------------------
;------ for WINDOWS console writes, we must call WINC -----------------------
;----------------------------------------------------------------------------

    ifdef _WINDOWS
WR21:  ;;	mov	AX,seg ___winc_bdos
      ;;	mov	ES,AX
        pop ES	    ;;  DS is on stack, so pop it into es
        push ES	    ;;  Put DS back onto stack

	mov	AH,DOS_WRITE
	call 	ES:___winc_bdos
	jmp	WJ21
    endif	
    
;----------------------------------------------------------------------------
;------ jump to indicate a stack overflow and terminate the program ---------
;----------------------------------------------------------------------------
	
WS15:	jmp	___stk_ovfl	
	
;----------------------------------------------------------------------------
;------ we come here when either _O_APPEND or _O_TEXT was specified -- ------
;-------- first, check for _O_APPEND -- if it isn't set, _O_TEXT must have --
;-------- been --------------------------------------------------------------
;----------------------------------------------------------------------------

WR9:	test	___fhnd_info[BX],FHND_APPEND
	jz	WS11			; jump to _O_TEXT processing
	
;----------------------------------------------------------------------------
;------ if _O_APPEND is specified, call the operating system to position ----
;-------- to end-of-file -- we use CX here, so we need to restore it --------
;----------------------------------------------------------------------------
	
	xor	CX,CX			; DX:CX = offset from end
	xor	DX,DX
	mov	AX,DOS_SEEK_END
	bdos
	jb	WE1
	
	mov	CX,len[BP]		; restore CX
	
;----------------------------------------------------------------------------
;------ if the file is not open in text mode, write the user's buffer -------
;-------- directly to disk --------------------------------------------------
;----------------------------------------------------------------------------

	test	___fhnd_info[BX],FHND_TEXT
	jz	WJ9
WS11:	
	
;----------------------------------------------------------------------------
;------ we will do all string operations in the UP direction ----------------
;----------------------------------------------------------------------------

    	cld
    	
;----------------------------------------------------------------------------
;------ count the number of LFs in the user's buffer -- address the buffer --
;-------- using ES:DI -------------------------------------------------------
;----------------------------------------------------------------------------

	push	DI			; this is still the caller's DI

    if LPTR
	les	DI,buffer[BP]
    else
    	mov	ES,user_sel[BP]
        mov	DI,buffer[BP]
    endif
    
;----------------------------------------------------------------------------
;------ now scan and count LFs into DX  -- note that the REP instruction ----
;-------- will not affect the condition code if CX == 0 -- but CX is never --
;-------- zero the first time through, and on subsequent iterations of the --
;-------- loop the INC DX instruction will have set NZ, so the loop will ----
;-------- always terminate --------------------------------------------------
;----------------------------------------------------------------------------

	mov	AL,LF
	mov	DX,0
WL1:	repnz	scasb
	jnz	WX1			; jump if no more LFs
	inc	DX
	jmp	WL1

WX1:	pop	DI			; restore the saved DI value

;----------------------------------------------------------------------------
;------ if there are no LFs, then we don't need to do any translation -- ----
;-------- set CX back to the length to read, and re-join the processing -----
;-------- above -------------------------------------------------------------
;----------------------------------------------------------------------------

	cmp	DX,0
	jne	WS25			; jump if no tranlation needed
	mov	CX,len[BP]
	jmp	WJ9
WS25:	
;----------------------------------------------------------------------------
;------ we need a fancier stack frame in order to do text mode writes -- ----
;-------- note that for near data models, the user_sel parameter has --------
;-------- been pushed onto the stack ----------------------------------------
;----------------------------------------------------------------------------
    	
    if SPTR
	sub	SP,localbytes-2
    else
    	sub	SP,localbytes	
    endif
	
;----------------------------------------------------------------------------
;------ save the calculated number of line feeds ----------------------------
;----------------------------------------------------------------------------

	mov	lf_ct[BP],DX
	
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

WS10:	call	__chkstack
	sub	AX,MINSTKFREE
	jb	WS15			; jump if out of stack space
	and	AX,0FFFEH
	cmp	AX,4
	jbe	WS15			; jump if out of stack space

;----------------------------------------------------------------------------
;------ calculate maximum buffer size needed -- the size of the user's ------
;-------- buffer plus one for each CR we will add (which is the same as -----
;-------- the number of LFs in the buffer) -- plus one to allow an extra ----
;-------- CR to be thrown in at any time -- round this up to a multiple of --
;-------- of two, since the space will be allocated on the stack -- if ------
;-------- there are any overflows in this calculation, use all of the -------
;-------- available stack space ---------------------------------------------
;----------------------------------------------------------------------------

	mov	DX,len[BP]
	add	DX,lf_ct[BP]
	jc	WS4
	add	DX,2			; 1 for buffer pad + 1 for rounding
	jc	WS4
	and	DX,0FFFEH
	
;----------------------------------------------------------------------------
;------ if there is not enough space to allocate the full working buffer, ---
;-------- use what is available ---------------------------------------------
;----------------------------------------------------------------------------
	
	cmp	AX,DX
	jae	WS3
WS4:	mov	DX,AX
WS3:	

;----------------------------------------------------------------------------
;------ allocate the working buffer on the stack ----------------------------
;----------------------------------------------------------------------------

	sub	SP,DX
	
;----------------------------------------------------------------------------
;-------- set up ES and work_buf to point to the first byte of this buffer --
;----------------------------------------------------------------------------

	push	SS
	pop	ES
	mov	work_buf[BP],SP
	
;----------------------------------------------------------------------------
;------ save critical registers ---------------------------------------------
;----------------------------------------------------------------------------

	push    SI
	push	DI
	push	DS
	
;----------------------------------------------------------------------------
;------ set up DI to address the working buffer -----------------------------
;----------------------------------------------------------------------------

	mov	DI,work_buf[BP]

;----------------------------------------------------------------------------
;------ the work_limit variable is set to the last byte of the working ------
;-------- buffer -- by subtracting one from DX here, we avoid having to -----
;-------- make an extra check to see if a CR-LF pair will fit in the work ---
;-------- buffer where only a LF appeared in the users buffer ---------------
;----------------------------------------------------------------------------
	
	dec	DX
	add	DX,DI
	mov	work_limit[BP],DX

;----------------------------------------------------------------------------
;------ here are the principal variables in the main loop below: ------------
;-------- DS:SI -- points to the next byte to be examined in the user's -----
;--------          buffer ---------------------------------------------------
;-------- CX -- the number of bytes remaining to be processed in the user's -
;--------       buffer ------------------------------------------------------
;-------- ES:DI -- the next available byte in the working buffer ------------
;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
;------ initialize DS:SI and CX for the loop below (ES:DI and DX already ----
;-------- have their proper initial values) -- in the near data models, the -
;-------- selector for the user's buffer was set on entry to this routine ---
;-------- -- in the remaining models, we must set it now --------------------
;----------------------------------------------------------------------------

    if LPTR
	lds	SI,buffer[BP]
    	mov	user_sel[BP],DS
    else
    	mov	DS,user_sel[BP]
        mov	SI,buffer[BP]
    endif
    
	mov	CX,len[BP]
	
;----------------------------------------------------------------------------
;------ set the no_lf flag to FALSE (it will be set if it is discovered -----
;-------- that the user's buffer does not end in a line feed) ---------------
;----------------------------------------------------------------------------

	mov	no_lf[BP],FALSE
   	
;----------------------------------------------------------------------------
;------ each time around this loop we will move a string of bytes -----------
;-------- terminated by a line-feed character to the working buffer, --------
;-------- replacing the LF by a CR-LF pair ----------------------------------
;----------------------------------------------------------------------------
WL5:
;----------------------------------------------------------------------------
;------ first, scan the user's buffer for a line-feed character -- we will --
;-------- need to save ES:DI temporarily and set it equal to DS:SI, the -----
;-------- source buffer pointer ---------------------------------------------
;----------------------------------------------------------------------------
	
	push	ES
	mov	DX,DI
	
	mov	ES,user_sel[BP]
	mov	DI,SI

	mov	al,LF	
	repnz	scasb
	
;----------------------------------------------------------------------------
;------ if we reach the end of the string without finding a LF character ----
;-------- we jump below and set the no_lf flag, which will eventually lead --
;-------- to the termination of this main loop ------------------------------
;----------------------------------------------------------------------------
	
	jnz	WR18			; jump if no more LFs
WJ18:	
;----------------------------------------------------------------------------
;------ we have identified a string terminated by a line-feed -- calculate --
;-------- the length of the string for the MOVSW below, after first saving --
;-------- CX (which has the number of user buffer bytes left to scan) -------
;----------------------------------------------------------------------------

	mov	user_rem[BP],CX
	
	mov	CX,DI
	sub	CX,SI
	
;----------------------------------------------------------------------------
;------ restore ES and DI (working buffer pointer) --------------------------
;----------------------------------------------------------------------------

	mov	DI,DX
	pop	ES
	
;----------------------------------------------------------------------------
;------ see if there is room in the buffer to move the bytes ----------------
;----------------------------------------------------------------------------

	add	DX,CX
	cmp	DX,work_limit[BP]
	ja	WS6
	
;----------------------------------------------------------------------------
;------ if the no_lf flag is set, then this piece does not end in a LF byte -
;-------- because it is the last piece to move ------------------------------
;----------------------------------------------------------------------------

	cmp	no_lf[BP],TRUE
	je	WS19			; jump if this is the last piece
	
;----------------------------------------------------------------------------
;------ move an even number of words from the user's buffer to the working --
;-------- buffer -- copy the final LF if necessary to fill out the last -----
;-------- word (we will the overwrite it with a CR below) -- if the LF is ---
;-------- not copied, we need to increment SI so that it will be pointing ---
;-------- past the LF in the user's buffer, and DI so that it will be -------
;-------- pointing past the point where the CR goes -------------------------
;----------------------------------------------------------------------------

	test	DI,1
	jz	WS24			; jump if dest aligned on word bound
	
	sub	CX,1
	jc	WS16			; jump if CX was zero
	movsb				; word-align the destination

WS24:	shr	CX,1
	rep	movsw
	jnc	WS16
	inc	SI
	inc	DI
WS16:	
;----------------------------------------------------------------------------
;------ insert a CR and a LF into the working buffer -- the CR took the -----
;-------- place of the original LF, but the LF is a new character, and we ---
;-------- will have to add one to DI after moving it in -- note that there --
;-------- will always be room for the LF, since work_limit is actually one --
;-------- less than the size of the working buffer --------------------------
;----------------------------------------------------------------------------

	mov	word ptr ES:[DI-1],CRLF
	inc	DI
	
;----------------------------------------------------------------------------
;------ restore CX to be the number of bytes remaining in the user's buffer -
;-------- -- if there are more characters in the buffer, continue scanning --
;----------------------------------------------------------------------------
	
	mov	CX,user_rem[BP]
	cmp	CX,0
	jnz	WL5
	
;----------------------------------------------------------------------------
;------ write out the working buffer and jump to return the number of bytes -
;-------- written -----------------------------------------------------------
;----------------------------------------------------------------------------

	call	write_buffer
	jmp	WS21
	
;----------------------------------------------------------------------------
;------ when we reach the end of the string without finding a LF byte, we ---
;-------- jump here to set the no_lf flag, which will eventually be ---------
;-------- responsible for terminating the main loop -------------------------
;----------------------------------------------------------------------------
	
WR18:	mov	no_lf[BP],TRUE
	jmp	WJ18	
	
;----------------------------------------------------------------------------
;------ when there are more bytes in the string to be moved than are --------
;-------- available in the working buffer, we jump here from the code above -
;-------- we will move in enough of the characters to fill out the work -----
;-------- buffer (since there was not room for the entire string, we will ---
;-------- not have to do any translation) -- the number of bytes still ------
;-------- available in the working buffer is the buffer limit minus DI -- ---
;-------- note that this calculation may yeild zero (if the buffer was ------
;-------- exactly full to the limit) or even negative one, since we have ----
;-------- allocated one buffer space past the limit for the CR added after --
;-------- a LF which fell in the last position ------------------------------
;----------------------------------------------------------------------------

WS6:	mov	CX,work_limit[BP]
	sub	CX,DI
	jbe	WS7			; jump if buffer is already full
	
;----------------------------------------------------------------------------
;------ move the characters from the user's buffer to the working buffer -- -
;-------- we only come here when we know that there is no LF in the data ----
;-------- and therefore we don't need to do any translation -----------------
;----------------------------------------------------------------------------

WS19:	test	DI,1
	jz	WS23			; jump if dest aligned on word bound
	
	sub	CX,1
	jc	WS7			; jump if CX was zero
	movsb				; word-align destination

WS23:	shr	CX,1
	rep	movsw
	jnc	WS7
	movsb
WS7:	
;----------------------------------------------------------------------------
;------ we also need to adjust user_rem, which was updated assuming that ----
;-------- all of the string would fit into the working buffer -- DX still ---
;-------- holds the location where DI would have ended if we had moved the --
;-------- whole string -- the difference is the adjustment factor for -------
;-------- user_rem ----------------------------------------------------------
;----------------------------------------------------------------------------

	sub	DX,DI
	add	user_rem[BP],DX

;----------------------------------------------------------------------------
;------ write out the working buffer ----------------------------------------
;----------------------------------------------------------------------------

	call	write_buffer

;----------------------------------------------------------------------------
;------ see if there is anything more to do ---------------------------------
;----------------------------------------------------------------------------

	mov	CX,user_rem[BP]
	or	CX,CX
	jz	WS21			; jump if finished

;----------------------------------------------------------------------------
;------ re-initialize the working buffer pointers and jump to fill it up ----
;-------- again -------------------------------------------------------------
;----------------------------------------------------------------------------

	mov	DI,work_buf[BP]
	jmp	WL5

;----------------------------------------------------------------------------
;------ there is no space left on the disk and none of the user's buffer ----
;-------- was written -- set ENOSPC and return -1 ---------------------------
;----------------------------------------------------------------------------

WE4:	mov     AX,ENOSPC
        push    AX
        call    __SET_ERRNO
	jmp	WS8

;----------------------------------------------------------------------------
;------ all other errors (invalid handle, or access denied) -- set EBADF ----
;-------- and return -1 -----------------------------------------------------
;----------------------------------------------------------------------------

WE3:	mov     AX,EBADF
        push    AX
        call    __SET_ERRNO
WS8:	mov	AX,-1
	jmp	WX0

;----------------------------------------------------------------------------
;------ a successful write! -- return the number of bytes the user asked us -
;-------- to write ----------------------------------------------------------
;----------------------------------------------------------------------------
	
WS21:	mov	AX,len[BP]
	
;----------------------------------------------------------------------------
;------	restore the registers, clear the stack frame and return -------------
;----------------------------------------------------------------------------

WX0:    pop	DS
	pop	DI
	pop	SI
	mov	SP,BP
	WINLEAVE
	ret
c_endp	write

;****************************************************************************
; write_buffer
;
; This routine writes out the working buffer, and handles errors, including
; partial writes due to insufficient disk space.
;
; On entry, DI points to the first unused character in the working buffer
; 
;****************************************************************************

write_buffer proc near

;----------------------------------------------------------------------------
;------ save the segment registers ------------------------------------------
;----------------------------------------------------------------------------

	push	DS
	push	ES

;----------------------------------------------------------------------------
;------ set up parameters to perform the write ------------------------------
;----------------------------------------------------------------------------

	mov	BX,fd[BP]		; file handle
	push	SS
	pop	DS			; segment of working buffer
	mov	DX,work_buf[BP]		; offset of working buffer
	mov	CX,DI
	sub	CX,work_buf[BP]		; number of bytes to write

;----------------------------------------------------------------------------
;------ under WINDOWS console writes (fd < 5) must be filtered by WINC -- ---
;-------- if appropriate, jump and call WINC to do the write ----------------
;----------------------------------------------------------------------------

    ifdef _WINDOWS
	cmp	BX,5
	jb	BR2
    endif	

;----------------------------------------------------------------------------
;------ call the operating system to write out the buffer -- check for an ---
;-------- error -------------------------------------------------------------
;----------------------------------------------------------------------------

	bdos	DOS_WRITE
BJ2:	
	jc	BE1

;----------------------------------------------------------------------------
;------ calculate number of unwritten characters -- if all characters were --
;-------- written, we are done ----------------------------------------------
;----------------------------------------------------------------------------

	sub	CX,AX
	jz	BX0

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

	mov	BX,DI			; save DI

	push	SS
	pop	ES
	mov	DI,work_buf[BP]
	add	DI,AX			; to get addr of first unwritten byte
	inc	DI			; address of second unwritten byte

;----------------------------------------------------------------------------
;------ right now CX has the number of unwritten bytes (total (3) above) -- -
;-------- move it to DX, where we will calculate the number of unwritten ----
;-------- original user buffer bytes (total (3) - total (4)) above ----------
;----------------------------------------------------------------------------

	mov	DX,CX
	
;----------------------------------------------------------------------------
;------ CX will control the search loop -- set it to one less than the ------
;-------- number of unwritten bytes (since we will skip the first unwritten -
;-------- byte in our search) -- if there is exactly one unwritten byte -----
;-------- we do not need to search at all -----------------------------------
;----------------------------------------------------------------------------
	
	dec	CX
	jz	BX1			; jump if no bytes to search
	
;----------------------------------------------------------------------------
;------ look for LFs in the unwritten data -- decrement DX for each one -----
;-------- found -------------------------------------------------------------
;----------------------------------------------------------------------------

	mov	AL,LF
BL1:	repnz	scasb
	jnz	BX1			; jump if no more LFs
	dec	DX
	jmp	BL1
BX1:
	mov	DI,BX			; restore DI
	
;----------------------------------------------------------------------------
;------ calculate the number of successfully written user bytes, as ---------
;-------- described above ---------------------------------------------------
;----------------------------------------------------------------------------

	mov	AX,len[BP]		; size of user's buffer
	sub	AX,user_rem[BP]		; unprocessed user buffer bytes
	sub	AX,DX			; unwritten user buffer bytes
	jz	BE2

;----------------------------------------------------------------------------
;------ toss the stack frame and jump to return -----------------------------
;----------------------------------------------------------------------------

	add	SP,6
	jmp	WX0

;----------------------------------------------------------------------------
;------ there's no space left on the disk and none of the user's buffer was -
;-------- written -- toss the stack frame and jump to the main routine ------
;-------- ENOSPC code -------------------------------------------------------
;----------------------------------------------------------------------------

BE2:	add	SP,6
	jmp	WE3		

;----------------------------------------------------------------------------
;------ all other errors -- toss the stack frame and jump to the main -------
;-------- routine EBADF code ------------------------------------------------
;----------------------------------------------------------------------------

BE1:	add	SP,6
	jmp	WE4
	
;----------------------------------------------------------------------------
;------ on successful completion of the write, restore the segment ----------
;-------- registers and return to continue processing -----------------------
;----------------------------------------------------------------------------

BX0:	pop	ES
	pop	DS
	ret	
	
;----------------------------------------------------------------------------
;------ for WINDOWS console writes, we must call WINC -----------------------
;----------------------------------------------------------------------------

    ifdef _WINDOWS
BR2:	;;mov	AX,seg ___winc_bdos
	;;mov	ES,AX
        pop ES   ;; DS is not top of stack, set ES to it
        push ES  ;; Put DS back on stack

	mov	AH,DOS_WRITE
	call 	ES:___winc_bdos
	jmp	BJ2
    endif	
write_buffer endp

    if SPTR
    
;****************************************************************************
; int __clib _writex (int fd, void __near *buffer, unsigned len, 
;                     unsigned segment)
; 
; The _writex function, available only in the near data models, accepts a
; segment address for the buffer as well as a near pointer.  The pointer and
; segment address are combined to locate the buffer.  In all other respects,
; _writex is identical to write.
;****************************************************************************

;----------------------------------------------------------------------------
;------	parameters ----------------------------------------------------------
;----------------------------------------------------------------------------

fd         = P
buffer     = P+2
len        = P+2+SIZEPTR
sel        = P+2+SIZEPTR+2

;----------------------------------------------------------------------------
;------	establish the stack frame -------------------------------------------
;----------------------------------------------------------------------------

func	_writex
	WINENTER
	
;----------------------------------------------------------------------------
;------	set the first word of local storage equal to the selector of the ----
;-------- user's data buffer -- it will be referenced here by the main ------
;-------- write routine above -----------------------------------------------
;----------------------------------------------------------------------------

	mov	ES,sel[BP]
	push	ES
    
;----------------------------------------------------------------------------
;------	jump to join write code ---------------------------------------------
;----------------------------------------------------------------------------

	jmp	WJ0
c_endp	_writex

    endif ; SPTR
   
	endcode write
	end
