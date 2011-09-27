ORIGIN		= Symantec C++
ORIGIN_VER	= Version 7.22
VERSION		= RELEASE

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 0
NDEBUG		= 1
!ENDIF

PROJ		= ALLMFC
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

DEFFILE		= allmfc.DEF

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -mn -C -WA -S -3 -a8 -c -g 
LFLAGS		=  /CO /DE /PACKF /XN /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:1048576 /A:512
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
LFLAGS		=  /DE /E /PACKF /XN /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:1048576 /A:512
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

INCLUDES	= 

INCLUDEDOBJS	= 

OBJS		= 

RCFILES		= 

RESFILES	= 

HELPFILES	= 

PROJS		= nafxcw.prj nafxcwd.prj smfc4.prj  \
		smfc4d.prj smfc4u.prj smfc4ud.prj smfcd4d.prj  \
		smfcd4ud.prj smfcn4d.prj smfcn4ud.prj smfco4d.prj  \
		smfco4ud.prj uafxcw.prj uafxcwd.prj 

BATS		= 

PRECOMPILE	=  nafxcw?Pprj  nafxcwd?Pprj  smfc4?Pprj  \
		 smfc4d?Pprj  smfc4u?Pprj  smfc4ud?Pprj  smfcd4d?Pprj  \
		 smfcd4ud?Pprj  smfcn4d?Pprj  smfcn4ud?Pprj  smfco4d?Pprj  \
		 smfco4ud?Pprj  uafxcw?Pprj  uafxcwd?Pprj 

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
allmfc.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<

$$SCW$$.EXE
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
allmfc.DEF;
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
		-del ALLMFC.dpd
		$(MAKE) -fnafxcw.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fnafxcwd.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		cd smfc4\.
		$(MAKE) -fsmfc4.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		cd smfc4d\.
		$(MAKE) -fsmfc4d.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		cd smfc4u\.
		$(MAKE) -fsmfc4u.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		cd smfc4ud\.
		$(MAKE) -fsmfc4ud.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		cd smfcd4d\.
		$(MAKE) -fsmfcd4d.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		cd smfcd4ud\.
		$(MAKE) -fsmfcd4ud.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		cd smfcn4d\.
		$(MAKE) -fsmfcn4d.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		cd smfcn4ud\.
		$(MAKE) -fsmfcn4ud.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		cd smfco4d\.
		$(MAKE) -fsmfco4d.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		cd smfco4ud\.
		$(MAKE) -fsmfco4ud.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		cd ..\..\..\..\..\..\..\..\..\..\.
		$(MAKE) -fuafxcw.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fuafxcwd.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:

res:		cleanres $(RCFILES) all


link:


nafxcw?Pprj:
		$(MAKE) -fnafxcw.mak "SUB_DEBUG=$(DEBUG)"

nafxcwd?Pprj:
		$(MAKE) -fnafxcwd.mak "SUB_DEBUG=$(DEBUG)"

smfc4?Pprj:
		cd smfc4\.
		$(MAKE) -fsmfc4.mak "SUB_DEBUG=$(DEBUG)"

smfc4d?Pprj:
		cd smfc4d\.
		$(MAKE) -fsmfc4d.mak "SUB_DEBUG=$(DEBUG)"

smfc4u?Pprj:
		cd smfc4u\.
		$(MAKE) -fsmfc4u.mak "SUB_DEBUG=$(DEBUG)"

smfc4ud?Pprj:
		cd smfc4ud\.
		$(MAKE) -fsmfc4ud.mak "SUB_DEBUG=$(DEBUG)"

smfcd4d?Pprj:
		cd smfcd4d\.
		$(MAKE) -fsmfcd4d.mak "SUB_DEBUG=$(DEBUG)"

smfcd4ud?Pprj:
		cd smfcd4ud\.
		$(MAKE) -fsmfcd4ud.mak "SUB_DEBUG=$(DEBUG)"

smfcn4d?Pprj:
		cd smfcn4d\.
		$(MAKE) -fsmfcn4d.mak "SUB_DEBUG=$(DEBUG)"

smfcn4ud?Pprj:
		cd smfcn4ud\.
		$(MAKE) -fsmfcn4ud.mak "SUB_DEBUG=$(DEBUG)"

smfco4d?Pprj:
		cd smfco4d\.
		$(MAKE) -fsmfco4d.mak "SUB_DEBUG=$(DEBUG)"

smfco4ud?Pprj:
		cd smfco4ud\.
		$(MAKE) -fsmfco4ud.mak "SUB_DEBUG=$(DEBUG)"

uafxcw?Pprj:
		$(MAKE) -fuafxcw.mak "SUB_DEBUG=$(DEBUG)"

uafxcwd?Pprj:
		$(MAKE) -fuafxcwd.mak "SUB_DEBUG=$(DEBUG)"





!IF EXIST (ALLMFC.dpd)
!INCLUDE ALLMFC.dpd
!ENDIF


