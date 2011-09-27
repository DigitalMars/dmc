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

PROJ		= samples
APPTYPE		= WIN32 CONSOLE
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

DEFFILE		= samples.def

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -mn -C -WA -S -3 -a8 -c -g 
LFLAGS		=  /CO /NOI /DE /PACKF /XN /NT /ENTRY:mainCRTStartup /VERS:1.0 /BAS:4194304 /A:512
DEFINES		= -D_CONSOLE
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -mn -o+time -WA -3 -a8 -c 
LFLAGS		=  /NOI /DE /E /PACKF /XN /NT /ENTRY:mainCRTStartup /VERS:1.0 /BAS:4194304 /A:512
DEFINES		= -D_CONSOLE
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

PROJS		= tbitmap1.prj tbitmap2.prj tdepth.prj  \
		teval.prj tfog.prj tline.prj tlogo.prj  \
		tpoint.prj tprim.prj tselect.prj tstar.prj  \
		tstencil.prj ttri.prj twave.prj taccum.prj  \
		tcopy.prj tnurb.PRJ tquad.prj tspeed.prj  \
		tsphere.prj tstretch.PRJ ttexture.prj 

BATS		= 

PRECOMPILE	=  tbitmap1?Pprj  tbitmap2?Pprj  tdepth?Pprj  \
		 teval?Pprj  tfog?Pprj  tline?Pprj  tlogo?Pprj  \
		 tpoint?Pprj  tprim?Pprj  tselect?Pprj  tstar?Pprj  \
		 tstencil?Pprj  ttri?Pprj  twave?Pprj  taccum?Pprj  \
		 tcopy?Pprj  tnurb?PPRJ  tquad?Pprj  tspeed?Pprj  \
		 tsphere?Pprj  tstretch?PPRJ  ttexture?Pprj 

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
advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
samples.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<

$$SCW$$.EXE
NUL
advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
samples.def;
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
		-del samples.dpd
		$(MAKE) -ftbitmap1.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftbitmap2.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftdepth.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fteval.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftfog.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftline.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftlogo.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftpoint.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftprim.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftselect.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftstar.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftstencil.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fttri.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftwave.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftaccum.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftcopy.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftnurb.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftquad.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftspeed.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftsphere.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftstretch.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -fttexture.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:

res:		cleanres $(RCFILES) all


link:


tbitmap1?Pprj:
		$(MAKE) -ftbitmap1.mak "SUB_DEBUG=$(DEBUG)"

tbitmap2?Pprj:
		$(MAKE) -ftbitmap2.mak "SUB_DEBUG=$(DEBUG)"

tdepth?Pprj:
		$(MAKE) -ftdepth.mak "SUB_DEBUG=$(DEBUG)"

teval?Pprj:
		$(MAKE) -fteval.mak "SUB_DEBUG=$(DEBUG)"

tfog?Pprj:
		$(MAKE) -ftfog.mak "SUB_DEBUG=$(DEBUG)"

tline?Pprj:
		$(MAKE) -ftline.mak "SUB_DEBUG=$(DEBUG)"

tlogo?Pprj:
		$(MAKE) -ftlogo.mak "SUB_DEBUG=$(DEBUG)"

tpoint?Pprj:
		$(MAKE) -ftpoint.mak "SUB_DEBUG=$(DEBUG)"

tprim?Pprj:
		$(MAKE) -ftprim.mak "SUB_DEBUG=$(DEBUG)"

tselect?Pprj:
		$(MAKE) -ftselect.mak "SUB_DEBUG=$(DEBUG)"

tstar?Pprj:
		$(MAKE) -ftstar.mak "SUB_DEBUG=$(DEBUG)"

tstencil?Pprj:
		$(MAKE) -ftstencil.mak "SUB_DEBUG=$(DEBUG)"

ttri?Pprj:
		$(MAKE) -fttri.mak "SUB_DEBUG=$(DEBUG)"

twave?Pprj:
		$(MAKE) -ftwave.mak "SUB_DEBUG=$(DEBUG)"

taccum?Pprj:
		$(MAKE) -ftaccum.mak "SUB_DEBUG=$(DEBUG)"

tcopy?Pprj:
		$(MAKE) -ftcopy.mak "SUB_DEBUG=$(DEBUG)"

tnurb?PPRJ:
		$(MAKE) -ftnurb.mak "SUB_DEBUG=$(DEBUG)"

tquad?Pprj:
		$(MAKE) -ftquad.mak "SUB_DEBUG=$(DEBUG)"

tspeed?Pprj:
		$(MAKE) -ftspeed.mak "SUB_DEBUG=$(DEBUG)"

tsphere?Pprj:
		$(MAKE) -ftsphere.mak "SUB_DEBUG=$(DEBUG)"

tstretch?PPRJ:
		$(MAKE) -ftstretch.mak "SUB_DEBUG=$(DEBUG)"

ttexture?Pprj:
		$(MAKE) -fttexture.mak "SUB_DEBUG=$(DEBUG)"





!IF EXIST (samples.dpd)
!INCLUDE samples.dpd
!ENDIF


