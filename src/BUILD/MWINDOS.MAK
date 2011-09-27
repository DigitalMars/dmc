ORIGIN		= Digital Mars C++
ORIGIN_VER	= Version 7.20
VERSION		= RELEASE

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 0
NDEBUG		= 1
!ENDIF

PROJ		= mwindos
APPTYPE		= WINDOWS LIB
PROJTYPE	= LIB

CC		= SC
CPP		= SPP
MAKE		= SMAKE
RC		= RCC
HC		= HC31
ASM		= SC
DISASM		= OBJ2ASM
LNK		= LINK
DLLS		= 

HEADERS		= 

DEFFILE		= mwindos.DEF

!IF $(DEBUG)
OUTPUTDIR	= SWM
!IF EXIST (SWM)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\..\LIB
!IF EXIST (..\..\LIB)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

LIBS		= 

CFLAGS		=  -Ae -Ar -r -mm -W1 -NL -4 -a2 -c -wx -w- -w12 -g -gd 
CFLAGS_1	=  -Ae -Ar -Aa -r -mm -W1 -NL -4 -a2 -c -wx -w- -w12 -g -gd
DEFINES_1	= 
CFLAGS_2	=  -Ae -Ar -Aa -r -mm -W1 -NL -4 -a2 -c -wx -w- -w12 -g -gd
DEFINES_2	= 
CFLAGS_3	=  -Ae -Ar -Aa -r -mm -W1 -NL -4 -a2 -c -wx -w- -w12 -g -gd
DEFINES_3	= 
LFLAGS		=  /CO /DE /PACKF /XU /A:512
DEFINES		= 
!ELSE
OUTPUTDIR	= SWM
!IF EXIST (SWM)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\..\LIB
!IF EXIST (..\..\LIB)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

LIBS		= 

CFLAGS		=  -Ae -Ar -r -mm -o+time -W1 -NL -4 -a2 -Nc -c -wx -w- -w12 
CFLAGS_1	=  -Ae -Ar -Aa -r -mm -o+time -W1 -NL -4 -a2 -Nc -c -wx -w- -w12
DEFINES_1	= 
CFLAGS_2	=  -Ae -Ar -Aa -r -mm -o+time -W1 -NL -4 -a2 -Nc -c -wx -w- -w12
DEFINES_2	= 
CFLAGS_3	=  -Ae -Ar -Aa -r -mm -o+time -W1 -NL -4 -a2 -Nc -c -wx -w- -w12
DEFINES_3	= 
LFLAGS		=  /DE /E /PACKF /XU /A:512
DEFINES		= 
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C /NOI 
RESFLAGS	=  -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= M

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I..\INCLUDE -I..\..\INCLUDE

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\argcargv.OBJ  $(OUTPUTDIR)\winio.OBJ  $(OUTPUTDIR)\wmhandlr.OBJ  \
		 $(OUTPUTDIR)\scver.OBJ 

RCFILES		= 

RESFILES	= 

SYMS		= 

HELPFILES	= 

BATS		= 

.SUFFIXES: .C .CP .CPP .CXX .CC .H .HPP .HXX .COM .EXE .DLL .LIB .RTF .DLG .ASM .RES .RC .OBJ 

.C.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.c

.CPP.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cpp

.CXX.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cxx

.CC.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cc

.CP.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cp

.H.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.hxx

.C.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)   $*.c   -o$*.lst

.CPP.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.cpp -o$*.lst

.CXX.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.cxx -o$*.lst

.CP.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)  $*.cp  -o$*.lst

.CC.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)  $*.cc  -o$*.lst

.ASM.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.asm -o$*.lst

.OBJ.COD:
	$(DISASM) $*.OBJ -c

.OBJ.EXE:
	$(LNK) $(LFLAGS) @$(PROJ).LNK

.RTF.HLP:
	$(HC) $(HELPFLAGS) $*.HPJ

.ASM.OBJ:
	$(ASM) $(AFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.asm

.RC.RES: 
	$(RC) $(RCDEFINES) $(RESFLAGS) $(INCLUDES) $*.rc -o$*.res

.DLG.RES:
	echo ^#include "windows.h" >$$$*.rc
	echo ^IF EXIST "$*.h" >>$$$*.rc
	echo ^#include "$*.h" >>$$$*.rc
	echo ^#include "$*.dlg" >>$$$*.rc
	$(RC) $(RCDEFINES) $(RESFLAGS) $(INCLUDES) $$$*.rc
	-del $*.res
	-ren $$$*.res $*.res



all:	createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done

createdir:
	$(CREATEOUTPUTDIR)
	$(CREATETARGETDIR)

$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) 
			-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 +$(OUTPUTDIR)\argcargv.OBJ +$(OUTPUTDIR)\winio.OBJ +$(OUTPUTDIR)\wmhandlr.OBJ &
		 +$(OUTPUTDIR)\scver.OBJ
<<
			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		-echo $(PROJ).$(PROJTYPE) done

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(TARGETDIR)\$(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del $(OBJS)

cleanres:

res:		cleanres $(RCFILES) all


link:
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 +$(OUTPUTDIR)\argcargv.OBJ +$(OUTPUTDIR)\winio.OBJ +$(OUTPUTDIR)\wmhandlr.OBJ &
		 +$(OUTPUTDIR)\scver.OBJ
<<
		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)







$(OUTPUTDIR)\argcargv.OBJ:	..\winio\argcargv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\argcargv.obj ..\winio\argcargv.c



$(OUTPUTDIR)\winio.OBJ:	..\winio\winio.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winio.obj ..\winio\winio.c



$(OUTPUTDIR)\wmhandlr.OBJ:	..\winio\wmhandlr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wmhandlr.obj ..\winio\wmhandlr.c



$(OUTPUTDIR)\scver.OBJ:	..\core\scver.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\scver.obj ..\core\scver.c



$(OUTPUTDIR)\anew2.OBJ:	..\core\anew2.cpp
		$(CC) $(CFLAGS_1) $(DEFINES_1) $(INCLUDES) -o$(OUTPUTDIR)\anew2.obj ..\core\anew2.cpp



$(OUTPUTDIR)\anew.OBJ:	..\core\anew.cpp
		$(CC) $(CFLAGS_2) $(DEFINES_2) $(INCLUDES) -o$(OUTPUTDIR)\anew.obj ..\core\anew.cpp



$(OUTPUTDIR)\adelete.OBJ:	..\core\adelete.cpp
		$(CC) $(CFLAGS_3) $(DEFINES_3) $(INCLUDES) -o$(OUTPUTDIR)\adelete.obj ..\core\adelete.cpp




