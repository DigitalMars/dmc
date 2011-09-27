ORIGIN		= Digital Mars C++
ORIGIN_VER	= Version 7.22
VERSION		= DEBUG

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 1
NDEBUG		= 0
!ENDIF

PROJ		= dllasmd
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

HEADERS		= 

DEFFILE		= dllasmd.DEF

!IF $(DEBUG)
OUTPUTDIR	= dllasmd
!IF EXIST (dllasmd)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= 

CFLAGS		=  -HIprecomp.h -Ae -Ar -r -j0 -mn -C -D_WINDLL -NL -5 -a8 -c -wx 
LFLAGS		=  /CO /NOI /DE /XN /A:512
DEFINES		= -D_MT
!ELSE
OUTPUTDIR	= dllasmd
!IF EXIST (dllasmd)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= 

CFLAGS		=  -HIprecomp.h -Ae -Ar -r -j0 -mn -C -D_WINDLL -NL -5 -a8 -Nc -c -wx 
LFLAGS		=  /CO /NOI /DE /XN /A:512
DEFINES		= -D_MT
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C /NOI 
RESFLAGS	=  -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= N

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I..\include -I..\..\include

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\CLEARBSS.OBJ  $(OUTPUTDIR)\MEMCPY.OBJ  $(OUTPUTDIR)\ITOA.OBJ  \
		 $(OUTPUTDIR)\STRCAT.OBJ  $(OUTPUTDIR)\EXCPTLST.OBJ  $(OUTPUTDIR)\FATERR.OBJ  $(OUTPUTDIR)\LLMATH.OBJ  \
		 $(OUTPUTDIR)\_EXIT.OBJ  $(OUTPUTDIR)\_EXIT2.OBJ  $(OUTPUTDIR)\CINIT.OBJ  $(OUTPUTDIR)\_8087.OBJ  \
		 $(OUTPUTDIR)\FLT87.OBJ  $(OUTPUTDIR)\LDEXP.OBJ  $(OUTPUTDIR)\MEMICMP.OBJ  $(OUTPUTDIR)\DOUBLE.OBJ  \
		 $(OUTPUTDIR)\PATCH32.OBJ  $(OUTPUTDIR)\STRNCPY.OBJ  $(OUTPUTDIR)\write.OBJ  $(OUTPUTDIR)\chkstk.OBJ  \
		 $(OUTPUTDIR)\read.OBJ  $(OUTPUTDIR)\fputc.OBJ  $(OUTPUTDIR)\STRCMPL.OBJ  $(OUTPUTDIR)\ATOI.OBJ  \
		 $(OUTPUTDIR)\ATOL.OBJ 

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
dllasmd\CLEARBSS.OBJ+
dllasmd\MEMCPY.OBJ+
dllasmd\ITOA.OBJ+
dllasmd\STRCAT.OBJ+
dllasmd\EXCPTLST.OBJ+
dllasmd\FATERR.OBJ+
dllasmd\LLMATH.OBJ+
dllasmd\_EXIT.OBJ+
dllasmd\_EXIT2.OBJ+
dllasmd\CINIT.OBJ+
dllasmd\_8087.OBJ+
dllasmd\FLT87.OBJ+
dllasmd\LDEXP.OBJ+
dllasmd\MEMICMP.OBJ+
dllasmd\DOUBLE.OBJ+
dllasmd\PATCH32.OBJ+
dllasmd\STRNCPY.OBJ+
dllasmd\write.OBJ+
dllasmd\chkstk.OBJ+
dllasmd\read.OBJ+
dllasmd\fputc.OBJ+
dllasmd\STRCMPL.OBJ+
dllasmd\ATOI.OBJ+
dllasmd\ATOL.OBJ
$$SCW$$.LIB
NUL

dllasmd.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
dllasmd\CLEARBSS.OBJ+
dllasmd\MEMCPY.OBJ+
dllasmd\ITOA.OBJ+
dllasmd\STRCAT.OBJ+
dllasmd\EXCPTLST.OBJ+
dllasmd\FATERR.OBJ+
dllasmd\LLMATH.OBJ+
dllasmd\_EXIT.OBJ+
dllasmd\_EXIT2.OBJ+
dllasmd\CINIT.OBJ+
dllasmd\_8087.OBJ+
dllasmd\FLT87.OBJ+
dllasmd\LDEXP.OBJ+
dllasmd\MEMICMP.OBJ+
dllasmd\DOUBLE.OBJ+
dllasmd\PATCH32.OBJ+
dllasmd\STRNCPY.OBJ+
dllasmd\write.OBJ+
dllasmd\chkstk.OBJ+
dllasmd\read.OBJ+
dllasmd\fputc.OBJ+
dllasmd\STRCMPL.OBJ+
dllasmd\ATOI.OBJ+
dllasmd\ATOL.OBJ
$$SCW$$.LIB
NUL

dllasmd.DEF;
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
 + $(OUTPUTDIR)\CLEARBSS.OBJ + $(OUTPUTDIR)\MEMCPY.OBJ + $(OUTPUTDIR)\ITOA.OBJ &
		 + $(OUTPUTDIR)\STRCAT.OBJ + $(OUTPUTDIR)\EXCPTLST.OBJ + $(OUTPUTDIR)\FATERR.OBJ + $(OUTPUTDIR)\LLMATH.OBJ &
		 + $(OUTPUTDIR)\_EXIT.OBJ + $(OUTPUTDIR)\_EXIT2.OBJ + $(OUTPUTDIR)\CINIT.OBJ + $(OUTPUTDIR)\_8087.OBJ &
		 + $(OUTPUTDIR)\FLT87.OBJ + $(OUTPUTDIR)\LDEXP.OBJ + $(OUTPUTDIR)\MEMICMP.OBJ + $(OUTPUTDIR)\DOUBLE.OBJ &
		 + $(OUTPUTDIR)\PATCH32.OBJ + $(OUTPUTDIR)\STRNCPY.OBJ + $(OUTPUTDIR)\write.OBJ + $(OUTPUTDIR)\chkstk.OBJ &
		 + $(OUTPUTDIR)\read.OBJ + $(OUTPUTDIR)\fputc.OBJ + $(OUTPUTDIR)\STRCMPL.OBJ + $(OUTPUTDIR)\ATOI.OBJ &
		 + $(OUTPUTDIR)\ATOL.OBJ
<<
			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(TARGETDIR)\$(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del dllasmd.dpd
		-del $(OBJS)

cleanres:

res:		cleanres $(RCFILES) all


link:
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 + $(OUTPUTDIR)\CLEARBSS.OBJ + $(OUTPUTDIR)\MEMCPY.OBJ + $(OUTPUTDIR)\ITOA.OBJ &
		 + $(OUTPUTDIR)\STRCAT.OBJ + $(OUTPUTDIR)\EXCPTLST.OBJ + $(OUTPUTDIR)\FATERR.OBJ + $(OUTPUTDIR)\LLMATH.OBJ &
		 + $(OUTPUTDIR)\_EXIT.OBJ + $(OUTPUTDIR)\_EXIT2.OBJ + $(OUTPUTDIR)\CINIT.OBJ + $(OUTPUTDIR)\_8087.OBJ &
		 + $(OUTPUTDIR)\FLT87.OBJ + $(OUTPUTDIR)\LDEXP.OBJ + $(OUTPUTDIR)\MEMICMP.OBJ + $(OUTPUTDIR)\DOUBLE.OBJ &
		 + $(OUTPUTDIR)\PATCH32.OBJ + $(OUTPUTDIR)\STRNCPY.OBJ + $(OUTPUTDIR)\write.OBJ + $(OUTPUTDIR)\chkstk.OBJ &
		 + $(OUTPUTDIR)\read.OBJ + $(OUTPUTDIR)\fputc.OBJ + $(OUTPUTDIR)\STRCMPL.OBJ + $(OUTPUTDIR)\ATOI.OBJ &
		 + $(OUTPUTDIR)\ATOL.OBJ
<<
		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (dllasmd.dpd)
!INCLUDE dllasmd.dpd
!ENDIF



$(OUTPUTDIR)\CLEARBSS.OBJ:	..\win32\CLEARBSS.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\CLEARBSS.obj ..\win32\CLEARBSS.ASM



$(OUTPUTDIR)\MEMCPY.OBJ:	..\core32\MEMCPY.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MEMCPY.obj ..\core32\MEMCPY.ASM



$(OUTPUTDIR)\ITOA.OBJ:	..\core32\ITOA.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ITOA.obj ..\core32\ITOA.ASM



$(OUTPUTDIR)\STRCAT.OBJ:	..\core32\STRCAT.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\STRCAT.obj ..\core32\STRCAT.ASM



$(OUTPUTDIR)\EXCPTLST.OBJ:	..\win32\EXCPTLST.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\EXCPTLST.obj ..\win32\EXCPTLST.ASM



$(OUTPUTDIR)\FATERR.OBJ:	..\core32\FATERR.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\FATERR.obj ..\core32\FATERR.ASM



$(OUTPUTDIR)\LLMATH.OBJ:	..\core32\LLMATH.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\LLMATH.obj ..\core32\LLMATH.ASM



$(OUTPUTDIR)\_exit.obj:	..\core32\_exit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_exit.obj ..\core32\_exit.c



$(OUTPUTDIR)\_exit2.obj:	..\core32\_exit2.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_exit2.obj ..\core32\_exit2.c



$(OUTPUTDIR)\CINIT.OBJ:	..\core32\CINIT.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\CINIT.obj ..\core32\CINIT.ASM



$(OUTPUTDIR)\_8087.OBJ:	..\win32\_8087.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_8087.obj ..\win32\_8087.ASM



$(OUTPUTDIR)\FLT87.OBJ:	..\core32\FLT87.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\FLT87.obj ..\core32\FLT87.ASM



$(OUTPUTDIR)\LDEXP.OBJ:	..\core32\LDEXP.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\LDEXP.obj ..\core32\LDEXP.ASM



$(OUTPUTDIR)\MEMICMP.OBJ:	..\core32\MEMICMP.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MEMICMP.obj ..\core32\MEMICMP.ASM



$(OUTPUTDIR)\DOUBLE.OBJ:	..\core32\DOUBLE.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\DOUBLE.obj ..\core32\DOUBLE.ASM



$(OUTPUTDIR)\PATCH32.OBJ:	..\core32\PATCH32.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\PATCH32.obj ..\core32\PATCH32.ASM



$(OUTPUTDIR)\STRNCPY.OBJ:	..\core32\STRNCPY.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\STRNCPY.obj ..\core32\STRNCPY.ASM



$(OUTPUTDIR)\write.OBJ:	..\core32\write.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\write.obj ..\core32\write.asm



$(OUTPUTDIR)\chkstk.OBJ:	..\win32\chkstk.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\chkstk.obj ..\win32\chkstk.asm



$(OUTPUTDIR)\read.OBJ:	..\core32\read.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\read.obj ..\core32\read.asm



$(OUTPUTDIR)\fputc.OBJ:	..\core32\fputc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fputc.obj ..\core32\fputc.asm



$(OUTPUTDIR)\STRCMPL.OBJ:	..\CORE32\STRCMPL.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\STRCMPL.obj ..\CORE32\STRCMPL.ASM



$(OUTPUTDIR)\ATOI.OBJ:	..\CORE32\ATOI.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ATOI.obj ..\CORE32\ATOI.ASM



$(OUTPUTDIR)\ATOL.OBJ:	..\CORE32\ATOL.ASM
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ATOL.obj ..\CORE32\ATOL.ASM




