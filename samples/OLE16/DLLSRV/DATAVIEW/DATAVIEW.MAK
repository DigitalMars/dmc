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

PROJ		= DATAVIEW
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

HEADERS		= dataview.h ..\..\..\..\include\windows.h ..\..\..\..\include\win16\WINDOWS.H  \
		..\..\..\..\include\ole2.h ..\..\..\..\include\win16\OLE2.H ..\..\..\..\include\string.h ..\..\..\..\include\compobj.h  \
		..\..\..\..\include\win16\COMPOBJ.H ..\..\..\..\include\win16\scode.h ..\..\..\..\include\win16\coguid.h ..\..\..\..\include\win16\oleguid.h  \
		..\..\..\..\include\dvobj.h ..\..\..\..\include\win16\DVOBJ.H ..\..\..\..\include\storage.h ..\..\..\..\include\win16\STORAGE.H  \
		..\..\..\..\include\moniker.h ..\..\..\..\include\win16\MONIKER.H ..\..\..\..\include\initguid.h ..\..\..\..\include\win16\initguid.h  \
		stockdlg.h stockutl.h ..\..\..\..\include\stdlib.h ..\prgidmon.h  \
		..\..\..\..\include\olestd.h ..\..\..\..\include\win16\OLESTD.H ..\..\..\..\include\dos.h ..\..\..\..\include\commdlg.h  \
		..\..\..\..\include\win16\COMMDLG.H ..\..\..\..\include\shellapi.h ..\..\..\..\include\win16\shellapi.h ..\..\..\..\include\ole2ui.h  \
		..\..\..\..\include\win16\OLE2UI.H ..\..\..\..\include\dlgs.h ..\..\..\..\include\win16\DLGS.H ..\..\..\..\include\stdio.h  \
		resource.h ..\..\..\..\include\ctype.h ..\..\ole2ui\olestd.h ..\..\ole2ui\ole2ui.h 

DEFFILE		= dataview.def

!IF $(DEBUG)
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\..\bin
!IF EXIST (..\..\bin)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		= 
SYMS		= 
LIBS		= loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 

CFLAGS		=  -mlw -C -WD -S -2 -a2 -c -w- -w2 -w6 -w7 -g 
LFLAGS		=  /CO /PAC:65530 /DE /PACKF /XU /A:16 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC  -31 :.\stockrc.RES
DEFINES		= -D_DEBUG=1
!ELSE
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\..\bin
!IF EXIST (..\..\bin)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		= 
SYMS		= 
LIBS		= loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 

CFLAGS		=  -mlw -o+time -WD -2 -a2 -c -w- -w2 -w7 
LFLAGS		=  /DE /E /PACKF /REO /WI /XU /A:16 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC  -31 :.\stockrc.RES
DEFINES		= 
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

INCLUDES	= -I. -I..\..\ole2ui\res\static -I..\..\ole2ui\res\usa

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\dataview.OBJ  $(OUTPUTDIR)\dvcache.OBJ  $(OUTPUTDIR)\dvrun.OBJ  \
		 $(OUTPUTDIR)\dvstock.OBJ  $(OUTPUTDIR)\libmain.OBJ  $(OUTPUTDIR)\stockdlg.OBJ  $(OUTPUTDIR)\stockutl.OBJ 

RCFILES		= $(OUTPUTDIR)\stockrc.RES

RESFILES	= 

HELPFILES	= 

PROJS		= ..\prgidmon.prj 

BATS		= 

PRECOMPILE	=  ?P?P?Bprgidmon?Pprj 

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
.\dataview.OBJ+
.\dvcache.OBJ+
.\dvrun.OBJ+
.\dvstock.OBJ+
.\libmain.OBJ+
.\stockdlg.OBJ+
.\stockutl.OBJ
..\..\bin\$$SCW$$.DLL
NUL
..\..\lib\ loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
dataview.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\dataview.OBJ+
.\dvcache.OBJ+
.\dvrun.OBJ+
.\dvstock.OBJ+
.\libmain.OBJ+
.\stockdlg.OBJ+
.\stockutl.OBJ
..\..\bin\$$SCW$$.DLL
NUL
..\..\lib\ loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
dataview.def;
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
.\dataview.OBJ+
.\dvcache.OBJ+
.\dvrun.OBJ+
.\dvstock.OBJ+
.\libmain.OBJ+
.\stockdlg.OBJ+
.\stockutl.OBJ
..\..\bin\$$SCW$$.DLL
NUL
..\..\lib\ loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
dataview.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\dataview.OBJ+
.\dvcache.OBJ+
.\dvrun.OBJ+
.\dvstock.OBJ+
.\libmain.OBJ+
.\stockdlg.OBJ+
.\stockutl.OBJ
..\..\bin\$$SCW$$.DLL
NUL
..\..\lib\ loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
dataview.def;
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
		-del DATAVIEW.dpd
		-del $(OBJS)
		-del $(OUTPUTDIR)\stockrc.RES
		cd ..\.
		$(MAKE) -fprgidmon.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:
		-del $(OUTPUTDIR)\stockrc.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\dataview.OBJ+
.\dvcache.OBJ+
.\dvrun.OBJ+
.\dvstock.OBJ+
.\libmain.OBJ+
.\stockdlg.OBJ+
.\stockutl.OBJ
..\..\bin\$$SCW$$.DLL
NUL
..\..\lib\ loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
dataview.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\dataview.OBJ+
.\dvcache.OBJ+
.\dvrun.OBJ+
.\dvstock.OBJ+
.\libmain.OBJ+
.\stockdlg.OBJ+
.\stockutl.OBJ
..\..\bin\$$SCW$$.DLL
NUL
..\..\lib\ loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
dataview.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB



?P?P?Bprgidmon?Pprj:
		cd ..\.
		$(MAKE) -fprgidmon.mak "SUB_DEBUG=$(DEBUG)"





movelib?Pbat:
		..\..\bin\movelib.bat 



!IF EXIST (DATAVIEW.dpd)
!INCLUDE DATAVIEW.dpd
!ENDIF



$(OUTPUTDIR)\dataview.OBJ:	dataview.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dataview.obj dataview.cpp



$(OUTPUTDIR)\dvcache.OBJ:	dvcache.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dvcache.obj dvcache.cpp



$(OUTPUTDIR)\dvrun.OBJ:	dvrun.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dvrun.obj dvrun.cpp



$(OUTPUTDIR)\dvstock.OBJ:	dvstock.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dvstock.obj dvstock.cpp



$(OUTPUTDIR)\libmain.OBJ:	libmain.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\libmain.obj libmain.cpp



$(OUTPUTDIR)\stockdlg.OBJ:	stockdlg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stockdlg.obj stockdlg.cpp



$(OUTPUTDIR)\stockutl.OBJ:	stockutl.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stockutl.obj stockutl.cpp



$(OUTPUTDIR)\stockrc.RES:	stockrc.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stockrc.res stockrc.rc




