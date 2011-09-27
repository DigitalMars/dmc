# OLDSTR.MAK September 16, 1993
# Copyright (C) 1985-1993 by Digital Mars
# All Rights Reserved
# makefile includes for old IO streams library
# $Revision: 1.1.1.1 $


D = SD$(MODEL)

IN_LIB      = $D\in1.OBJ   $D\in2.OBJ   $D\in3.OBJ   $D\in4.OBJ   $D\in5.OBJ   $D\in6.OBJ   \
              $D\in7.OBJ   $D\in8.OBJ   $D\in9.OBJ   $D\in10.OBJ  $D\in11.OBJ  $D\in12.OBJ  \
              $D\in13.OBJ  $D\in14.OBJ  $D\in15.OBJ  $D\in16.OBJ  $D\in17.OBJ  $D\in18.OBJ  \
              $D\in19.OBJ  $D\in20.OBJ
              
OUT_LIB     = $D\out1.OBJ  $D\out2.OBJ  $D\out3.OBJ  $D\out4.OBJ  $D\out5.OBJ  $D\out6.OBJ  \
              $D\out7.OBJ  $D\out8.OBJ  $D\out9.OBJ
              

OLDSTR_LIB  = $(IN_LIB) $(OUT_LIB) CIRCBUF.OBJ FILEBUF.OBJ FORM.OBJ STREAM.OBJ
             
$(IN_LIB)      : ..\OLDSTR\IN.CPP 
  SC -w12 -Ae -Ar -c -o -D$(OS) -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* %s

$(OUT_LIB)     : ..\OLDSTR\OUT.CPP
  SC -w12 -Ae -Ar -c -o -D$(OS) -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* %s

ALL : create.dir ..\..\LIB\OLDSTR$(MODEL).LIB

create.dir:
	@-if not exist $D\*.* mkdir $D

clean: 
    -del $(OLDSTR_LIB)

..\..\LIB\OLDSTR$(MODEL).LIB : $(OLDSTR_LIB)
   IF EXIST ..\..\LIB\OLDSTR$(MODEL).LIB DEL ..\..\LIB\OLDSTR$(MODEL).LIB
   LIB /C /NOI /PAGESIZE:16 @<<
     ..\..\LIB\OLDSTR$(MODEL)
     +$(OLDSTR_LIB: =&^
     +)
     ;
<<
   
