;_ read.asm
; Copyright 1993 by Digital Mars
; All Rights Reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	include stdio.asm

	extrn __SET_ERRNO:near
        
    ifdef _WIN32
    extrn _ReadFile@20:near
	extrn _SetFilePointer@16:near
	comm  __osfhnd:dword:0F0h
	comm  ___fhnd_info:byte:3Ch
    else
        extrn  ___fhnd_info:byte
    endif        

	begcode read
	c_public read

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

next_compressed_byte = dword ptr -4
    ifdef _WIN32
actual_bytes_read    = dword ptr -8    
localbytes = 8
    else
localbytes = 4
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

EOF_MARKER   = 1AH
CR	     = 0DH
LF	     = 0AH
DOS_SEEK_CUR = 4201H
FILE_CURRENT = 01H
DOS_READ     = 3FH
FHND_APPEND  = 04H
FHND_DEVICE  = 08H
FHND_EOF     = 01H
FHND_TEXT    = 10H

;----------------------------------------------------------------------------
;------ set up the stack frame ----------------------------------------------
;----------------------------------------------------------------------------

func	read
	push	EBP
	mov	EBP,ESP
	sub	ESP,localbytes
	push	EBX
	
;----------------------------------------------------------------------------
;------ load and validate the file handle to EBX ----------------------------
;----------------------------------------------------------------------------

	mov	EBX,fd[EBP]
	cmp	EBX,_NFILE	; number of handles supported
	jnb	short RE12
	
;----------------------------------------------------------------------------
;------ ECX holds the number of bytes to read -- if it's zero, we're done ---
;----------------------------------------------------------------------------
	
	mov	ECX,len[EBP]
	or	ECX,ECX
	je	short RS14
	
;----------------------------------------------------------------------------
;------- if the file is text-mode, it requires extra processing -------------
;----------------------------------------------------------------------------

	test	___fhnd_info[EBX],FHND_TEXT
	jnz	short RS9		; jump if text mode

;----------------------------------------------------------------------------
;------ set up the buffer pointer for the read ------------------------------
;----------------------------------------------------------------------------

	mov	EDX,buffer[EBP]

;----------------------------------------------------------------------------
;------ call the operating system to do the read -- check for error ---------
;----------------------------------------------------------------------------

        push	ECX
        push	EDX
    ifdef _WIN32
	push	0			; lpOverlapped
	lea	EAX,actual_bytes_read[EBP]
	push	EAX			; lpNumberOfBytesRead
	push	ECX			; nNumberOfBytesToRead
	push	EDX			; lpBuffer
	push	dword ptr __osfhnd[4*EBX] ;hFile
	call	_ReadFile@20
	pop	EDX
	pop	ECX
	or	EAX,EAX
	jz	short RE12
	mov	EAX,actual_bytes_read[EBP]
    else
	bdos	DOS_READ
	pop	EDX
	pop	ECX
	jc	short RE12
    endif	
    	
;----------------------------------------------------------------------------
;------ and return ----------------------------------------------------------
;----------------------------------------------------------------------------
RS13:
	pop	EBX
	mov	ESP,EBP
	pop	EBP
	ret
	
;----------------------------------------------------------------------------
;------ we come here if ECX is zero:  just return a zero --------------------
;----------------------------------------------------------------------------
	
RS14:	xor	EAX,EAX
	jmp	short RS13
	
;----------------------------------------------------------------------------
;------ set the error code and return a -1 ----------------------------------
;----------------------------------------------------------------------------

RE12:   push    dword ptr EBADF
        call    __SET_ERRNO
	mov	EAX,-1
	jmp	short RS13

;----------------------------------------------------------------------------
;------ save ESI and EDI for the text mode conversion -----------------------
;----------------------------------------------------------------------------

RS9:	push	ESI
	push	EDI
	
;----------------------------------------------------------------------------
;------	EDI points to the beginning of the buffer -- it will be used scan ---
;-------- the buffer for EOF and CR/LF pairs --------------------------------
;----------------------------------------------------------------------------

	mov	EDI,buffer[EBP]
    
;----------------------------------------------------------------------------
;------ turn off the EOF flag -----------------------------------------------
;----------------------------------------------------------------------------

	and	___fhnd_info[EBX],not FHND_EOF

;----------------------------------------------------------------------------
;------ we will do all string operations in the UP direction -- set ES ------
;-------- equal to DS for string operations ---------------------------------
;----------------------------------------------------------------------------

    	cld
    	
    	push	DS
    	pop	ES
    	
;----------------------------------------------------------------------------
;------ each time around loop 1 we will fill the buffer up completely -- ----
;-------- if we convert one or more CR-LF pairs to CRs there will be more ---
;-------- room in the buffer and we will come back to the top of LOOP1 to ---
;-------- fill it out with a few more characters from the file, go on to ----
;-------- strip out any additional CR-LF pairs that were read in, and so ----
;-------- forth -- at the top of this loop, ES:DI points to the first empty -
;-------- character of the buffer, and ECX holds the number of empty spaces -
;-------- remaining ---------------------------------------------------------
;----------------------------------------------------------------------------

RL1:	
;----------------------------------------------------------------------------
;------ call the operating system to do the read and then check for errors --
;----------------------------------------------------------------------------

	mov	EDX,EDI
	
        push	ECX
        push	EDX
    ifdef _WIN32
    push	0			; lpOverlapped
	lea	EAX,actual_bytes_read[EBP]
	push	EAX			; lpNumberOfBytesRead
	push	ECX			; nNumberOfBytesToRead
	push	EDX			; lpBuffer
	push	dword ptr __osfhnd[4*EBX] ; hFile
	call	_ReadFile@20
	pop	EDX
	pop	ECX
	or	EAX,EAX
	jz	RE15
	mov	EAX,actual_bytes_read[EBP]
    else
	bdos	DOS_READ
	pop	EDX
	pop	ECX
	jc	RE15
    endif	

;----------------------------------------------------------------------------
;------ set EDX to point to the first byte past the good data ---------------
;----------------------------------------------------------------------------	

	add	EDX,EAX

;----------------------------------------------------------------------------
;------ we are at EOF if less bytes come back than we asked for (or we may --
;-------- be reading from the console, but either way we don't want to go ---
;-------- back to the file for more data) -----------------------------------
;----------------------------------------------------------------------------

	cmp	EAX,ECX
	je	short RS16		; jump if we got all the bytes

;----------------------------------------------------------------------------
;------ indicate EOF and set ECX to the actual number of bytes read ---------
;----------------------------------------------------------------------------

	or	___fhnd_info[EBX],FHND_EOF
	mov	ECX,EAX
RS16:
;----------------------------------------------------------------------------
;------ if we didn't read anything, we are done -----------------------------
;----------------------------------------------------------------------------

	or	ECX,ECX
	jz	RX1			; jump if no data came back

;----------------------------------------------------------------------------
;------	scan the data just read in (EDI for ECX bytes) for an EOF (^Z) ------
;-------- byte -- don't lose EDI, it's the only pointer to the data we have -
;----------------------------------------------------------------------------

	push	EDI
	mov	AL,EOF_MARKER
	repnz	SCASB
	jz	short RR6		; jump if EOF found
RJ6:	pop	EDI

;----------------------------------------------------------------------------
;------	set up for the loop below -- set ECX to the number of bytes in the --
;-------- buffer, AL to the character we will search for (CR) ---------------
;----------------------------------------------------------------------------

 	mov	ECX,EDX
	sub	ECX,EDI
	mov	AL,CR
	
;----------------------------------------------------------------------------
;------	we set ESI to 0 to indicate that we have not yet found a CR-LF pair -
;----------------------------------------------------------------------------

	xor	ESI,ESI
	
;----------------------------------------------------------------------------
;------ loop 2 looks for the first CR-LF pair -- if none is found, there ----
;-------- may still be a CR in the final position of the buffer -- if so, ---
;-------- we will need to read ahead in the file to see if it is followed ---
;-------- by a CR -- if neither of these conditions occurs, we are ready to -
;-------- return ------------------------------------------------------------
;----------------------------------------------------------------------------

RL2: 	repnz	SCASB
    	jnz	RX1			; jump if no potential CR-LFs found
	cmp	EDI,EDX
	jae	RS7			; jump if CR is last char of buffer
	cmp	byte ptr [EDI],LF
	jnz	short RL2		; fall through if CR-LF pair found
	
;----------------------------------------------------------------------------
;------	we have found a CR-LF pair and will need to compress the buffer -- --
;-------- set next_compressed_byte pointing to the CR which will later be ---
;-------- overwritten and ESI to the LF which is the beginning of the -------
;-------- string which will overwrite it ------------------------------------
;----------------------------------------------------------------------------

	mov	ESI,EDI
	mov	next_compressed_byte[EBP],EDI
	dec	next_compressed_byte[EBP]
	
;----------------------------------------------------------------------------
;------ each time around loop 3 another CR-LF pair is identified and the ----
;-------- data in the buffer is moved down to compress out the CRs -- set ---
;-------- up for LOOP4 as for LOOP2 above -- ECX is the number of bytes -----
;-------- left in the buffer, and AL is the CR we will be searching for -----
;----------------------------------------------------------------------------

RL3:	mov	ECX,EDX
	sub	ECX,EDI
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
    	jnz	RX3			; jump if no potential CR-LFs found
	cmp	EDI,EDX
	jae	short RS7		; jump if CR is last char of buffer
	cmp	byte ptr [EDI],LF
	jnz	short RL4		; fall through if CR-LF pair found
	
;----------------------------------------------------------------------------
;------	we have found another CR-LF pair and are ready to move a piece -- ---
;-------- the piece begins at ESI (a LF character), and ends just before ----
;-------- the CR at EDI-1, so its length is given by the following ----------
;-------- calculation into ECX: ---------------------------------------------
;----------------------------------------------------------------------------

	mov	ECX,EDI
	dec	ECX
	sub	ECX,ESI
	
;----------------------------------------------------------------------------
;------	now we use movsd to copy the piece -- the source of the move is ESI -
;-------- (not coincidentally), and the destination is next_compressed_byte -
;-------- ECX has the proper length -----------------------------------------
;----------------------------------------------------------------------------

	mov	EAX,EDI			; save EDI -- we're not done w/it yet
	
	mov	EDI,next_compressed_byte[EBP]
	
	push	ECX
	shr	ECX,2
	rep	movsd
	pop	ECX
	and	ECX,3
	rep	movsb
	
	mov	next_compressed_byte[EBP],EDI
	
	mov	EDI,EAX			; restore EDI
	
;----------------------------------------------------------------------------
;------	after the move above, ESI points to the CR of our CR-LF pair -- we --
;-------- increment ESI now so that the CR will be squeezed out -------------
;----------------------------------------------------------------------------

	inc	ESI
	
;----------------------------------------------------------------------------
;------	and go back to look for another CR-LF -------------------------------	
;----------------------------------------------------------------------------

	jmp	short RL3

;----------------------------------------------------------------------------
;------ we come here when we discover an EOF (^Z) character in the data -- --
;-------- this indicates EOF, even for console input -- EDI points just -----
;-------- past the EOF marker, so subtracting one will back it up to point --
;-------- exactly to the EOF ------------------------------------------------
;----------------------------------------------------------------------------

RR6:	dec	EDI

;----------------------------------------------------------------------------
;------ we can't do an lseek on the console, so test for it and skip if so --
;----------------------------------------------------------------------------

	test	___fhnd_info[EBX],FHND_DEVICE
	jnz	short RS19
	
;----------------------------------------------------------------------------
;------	for data files, back up the file pointer to the EOF character -- ----
;-------- this will allow the user to overwrite the EOF and extend the file -
;-------- EDI points to the EOF marker here and EDX just beyond the last ----
;-------- good data byte in the buffer -- subtract EDI from EDX to get the --
;-------- number of bytes following the EOF marker -- this is the number of -
;-------- bytes to back up --------------------------------------------------
;----------------------------------------------------------------------------

	sub	EDX,EDI
	
    ifdef _WIN32
	push	FILE_CURRENT		; dwMoveMethod
	push	0			; lpDistanceToMoveHigh
        push	EDX			; lDistanceToMove
        push	dword ptr __osfhnd[4*EBX] ; hFile
	call	_SetFilePointer@16
	cmp	AX,-1
	je	short RE15
    else
	neg	EDX
	mov	ECX,-1			; to sign-extend CX:DX
	mov	EAX,DOS_SEEK_CUR
	push	ECX
	push	EDX
	bdos
	pop	EDX
	pop	ECX
	jc	short RE15
    endif	
	
;----------------------------------------------------------------------------
;------	indicate EOF -- update EDX so it points to the EOF byte, which is ---
;-------- now the first byte after the valid data in the buffer; and return -
;-------- to join the processing of the line above --------------------------
;----------------------------------------------------------------------------

RS19:	or	___fhnd_info[EBX],FHND_EOF
	mov	EDX,EDI
	jmp	RJ6
	
;----------------------------------------------------------------------------
;------ when the value in EBX is out of range, or when any DOS call returns -
;-------- with an unexpected error, we set errno to EBADF and return -1 -----
;----------------------------------------------------------------------------

RE15:   push    dword ptr EBADF
        call    __SET_ERRNO
	mov	EAX,-1
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

RS7:	test	___fhnd_info[EBX],FHND_EOF
	jnz	short RX3 
	
;----------------------------------------------------------------------------	
;------	read the next file character over the CR in the last position of ----
;-------- the buffer --------------------------------------------------------
;----------------------------------------------------------------------------	

	dec	EDX
	
;----------------------------------------------------------------------------
;------ call the operating system to do the read and then check for errors --
;----------------------------------------------------------------------------

	mov	ECX,1
	
        push	ECX
        push	EDX
    ifdef _WIN32
	push	0			; lpOverlapped
	lea	EAX,actual_bytes_read[EBP]
	push	EAX			; lpNumberOfBytesRead
	push	ECX			; nNumberOfBytesToRead
	push	EDX			; lpBuffer
	push	dword ptr __osfhnd[4*EBX] ; hFile
	call	_ReadFile@20
	pop	EDX
	pop	ECX
	or	EAX,EAX
	jz	short RE15
	mov	EAX,actual_bytes_read[EBP]
    else
	bdos	DOS_READ
	pop	EDX
	pop	ECX
	jc	short RE15
    endif	

;----------------------------------------------------------------------------
;------	if there was no character to read, we were at EOF and didn't know ---
;-------- it -- jump to set the eof flags -----------------------------------
;----------------------------------------------------------------------------

	or	EAX,EAX
	jz	short RS10		; jump if now at EOF
	
;----------------------------------------------------------------------------
;------	if the character is a line feed, we are done -- it has now replaced -
;-------- the CR in the buffer and the file pointer is ready to read the ----
;-------- next character after the LF ---------------------------------------
;----------------------------------------------------------------------------

	cmp	BYTE PTR -1[EDI],0AH
	je	short RX3
	
;----------------------------------------------------------------------------
;------	otherwise, the next character was not a LF -- we must first replace -
;-------- the CR in the data buffer: ----------------------------------------
;----------------------------------------------------------------------------
	
	mov	byte ptr -1[EDI],0DH
	
;----------------------------------------------------------------------------
;------ now back up so that the next read will get this character again -----
;----------------------------------------------------------------------------

    ifdef _WIN32
	push	FILE_CURRENT		; dwMoveMethod
	push	0			; lpDistanceToMoveHigh
        push	-1			; lDistanceToMove
        push	dword ptr __osfhnd[4*EBX] ; hFile
	call	_SetFilePointer@16
	cmp	AX,-1
	je	short RE15
    else
	mov	ECX,-1
	mov	EDX,ECX			; to sign-extend CX:DX
	mov	EAX,DOS_SEEK_CUR
	push	ECX
	push	EDX
	bdos
	pop	EDX
	pop	ECX
	jc	short RE15
    endif	

;----------------------------------------------------------------------------
;------ and jump to use the buffer with the CR at the end -------------------
;----------------------------------------------------------------------------
	
	jmp	short RX3
	
;----------------------------------------------------------------------------
;------	if we found eof while trying to peek ahead past a CR, set the EOF ---
;-------- flags (see the set_eof routine below for more information about ---
;-------- these flags), and use the buffer with the CR at the end -----------
;----------------------------------------------------------------------------
	
RS10:	or	___fhnd_info[EBX],FHND_EOF
	
;----------------------------------------------------------------------------
;------	we come here when all CR-LF pairs have been located -- if ESI is ----
;-------- still 0, it means that we found no CR-LFs and no compression ------
;-------- needs to be done -- return right away -----------------------------
;----------------------------------------------------------------------------

RX3:	cmp	ESI,0
	je	short RX1
	
;----------------------------------------------------------------------------
;------ we found at least one CR-LF pair, so we need to compress the last ---
;-------- piece of the buffer -- the source of the move is ESI and the ------
;-------- destination is next_compressed_byte -- eCX is set to the number ---
;-------- of bytes from ESI to the end of the buffer ------------------------
;----------------------------------------------------------------------------
	
	mov	ECX,EDI
	sub	ECX,ESI
	mov	EDI,next_compressed_byte[EBP]
	
	mov	EAX,ECX
	shr	ECX,2
	rep	movsd
	mov	ECX,EAX
	and	ECX,3
	rep	movsb

;----------------------------------------------------------------------------
;------ for console input, we are done (since console input is always one ---
;-------- and only one line of input -- test for this condition -------------
;----------------------------------------------------------------------------

	test	___fhnd_info[EBX],FHND_DEVICE
	jnz	short RX1
	
;----------------------------------------------------------------------------
;------	calculate the number of bytes remaining in the buffer -- if there ---
;-------- are none, we are done ---------------------------------------------
;----------------------------------------------------------------------------

	mov	ECX,ESI
	sub	ECX,EDI
	jz	short RX1
	
;----------------------------------------------------------------------------
;------ unless we have detected the end of the file, return to the top of ---
;-------- the loop to try and fill the buffer out -- EDI and ECX are used ---
;-------- to pass the location and length of the unused tail of the buffer --
;-------- to the code at the top of the loop --------------------------------
;----------------------------------------------------------------------------

	test	___fhnd_info[EBX],FHND_EOF
	jz	RL1
	
;----------------------------------------------------------------------------
;------	return the number of characters in the buffer -----------------------
;----------------------------------------------------------------------------
	
RX1:	mov	EAX,EDI
	sub	EAX,buffer[EBP]
	
;----------------------------------------------------------------------------
;------	restore the registers, clear the stack frame and return -------------
;----------------------------------------------------------------------------

RX0:	pop	EDI
	pop	ESI
	pop	EBX
	mov	ESP,EBP
	pop	EBP
	ret
	
c_endp	read

	endcode read
	end
