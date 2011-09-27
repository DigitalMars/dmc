# SWAPC.MAK 
# Copyright (C) 1995 by Digital Mars
# All Rights Reserved
# makefile for _SWAPC.OBJ to be included as part of SDC.PRJ 
# $Revision: 1.1.1.1 $

D = SDC
CFLAGS  = -w12 -wx -Ae -Ar -c -o+time -mc -DMSDOS  -r -Nc -NL -I..\..\INCLUDE -I..\CORE16  

ALL: create.dir $D\_SWAPC.OBJ

create.dir:
	@-if not exist $D\*.* mkdir $D

$D\_SWAPC.OBJ : ..\DOS16\SWAP.ASM
    sc $(CFLAGS) ..\DOS16\SWAP.ASM -o$D\_SWAPC.OBJ
    copy $D\_SWAPC.OBJ ..\..\LIB
    
clean:
    -del $D\_SWAPC.OBJ
    
    
