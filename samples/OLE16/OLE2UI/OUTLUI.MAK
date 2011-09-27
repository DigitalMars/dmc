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

PROJ		= OUTLUI
APPTYPE		= WINDOWS DLL
PROJTYPE	= DLL

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

HEADERS		= ole2ui.h ..\..\..\include\windows.h ..\..\..\include\win16\WINDOWS.H  \
		..\..\..\include\shellapi.h ..\..\..\include\win16\shellapi.h ..\..\..\include\ole2.h ..\..\..\include\win16\OLE2.H  \
		..\..\..\include\string.h ..\..\..\include\compobj.h ..\..\..\include\win16\COMPOBJ.H ..\..\..\include\win16\scode.h  \
		..\..\..\include\win16\coguid.h ..\..\..\include\win16\oleguid.h ..\..\..\include\dvobj.h ..\..\..\include\win16\DVOBJ.H  \
		..\..\..\include\storage.h ..\..\..\include\win16\STORAGE.H ..\..\..\include\moniker.h ..\..\..\include\win16\MONIKER.H  \
		..\..\..\include\dlgs.h ..\..\..\include\win16\DLGS.H olestd.h ..\..\..\include\dos.h  \
		..\..\..\include\commdlg.h ..\..\..\include\win16\COMMDLG.H common.h utility.h  \
		busy.h ..\..\..\include\ctype.h ..\..\..\include\toolhelp.h ..\..\..\include\win16\TOOLHELP.H  \
		..\..\..\include\malloc.h ..\..\..\include\stdlib.h ..\..\..\include\heapstat.h geticon.h  \
		regdb.h convert.h ..\..\..\include\stdarg.h ..\..\..\include\stdio.h  \
		..\..\..\include\limits.h dballoc.h uiclass.h enumfetc.h  \
		..\..\..\include\memory.h ..\..\..\include\cderr.h ..\..\..\include\win16\CDERR.H icon.h  \
		iconbox.h ..\..\..\include\direct.h insobj.h resimage.h  \
		edlinks.h msgfiltr.h pastespl.h stdpal.h  \
		suminfo.h ..\..\..\include\variant.h ..\..\..\include\win16\VARIANT.H wn_dos.h  \
		..\..\..\include\print.h ..\..\..\include\win16\PRINT.H 

DEFFILE		= outlui.def

!IF $(DEBUG)
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\bin
!IF EXIST (..\bin)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		= 
SYMS		= 
LIBS		= LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 

CFLAGS		=  -Jm -mlw -C -WD -S -2 -a2 -c -w- -w2 -w6 -w7 -g 
LFLAGS		=  /CO /PAC:65530 /DE /PACKF /XU /A:16 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC  -31 :.\ole2ui.RES
DEFINES		= -D_LIBNAME=OUTLUI -DDLL_VER=1
!ELSE
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\bin
!IF EXIST (..\bin)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		= 
SYMS		= 
LIBS		= LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 

CFLAGS		=  -Jm -mlw -C -o+time -WD -2 -a2 -c -w- -w2 -w6 -w7 -w12 
LFLAGS		=  /PAC:65530 /DE /PACKF /XU /A:16 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC -k -31 :.\ole2ui.RES
DEFINES		= -D_LIBNAME=OUTLUI -DDLL_VER=1
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C 
RESFLAGS	=  
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= L

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I. -Ires\static -Ires\usa

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\busy.OBJ  $(OUTPUTDIR)\common.OBJ  $(OUTPUTDIR)\convert.OBJ  \
		 $(OUTPUTDIR)\dbgutil.OBJ  $(OUTPUTDIR)\dllfuncs.OBJ  $(OUTPUTDIR)\drawicon.OBJ  $(OUTPUTDIR)\enumfetc.OBJ  \
		 $(OUTPUTDIR)\enumstat.OBJ  $(OUTPUTDIR)\geticon.OBJ  $(OUTPUTDIR)\hatch.OBJ  $(OUTPUTDIR)\icon.OBJ  \
		 $(OUTPUTDIR)\iconbox.OBJ  $(OUTPUTDIR)\insobj.OBJ  $(OUTPUTDIR)\links.OBJ  $(OUTPUTDIR)\msgfiltr.OBJ  \
		 $(OUTPUTDIR)\objfdbk.OBJ  $(OUTPUTDIR)\ole2ui.OBJ  $(OUTPUTDIR)\olestd.OBJ  $(OUTPUTDIR)\oleutl.OBJ  \
		 $(OUTPUTDIR)\pastespl.OBJ  $(OUTPUTDIR)\precomp.OBJ  $(OUTPUTDIR)\regdb.OBJ  $(OUTPUTDIR)\resimage.OBJ  \
		 $(OUTPUTDIR)\stdpal.OBJ  $(OUTPUTDIR)\targtdev.OBJ  $(OUTPUTDIR)\utility.OBJ  $(OUTPUTDIR)\dballoc.OBJ  \
		 $(OUTPUTDIR)\suminfo.OBJ 

RCFILES		= $(OUTPUTDIR)\ole2ui.RES

RESFILES	= 

HELPFILES	= 

BATS		= 

PRECOMPILE	= 

POSTCOMPILE	= 

POSTLINK	=  movelib?Pbat 

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
.\busy.OBJ+
.\common.OBJ+
.\convert.OBJ+
.\dbgutil.OBJ+
.\dllfuncs.OBJ+
.\drawicon.OBJ+
.\enumfetc.OBJ+
.\enumstat.OBJ+
.\geticon.OBJ+
.\hatch.OBJ+
.\icon.OBJ+
.\iconbox.OBJ+
.\insobj.OBJ+
.\links.OBJ+
.\msgfiltr.OBJ+
.\objfdbk.OBJ+
.\ole2ui.OBJ+
.\olestd.OBJ+
.\oleutl.OBJ+
.\pastespl.OBJ+
.\precomp.OBJ+
.\regdb.OBJ+
.\resimage.OBJ+
.\stdpal.OBJ+
.\targtdev.OBJ+
.\utility.OBJ+
.\dballoc.OBJ+
.\suminfo.OBJ
..\bin\$$SCW$$.DLL
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
outlui.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\busy.OBJ+
.\common.OBJ+
.\convert.OBJ+
.\dbgutil.OBJ+
.\dllfuncs.OBJ+
.\drawicon.OBJ+
.\enumfetc.OBJ+
.\enumstat.OBJ+
.\geticon.OBJ+
.\hatch.OBJ+
.\icon.OBJ+
.\iconbox.OBJ+
.\insobj.OBJ+
.\links.OBJ+
.\msgfiltr.OBJ+
.\objfdbk.OBJ+
.\ole2ui.OBJ+
.\olestd.OBJ+
.\oleutl.OBJ+
.\pastespl.OBJ+
.\precomp.OBJ+
.\regdb.OBJ+
.\resimage.OBJ+
.\stdpal.OBJ+
.\targtdev.OBJ+
.\utility.OBJ+
.\dballoc.OBJ+
.\suminfo.OBJ
..\bin\$$SCW$$.DLL
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
outlui.def;
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
.\busy.OBJ+
.\common.OBJ+
.\convert.OBJ+
.\dbgutil.OBJ+
.\dllfuncs.OBJ+
.\drawicon.OBJ+
.\enumfetc.OBJ+
.\enumstat.OBJ+
.\geticon.OBJ+
.\hatch.OBJ+
.\icon.OBJ+
.\iconbox.OBJ+
.\insobj.OBJ+
.\links.OBJ+
.\msgfiltr.OBJ+
.\objfdbk.OBJ+
.\ole2ui.OBJ+
.\olestd.OBJ+
.\oleutl.OBJ+
.\pastespl.OBJ+
.\precomp.OBJ+
.\regdb.OBJ+
.\resimage.OBJ+
.\stdpal.OBJ+
.\targtdev.OBJ+
.\utility.OBJ+
.\dballoc.OBJ+
.\suminfo.OBJ
..\bin\$$SCW$$.DLL
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
outlui.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\busy.OBJ+
.\common.OBJ+
.\convert.OBJ+
.\dbgutil.OBJ+
.\dllfuncs.OBJ+
.\drawicon.OBJ+
.\enumfetc.OBJ+
.\enumstat.OBJ+
.\geticon.OBJ+
.\hatch.OBJ+
.\icon.OBJ+
.\iconbox.OBJ+
.\insobj.OBJ+
.\links.OBJ+
.\msgfiltr.OBJ+
.\objfdbk.OBJ+
.\ole2ui.OBJ+
.\olestd.OBJ+
.\oleutl.OBJ+
.\pastespl.OBJ+
.\precomp.OBJ+
.\regdb.OBJ+
.\resimage.OBJ+
.\stdpal.OBJ+
.\targtdev.OBJ+
.\utility.OBJ+
.\dballoc.OBJ+
.\suminfo.OBJ
..\bin\$$SCW$$.DLL
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
outlui.def;
<<
!ENDIF

			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del OUTLUI.dpd
		-del $(OBJS)
		-del $(OUTPUTDIR)\ole2ui.RES

cleanres:
		-del $(OUTPUTDIR)\ole2ui.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\busy.OBJ+
.\common.OBJ+
.\convert.OBJ+
.\dbgutil.OBJ+
.\dllfuncs.OBJ+
.\drawicon.OBJ+
.\enumfetc.OBJ+
.\enumstat.OBJ+
.\geticon.OBJ+
.\hatch.OBJ+
.\icon.OBJ+
.\iconbox.OBJ+
.\insobj.OBJ+
.\links.OBJ+
.\msgfiltr.OBJ+
.\objfdbk.OBJ+
.\ole2ui.OBJ+
.\olestd.OBJ+
.\oleutl.OBJ+
.\pastespl.OBJ+
.\precomp.OBJ+
.\regdb.OBJ+
.\resimage.OBJ+
.\stdpal.OBJ+
.\targtdev.OBJ+
.\utility.OBJ+
.\dballoc.OBJ+
.\suminfo.OBJ
..\bin\$$SCW$$.DLL
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
outlui.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\busy.OBJ+
.\common.OBJ+
.\convert.OBJ+
.\dbgutil.OBJ+
.\dllfuncs.OBJ+
.\drawicon.OBJ+
.\enumfetc.OBJ+
.\enumstat.OBJ+
.\geticon.OBJ+
.\hatch.OBJ+
.\icon.OBJ+
.\iconbox.OBJ+
.\insobj.OBJ+
.\links.OBJ+
.\msgfiltr.OBJ+
.\objfdbk.OBJ+
.\ole2ui.OBJ+
.\olestd.OBJ+
.\oleutl.OBJ+
.\pastespl.OBJ+
.\precomp.OBJ+
.\regdb.OBJ+
.\resimage.OBJ+
.\stdpal.OBJ+
.\targtdev.OBJ+
.\utility.OBJ+
.\dballoc.OBJ+
.\suminfo.OBJ
..\bin\$$SCW$$.DLL
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
outlui.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB





movelib?Pbat:
		..\bin\movelib.bat 



!IF EXIST (OUTLUI.dpd)
!INCLUDE OUTLUI.dpd
!ENDIF



$(OUTPUTDIR)\busy.OBJ:	busy.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\busy.obj busy.c



$(OUTPUTDIR)\common.OBJ:	common.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\common.obj common.c



$(OUTPUTDIR)\convert.OBJ:	convert.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\convert.obj convert.c



$(OUTPUTDIR)\dbgutil.OBJ:	dbgutil.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbgutil.obj dbgutil.c



$(OUTPUTDIR)\dllfuncs.OBJ:	dllfuncs.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dllfuncs.obj dllfuncs.c



$(OUTPUTDIR)\drawicon.OBJ:	drawicon.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\drawicon.obj drawicon.c



$(OUTPUTDIR)\enumfetc.OBJ:	enumfetc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\enumfetc.obj enumfetc.c



$(OUTPUTDIR)\enumstat.OBJ:	enumstat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\enumstat.obj enumstat.c



$(OUTPUTDIR)\geticon.OBJ:	geticon.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\geticon.obj geticon.c



$(OUTPUTDIR)\hatch.OBJ:	hatch.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\hatch.obj hatch.c



$(OUTPUTDIR)\icon.OBJ:	icon.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\icon.obj icon.c



$(OUTPUTDIR)\iconbox.OBJ:	iconbox.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\iconbox.obj iconbox.c



$(OUTPUTDIR)\insobj.OBJ:	insobj.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\insobj.obj insobj.c



$(OUTPUTDIR)\links.OBJ:	links.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\links.obj links.c



$(OUTPUTDIR)\msgfiltr.OBJ:	msgfiltr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\msgfiltr.obj msgfiltr.c



$(OUTPUTDIR)\objfdbk.OBJ:	objfdbk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\objfdbk.obj objfdbk.c



$(OUTPUTDIR)\ole2ui.OBJ:	ole2ui.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ole2ui.obj ole2ui.c



$(OUTPUTDIR)\olestd.OBJ:	olestd.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olestd.obj olestd.c



$(OUTPUTDIR)\oleutl.OBJ:	oleutl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleutl.obj oleutl.c



$(OUTPUTDIR)\pastespl.OBJ:	pastespl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\pastespl.obj pastespl.c



$(OUTPUTDIR)\precomp.OBJ:	precomp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\precomp.obj precomp.c



$(OUTPUTDIR)\regdb.OBJ:	regdb.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\regdb.obj regdb.c



$(OUTPUTDIR)\resimage.OBJ:	resimage.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\resimage.obj resimage.c



$(OUTPUTDIR)\stdpal.OBJ:	stdpal.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stdpal.obj stdpal.c



$(OUTPUTDIR)\targtdev.OBJ:	targtdev.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\targtdev.obj targtdev.c



$(OUTPUTDIR)\utility.OBJ:	utility.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\utility.obj utility.c



$(OUTPUTDIR)\dballoc.OBJ:	dballoc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dballoc.obj dballoc.cpp



$(OUTPUTDIR)\suminfo.OBJ:	suminfo.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\suminfo.obj suminfo.cpp



$(OUTPUTDIR)\ole2ui.RES:	ole2ui.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ole2ui.res ole2ui.rc




