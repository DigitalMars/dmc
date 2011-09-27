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

PROJ		= SIMPSVR
APPTYPE		= WINDOWS EXE
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

HEADERS		= pre.h ..\..\..\include\windows.h ..\..\..\include\win16\WINDOWS.H  \
		..\..\..\include\ole2.h ..\..\..\include\win16\OLE2.H ..\..\..\include\string.h ..\..\..\include\compobj.h  \
		..\..\..\include\win16\COMPOBJ.H ..\..\..\include\win16\scode.h ..\..\..\include\win16\coguid.h ..\..\..\include\win16\oleguid.h  \
		..\..\..\include\dvobj.h ..\..\..\include\win16\DVOBJ.H ..\..\..\include\storage.h ..\..\..\include\win16\STORAGE.H  \
		..\..\..\include\moniker.h ..\..\..\include\win16\MONIKER.H ..\..\..\include\ole2ui.h ..\..\..\include\win16\OLE2UI.H  \
		..\..\..\include\shellapi.h ..\..\..\include\win16\shellapi.h ..\..\..\include\dlgs.h ..\..\..\include\win16\DLGS.H  \
		..\..\..\include\win16\olestd.h ..\..\..\include\dos.h ..\..\..\include\commdlg.h ..\..\..\include\win16\COMMDLG.H  \
		..\..\..\include\assert.h simpsvr.h resource.h ..\..\..\include\ole2ver.h  \
		..\..\..\include\win16\OLE2VER.H obj.h ioipao.h ioipo.h  \
		ioo.h ips.h ido.h iec.h  \
		app.h doc.h icf.h ..\..\..\include\initguid.h  \
		..\..\..\include\win16\initguid.h ..\..\..\include\math.h ..\ole2ui\olestd.h ..\ole2ui\ole2ui.h 

DEFFILE		= simpsvr.def

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

SYMROOT		=  .\pre 
SYMS		= $(SYMROOT).SYM 
LIBS		= loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 

CFLAGS		=  -cpp -HO -ml -C -WA -S -2 -a2 -c -H -w- -w2 -w6 -w7 -g 
LFLAGS		=  /CO /PAC:65530 /DE /PACKF /XU /A:16 /RC  -31 :.\simpsvr.RES
DEFINES		= -D_DEBUG=1
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

SYMROOT		=  .\pre 
SYMS		= $(SYMROOT).SYM 
LIBS		= loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 

CFLAGS		=  -cpp -HO -ml -C -o+time -WA -2 -a2 -c -H -w- -w2 -w6 -w7 
LFLAGS		=  /PAC:65530 /DE /PACKF /XU /A:16 /RC  -31 :.\simpsvr.RES
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

INCLUDES	= -I. -I..\ole2ui\res\static -I..\ole2ui\res\usa

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\app.OBJ  $(OUTPUTDIR)\doc.OBJ  $(OUTPUTDIR)\icf.OBJ  \
		 $(OUTPUTDIR)\ido.OBJ  $(OUTPUTDIR)\iec.OBJ  $(OUTPUTDIR)\ioipao.OBJ  $(OUTPUTDIR)\ioipo.OBJ  \
		 $(OUTPUTDIR)\ioo.OBJ  $(OUTPUTDIR)\ips.OBJ  $(OUTPUTDIR)\obj.OBJ  $(OUTPUTDIR)\pre.OBJ  \
		 $(OUTPUTDIR)\simpsvr.OBJ 

RCFILES		= $(OUTPUTDIR)\simpsvr.RES

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o.\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o.\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o.\$(*B).sym $*.hxx

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
.\pre.PCO+
.\app.OBJ+
.\doc.OBJ+
.\icf.OBJ+
.\ido.OBJ+
.\iec.OBJ+
.\ioipao.OBJ+
.\ioipo.OBJ+
.\ioo.OBJ+
.\ips.OBJ+
.\obj.OBJ+
.\pre.OBJ+
.\simpsvr.OBJ
..\bin\$$SCW$$.EXE
NUL
..\lib\ loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
simpsvr.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\pre.PCO+
.\app.OBJ+
.\doc.OBJ+
.\icf.OBJ+
.\ido.OBJ+
.\iec.OBJ+
.\ioipao.OBJ+
.\ioipo.OBJ+
.\ioo.OBJ+
.\ips.OBJ+
.\obj.OBJ+
.\pre.OBJ+
.\simpsvr.OBJ
..\bin\$$SCW$$.EXE
NUL
..\lib\ loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
simpsvr.def;
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
	
$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) $(DEFFILE)
			-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\pre.PCO+
.\app.OBJ+
.\doc.OBJ+
.\icf.OBJ+
.\ido.OBJ+
.\iec.OBJ+
.\ioipao.OBJ+
.\ioipo.OBJ+
.\ioo.OBJ+
.\ips.OBJ+
.\obj.OBJ+
.\pre.OBJ+
.\simpsvr.OBJ
..\bin\$$SCW$$.EXE
NUL
..\lib\ loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
simpsvr.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\pre.PCO+
.\app.OBJ+
.\doc.OBJ+
.\icf.OBJ+
.\ido.OBJ+
.\iec.OBJ+
.\ioipao.OBJ+
.\ioipo.OBJ+
.\ioo.OBJ+
.\ips.OBJ+
.\obj.OBJ+
.\pre.OBJ+
.\simpsvr.OBJ
..\bin\$$SCW$$.EXE
NUL
..\lib\ loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
simpsvr.def;
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
		-del SIMPSVR.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del $(OUTPUTDIR)\simpsvr.RES

cleanres:
		-del $(OUTPUTDIR)\simpsvr.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\pre.PCO+
.\app.OBJ+
.\doc.OBJ+
.\icf.OBJ+
.\ido.OBJ+
.\iec.OBJ+
.\ioipao.OBJ+
.\ioipo.OBJ+
.\ioo.OBJ+
.\ips.OBJ+
.\obj.OBJ+
.\pre.OBJ+
.\simpsvr.OBJ
..\bin\$$SCW$$.EXE
NUL
..\lib\ loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
simpsvr.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\pre.PCO+
.\app.OBJ+
.\doc.OBJ+
.\icf.OBJ+
.\ido.OBJ+
.\iec.OBJ+
.\ioipao.OBJ+
.\ioipo.OBJ+
.\ioo.OBJ+
.\ips.OBJ+
.\obj.OBJ+
.\pre.OBJ+
.\simpsvr.OBJ
..\bin\$$SCW$$.EXE
NUL
..\lib\ loleuic.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
simpsvr.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (SIMPSVR.dpd)
!INCLUDE SIMPSVR.dpd
!ENDIF



$(OUTPUTDIR)\app.OBJ:	app.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\app.obj app.cpp



$(OUTPUTDIR)\doc.OBJ:	doc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\doc.obj doc.cpp



$(OUTPUTDIR)\icf.OBJ:	icf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\icf.obj icf.cpp



$(OUTPUTDIR)\ido.OBJ:	ido.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ido.obj ido.cpp



$(OUTPUTDIR)\iec.OBJ:	iec.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\iec.obj iec.cpp



$(OUTPUTDIR)\ioipao.OBJ:	ioipao.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ioipao.obj ioipao.cpp



$(OUTPUTDIR)\ioipo.OBJ:	ioipo.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ioipo.obj ioipo.cpp



$(OUTPUTDIR)\ioo.OBJ:	ioo.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ioo.obj ioo.cpp



$(OUTPUTDIR)\ips.OBJ:	ips.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ips.obj ips.cpp



$(OUTPUTDIR)\obj.OBJ:	obj.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\obj.obj obj.cpp



$(OUTPUTDIR)\pre.OBJ:	pre.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\pre.obj pre.cpp



$(OUTPUTDIR)\simpsvr.OBJ:	simpsvr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\simpsvr.obj simpsvr.cpp



$(OUTPUTDIR)\simpsvr.RES:	simpsvr.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\simpsvr.res simpsvr.rc


$(SYMROOT).SYM: pre.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO pre.h



