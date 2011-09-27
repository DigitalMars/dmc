# CT.MAK 
# Copyright (C) 1995 by Digital Mars
# All Rights Reserved
# makefile for CT.OBJ included as part of SDS.PRJ 
# $Revision: 1.1.1.1 $

D = SDS
AFLAGS  = -D_MBCS -DMSDOS=1 -I..\INCLUDE -I..\..\INCLUDE -DI8086T=1 -Mx   

ALL: create.dir  $D\CT.OBJ

create.dir:
	@-if not exist $D\*.* mkdir $D

$D\CT.OBJ : ..\DOS16\C.ASM
    MASM $(AFLAGS) ..\DOS16\C.ASM,$D\CT.OBJ;
    copy $D\CT.OBJ ..\..\LIB
    
clean:
    -del $D\CT.OBJ
    
    
