; Copyright (C) 1994 by Digital Mars
; All Rights Reserved

include	macros.asm

begdata
  if LPTR	
    c_public _baslnk
    __baslnk		dw	offset DGROUP:__baslnk
			dw	seg DGROUP:__baslnk
			dw	0
  else
    ifndef _WINDOWS
      c_public _baslnk
      __baslnk		dw	offset DGROUP:__baslnk	;starting link for storage allocator
			dw	0	;give it a size of 0 so it is never allocated
    endif	
  endif	
enddata

end
