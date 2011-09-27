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

PROJ		= GDIDEMO
APPTYPE		= WIN32 EXE
PROJTYPE	= EXE

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

HEADERS		= ..\..\..\..\include\win32\pshpack4.h maze.h xform.h  \
		poly.h draw.h bounce.h gdidemo.h  \
		..\..\..\..\include\win32\objidl.h ..\..\..\..\include\win32\unknwn.h ..\..\..\..\include\win32\wtypes.h ..\..\..\..\include\win32\ole2.h  \
		..\..\..\..\include\win32\windows.h ..\..\..\..\include\win32\rpcndr.h ..\..\..\..\include\win32\rpc.h ..\..\..\..\include\win32\poppack.h  \
		..\..\..\..\include\win32\pshpack2.h 

DEFFILE		= gdidemo.def

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
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Jm -mn -C -WA -S -3 -a8 -c -w- -w2 -w6 -g 
LFLAGS		=  /CO /DE /PACKF /XN /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:1048576 /A:512 /RC   :gdidemo.RES
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
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Jm -mn -o+time -WA -3 -a8 -c -w- -w2 -w6 
LFLAGS		=  /DE /E /PACKF /XN /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:1048576 /A:512 /RC   :gdidemo.RES
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

OBJS		=  $(OUTPUTDIR)\bounce.OBJ  $(OUTPUTDIR)\dialog.OBJ  $(OUTPUTDIR)\draw.OBJ  \
		 $(OUTPUTDIR)\gdidemo.OBJ  $(OUTPUTDIR)\init.OBJ  $(OUTPUTDIR)\maze.OBJ  $(OUTPUTDIR)\poly.OBJ  \
		 $(OUTPUTDIR)\wininfo.OBJ  $(OUTPUTDIR)\xform.OBJ 

RCFILES		= 

RESFILES	=  gdidemo.RES 

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
.\bounce.OBJ+
.\dialog.OBJ+
.\draw.OBJ+
.\gdidemo.OBJ+
.\init.OBJ+
.\maze.OBJ+
.\poly.OBJ+
.\wininfo.OBJ+
.\xform.OBJ
.\$$SCW$$.EXE
NUL
.\ KERNEL32.LIB GDI32.LIB USER32.LIB 
gdidemo.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\bounce.OBJ+
.\dialog.OBJ+
.\draw.OBJ+
.\gdidemo.OBJ+
.\init.OBJ+
.\maze.OBJ+
.\poly.OBJ+
.\wininfo.OBJ+
.\xform.OBJ
.\$$SCW$$.EXE
NUL
.\ KERNEL32.LIB GDI32.LIB USER32.LIB 
gdidemo.def;
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
	
$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) $(DEFFILE)
			-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\bounce.OBJ+
.\dialog.OBJ+
.\draw.OBJ+
.\gdidemo.OBJ+
.\init.OBJ+
.\maze.OBJ+
.\poly.OBJ+
.\wininfo.OBJ+
.\xform.OBJ
.\$$SCW$$.EXE
NUL
.\ KERNEL32.LIB GDI32.LIB USER32.LIB 
gdidemo.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\bounce.OBJ+
.\dialog.OBJ+
.\draw.OBJ+
.\gdidemo.OBJ+
.\init.OBJ+
.\maze.OBJ+
.\poly.OBJ+
.\wininfo.OBJ+
.\xform.OBJ
.\$$SCW$$.EXE
NUL
.\ KERNEL32.LIB GDI32.LIB USER32.LIB 
gdidemo.def;
<<
!ENDIF

			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del GDIDEMO.dpd
		-del $(OBJS)

cleanres:

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\bounce.OBJ+
.\dialog.OBJ+
.\draw.OBJ+
.\gdidemo.OBJ+
.\init.OBJ+
.\maze.OBJ+
.\poly.OBJ+
.\wininfo.OBJ+
.\xform.OBJ
.\$$SCW$$.EXE
NUL
.\ KERNEL32.LIB GDI32.LIB USER32.LIB 
gdidemo.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\bounce.OBJ+
.\dialog.OBJ+
.\draw.OBJ+
.\gdidemo.OBJ+
.\init.OBJ+
.\maze.OBJ+
.\poly.OBJ+
.\wininfo.OBJ+
.\xform.OBJ
.\$$SCW$$.EXE
NUL
.\ KERNEL32.LIB GDI32.LIB USER32.LIB 
gdidemo.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (GDIDEMO.dpd)
!INCLUDE GDIDEMO.dpd
!ENDIF



$(OUTPUTDIR)\bounce.OBJ:	bounce.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\bounce.obj bounce.c



$(OUTPUTDIR)\dialog.OBJ:	dialog.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dialog.obj dialog.c



$(OUTPUTDIR)\draw.OBJ:	draw.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\draw.obj draw.c



$(OUTPUTDIR)\gdidemo.OBJ:	gdidemo.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\gdidemo.obj gdidemo.c



$(OUTPUTDIR)\init.OBJ:	init.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\init.obj init.c



$(OUTPUTDIR)\maze.OBJ:	maze.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\maze.obj maze.c



$(OUTPUTDIR)\poly.OBJ:	poly.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\poly.obj poly.c



$(OUTPUTDIR)\wininfo.OBJ:	wininfo.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wininfo.obj wininfo.c



$(OUTPUTDIR)\xform.OBJ:	xform.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\xform.obj xform.c




