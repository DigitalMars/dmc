;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; We don't support 8087 emulators, so if you are
; linking in code that has the emulator fixups, these
; definitions will turn them into inline 8087 code.

	public	FIARQQ,FJARQQ,FICRQQ,FJCRQQ
	public	FIDRQQ,FIERQQ,FISRQQ,FJSRQQ
	public	FIWRQQ

FIARQQ	=	0
FJARQQ	=	0
FICRQQ	=	0
FJCRQQ	=	0
FIDRQQ	=	0
FIERQQ	=	0
FISRQQ	=	0
FJSRQQ	=	0
FIWRQQ	=	0

	end
