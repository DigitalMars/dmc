; Copyright (C) 1995 by Digital Mars
; All Rights Reserved

ifdef _WIN32
include macros.asm

 	begdata
	public	__tls_array
__tls_array	equ	2ch
	enddata

.tls segment word public 'tls'
     public __tlsstart
__tlsstart dd offset FLAT:__tlsstart
.tls ends

.tls$ZZZ segment word public 'tls'
    public __tlsend
__tlsend dd offset FLAT:__tlsend
.tls$ZZZ ends

 endif
	end


