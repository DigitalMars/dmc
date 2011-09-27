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

PROJ		= debug
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

HEADERS		= 

DEFFILE		= debug.DEF

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -mn -C -WA -S -3 -a8 -c -gf 
LFLAGS		=  /CO /NOI /DE /XN /NT /ENTRY:WinMainCRTStartup /BAS:4194304 /A:512
DEFINES		= 
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -mn -o+time -WA -3 -a8 -c 
LFLAGS		=  /NOI /DE /E /PACKF /XN /NT /ENTRY:WinMainCRTStartup /BAS:4194304 /A:512
DEFINES		= 
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C 
RESFLAGS	=  -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= N

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= 

INCLUDEDOBJS	= 

OBJS		= 

RCFILES		= 

RESFILES	= 

HELPFILES	= 

PROJS		= EAFXISD.PRJ NAFXCWD.PRJ NAFXISD.PRJ  \
		SFC42D.PRJ SFC42UD.PRJ SFCD42D.PRJ SFCD42UD.PRJ  \
		SFCN42D.PRJ SFCN42UD.PRJ SFCO42D.PRJ SFCO42UD.PRJ  \
		SFCS42D.PRJ SFCS42UD.PRJ UAFXCWD.PRJ 

BATS		= 

PRECOMPILE	=  EAFXISD?PPRJ  NAFXCWD?PPRJ  NAFXISD?PPRJ  \
		 SFC42D?PPRJ  SFC42UD?PPRJ  SFCD42D?PPRJ  SFCD42UD?PPRJ  \
		 SFCN42D?PPRJ  SFCN42UD?PPRJ  SFCO42D?PPRJ  SFCO42UD?PPRJ  \
		 SFCS42D?PPRJ  SFCS42UD?PPRJ  UAFXCWD?PPRJ 

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

!IF $(DEBUG)
.OBJ.EXE:
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK

$$SCW$$.EXE
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
debug.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<

$$SCW$$.EXE
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
debug.DEF;
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
	
$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(SYMS) $(RCFILES) $(RESFILES) $(HELPFILES) $(DEFFILE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(OUTPUTDIR)\SCPH.SYM
		-del debug.dpd
		$(MAKE) -feafxisd.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fnafxcwd.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fnafxisd.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsfc42d.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsfc42ud.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsfcd42d.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsfcd42ud.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsfcn42d.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsfcn42ud.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsfco42d.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsfco42ud.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsfcs42d.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fsfcs42ud.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fuafxcwd.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:

res:		cleanres $(RCFILES) all


link:


EAFXISD?PPRJ:
		$(MAKE) -feafxisd.mak "SUB_DEBUG=$(DEBUG)"

NAFXCWD?PPRJ:
		$(MAKE) -fnafxcwd.mak "SUB_DEBUG=$(DEBUG)"

NAFXISD?PPRJ:
		$(MAKE) -fnafxisd.mak "SUB_DEBUG=$(DEBUG)"

SFC42D?PPRJ:
		$(MAKE) -fsfc42d.mak "SUB_DEBUG=$(DEBUG)"

SFC42UD?PPRJ:
		$(MAKE) -fsfc42ud.mak "SUB_DEBUG=$(DEBUG)"

SFCD42D?PPRJ:
		$(MAKE) -fsfcd42d.mak "SUB_DEBUG=$(DEBUG)"

SFCD42UD?PPRJ:
		$(MAKE) -fsfcd42ud.mak "SUB_DEBUG=$(DEBUG)"

SFCN42D?PPRJ:
		$(MAKE) -fsfcn42d.mak "SUB_DEBUG=$(DEBUG)"

SFCN42UD?PPRJ:
		$(MAKE) -fsfcn42ud.mak "SUB_DEBUG=$(DEBUG)"

SFCO42D?PPRJ:
		$(MAKE) -fsfco42d.mak "SUB_DEBUG=$(DEBUG)"

SFCO42UD?PPRJ:
		$(MAKE) -fsfco42ud.mak "SUB_DEBUG=$(DEBUG)"

SFCS42D?PPRJ:
		$(MAKE) -fsfcs42d.mak "SUB_DEBUG=$(DEBUG)"

SFCS42UD?PPRJ:
		$(MAKE) -fsfcs42ud.mak "SUB_DEBUG=$(DEBUG)"

UAFXCWD?PPRJ:
		$(MAKE) -fuafxcwd.mak "SUB_DEBUG=$(DEBUG)"





!IF EXIST (debug.dpd)
!INCLUDE debug.dpd
!ENDIF


