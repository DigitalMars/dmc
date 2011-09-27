# TLSSEG.MAK 
# Copyright (C) 1995 by Digital Mars
# All Rights Reserved
# makefile for TLLSEG.OBJ included as part of SNN.PRJ
# NOTE: TLSSEG.OBJ Must be compiled with the Masm -ml flag
#    otherwise __declspec(thread) will not work properly..
# $Revision: 1.1.1.1 $

D = SNN
MFLAGS = -D_WIN32 -DM_I386=1 -I..\..\include -D_MT=1 -ml -DI8086N 

ALL : create.dir $D\TLSSEG.OBJ

create.dir:
	@-if not exist $D\*.* md $D
	
clean:
    -del $D\TLSSEG.OBJ


$D\TLSSEG.OBJ : ..\WIN32\TLSSEG.ASM 
    MASM386  $(MFLAGS) ..\WIN32\TLSSEG.ASM,$D\TLSSEG.OBJ;
    