# SWAPL.MAK 
# Copyright (C) 1995 by Digital Mars
# All Rights Reserved
# makefile _SWAPL.OBJ included as part of SDL.PRJ 
# $Revision: 1.1.1.1 $

D = SDL
CFLAGS  = -w12 -wx -Ae -Ar -c -o+time -ml -DMSDOS  -r -Nc -NL -I..\..\INCLUDE -I..\CORE16  

ALL: create.dir  $D\_SWAPL.OBJ

create.dir:
	@-if not exist $D\*.* mkdir $D

$D\_SWAPL.OBJ : ..\DOS16\SWAP.ASM
    sc $(CFLAGS) ..\DOS16\SWAP.ASM -o$D\_SWAPL.OBJ
    copy $D\_SWAPL.OBJ ..\..\LIB
    
clean:
    -del $D\_SWAPL.OBJ
    
    
