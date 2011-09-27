; Copyright Digital Mars 1993
; $Revision: 1.1.1.1 $

          .386
          public  _coreleft
_TEXT     segment public 'CODE'
          assume  CS:_TEXT
          extrn   __x386_coreleft:near
_coreleft proc    near
	  jmp	  __x386_coreleft
_coreleft endp
_TEXT     ends
end

