;_ read.asm
; Copyright 1993 by Digital Mars
; All Rights Reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	include stdio.asm

if LCODE
extrn __SET_ERRNO:far
else
extrn __SET_ERRNO:near
endif
   
    ifdef _WINDOWS
		begdata
        	public	___winc_bdos
___winc_bdos 	dd	local_bdos        
		enddata
    endif
		
        extrn  ___fhnd_info:byte

	begcode read

        
	c_public read
    if SPTR
	c_public _readx
    endif

;****************************************************************************
; int __clib read (int fd, void *buffer, unsigned len)
; 
; The read function gets the number of bytes specified in len from the file
; associated with the file descriptor fd.  The bytes are read into buffer.
; After the read operation, the pointer points to the next unread byte.
;
; This function returns the number of characters put into the buffer, which
; may be less than len if end of file was encountered.  If an error occurs
; when reading the file, a -1 is returned and errno is set to EBADF.
;
; In text mode, each carriage-return-line-feed pair is replaced with a single
; line feed character.  Only the single line-feed character is counted in the
; return value.  Also, in text mode, a CTRL/Z character is treated as an end-
; of-file indicator.
;****************************************************************************

;----------------------------------------------------------------------------
;------	stack frame variables -----------------------------------------------
;----------------------------------------------------------------------------

buffer_sel           = word ptr -2 ; this must be the first local variable
next_compressed_byte = word ptr -4
fhnd	             = byte ptr -6
localbytes = 6

;----------------------------------------------------------------------------
;------	parameters ----------------------------------------------------------
;----------------------------------------------------------------------------

fd         = P
buffer     = P+2
len        = P+2+SIZEPTR

;----------------------------------------------------------------------------
;------	some miscellaneous manifest constants -------------------------------
;----------------------------------------------------------------------------

EOF_MARKER   = 1AH
CR	     = 0DH
LF	     = 0AH
DOS_SEEK_CUR = 4201H
DOS_READ     = 3FH
FHND_APPEND  = 04H
FHND_DEVICE  = 08H
FHND_EOF     = 01H
FHND_TEXT    = 10H

;----------------------------------------------------------------------------
;------ set up the stack frame -- in the near data models, we push DS onto --
;-------- the stack as the proper value for user_sel (-2[BP]) -- the --------
;-------- readx routine (below) will set this to a different value and ------
;-------- jump to RJ0 -------------------------------------------------------
;----------------------------------------------------------------------------

func	read
	WINENTER
	
    if SPTR
    	push	DS
    endif
	
;----------------------------------------------------------------------------
;------ load and validate the file handle to BX -----------------------------
;----------------------------------------------------------------------------

RJ0:	mov	BX,fd[BP]
	cmp	BX,_nfile	; number of handles supported
	jnb	RE12
	
;----------------------------------------------------------------------------
;------ CX will hold the number of bytes to read -- if it's zero, we're -----
;-------- done --------------------------------------------------------------
;----------------------------------------------------------------------------
	
	mov	CX,len[BP]
	cmp	CX,0
	je	RS14
	
;----------------------------------------------------------------------------
;------- if the file is text-mode, it requires extra processing -------------
;----------------------------------------------------------------------------

	test	___fhnd_info[BX],FHND_TEXT
	jnz	RS9			; jump if text mode

;----------------------------------------------------------------------------
;------ set up the buffer pointer for the read -- be sure to save the -------
;-------- caller's DS -------------------------------------------------------
;----------------------------------------------------------------------------

	push	DS

    if LPTR
	lds	DX,buffer[BP]
    else	
	mov	DS,buffer_sel[BP]
	mov	DX,buffer[BP]
    endif

;----------------------------------------------------------------------------
;------ under WINDOWS, console reads (fd < 5) must be filtered by WINC -- ---
;-------- jump if appropriate and call WINC ---------------------------------
;----------------------------------------------------------------------------

    ifdef _WINDOWS
	cmp	BX,5
	jb	RR21
    endif
    
;----------------------------------------------------------------------------
;------ call the operating system to do the read -- restore DS and check ----
;-------- for error ---------------------------------------------------------
;----------------------------------------------------------------------------

	bdos	DOS_READ
RJ21:	
	pop	DS
	jc	RE12
    	
;----------------------------------------------------------------------------
;------ and return ----------------------------------------------------------
;----------------------------------------------------------------------------
RS13:
    if SPTR
    	mov	SP,BP
    endif

	WINLEAVE
	ret
	
;----------------------------------------------------------------------------
;------ we come here if CX is zero:  just return a zero ---------------------
;----------------------------------------------------------------------------
	
RS14:	xor	AX,AX
	jmp	short RS13
	
;----------------------------------------------------------------------------
;------ set the error code and return a -1 ----------------------------------
;----------------------------------------------------------------------------

RE12:	mov     AX,EBADF
        push    AX
        call    __SET_ERRNO
	mov	AX,-1
	jmp	RS13

;----------------------------------------------------------------------------
;------ for WINDOWS console writes, we must call WINC -----------------------
;----------------------------------------------------------------------------

    ifdef _WINDOWS
RR21:	;;mov	AX,seg ___winc_bdos
	;;mov	ES,AX
        pop ES   ;; DS is on top of stack, put it as seg .
        push ES  ;; put DS back on stack.

	mov	AH,DOS_READ
	call 	ES:___winc_bdos
	jmp	RJ21
    endif	
    
;----------------------------------------------------------------------------
;------ pass console reads under WINDOWS to WINC ----------------------------
;----------------------------------------------------------------------------

    ifdef _WINDOWS
RS17:	pop     AX   ;; Get DS
        push    AX   ;; Put DS back onto stack
        push	ES
	;;mov	AX,seg ___winc_bdos
	mov	ES,AX
	mov	AH,DOS_READ
	call	ES:___winc_bdos
	pop	ES
	jmp	RS18
    endif

;----------------------------------------------------------------------------
;------ we need a fancier stack frame in order to do text mode reads -- -----
;-------- note that for near data models, the buffer_sel local variable -----
;-------- already been pushed onto the stack, so we do not allocate space ---
;-------- for it here -------------------------------------------------------
;----------------------------------------------------------------------------
RS9:    	
    if SPTR
	sub	SP,localbytes-2
    else
    	sub	SP,localbytes	
    endif
	push    SI
	push	DI
	push	DS
	
;----------------------------------------------------------------------------
;------	ES:DI points to the beginning of the buffer -- DI will be used to ---
;-------- scan the buffer for EOF and CR/LF pairs ---------------------------
;----------------------------------------------------------------------------

    if LPTR
	les	DI,buffer[BP]
    else
        mov	DI,buffer[BP]
        mov	ES,buffer_sel[BP]
    endif
    
;----------------------------------------------------------------------------
;------ move the open mode flags to local storage for reference, since DS ---
;-------- will be used for addressing the data buffer -- turn off the local -
;-------- eof flag ----------------------------------------------------------
;----------------------------------------------------------------------------

	mov	AL,___fhnd_info[BX]
	and	AL,not FHND_EOF
	mov	fhnd[BP],AL
	
;----------------------------------------------------------------------------
;------ we will do all string operations in the UP direction ----------------
;----------------------------------------------------------------------------

    	cld
    	
;----------------------------------------------------------------------------
;------ each time around loop 1 we will fill the buffer up completely -- ----
;-------- if we convert one or more CR-LF pairs to CRs there will be more ---
;-------- room in the buffer and we will come back to the top of LOOP1 to ---
;-------- fill it out with a few more characters from the file, go on to ----
;-------- strip out any additional CR-LF pairs that were read in, and so ----
;-------- forth -- at the top of this loop, ES:DI points to the first empty -
;-------- character of the buffer, and CX holds the number of empty spaces --
;-------- remaining ---------------------------------------------------------
;----------------------------------------------------------------------------

RL1:	
;----------------------------------------------------------------------------
;------ address the buffer (or it's empty tail if it is partially full) for -
;-------- the read operation -- BX (the file handle) and CX (the number of --
;-------- bytes to read) are already set ------------------------------------
;----------------------------------------------------------------------------

	push	ES
	pop	DS
	mov	DX,DI			; DS:DI is the buffer location
	
;----------------------------------------------------------------------------
;------ under WINDOWS, all console reads (fd < 5) must be passed to WINC -- -
;-------- jump to the WINC code if appropriate ------------------------------
;----------------------------------------------------------------------------

    ifdef _WINDOWS
    	cmp	BX,5
    	jb	RS17
    endif	
    
;----------------------------------------------------------------------------
;------ call the operating system to do the read and then check for errors --
;----------------------------------------------------------------------------

	bdos	DOS_READ
RS18:	
	jc	RE15A			; access denied or invalid handle

;----------------------------------------------------------------------------
;------ set DX to point to the first byte past the good data ----------------
;----------------------------------------------------------------------------	

	add	DX,AX
	
;----------------------------------------------------------------------------
;------ we are at EOF if less bytes come back than we asked for (or we may --
;-------- be reading from the console, but either way we don't want to go ---
;-------- back to the file for more data) -----------------------------------
;----------------------------------------------------------------------------

	cmp	AX,CX
	je	RS16			; jump if we got all the bytes
	
;----------------------------------------------------------------------------
;------ indicate EOF and set CX to the actual number of bytes read ----------
;----------------------------------------------------------------------------

	or	fhnd[BP],FHND_EOF
	mov	CX,AX
RS16:
;----------------------------------------------------------------------------
;------ if we didn't read anything, we are done -----------------------------
;----------------------------------------------------------------------------

	or	CX,CX
	jz	RX1A			; jump if no data came back
	
;----------------------------------------------------------------------------
;------	scan the data just read in (ES:DI for CX bytes) for an EOF (^Z) -----
;-------- byte -- don't lose DI, it's the only pointer to the data we have --
;----------------------------------------------------------------------------

	push	DI
    	mov	AL,EOF_MARKER
    	repnz	SCASB
    	jz	RR6			; jump if EOF found
RJ6:	pop	DI

;----------------------------------------------------------------------------
;------	set up for the loop below -- set CX to the number of bytes in the ---
;-------- buffer, AL to the character we will search for (CR) ---------------
;----------------------------------------------------------------------------

 	mov	CX,DX
	sub	CX,DI
	mov	AL,CR
	
;----------------------------------------------------------------------------
;------	we set SI to 0 to indicate that we have not yet found a CR-LF pair --
;----------------------------------------------------------------------------

	xor	SI,SI
	
;----------------------------------------------------------------------------
;------ loop 2 looks for the first CR-LF pair -- if none is found, there ----
;-------- may still be a CR in the final position of the buffer -- if so, ---
;-------- we will need to read ahead in the file to see if it is followed ---
;-------- by a CR -- if neither of these conditions occurs, we are ready to -
;-------- return ------------------------------------------------------------
;----------------------------------------------------------------------------

RL2: 	repnz	SCASB
    	jnz	RX1A			; jump if no potential CR-LFs found
	cmp	DI,DX
	jb      RS5
	jmp	RS7			; jump if CR is last char of buffer
RS5:	cmp	byte ptr ES:[DI],LF
	jnz	RL2			; fall through if CR-LF pair found
	
;----------------------------------------------------------------------------
;------	we have found a CR-LF pair and will need to compress the buffer -- --
;-------- set next_compressed_byte pointing to the CR which will later be ---
;-------- overwritten and SI to the LF which is the beginning of the string -
;-------- which will overwrite it -------------------------------------------
;----------------------------------------------------------------------------

	mov	SI,DI
	mov	next_compressed_byte[BP],DI
	dec	next_compressed_byte[BP]
	
;----------------------------------------------------------------------------
;------ each time around loop 3 another CR-LF pair is identified and the ----
;-------- data in the buffer is moved down to compress out the CRs -- set ---
;-------- up for LOOP4 as for LOOP2 above -- CX is the number of bytes left -
;-------- in the buffer, and AL is the CR we will be searching for ----------
;----------------------------------------------------------------------------

RL3:	mov	CX,DX
	sub	CX,DI
	mov	AL,0DH
	
;----------------------------------------------------------------------------
;------	loop 4 is identical to loop 2 above, and searches for the next ------
;-------- CR-LF pair, or a single CR terminating the data -- it is ----------
;-------- duplicated because what we want to do coming out of it is much ----
;-------- different in each case -- it could be made into a subroutine, but -
;-------- this is the most time-critical part of the code (and it's not -----
;-------- very big) ---------------------------------------------------------
;----------------------------------------------------------------------------

RL4:	repnz	SCASB
    	jnz	RX3A			; jump if no potential CR-LFs found
	cmp	DI,DX
	jae	RS7			; jump if CR is last char of buffer
	cmp	byte ptr ES:[DI],LF
	jnz	RL4			; fall through if CR-LF pair found
	
;----------------------------------------------------------------------------
;------	we have found another CR-LF pair and are ready to move a piece -- ---
;-------- the piece begins at SI (a LF character), and ends just before the -
;-------- CR at DI-1, so its length is given by the following calculation ---
;-------- into CX: ----------------------------------------------------------
;----------------------------------------------------------------------------

	mov	CX,DI
	dec	CX
	sub	CX,SI
	
;----------------------------------------------------------------------------
;------	now we use movsw to copy the piece -- the source of the move is SI --
;-------- (not coincidentally), and the destination is next_compressed_byte -
;-------- CX has the proper length -- misaligned destination operands pay a -
;-------- penalty in execution speed, so if the average move size is large --
;-------- enough (in this case about 10 bytes) it pays to check for and -----
;-------- correct the mis-alignment -----------------------------------------
;----------------------------------------------------------------------------

	mov	AX,DI			; save DI -- we're not done w/it yet
	
	mov	DI,next_compressed_byte[BP]
	test	DI,1
	jz	RS24			; jump if dest aligned on word bound
	
	sub	CX,1
	jc	RS8			; jump if CX was zero
	movsb
	
RS24:	shr	CX,1
	rep	movsw
	jnc	RS8
	movsb
RS8:	mov	next_compressed_byte[BP],DI
	mov	DI,AX			; restore DI
	
;----------------------------------------------------------------------------
;------	after the move above, SI points to the CR of our CR-LF pair -- we ---
;-------- increment SI now so that the CR will be squeezed out --------------
;----------------------------------------------------------------------------

	inc	SI
	
;----------------------------------------------------------------------------
;------	and go back to look for another CR-LF -------------------------------	
;----------------------------------------------------------------------------

	jmp	RL3

;----------------------------------------------------------------------------
;------ some jumps to help reach the various far reaches of this loop -------
;----------------------------------------------------------------------------
	
RX3A:	jmp	RX3	
RX1A:	jmp	RX1
RE15A:	jmp	RE15
	
;----------------------------------------------------------------------------
;------ we come here when we discover an EOF (^Z) character in the data -- --
;-------- this indicates EOF, even for console input -- DI points just past -
;-------- the EOF marker, so subtracting one will back it up to point -------
;-------- exactly to the EOF ------------------------------------------------
;----------------------------------------------------------------------------

RR6:	dec	di

;----------------------------------------------------------------------------
;------ we can't do an lseek on the console, so test for it and skip if so --
;----------------------------------------------------------------------------

	test	fhnd[BP],FHND_DEVICE
	jnz	RS19
	
;----------------------------------------------------------------------------
;------	for data files, back up the file pointer to the EOF character -- ----
;-------- this will allow the user to overwrite the EOF and extend the file -
;-------- DI points to the EOF marker here and DX just beyond the last good -
;-------- data byte in the buffer -- subtract DI from DX to get the number --
;-------- of bytes following the EOF marker -- this is the number of bytes --
;-------- to back up --------------------------------------------------------
;----------------------------------------------------------------------------

	mov	AX,DOS_SEEK_CUR
	sub	DX,DI
	neg	DX
	mov	CX,-1			; to sign-extend CX:DX
	bdos
	jc	RE15
	
;----------------------------------------------------------------------------
;------	indicate EOF -- update DX so it points to the EOF byte, which is ----
;-------- now the first byte after the valid data in the buffer; and return -
;-------- to join the processing of the line above --------------------------
;----------------------------------------------------------------------------

RS19:	or	fhnd[BP],FHND_EOF
	mov	DX,DI
	jmp	RJ6
	
;----------------------------------------------------------------------------
;------ when the value in BX is out of range, or when any DOS call returns --
;-------- with an unexpected error, we set errno to EBADF and return -1 -----
;----------------------------------------------------------------------------

RE15:	mov     AX,EBADF
        push    AX
        call    __SET_ERRNO
	mov	AX,-1
	jmp	RX0
	
;----------------------------------------------------------------------------
;------ we come here when the last character in the buffer is CR -- we need -
;-------- to peek at the next character in the file to see if it is a line --
;-------- feed -- if so, we compress the CR by replacing it with a line -----
;-------- feed and leaving the file pointer pointing after the LF character -
;-------- -- if the next character is not a line feed, we leave the CR in ---
;-------- the buffer and back up the file pointer so it will read the -------
;-------- character again ---------------------------------------------------
;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
;------	if we are at EOF, the CR is not followed by LF -- leave it in place -
;----------------------------------------------------------------------------

RS7:	test	fhnd[BP],FHND_EOF
	jnz	RX3
	
;----------------------------------------------------------------------------	
;------	read the next file character over the CR in the last position of ----
;-------- the buffer --------------------------------------------------------
;----------------------------------------------------------------------------	

	dec	DX
	mov	CX,1
	
;----------------------------------------------------------------------------
;------ under WINDOWS, all console reads (fd < 5) must be passed to WINC -- -
;-------- jump to the WINC code if appropriate ------------------------------
;----------------------------------------------------------------------------

    ifdef _WINDOWS
    	cmp	BX,5
    	jb	RS22
    endif	
    
;----------------------------------------------------------------------------
;------ call the operating system to do the read and then check for errors --
;----------------------------------------------------------------------------

	bdos	DOS_READ
RS23:	
	jc	RE15			; access denied or invalid handle

;----------------------------------------------------------------------------
;------	if there was no character to read, we were at EOF and didn't know ---
;-------- it -- jump to set the eof flags -----------------------------------
;----------------------------------------------------------------------------

	cmp	AX,0
	je	RS10			; jump if now at EOF
	
;----------------------------------------------------------------------------
;------	if the character is a line feed, we are done -- it has now replaced -
;-------- the CR in the buffer and the file pointer is ready to read the ----
;-------- next character after the LF ---------------------------------------
;----------------------------------------------------------------------------

	cmp	BYTE PTR -1[DI],0AH
	je	RX3
	
;----------------------------------------------------------------------------
;------	otherwise, the next character was not a LF -- we must first replace -
;-------- the CR in the data buffer: ----------------------------------------
;----------------------------------------------------------------------------
	
	mov	byte ptr -1[DI],0DH
	
;----------------------------------------------------------------------------
;------ now back up so that the next read will get this character again -----
;----------------------------------------------------------------------------

	mov	AX,DOS_SEEK_CUR
	mov	CX,-1
	mov	DX,CX
	bdos
	jc	RE15

;----------------------------------------------------------------------------
;------ and jump to use the buffer with the CR at the end -------------------
;----------------------------------------------------------------------------
	
	jmp	RX3
	
;----------------------------------------------------------------------------
;------	if we found eof while trying to peek ahead past a CR, set the EOF ---
;-------- flags (see the set_eof routine below for more information about ---
;-------- these flags), and use the buffer with the CR at the end -----------
;----------------------------------------------------------------------------
	
RS10:	or	fhnd[BP],FHND_EOF
	
;----------------------------------------------------------------------------
;------	we come here when all CR-LF pairs have been located -- if SI is -----
;-------- still 0, it means that we found no CR-LFs and no compression ------
;-------- needs to be done -- return right away -----------------------------
;----------------------------------------------------------------------------

RX3:	cmp	SI,0
	je	RX1
	
;----------------------------------------------------------------------------
;------ we found at least one CR-LF pair, so we need to compress the last ---
;-------- piece of the buffer -- the source of the move is SI and the -------
;-------- destination is next_compressed_byte -- CX is set to the number of -
;-------- bytes from SI to the end of the buffer ----------------------------
;----------------------------------------------------------------------------
	
	mov	CX,DI
	sub	CX,SI
	mov	DI,next_compressed_byte[BP]
	
	test	DI,1
	jz	RS25			; jump if dest aligned on word bound
	
	sub	CX,1
	jc	RS11			; jump if CX was zero
	movsb

RS25:	shr	CX,1
	rep	movsw
	jnc	RS11
	movsb
RS11:

;----------------------------------------------------------------------------
;------ for console input, we are done (since console input is always one ---
;-------- and only one line of input -- test for this condition -------------
;----------------------------------------------------------------------------

	test	fhnd[BP],FHND_DEVICE
	jnz	RX1
	
;----------------------------------------------------------------------------
;------	calculate the number of bytes remaining in the buffer -- if there ---
;-------- are none, we are done ---------------------------------------------
;----------------------------------------------------------------------------

	mov	CX,SI
	sub	CX,DI
	jz	RX1
	
;----------------------------------------------------------------------------
;------ unless we have detected the end of the file, return to the top of ---
;-------- the loop to try and fill the buffer out -- DI and CX are used to --
;-------- pass the location and length of the unused tail of the buffer to --
;-------- the code at the top of the loop -----------------------------------
;----------------------------------------------------------------------------

	test	fhnd[BP],FHND_EOF
	jz	RL1A
	
;----------------------------------------------------------------------------
;------	return the number of characters in the buffer -----------------------
;----------------------------------------------------------------------------
	
RX1:	mov	AX,DI
	sub	AX,buffer[BP]
	
;----------------------------------------------------------------------------
;------	restore the registers, clear the stack frame and return -------------
;----------------------------------------------------------------------------

RX0:    pop	DS
	pop	DI
	pop	SI
	mov	SP,BP
	WINLEAVE
	ret
	
;----------------------------------------------------------------------------
;------ pass console reads under WINDOWS to WINC ----------------------------
;----------------------------------------------------------------------------

    ifdef _WINDOWS
RS22:	pop     AX  ;; Get DS
        push    AX  ;; Put DS back onto stack
	push	ES
	;;mov	AX,seg ___winc_bdos
	mov	ES,AX
	mov	AH,DOS_READ
	call	ES:___winc_bdos
	pop	ES
	jmp	RS23
    endif

;----------------------------------------------------------------------------
;------ some help in getting from the bottom of the loop to the top ---------
;----------------------------------------------------------------------------
RL1A:	jmp	RL1

c_endp	read

    if SPTR
    
;****************************************************************************
; int __clib _readx (int fd, void __near *buffer, unsigned len, 
;                    unsigned segment)
; 
; The _readx function, available only in the near data models, accepts a
; segment address for the buffer as well as a near pointer.  The pointer and
; segment address are combined to locate the buffer.  In all other respects,
; _readx is identical to read.
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

func	_readx
	WINENTER
	
;----------------------------------------------------------------------------
;------	set the first word of local storage equal to the selector of the ----
;-------- user's data buffer -- it will be referenced here by the main ------
;-------- write routine above -----------------------------------------------
;----------------------------------------------------------------------------

	mov	ES,sel[BP]
	push	ES
    
;----------------------------------------------------------------------------
;------	jump to join read code ----------------------------------------------
;----------------------------------------------------------------------------

	jmp	RJ0
c_endp	_readx

    endif ; SPTR
   
;****************************************************************************
; local_bdos
; 
; parameters and return values exactly match DOS INT 21H
; 
; This default bdos routine is replaced when running under WINC to allow
; filtering and emulation of console reads and writes.
;****************************************************************************
local_bdos proc far
	int	21H
	ret
local_bdos endp

	endcode read
	end
