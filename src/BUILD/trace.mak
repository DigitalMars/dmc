# TRACE.MAK 
# Copyright (C) 1995 by Digital Mars
# All Rights Reserved
# makefile for TRACE.LIB included as part of SNN.PRJ
# $Revision: 1.1.1.1 $

D = SNN

!IF EXIST(..\WIN32\TRACE\TRACE.C)

CFLAGS= -Ae -Ar -r -mn -o+time -NL -5 -a8 -Nc -c -wx -w- -w12 -D_MT -I..\..\INCLUDE

ALL : create.dir ..\WIN32\TRACE.LIB

create.dir:
	@-if not exist $D\*.* md $D
	
clean:
    -del $D\TRACE.OBJ

..\WIN32\TRACE.LIB: $D\TRACE.OBJ
   -del ..\WIN32\TRACE.LIB
   LIB /C /NOI @<<TRACE.RSP
   ..\WIN32\TRACE.LIB
   +$D\TRACE.OBJ
   ..\WIN32\TRACE
   ;
<<

$D\TRACE.OBJ : ..\WIN32\TRACE\TRACE.C ..\..\INCLUDE\TRACE.H
	SC $(CFLAGS) -o$D\TRACE.OBJ ..\WIN32\TRACE\TRACE.C
	
!ELSE

ALL: create.dir

create.dir :
    @-if not exist $D\*.* md $D
    
clean:
    -del $D\TRACE.OBJ


!ENDIF

