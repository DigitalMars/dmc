# DOSXOBJ.MAK 
# Copyright (C) 1995 by Digital Mars
# All Rights Reserved
# makefile for CX.OBJ and REALMODE.OBJ included as part of SDX.PRJ 
# $Revision: 1.1.1.1 $

D = SDX
CFLAGS  = -D_MBCS -DMSDOS=1 -I..\INCLUDE -mx -r -wx -w12 -Ae -Ar -c -o+time -Nc -NL -I..\..\INCLUDE   

ALL: create.dir  $D\CX.OBJ $D\REALMODE.OBJ COPYOBJ

copyobj:
	copy $D\CX.OBJ ..\..\LIB
	copy $D\REALMODE.OBJ ..\..\LIB


create.dir:
	@-if not exist $D\*.* mkdir $D

$D\CX.OBJ : ..\DOS32\CX.ASM
    SC $(CFLAGS) -o$D\CX.OBJ ..\DOS32\CX.ASM
    
$D\REALMODE.OBJ : ..\DOS32\REALMODE.ASM
   SC $(CFLAGS) -o$D\REALMODE.OBJ ..\DOS32\REALMODE.ASM
       
clean:
    -del $D\CX.OBJ
    -del $D\REALMODE.OBJ
    
    
