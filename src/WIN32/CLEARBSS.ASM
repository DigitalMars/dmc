;; clearbss.asm    
;; Copyright 1994 Digital Mars Corporation

	    DOSSEG

include macros.asm
	   
       extrn __edata:byte   ;; Points at first byte in _BSS
       extrn __end:byte     ;; Points at first byte following _BSS


	   public __clearbss

	begdata
__clearbss_called dd 0
    enddata

       begcode CLEARBSS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  void _clearbss(void)
;
;     This routine will grab the base of bss
;   and get the size and zero out the entire segment.
;   This is to work around a Chicago problem.  

__clearbss  proc near
            push EDI   ;; Save EDI
            push ECX   ;; Save ECX
            mov EAX,__clearbss_called
            or EAX,EAX
            jnz __noclear ; If already cleared, don't do it again
            mov EDI, offset FLAT:__edata  ;; Grab  _BSS
            mov ECX, offset FLAT:__end    ;; Grab  end of _BSS
            sub ECX,EDI  ;; ENDBSS - _BSS == Size of BSS
            jcxz  __noclear       ;; If segment is 0, nothing to do 
            xor EAX,EAX    
	        cld           ; Watch out for direction flag errors.
            rep stosb     ; clear it out
            mov ECX, 1    ; set the flag
            mov __clearbss_called, ECX
__noclear:            
            pop ECX
            pop EDI
            ret
__clearbss  endp
           endcode CLEARBSS
           end



