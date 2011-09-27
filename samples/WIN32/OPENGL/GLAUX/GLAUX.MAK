ORIGIN		= Symantec C++
ORIGIN_VER	= Version 7.22
VERSION		= DEBUG

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 1
NDEBUG		= 0
!ENDIF

PROJ		= glaux
APPTYPE		= WIN32 LIB
PROJTYPE	= LIB

CC		= SC
CPP		= SPP
JAVAC		= sj
MAKE		= SMAKE
RC		= RCC
HC		= HC31
ASM		= SC
DISASM		= OBJ2ASM
LNK		= LINK
DLLS		= 

HEADERS		= ..\..\..\..\include\win32\pshpack4.h glos.h teapot.h  \
		3d.h tk.h ..\..\..\..\include\win32\objidl.h ..\..\..\..\include\win32\unknwn.h  \
		..\..\..\..\include\win32\wtypes.h ..\..\..\..\include\win32\ole2.h ..\..\..\..\include\win32\windows.h ..\..\..\..\include\win32\rpcndr.h  \
		..\..\..\..\include\win32\rpc.h ..\..\..\..\include\win32\poppack.h ..\..\..\..\include\win32\pshpack2.h 

DEFFILE		= glaux.def

!IF $(DEBUG)
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= .
!IF EXIST (.)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		= 
SYMS		= 
LIBS		= 

CFLAGS		=  -Jm -mn -C -D_WINDOWS -Pz -S -3 -a8 -c -w- -w2 -g 
LFLAGS		=  /CO /NOI /DE /PACKF /XN /A:512
DEFINES		= 
!ELSE
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= .
!IF EXIST (.)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		= 
SYMS		= 
LIBS		= 

CFLAGS		=  -Jm -mn -o+time -D_WINDOWS -Pz -5 -a8 -c -w- -w2 
LFLAGS		=  /NOI /DE /E /PACKF /XN /A:512
DEFINES		= 
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C /P:512 
RESFLAGS	=  -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= N

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I.

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\font.OBJ  $(OUTPUTDIR)\glaux.OBJ  $(OUTPUTDIR)\image.OBJ  \
		 $(OUTPUTDIR)\shapes.OBJ  $(OUTPUTDIR)\teapot.OBJ  $(OUTPUTDIR)\tk.OBJ  $(OUTPUTDIR)\tkdib.OBJ  \
		 $(OUTPUTDIR)\tkfont.OBJ  $(OUTPUTDIR)\tkimage.OBJ  $(OUTPUTDIR)\vect3d.OBJ  $(OUTPUTDIR)\xform.OBJ 

RCFILES		= 

RESFILES	= 

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

!IF $(DEBUG)
.OBJ.EXE:
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\font.OBJ+
.\glaux.OBJ+
.\image.OBJ+
.\shapes.OBJ+
.\teapot.OBJ+
.\tk.OBJ+
.\tkdib.OBJ+
.\tkfont.OBJ+
.\tkimage.OBJ+
.\vect3d.OBJ+
.\xform.OBJ
.\$$SCW$$.LIB
NUL
.\ 
glaux.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\font.OBJ+
.\glaux.OBJ+
.\image.OBJ+
.\shapes.OBJ+
.\teapot.OBJ+
.\tk.OBJ+
.\tkdib.OBJ+
.\tkfont.OBJ+
.\tkimage.OBJ+
.\vect3d.OBJ+
.\xform.OBJ
.\$$SCW$$.LIB
NUL
.\ 
glaux.def;
<<
!ENDIF

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



all:	noteout createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done


all2:	createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done

noteout:
	REM Output to $(OUTPUTDIR)

createdir:
	$(CREATEOUTPUTDIR)
	$(CREATETARGETDIR)
	
$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) 
			-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 + $(OUTPUTDIR)\font.OBJ + $(OUTPUTDIR)\glaux.OBJ + $(OUTPUTDIR)\image.OBJ &
		 + $(OUTPUTDIR)\shapes.OBJ + $(OUTPUTDIR)\teapot.OBJ + $(OUTPUTDIR)\tk.OBJ + $(OUTPUTDIR)\tkdib.OBJ &
		 + $(OUTPUTDIR)\tkfont.OBJ + $(OUTPUTDIR)\tkimage.OBJ + $(OUTPUTDIR)\vect3d.OBJ + $(OUTPUTDIR)\xform.OBJ
<<
			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del glaux.dpd
		-del $(OBJS)

cleanres:

res:		cleanres $(RCFILES) all


link:
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 + $(OUTPUTDIR)\font.OBJ + $(OUTPUTDIR)\glaux.OBJ + $(OUTPUTDIR)\image.OBJ &
		 + $(OUTPUTDIR)\shapes.OBJ + $(OUTPUTDIR)\teapot.OBJ + $(OUTPUTDIR)\tk.OBJ + $(OUTPUTDIR)\tkdib.OBJ &
		 + $(OUTPUTDIR)\tkfont.OBJ + $(OUTPUTDIR)\tkimage.OBJ + $(OUTPUTDIR)\vect3d.OBJ + $(OUTPUTDIR)\xform.OBJ
<<
		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (glaux.dpd)
!INCLUDE glaux.dpd
!ENDIF



$(OUTPUTDIR)\font.OBJ:	font.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\font.obj font.c



$(OUTPUTDIR)\glaux.OBJ:	glaux.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\glaux.obj glaux.c



$(OUTPUTDIR)\image.OBJ:	image.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\image.obj image.c



$(OUTPUTDIR)\shapes.OBJ:	shapes.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\shapes.obj shapes.c



$(OUTPUTDIR)\teapot.OBJ:	teapot.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\teapot.obj teapot.c



$(OUTPUTDIR)\tk.OBJ:	tk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tk.obj tk.c



$(OUTPUTDIR)\tkdib.OBJ:	tkdib.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tkdib.obj tkdib.c



$(OUTPUTDIR)\tkfont.OBJ:	tkfont.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tkfont.obj tkfont.c



$(OUTPUTDIR)\tkimage.OBJ:	tkimage.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tkimage.obj tkimage.c



$(OUTPUTDIR)\vect3d.OBJ:	vect3d.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\vect3d.obj vect3d.c



$(OUTPUTDIR)\xform.OBJ:	xform.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\xform.obj xform.c




