;_ getcwd.asm   Wed Jul 12 1989   Modified by: Walter Bright */
; DOS386 support added by G. Eric Engstrom
; Copyright (C) 1985-1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

        include macros.asm
	include stdio.asm

extrn __SET_DOS_ERRNO:near
extrn __SET_ERRNO:near

        begdata
tmpbuf  db      ?,':','\'
        db      64 dup (?)
        enddata


        c_extrn         malloc,near

        begcode getcwd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get current working directory.
;       char *getcwd(char *pbuf,size_t Nbytes);
; Input:
;       pbuf    where to store the pathname
;       n       maximum length of pathname including terminating 0
; Returns:
;       if successful
;               pbuf
;       else
;               NULL
;               errno = ENOMEM (out of memory), ERANGE (pathname longer
;                       than n-1 characters)

        c_public        getcwd
	public		__getcwd

__getcwd proc
__getcwd endp

func    getcwd

        push    EBP
        mov     EBP,ESP
	uses	<ESI,EDI,ES>

        bdos    019h
        mov     DL,AL
        add     AL,'A'
        mov     tmpbuf,AL
        inc     DL              ;DL = drive number
        mov     ESI,offset DGROUP:tmpbuf+3
        bdos    47h             ;get current directory
	jc	short L3
        mov     ESI,offset DGROUP:tmpbuf

        ;CX = length of buffer required (strlen(tmpbuf) + 1)
        mov     EDI,ESI
        push    DS
        pop     ES
        mov     ECX,-1
	clr	EAX
        repne   scasb
        not     ECX

        _ifs    <dword ptr P[EBP]> ne 0, L33      ;if (pbuf != NULL)
	  push    ECX
	  _ifs    CX a <word ptr P+SIZEPTR[EBP]>, L4
	    movzx   ECX,word ptr P+SIZEPTR[EBP]
L4:       push    ECX
	  callm   malloc
	  add     ESP,4
	  pop     ECX
	  mov     EDI,EAX
	  tst     EAX
	  jz	  short Lenomem			;insufficient memory
	  jmp     short L5

L33:    _ifs    CX a <word ptr P+SIZEPTR[EBP]>, Lerange
        mov     EDI,P[EBP]
L5:
        push    DS
        pop     ES
        mov     EAX,EDI
        rep     movsb
L2:     unuse   <ES,EDI,ESI>
        pop     EBP
        ret

Lerange: push   dword ptr ERANGE
L41:    call    __SET_ERRNO
        jmp     L31
L3:	push    EAX
        call    __SET_DOS_ERRNO
        ;return (char *) NULL;
L31:    clr     EAX
        jmp     L2

Lenomem: push   dword ptr ENOMEM
        jmp     L41
c_endp  getcwd

        endcode getcwd

        end

