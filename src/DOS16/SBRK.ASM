;_ sbrk.asm   Mon Jun 18 1990	Modified by: Walter Bright
; OS2 support added by Nikki Locke May 1989		-D__OS2__
; Copyright (C) 1985-1991 by Walter Bright
; All rights reserved
; Written by Walter Bright
; $Revision: 1.1.1.1 $

include	macros.asm
include stdio.asm

    ifdef __OS2__
extrn	DOSALLOCSEG:far
extrn	DOSREALLOCSEG:far
    endif

; Storage allocator

    if LPTR
    if LCODE
        extrn _dos_avail:far
    else    
        extrn _dos_avail:near
    endif     
    endif

	begdata

	c_extrn	_psp,word
    ifndef DOS16RM
	c_extrn	_datapar,word
	c_extrn	_pastdata,word
    if SPTR
	c_extrn	_progpar,word
    else
	c_extrn	_totalpar,word
    endif
    endif

    if LPTR
fardata	dw	0
farsize	dw	0
    endif

	enddata

	begcode	sbrk

	c_public sbrk

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Request memory from operating system.
; Attempt to grow the data segment.
; Use:
;	p = sbrk(nbytes);
; Returns:
;	pointer to memory allocated
;	    (first word of allocated memory contains # of bytes allocated)
;	-1 if error

    ifdef DOS16RM
func	sbrk
	push	BP
	mov	BP,SP
	mov	AX,P[BP]		;get nbytes
	add	AX,15			;round
	and	AX,0FFF0h		;AX = # of bytes to allocate
	jz	sbrk3			;error if sbrk(0)
	mov	DX,AX			;save

	mov	CX, seg DGROUP
	lsl	BX, CX			;Get length of current DGROUP

	mov	CX,AX			;# of bytes to allocate
	add	cx, bx			;Can we enlarge it enough?
	jc	newseg			;No - must allocate a new segment.
	inc	CX			;'cuz BX is a segment limit, not size
	jz	newseg
	add	cx, 15
	shr	cx, 4
	jnz	pp1
	mov	CH, 10h			;grow default data seg to 64K
pp1:
	push	bx
	mov	bx, seg DGROUP
	mov	ES, bx			;Segment of start of DGROUP
	mov	bx, cx
	bdos	4Ah			;set new program size
	pop	BX			;get DGROUP segment limit
	jnc	sbrkok			;succeeded
	_if	AX ne 8, sbrk2	        ;if something is very wrong

	;Can't grow data segment. Try to allocate an independent block.
newseg:
	mov	CX,DX
	shr	CX,4
	mov	BX,CX			;BX = # of paragraphs req'd
	bdos	48h			;allocate memory
	jc	sbrk2			;failed
	clr	BX			;BX = offset of new block
sbrkok2:
	mov	ES,AX
	mov	ES:[BX],DX		;store size of new block
	mov	DX,AX
	mov	AX,BX
	pop	BP
	ret

sbrkok:
	inc	bx			;Change value from limit to offset.

	;Adjust number of bytes in DX to account for rounding
	;up DGROUP seg size to next paragraph. This must be right
	;or free() cannot coalesce adjacent blocks.
	mov	AL,BL
	neg	AL
	and	AX,0Fh
	add	DX,AX

	mov	ax, seg DGROUP		;Get segment of new data.
	jmp	sbrkok2
	
sbrk3:	;mov	AX,ENOMEM
sbrk2:	;errno_set
	mov	AX,-1
	cwd
	pop	BP
	ret
c_endp	sbrk

    else ; DOS16RM

func	sbrk
	push	BP
	mov	BP,SP
	mov	BX,P[BP]		;get nbytes
	add	BX,15			;round
	and	BX,0FFF0h		;BX = # of bytes to allocate
	jnz	sbr00
	jmp	sbrk3			;error if sbrk(0)
sbr00:	mov	DX,BX			;save
	mov	CL,4
	shr	BX,CL			;# of paragraphs to allocate
	mov	CX,BX			;save
    ifdef __OS2__
	mov	P[BP],DX		;save no of bytes to allocate
	add	BX,_datapar		;add in # already in data segment
	jc	sbradd			;more than 64k
	_if	BX a 0FFFH, sbradd	;more than 64k
	push	CX			;save no of paras to add
	mov	CL,4
	shl	BX,CL			;no of bytes
	push	BX			;new size requested
	push	DS			;current data segment
	call	DOSREALLOCSEG
	pop	CX			;restore no of paras added
	test	AX,AX
	jnz	sbradd			;cannot add to this segment
	mov	DX,P[BP]		;no of bytes allocated
	mov	AX,_pastdata		;pointer to allocated memory
	mov	BX,AX
	mov	[BX],DX			;store # of bytes allocated
	add	_pastdata,DX		;and remember for posterity
	add	_datapar,CX		;new size of data segment
    if LPTR
	mov	DX,DS			;segment selector
    endif
	jmp short sbrkok		;ok return

sbradd:
    if SPTR
	jmp short sbrk3			;out of memory
    else
	mov	AX,fardata
	test	AX,AX
	jz	sbrnew			;allocate a new segment
	mov	BX,farsize
	add	BX,P[BP]		;add requested size
	jc	sbrnew			;>=64k
	push	BX			;save size
	push	BX			;new size requested
	push	AX			;far data segment
	call	DOSREALLOCSEG
	pop	BX			;get back size
	test	AX,AX
	jnz	sbrnew			;cannot add to this segment
	mov	AX,farsize		;offset to new memory

	jmp short sbrlok

sbrnew:
	push	P[BP]
	push	DS
	pushi	AX,<offset DGROUP:fardata>	;address of where to put segment
	pushi	AX,0			;flag as non-shareable
	call	DOSALLOCSEG
	test	AX,AX
	jnz	sbrk2
	xor	AX,AX			;offset = 0 for new segment
	mov	BX,P[BP]		;no of bytes allocated
sbrlok:
	mov	CX,P[BP]		;no of bytes allocated
	mov	DX,fardata		;segment of memory block
	mov	farsize,BX		;replace old size with new
	mov	BX,AX
	mov	ES,DX
	mov	ES:[BX],CX		;store # of bytes allocated
    endif
sbrkok:

    else ;MSDOS
	;BX = CX = # of paragraphs to allocate
	;DX = number of bytes to allocate

    if SPTR
	add	BX,_datapar		;add in # already in data segment
	jc	sbrk3			;too much
	_if	BX a 0FFFh, sbrk3	;if > 64k
	add	BX,_progpar		;BX = total new size of program
    else
	add	BX,_totalpar		;BX = total new size of program
    endif
	push	ES
	mov	ES,_psp			;segment of start of program
	bdos	4Ah			;set new program size
	pop	ES

    if SPTR
	jc	sbrk2			;failed
	mov	AX,_pastdata		;pointer to allocated memory
	mov	BX,AX
	mov	[BX],DX			;store # of bytes allocated
	add	_pastdata,DX		;and remember for posterity
	add	_datapar,CX		;new size of data segment
    else
	jnc	sbrkok			;succeeded
	_if	AX ne 8, sbrk2	;if something is very wrong (DOS returns 8 for no memory)

	;Can't grow data segment. Try to allocate an independent block.
;	sub	BX,_totalpar		;BX = # of paragraphs req'd
	mov	BX,CX			;BX = # of paragraphs req'd
	bdos	48h			;allocate memory
	jc	sbrk2			;failed
	jmps	sbrk1			;success. AX = segment of new block
sbrkok:
	mov	AX,_psp
	add	AX,_totalpar		;AX = segment of new block
	add	_totalpar,CX		;new total size of program
sbrk1:
	clr	BX			;BX = offset of new block
	mov	ES,AX
	mov	ES:[BX],DX		;store size of new block
	mov	DX,AX
	mov	AX,BX
    endif
    endif ;__OS2__
	pop	BP
	ret

sbrk3:	;mov	AX,ENOMEM		;fake an ENOMEM
sbrk2:	;errno_set
	mov	AX,-1
    if LPTR
	cwd
    endif
	pop	BP
	ret
c_endp	sbrk
    endif ; DOS16RM
    
	c_public coreleft
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; How much memory is left to allocate?
; Use:
;   core = coreleft();
; Returns:
;   unsigned integer, number of bytes still available above highest
;      memory allocated so far
;
;	
func	coreleft	
    if LPTR
        call	_dos_avail
        or	dx,dx
        jz	cl1
        mov	ax,0FFFFh
cl1:        
        ret
    else
    ifdef DOS16RM
	mov	cx, seg DGROUP
	lsl	ax, cx			;Get length of current DGROUP
	inc	ax
	shr	ax, 4
    else
        mov	ax,_datapar		
        inc	ax
    endif
        neg	ax
        mov	cl, 4
        shl	ax, cl
        ret
    endif
        c_endp	coreleft
        endcode sbrk
	end
