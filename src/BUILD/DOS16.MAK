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

PROJ		= dos16
APPTYPE		= WIN32 EXE
PROJTYPE	= EXE

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

DEFFILE		= DOS16.DEF

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -ml -C -WA -S -2 -a8 -c -g -gd 
LFLAGS		=  /CO /NOI /DE /PACKF /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:4194304 /A:512
DEFINES		= 
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -ml -o+time -WA -2 -a8 -c 
LFLAGS		=  /NOI /DE /E /PACKF /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:4194304 /A:512
DEFINES		= 
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C 
RESFLAGS	=  -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= L

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= 

INCLUDEDOBJS	= 

OBJS		= 

RCFILES		= 

RESFILES	= 

SYMS		= 

HELPFILES	= 

PROJS		= sdl.prj sds.prj sdc.prj  \
		sdm.prj 

BATS		= 

PRECOMPILE	=  C?C?Bsource?B75?BRTL?Bsrc?Bbuild?Bsdl?Pprj  C?C?Bsource?B75?BRTL?Bsrc?Bbuild?Bsds?Pprj  C?C?Bsource?B75?BRTL?Bsrc?Bbuild?Bsdc?Pprj  \
		 C?C?Bsource?B75?BRTL?Bsrc?Bbuild?Bsdm?Pprj 

POSTCOMPILE	= 

POSTLINK	= 

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

$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(SYMS) $(RCFILES) $(RESFILES) $(HELPFILES) $(DEFFILE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		-echo $(PROJ).$(PROJTYPE) done

buildall:	clean	all


clean:
		-del $(OUTPUTDIR)\SCPH.SYM
		$(MAKE) -fsdl.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsds.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsdc.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsdm.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:

res:		cleanres $(RCFILES) all


link:

C?C?Bsource?B75?BRTL?Bsrc?Bbuild?Bsdl?Pprj:
		$(MAKE) -fsdl.mak "SUB_DEBUG=$(DEBUG)"
C?C?Bsource?B75?BRTL?Bsrc?Bbuild?Bsds?Pprj:
		$(MAKE) -fsds.mak "SUB_DEBUG=$(DEBUG)"
C?C?Bsource?B75?BRTL?Bsrc?Bbuild?Bsdc?Pprj:
		$(MAKE) -fsdc.mak "SUB_DEBUG=$(DEBUG)"
C?C?Bsource?B75?BRTL?Bsrc?Bbuild?Bsdm?Pprj:
		$(MAKE) -fsdm.mak "SUB_DEBUG=$(DEBUG)"







