ORIGIN		= Symantec Café
ORIGIN_VER	= Version 7.22
VERSION		= DEBUG

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 1
NDEBUG		= 0
!ENDIF

PROJ		= SFCO42D
APPTYPE		= WIN32 DLL
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

HEADERS		= ..\stdafx.h ..\..\..\include\afx.h ..\..\..\include\32-bit\afx.h  \
		..\..\..\include\afxver_.h ..\..\..\include\32-bit\afxver_.h ..\..\..\include\afxv_w32.h ..\..\..\include\32-bit\afxv_w32.h  \
		..\..\..\..\..\sdkrtl\750b4\include\windows.h ..\..\..\..\..\sdkrtl\750b4\include\win32\scdefs.h ..\..\..\..\..\sdkrtl\750b4\include\win32\windows.h f:\sc\include\excpt.h  \
		f:\sc\include\stdarg.h ..\..\..\..\..\sdkrtl\750b4\include\windef.h ..\..\..\..\..\sdkrtl\750b4\include\win32\windef.h ..\..\..\..\..\sdkrtl\750b4\include\winnt.h  \
		..\..\..\..\..\sdkrtl\750b4\include\win32\winnt.h f:\sc\include\ctype.h ..\..\..\..\..\sdkrtl\750b4\include\pshpack4.h ..\..\..\..\..\sdkrtl\750b4\include\win32\pshpack4.h  \
		..\..\..\..\..\sdkrtl\750b4\include\poppack.h ..\..\..\..\..\sdkrtl\750b4\include\win32\poppack.h ..\..\..\..\..\sdkrtl\750b4\include\win32\pshpack2.h f:\sc\include\string.h  \
		..\..\..\..\..\sdkrtl\750b4\include\winbase.h ..\..\..\..\..\sdkrtl\750b4\include\win32\winbase.h ..\..\..\..\..\sdkrtl\750b4\include\winerror.h ..\..\..\..\..\sdkrtl\750b4\include\win32\winerror.h  \
		..\..\..\..\..\sdkrtl\750b4\include\wingdi.h ..\..\..\..\..\sdkrtl\750b4\include\win32\wingdi.h ..\..\..\..\..\sdkrtl\750b4\include\pshpack1.h ..\..\..\..\..\sdkrtl\750b4\include\win32\pshpack1.h  \
		..\..\..\..\..\sdkrtl\750b4\include\pshpack2.h ..\..\..\..\..\sdkrtl\750b4\include\winuser.h ..\..\..\..\..\sdkrtl\750b4\include\win32\winuser.h 

DEFFILE		= SFCO42D.DEF

!IF $(DEBUG)
OUTPUTDIR	= DLLOD.W
!IF EXIST (DLLOD.W)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\..\..\BIN
!IF EXIST (..\..\..\BIN)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		=  DLLOD.W\stdafx 
SYMS		= $(SYMROOT).SYM 
LIBS		= uuid.lib uuid2.lib SFC42D.LIB  \
		SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -cpp -Ae -Ar -mn -C -D_WINDLL -NL -ND -5 -a8 -c -H -HDDLLOD.W -HO- -wx -w- -w2 -w3 -w7 -w17 -g 
LFLAGS		=  /CO /NOD /NOI /DE /XN /NT /ENTRY:_DllMainCRTStartup /VERS:4.0 /BAS:0x5FA00000 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :DLLOD.W\mfcole.RES
DEFINES		= -D_X86_ -D_DEBUG -D_AFXDLL -D_AFX_OLE_IMPL -D_MT -D_MBCS=1
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= uuid.lib uuid2.lib SFC42D.LIB  \
		SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -mn -o+time -WD -3 -a8 -c 
LFLAGS		=  /NOI /DO /DE /E /PACKF /XN /NT /ENTRY:_DllMainCRTStartup /VERS:1.0 /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :..\mfcole.RES
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

RCDEFINES	= -D_DEBUG

INCLUDES	= -I..\..\..\include

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\dllole.OBJ  $(OUTPUTDIR)\oleinit.OBJ  $(OUTPUTDIR)\olecnvrt.OBJ  \
		 $(OUTPUTDIR)\olemon.OBJ  $(OUTPUTDIR)\oledocvw.OBJ  $(OUTPUTDIR)\oledocob.OBJ  $(OUTPUTDIR)\oledoctg.OBJ  \
		 $(OUTPUTDIR)\oledocip.OBJ  $(OUTPUTDIR)\oleasmon.OBJ  $(OUTPUTDIR)\ctlquick.OBJ  $(OUTPUTDIR)\ctlnownd.OBJ  \
		 $(OUTPUTDIR)\olebar.OBJ  $(OUTPUTDIR)\olecli1.OBJ  $(OUTPUTDIR)\olecli2.OBJ  $(OUTPUTDIR)\olecli3.OBJ  \
		 $(OUTPUTDIR)\oledobj1.OBJ  $(OUTPUTDIR)\oledobj2.OBJ  $(OUTPUTDIR)\oledisp1.OBJ  $(OUTPUTDIR)\oledisp2.OBJ  \
		 $(OUTPUTDIR)\oledlgs1.OBJ  $(OUTPUTDIR)\oledlgs2.OBJ  $(OUTPUTDIR)\oledlgs3.OBJ  $(OUTPUTDIR)\oledata.OBJ  \
		 $(OUTPUTDIR)\oledoc1.OBJ  $(OUTPUTDIR)\oledoc2.OBJ  $(OUTPUTDIR)\oledrop1.OBJ  $(OUTPUTDIR)\oledrop2.OBJ  \
		 $(OUTPUTDIR)\olemsgf.OBJ  $(OUTPUTDIR)\oleenum.OBJ  $(OUTPUTDIR)\olefact.OBJ  $(OUTPUTDIR)\oleipfrm.OBJ  \
		 $(OUTPUTDIR)\olelink.OBJ  $(OUTPUTDIR)\olemisc.OBJ  $(OUTPUTDIR)\olestrm.OBJ  $(OUTPUTDIR)\olesvr1.OBJ  \
		 $(OUTPUTDIR)\olesvr2.OBJ  $(OUTPUTDIR)\olereg.OBJ  $(OUTPUTDIR)\oletsvr.OBJ  $(OUTPUTDIR)\oleui1.OBJ  \
		 $(OUTPUTDIR)\oleui2.OBJ  $(OUTPUTDIR)\oleunk.OBJ  $(OUTPUTDIR)\oleverb.OBJ  $(OUTPUTDIR)\olecall.OBJ  \
		 $(OUTPUTDIR)\olevar.OBJ  $(OUTPUTDIR)\viewrich.OBJ  $(OUTPUTDIR)\oledll.OBJ  $(OUTPUTDIR)\oletyplb.OBJ  \
		 $(OUTPUTDIR)\occmgr.OBJ  $(OUTPUTDIR)\occevent.OBJ  $(OUTPUTDIR)\occcont.OBJ  $(OUTPUTDIR)\occsite.OBJ  \
		 $(OUTPUTDIR)\occlock.OBJ  $(OUTPUTDIR)\occddx.OBJ  $(OUTPUTDIR)\occddxf.OBJ  $(OUTPUTDIR)\occdlg.OBJ  \
		 $(OUTPUTDIR)\ctlcache.OBJ  $(OUTPUTDIR)\ctlcore.OBJ  $(OUTPUTDIR)\ctlconn.OBJ  $(OUTPUTDIR)\ctldata.OBJ  \
		 $(OUTPUTDIR)\ctlevent.OBJ  $(OUTPUTDIR)\ctlmodul.OBJ  $(OUTPUTDIR)\ctlframe.OBJ  $(OUTPUTDIR)\ctlfont.OBJ  \
		 $(OUTPUTDIR)\ctlinplc.OBJ  $(OUTPUTDIR)\ctllic.OBJ  $(OUTPUTDIR)\oleconn.OBJ  $(OUTPUTDIR)\ctlobj.OBJ  \
		 $(OUTPUTDIR)\ctlpict.OBJ  $(OUTPUTDIR)\ctlpropx.OBJ  $(OUTPUTDIR)\ctlppg.OBJ  $(OUTPUTDIR)\ctlprop.OBJ  \
		 $(OUTPUTDIR)\ctlpset.OBJ  $(OUTPUTDIR)\ctlpstg.OBJ  $(OUTPUTDIR)\ctlpstm.OBJ  $(OUTPUTDIR)\ctlrefl.OBJ  \
		 $(OUTPUTDIR)\ctlreg.OBJ  $(OUTPUTDIR)\ctltrack.OBJ  $(OUTPUTDIR)\ctlview.OBJ  $(OUTPUTDIR)\olepset.OBJ  \
		 $(OUTPUTDIR)\ctlpbag.OBJ  $(OUTPUTDIR)\olepro32.OBJ  $(OUTPUTDIR)\ppgcolor.OBJ  $(OUTPUTDIR)\ppgfont.OBJ  \
		 $(OUTPUTDIR)\ppgpict.OBJ  $(OUTPUTDIR)\ppgstock.OBJ  $(OUTPUTDIR)\ctlinl.OBJ 

RCFILES		= $(OUTPUTDIR)\mfcole.RES

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oDLLOD.W\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oDLLOD.W\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oDLLOD.W\$(*B).sym $*.hxx

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
DLLOD.W\stdafx.PCO+
DLLOD.W\dllole.OBJ+
DLLOD.W\oleinit.OBJ+
DLLOD.W\olecnvrt.OBJ+
DLLOD.W\olemon.OBJ+
DLLOD.W\oledocvw.OBJ+
DLLOD.W\oledocob.OBJ+
DLLOD.W\oledoctg.OBJ+
DLLOD.W\oledocip.OBJ+
DLLOD.W\oleasmon.OBJ+
DLLOD.W\ctlquick.OBJ+
DLLOD.W\ctlnownd.OBJ+
DLLOD.W\olebar.OBJ+
DLLOD.W\olecli1.OBJ+
DLLOD.W\olecli2.OBJ+
DLLOD.W\olecli3.OBJ+
DLLOD.W\oledobj1.OBJ+
DLLOD.W\oledobj2.OBJ+
DLLOD.W\oledisp1.OBJ+
DLLOD.W\oledisp2.OBJ+
DLLOD.W\oledlgs1.OBJ+
DLLOD.W\oledlgs2.OBJ+
DLLOD.W\oledlgs3.OBJ+
DLLOD.W\oledata.OBJ+
DLLOD.W\oledoc1.OBJ+
DLLOD.W\oledoc2.OBJ+
DLLOD.W\oledrop1.OBJ+
DLLOD.W\oledrop2.OBJ+
DLLOD.W\olemsgf.OBJ+
DLLOD.W\oleenum.OBJ+
DLLOD.W\olefact.OBJ+
DLLOD.W\oleipfrm.OBJ+
DLLOD.W\olelink.OBJ+
DLLOD.W\olemisc.OBJ+
DLLOD.W\olestrm.OBJ+
DLLOD.W\olesvr1.OBJ+
DLLOD.W\olesvr2.OBJ+
DLLOD.W\olereg.OBJ+
DLLOD.W\oletsvr.OBJ+
DLLOD.W\oleui1.OBJ+
DLLOD.W\oleui2.OBJ+
DLLOD.W\oleunk.OBJ+
DLLOD.W\oleverb.OBJ+
DLLOD.W\olecall.OBJ+
DLLOD.W\olevar.OBJ+
DLLOD.W\viewrich.OBJ+
DLLOD.W\oledll.OBJ+
DLLOD.W\oletyplb.OBJ+
DLLOD.W\occmgr.OBJ+
DLLOD.W\occevent.OBJ+
DLLOD.W\occcont.OBJ+
DLLOD.W\occsite.OBJ+
DLLOD.W\occlock.OBJ+
DLLOD.W\occddx.OBJ+
DLLOD.W\occddxf.OBJ+
DLLOD.W\occdlg.OBJ+
DLLOD.W\ctlcache.OBJ+
DLLOD.W\ctlcore.OBJ+
DLLOD.W\ctlconn.OBJ+
DLLOD.W\ctldata.OBJ+
DLLOD.W\ctlevent.OBJ+
DLLOD.W\ctlmodul.OBJ+
DLLOD.W\ctlframe.OBJ+
DLLOD.W\ctlfont.OBJ+
DLLOD.W\ctlinplc.OBJ+
DLLOD.W\ctllic.OBJ+
DLLOD.W\oleconn.OBJ+
DLLOD.W\ctlobj.OBJ+
DLLOD.W\ctlpict.OBJ+
DLLOD.W\ctlpropx.OBJ+
DLLOD.W\ctlppg.OBJ+
DLLOD.W\ctlprop.OBJ+
DLLOD.W\ctlpset.OBJ+
DLLOD.W\ctlpstg.OBJ+
DLLOD.W\ctlpstm.OBJ+
DLLOD.W\ctlrefl.OBJ+
DLLOD.W\ctlreg.OBJ+
DLLOD.W\ctltrack.OBJ+
DLLOD.W\ctlview.OBJ+
DLLOD.W\olepset.OBJ+
DLLOD.W\ctlpbag.OBJ+
DLLOD.W\olepro32.OBJ+
DLLOD.W\ppgcolor.OBJ+
DLLOD.W\ppgfont.OBJ+
DLLOD.W\ppgpict.OBJ+
DLLOD.W\ppgstock.OBJ+
DLLOD.W\ctlinl.OBJ
..\..\..\BIN\$$SCW$$.DLL
NUL
..\..\..\lib\ uuid.lib uuid2.lib SFC42D.LIB SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
SFCO42D.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
..\dllole.OBJ+
..\oleinit.OBJ+
..\olecnvrt.OBJ+
..\olemon.OBJ+
..\oledocvw.OBJ+
..\oledocob.OBJ+
..\oledoctg.OBJ+
..\oledocip.OBJ+
..\oleasmon.OBJ+
..\ctlquick.OBJ+
..\ctlnownd.OBJ+
..\olebar.OBJ+
..\olecli1.OBJ+
..\olecli2.OBJ+
..\olecli3.OBJ+
..\oledobj1.OBJ+
..\oledobj2.OBJ+
..\oledisp1.OBJ+
..\oledisp2.OBJ+
..\oledlgs1.OBJ+
..\oledlgs2.OBJ+
..\oledlgs3.OBJ+
..\oledata.OBJ+
..\oledoc1.OBJ+
..\oledoc2.OBJ+
..\oledrop1.OBJ+
..\oledrop2.OBJ+
..\olemsgf.OBJ+
..\oleenum.OBJ+
..\olefact.OBJ+
..\oleipfrm.OBJ+
..\olelink.OBJ+
..\olemisc.OBJ+
..\olestrm.OBJ+
..\olesvr1.OBJ+
..\olesvr2.OBJ+
..\olereg.OBJ+
..\oletsvr.OBJ+
..\oleui1.OBJ+
..\oleui2.OBJ+
..\oleunk.OBJ+
..\oleverb.OBJ+
..\olecall.OBJ+
..\olevar.OBJ+
..\viewrich.OBJ+
..\oledll.OBJ+
..\oletyplb.OBJ+
..\occmgr.OBJ+
..\occevent.OBJ+
..\occcont.OBJ+
..\occsite.OBJ+
..\occlock.OBJ+
..\occddx.OBJ+
..\occddxf.OBJ+
..\occdlg.OBJ+
..\ctlcache.OBJ+
..\ctlcore.OBJ+
..\ctlconn.OBJ+
..\ctldata.OBJ+
..\ctlevent.OBJ+
..\ctlmodul.OBJ+
..\ctlframe.OBJ+
..\ctlfont.OBJ+
..\ctlinplc.OBJ+
..\ctllic.OBJ+
..\oleconn.OBJ+
..\ctlobj.OBJ+
..\ctlpict.OBJ+
..\ctlpropx.OBJ+
..\ctlppg.OBJ+
..\ctlprop.OBJ+
..\ctlpset.OBJ+
..\ctlpstg.OBJ+
..\ctlpstm.OBJ+
..\ctlrefl.OBJ+
..\ctlreg.OBJ+
..\ctltrack.OBJ+
..\ctlview.OBJ+
..\olepset.OBJ+
..\ctlpbag.OBJ+
..\olepro32.OBJ+
..\ppgcolor.OBJ+
..\ppgfont.OBJ+
..\ppgpict.OBJ+
..\ppgstock.OBJ+
..\ctlinl.OBJ
$$SCW$$.DLL
NUL
uuid.lib uuid2.lib SFC42D.LIB SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
SFCO42D.DEF;
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
DLLOD.W\stdafx.PCO+
DLLOD.W\dllole.OBJ+
DLLOD.W\oleinit.OBJ+
DLLOD.W\olecnvrt.OBJ+
DLLOD.W\olemon.OBJ+
DLLOD.W\oledocvw.OBJ+
DLLOD.W\oledocob.OBJ+
DLLOD.W\oledoctg.OBJ+
DLLOD.W\oledocip.OBJ+
DLLOD.W\oleasmon.OBJ+
DLLOD.W\ctlquick.OBJ+
DLLOD.W\ctlnownd.OBJ+
DLLOD.W\olebar.OBJ+
DLLOD.W\olecli1.OBJ+
DLLOD.W\olecli2.OBJ+
DLLOD.W\olecli3.OBJ+
DLLOD.W\oledobj1.OBJ+
DLLOD.W\oledobj2.OBJ+
DLLOD.W\oledisp1.OBJ+
DLLOD.W\oledisp2.OBJ+
DLLOD.W\oledlgs1.OBJ+
DLLOD.W\oledlgs2.OBJ+
DLLOD.W\oledlgs3.OBJ+
DLLOD.W\oledata.OBJ+
DLLOD.W\oledoc1.OBJ+
DLLOD.W\oledoc2.OBJ+
DLLOD.W\oledrop1.OBJ+
DLLOD.W\oledrop2.OBJ+
DLLOD.W\olemsgf.OBJ+
DLLOD.W\oleenum.OBJ+
DLLOD.W\olefact.OBJ+
DLLOD.W\oleipfrm.OBJ+
DLLOD.W\olelink.OBJ+
DLLOD.W\olemisc.OBJ+
DLLOD.W\olestrm.OBJ+
DLLOD.W\olesvr1.OBJ+
DLLOD.W\olesvr2.OBJ+
DLLOD.W\olereg.OBJ+
DLLOD.W\oletsvr.OBJ+
DLLOD.W\oleui1.OBJ+
DLLOD.W\oleui2.OBJ+
DLLOD.W\oleunk.OBJ+
DLLOD.W\oleverb.OBJ+
DLLOD.W\olecall.OBJ+
DLLOD.W\olevar.OBJ+
DLLOD.W\viewrich.OBJ+
DLLOD.W\oledll.OBJ+
DLLOD.W\oletyplb.OBJ+
DLLOD.W\occmgr.OBJ+
DLLOD.W\occevent.OBJ+
DLLOD.W\occcont.OBJ+
DLLOD.W\occsite.OBJ+
DLLOD.W\occlock.OBJ+
DLLOD.W\occddx.OBJ+
DLLOD.W\occddxf.OBJ+
DLLOD.W\occdlg.OBJ+
DLLOD.W\ctlcache.OBJ+
DLLOD.W\ctlcore.OBJ+
DLLOD.W\ctlconn.OBJ+
DLLOD.W\ctldata.OBJ+
DLLOD.W\ctlevent.OBJ+
DLLOD.W\ctlmodul.OBJ+
DLLOD.W\ctlframe.OBJ+
DLLOD.W\ctlfont.OBJ+
DLLOD.W\ctlinplc.OBJ+
DLLOD.W\ctllic.OBJ+
DLLOD.W\oleconn.OBJ+
DLLOD.W\ctlobj.OBJ+
DLLOD.W\ctlpict.OBJ+
DLLOD.W\ctlpropx.OBJ+
DLLOD.W\ctlppg.OBJ+
DLLOD.W\ctlprop.OBJ+
DLLOD.W\ctlpset.OBJ+
DLLOD.W\ctlpstg.OBJ+
DLLOD.W\ctlpstm.OBJ+
DLLOD.W\ctlrefl.OBJ+
DLLOD.W\ctlreg.OBJ+
DLLOD.W\ctltrack.OBJ+
DLLOD.W\ctlview.OBJ+
DLLOD.W\olepset.OBJ+
DLLOD.W\ctlpbag.OBJ+
DLLOD.W\olepro32.OBJ+
DLLOD.W\ppgcolor.OBJ+
DLLOD.W\ppgfont.OBJ+
DLLOD.W\ppgpict.OBJ+
DLLOD.W\ppgstock.OBJ+
DLLOD.W\ctlinl.OBJ
..\..\..\BIN\$$SCW$$.DLL
NUL
..\..\..\lib\ uuid.lib uuid2.lib SFC42D.LIB SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
SFCO42D.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
..\dllole.OBJ+
..\oleinit.OBJ+
..\olecnvrt.OBJ+
..\olemon.OBJ+
..\oledocvw.OBJ+
..\oledocob.OBJ+
..\oledoctg.OBJ+
..\oledocip.OBJ+
..\oleasmon.OBJ+
..\ctlquick.OBJ+
..\ctlnownd.OBJ+
..\olebar.OBJ+
..\olecli1.OBJ+
..\olecli2.OBJ+
..\olecli3.OBJ+
..\oledobj1.OBJ+
..\oledobj2.OBJ+
..\oledisp1.OBJ+
..\oledisp2.OBJ+
..\oledlgs1.OBJ+
..\oledlgs2.OBJ+
..\oledlgs3.OBJ+
..\oledata.OBJ+
..\oledoc1.OBJ+
..\oledoc2.OBJ+
..\oledrop1.OBJ+
..\oledrop2.OBJ+
..\olemsgf.OBJ+
..\oleenum.OBJ+
..\olefact.OBJ+
..\oleipfrm.OBJ+
..\olelink.OBJ+
..\olemisc.OBJ+
..\olestrm.OBJ+
..\olesvr1.OBJ+
..\olesvr2.OBJ+
..\olereg.OBJ+
..\oletsvr.OBJ+
..\oleui1.OBJ+
..\oleui2.OBJ+
..\oleunk.OBJ+
..\oleverb.OBJ+
..\olecall.OBJ+
..\olevar.OBJ+
..\viewrich.OBJ+
..\oledll.OBJ+
..\oletyplb.OBJ+
..\occmgr.OBJ+
..\occevent.OBJ+
..\occcont.OBJ+
..\occsite.OBJ+
..\occlock.OBJ+
..\occddx.OBJ+
..\occddxf.OBJ+
..\occdlg.OBJ+
..\ctlcache.OBJ+
..\ctlcore.OBJ+
..\ctlconn.OBJ+
..\ctldata.OBJ+
..\ctlevent.OBJ+
..\ctlmodul.OBJ+
..\ctlframe.OBJ+
..\ctlfont.OBJ+
..\ctlinplc.OBJ+
..\ctllic.OBJ+
..\oleconn.OBJ+
..\ctlobj.OBJ+
..\ctlpict.OBJ+
..\ctlpropx.OBJ+
..\ctlppg.OBJ+
..\ctlprop.OBJ+
..\ctlpset.OBJ+
..\ctlpstg.OBJ+
..\ctlpstm.OBJ+
..\ctlrefl.OBJ+
..\ctlreg.OBJ+
..\ctltrack.OBJ+
..\ctlview.OBJ+
..\olepset.OBJ+
..\ctlpbag.OBJ+
..\olepro32.OBJ+
..\ppgcolor.OBJ+
..\ppgfont.OBJ+
..\ppgpict.OBJ+
..\ppgstock.OBJ+
..\ctlinl.OBJ
$$SCW$$.DLL
NUL
uuid.lib uuid2.lib SFC42D.LIB SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
SFCO42D.DEF;
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
		-del $(TARGETDIR)\$(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del SFCO42D.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del $(OUTPUTDIR)\mfcole.RES

cleanres:
		-del $(OUTPUTDIR)\mfcole.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
DLLOD.W\stdafx.PCO+
DLLOD.W\dllole.OBJ+
DLLOD.W\oleinit.OBJ+
DLLOD.W\olecnvrt.OBJ+
DLLOD.W\olemon.OBJ+
DLLOD.W\oledocvw.OBJ+
DLLOD.W\oledocob.OBJ+
DLLOD.W\oledoctg.OBJ+
DLLOD.W\oledocip.OBJ+
DLLOD.W\oleasmon.OBJ+
DLLOD.W\ctlquick.OBJ+
DLLOD.W\ctlnownd.OBJ+
DLLOD.W\olebar.OBJ+
DLLOD.W\olecli1.OBJ+
DLLOD.W\olecli2.OBJ+
DLLOD.W\olecli3.OBJ+
DLLOD.W\oledobj1.OBJ+
DLLOD.W\oledobj2.OBJ+
DLLOD.W\oledisp1.OBJ+
DLLOD.W\oledisp2.OBJ+
DLLOD.W\oledlgs1.OBJ+
DLLOD.W\oledlgs2.OBJ+
DLLOD.W\oledlgs3.OBJ+
DLLOD.W\oledata.OBJ+
DLLOD.W\oledoc1.OBJ+
DLLOD.W\oledoc2.OBJ+
DLLOD.W\oledrop1.OBJ+
DLLOD.W\oledrop2.OBJ+
DLLOD.W\olemsgf.OBJ+
DLLOD.W\oleenum.OBJ+
DLLOD.W\olefact.OBJ+
DLLOD.W\oleipfrm.OBJ+
DLLOD.W\olelink.OBJ+
DLLOD.W\olemisc.OBJ+
DLLOD.W\olestrm.OBJ+
DLLOD.W\olesvr1.OBJ+
DLLOD.W\olesvr2.OBJ+
DLLOD.W\olereg.OBJ+
DLLOD.W\oletsvr.OBJ+
DLLOD.W\oleui1.OBJ+
DLLOD.W\oleui2.OBJ+
DLLOD.W\oleunk.OBJ+
DLLOD.W\oleverb.OBJ+
DLLOD.W\olecall.OBJ+
DLLOD.W\olevar.OBJ+
DLLOD.W\viewrich.OBJ+
DLLOD.W\oledll.OBJ+
DLLOD.W\oletyplb.OBJ+
DLLOD.W\occmgr.OBJ+
DLLOD.W\occevent.OBJ+
DLLOD.W\occcont.OBJ+
DLLOD.W\occsite.OBJ+
DLLOD.W\occlock.OBJ+
DLLOD.W\occddx.OBJ+
DLLOD.W\occddxf.OBJ+
DLLOD.W\occdlg.OBJ+
DLLOD.W\ctlcache.OBJ+
DLLOD.W\ctlcore.OBJ+
DLLOD.W\ctlconn.OBJ+
DLLOD.W\ctldata.OBJ+
DLLOD.W\ctlevent.OBJ+
DLLOD.W\ctlmodul.OBJ+
DLLOD.W\ctlframe.OBJ+
DLLOD.W\ctlfont.OBJ+
DLLOD.W\ctlinplc.OBJ+
DLLOD.W\ctllic.OBJ+
DLLOD.W\oleconn.OBJ+
DLLOD.W\ctlobj.OBJ+
DLLOD.W\ctlpict.OBJ+
DLLOD.W\ctlpropx.OBJ+
DLLOD.W\ctlppg.OBJ+
DLLOD.W\ctlprop.OBJ+
DLLOD.W\ctlpset.OBJ+
DLLOD.W\ctlpstg.OBJ+
DLLOD.W\ctlpstm.OBJ+
DLLOD.W\ctlrefl.OBJ+
DLLOD.W\ctlreg.OBJ+
DLLOD.W\ctltrack.OBJ+
DLLOD.W\ctlview.OBJ+
DLLOD.W\olepset.OBJ+
DLLOD.W\ctlpbag.OBJ+
DLLOD.W\olepro32.OBJ+
DLLOD.W\ppgcolor.OBJ+
DLLOD.W\ppgfont.OBJ+
DLLOD.W\ppgpict.OBJ+
DLLOD.W\ppgstock.OBJ+
DLLOD.W\ctlinl.OBJ
..\..\..\BIN\$$SCW$$.DLL
NUL
..\..\..\lib\ uuid.lib uuid2.lib SFC42D.LIB SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
SFCO42D.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
..\dllole.OBJ+
..\oleinit.OBJ+
..\olecnvrt.OBJ+
..\olemon.OBJ+
..\oledocvw.OBJ+
..\oledocob.OBJ+
..\oledoctg.OBJ+
..\oledocip.OBJ+
..\oleasmon.OBJ+
..\ctlquick.OBJ+
..\ctlnownd.OBJ+
..\olebar.OBJ+
..\olecli1.OBJ+
..\olecli2.OBJ+
..\olecli3.OBJ+
..\oledobj1.OBJ+
..\oledobj2.OBJ+
..\oledisp1.OBJ+
..\oledisp2.OBJ+
..\oledlgs1.OBJ+
..\oledlgs2.OBJ+
..\oledlgs3.OBJ+
..\oledata.OBJ+
..\oledoc1.OBJ+
..\oledoc2.OBJ+
..\oledrop1.OBJ+
..\oledrop2.OBJ+
..\olemsgf.OBJ+
..\oleenum.OBJ+
..\olefact.OBJ+
..\oleipfrm.OBJ+
..\olelink.OBJ+
..\olemisc.OBJ+
..\olestrm.OBJ+
..\olesvr1.OBJ+
..\olesvr2.OBJ+
..\olereg.OBJ+
..\oletsvr.OBJ+
..\oleui1.OBJ+
..\oleui2.OBJ+
..\oleunk.OBJ+
..\oleverb.OBJ+
..\olecall.OBJ+
..\olevar.OBJ+
..\viewrich.OBJ+
..\oledll.OBJ+
..\oletyplb.OBJ+
..\occmgr.OBJ+
..\occevent.OBJ+
..\occcont.OBJ+
..\occsite.OBJ+
..\occlock.OBJ+
..\occddx.OBJ+
..\occddxf.OBJ+
..\occdlg.OBJ+
..\ctlcache.OBJ+
..\ctlcore.OBJ+
..\ctlconn.OBJ+
..\ctldata.OBJ+
..\ctlevent.OBJ+
..\ctlmodul.OBJ+
..\ctlframe.OBJ+
..\ctlfont.OBJ+
..\ctlinplc.OBJ+
..\ctllic.OBJ+
..\oleconn.OBJ+
..\ctlobj.OBJ+
..\ctlpict.OBJ+
..\ctlpropx.OBJ+
..\ctlppg.OBJ+
..\ctlprop.OBJ+
..\ctlpset.OBJ+
..\ctlpstg.OBJ+
..\ctlpstm.OBJ+
..\ctlrefl.OBJ+
..\ctlreg.OBJ+
..\ctltrack.OBJ+
..\ctlview.OBJ+
..\olepset.OBJ+
..\ctlpbag.OBJ+
..\olepro32.OBJ+
..\ppgcolor.OBJ+
..\ppgfont.OBJ+
..\ppgpict.OBJ+
..\ppgstock.OBJ+
..\ctlinl.OBJ
$$SCW$$.DLL
NUL
uuid.lib uuid2.lib SFC42D.LIB SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
SFCO42D.DEF;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB





movelib?Pbat:
		movelib.bat 



!IF EXIST (SFCO42D.dpd)
!INCLUDE SFCO42D.dpd
!ENDIF



$(OUTPUTDIR)\mfcole.RES:	..\mfcole.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mfcole.res ..\mfcole.rc



$(OUTPUTDIR)\dllole.OBJ:	..\dllole.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dllole.obj ..\dllole.cpp



$(OUTPUTDIR)\oleinit.OBJ:	..\oleinit.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleinit.obj ..\oleinit.cpp



$(OUTPUTDIR)\olecnvrt.OBJ:	..\olecnvrt.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olecnvrt.obj ..\olecnvrt.cpp



$(OUTPUTDIR)\olemon.OBJ:	..\olemon.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olemon.obj ..\olemon.cpp



$(OUTPUTDIR)\oledocvw.OBJ:	..\oledocvw.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledocvw.obj ..\oledocvw.cpp



$(OUTPUTDIR)\oledocob.OBJ:	..\oledocob.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledocob.obj ..\oledocob.cpp



$(OUTPUTDIR)\oledoctg.OBJ:	..\oledoctg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledoctg.obj ..\oledoctg.cpp



$(OUTPUTDIR)\oledocip.OBJ:	..\oledocip.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledocip.obj ..\oledocip.cpp



$(OUTPUTDIR)\oleasmon.OBJ:	..\oleasmon.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleasmon.obj ..\oleasmon.cpp



$(OUTPUTDIR)\ctlquick.OBJ:	..\ctlquick.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlquick.obj ..\ctlquick.cpp



$(OUTPUTDIR)\ctlnownd.OBJ:	..\ctlnownd.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlnownd.obj ..\ctlnownd.cpp



$(OUTPUTDIR)\olebar.OBJ:	..\olebar.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olebar.obj ..\olebar.cpp



$(OUTPUTDIR)\olecli1.OBJ:	..\olecli1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olecli1.obj ..\olecli1.cpp



$(OUTPUTDIR)\olecli2.OBJ:	..\olecli2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olecli2.obj ..\olecli2.cpp



$(OUTPUTDIR)\olecli3.OBJ:	..\olecli3.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olecli3.obj ..\olecli3.cpp



$(OUTPUTDIR)\oledobj1.OBJ:	..\oledobj1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledobj1.obj ..\oledobj1.cpp



$(OUTPUTDIR)\oledobj2.OBJ:	..\oledobj2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledobj2.obj ..\oledobj2.cpp



$(OUTPUTDIR)\oledisp1.OBJ:	..\oledisp1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledisp1.obj ..\oledisp1.cpp



$(OUTPUTDIR)\oledisp2.OBJ:	..\oledisp2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledisp2.obj ..\oledisp2.cpp



$(OUTPUTDIR)\oledlgs1.OBJ:	..\oledlgs1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledlgs1.obj ..\oledlgs1.cpp



$(OUTPUTDIR)\oledlgs2.OBJ:	..\oledlgs2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledlgs2.obj ..\oledlgs2.cpp



$(OUTPUTDIR)\oledlgs3.OBJ:	..\oledlgs3.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledlgs3.obj ..\oledlgs3.cpp



$(OUTPUTDIR)\oledata.OBJ:	..\oledata.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledata.obj ..\oledata.cpp



$(OUTPUTDIR)\oledoc1.OBJ:	..\oledoc1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledoc1.obj ..\oledoc1.cpp



$(OUTPUTDIR)\oledoc2.OBJ:	..\oledoc2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledoc2.obj ..\oledoc2.cpp



$(OUTPUTDIR)\oledrop1.OBJ:	..\oledrop1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledrop1.obj ..\oledrop1.cpp



$(OUTPUTDIR)\oledrop2.OBJ:	..\oledrop2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledrop2.obj ..\oledrop2.cpp



$(OUTPUTDIR)\olemsgf.OBJ:	..\olemsgf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olemsgf.obj ..\olemsgf.cpp



$(OUTPUTDIR)\oleenum.OBJ:	..\oleenum.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleenum.obj ..\oleenum.cpp



$(OUTPUTDIR)\olefact.OBJ:	..\olefact.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olefact.obj ..\olefact.cpp



$(OUTPUTDIR)\oleipfrm.OBJ:	..\oleipfrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleipfrm.obj ..\oleipfrm.cpp



$(OUTPUTDIR)\olelink.OBJ:	..\olelink.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olelink.obj ..\olelink.cpp



$(OUTPUTDIR)\olemisc.OBJ:	..\olemisc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olemisc.obj ..\olemisc.cpp



$(OUTPUTDIR)\olestrm.OBJ:	..\olestrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olestrm.obj ..\olestrm.cpp



$(OUTPUTDIR)\olesvr1.OBJ:	..\olesvr1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olesvr1.obj ..\olesvr1.cpp



$(OUTPUTDIR)\olesvr2.OBJ:	..\olesvr2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olesvr2.obj ..\olesvr2.cpp



$(OUTPUTDIR)\olereg.OBJ:	..\olereg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olereg.obj ..\olereg.cpp



$(OUTPUTDIR)\oletsvr.OBJ:	..\oletsvr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oletsvr.obj ..\oletsvr.cpp



$(OUTPUTDIR)\oleui1.OBJ:	..\oleui1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleui1.obj ..\oleui1.cpp



$(OUTPUTDIR)\oleui2.OBJ:	..\oleui2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleui2.obj ..\oleui2.cpp



$(OUTPUTDIR)\oleunk.OBJ:	..\oleunk.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleunk.obj ..\oleunk.cpp



$(OUTPUTDIR)\oleverb.OBJ:	..\oleverb.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleverb.obj ..\oleverb.cpp



$(OUTPUTDIR)\olecall.OBJ:	..\olecall.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olecall.obj ..\olecall.cpp



$(OUTPUTDIR)\olevar.OBJ:	..\olevar.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olevar.obj ..\olevar.cpp



$(OUTPUTDIR)\viewrich.OBJ:	..\viewrich.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\viewrich.obj ..\viewrich.cpp



$(OUTPUTDIR)\oledll.OBJ:	..\oledll.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledll.obj ..\oledll.cpp



$(OUTPUTDIR)\oletyplb.OBJ:	..\oletyplb.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oletyplb.obj ..\oletyplb.cpp



$(OUTPUTDIR)\occmgr.OBJ:	..\occmgr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occmgr.obj ..\occmgr.cpp



$(OUTPUTDIR)\occevent.OBJ:	..\occevent.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occevent.obj ..\occevent.cpp



$(OUTPUTDIR)\occcont.OBJ:	..\occcont.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occcont.obj ..\occcont.cpp



$(OUTPUTDIR)\occsite.OBJ:	..\occsite.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occsite.obj ..\occsite.cpp



$(OUTPUTDIR)\occlock.OBJ:	..\occlock.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occlock.obj ..\occlock.cpp



$(OUTPUTDIR)\occddx.OBJ:	..\occddx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occddx.obj ..\occddx.cpp



$(OUTPUTDIR)\occddxf.OBJ:	..\occddxf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occddxf.obj ..\occddxf.cpp



$(OUTPUTDIR)\occdlg.OBJ:	..\occdlg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occdlg.obj ..\occdlg.cpp



$(OUTPUTDIR)\ctlcache.OBJ:	..\ctlcache.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlcache.obj ..\ctlcache.cpp



$(OUTPUTDIR)\ctlcore.OBJ:	..\ctlcore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlcore.obj ..\ctlcore.cpp



$(OUTPUTDIR)\ctlconn.OBJ:	..\ctlconn.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlconn.obj ..\ctlconn.cpp



$(OUTPUTDIR)\ctldata.OBJ:	..\ctldata.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctldata.obj ..\ctldata.cpp



$(OUTPUTDIR)\ctlevent.OBJ:	..\ctlevent.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlevent.obj ..\ctlevent.cpp



$(OUTPUTDIR)\ctlmodul.OBJ:	..\ctlmodul.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlmodul.obj ..\ctlmodul.cpp



$(OUTPUTDIR)\ctlframe.OBJ:	..\ctlframe.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlframe.obj ..\ctlframe.cpp



$(OUTPUTDIR)\ctlfont.OBJ:	..\ctlfont.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlfont.obj ..\ctlfont.cpp



$(OUTPUTDIR)\ctlinplc.OBJ:	..\ctlinplc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlinplc.obj ..\ctlinplc.cpp



$(OUTPUTDIR)\ctllic.OBJ:	..\ctllic.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctllic.obj ..\ctllic.cpp



$(OUTPUTDIR)\oleconn.OBJ:	..\oleconn.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleconn.obj ..\oleconn.cpp



$(OUTPUTDIR)\ctlobj.OBJ:	..\ctlobj.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlobj.obj ..\ctlobj.cpp



$(OUTPUTDIR)\ctlpict.OBJ:	..\ctlpict.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlpict.obj ..\ctlpict.cpp



$(OUTPUTDIR)\ctlpropx.OBJ:	..\ctlpropx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlpropx.obj ..\ctlpropx.cpp



$(OUTPUTDIR)\ctlppg.OBJ:	..\ctlppg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlppg.obj ..\ctlppg.cpp



$(OUTPUTDIR)\ctlprop.OBJ:	..\ctlprop.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlprop.obj ..\ctlprop.cpp



$(OUTPUTDIR)\ctlpset.OBJ:	..\ctlpset.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlpset.obj ..\ctlpset.cpp



$(OUTPUTDIR)\ctlpstg.OBJ:	..\ctlpstg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlpstg.obj ..\ctlpstg.cpp



$(OUTPUTDIR)\ctlpstm.OBJ:	..\ctlpstm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlpstm.obj ..\ctlpstm.cpp



$(OUTPUTDIR)\ctlrefl.OBJ:	..\ctlrefl.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlrefl.obj ..\ctlrefl.cpp



$(OUTPUTDIR)\ctlreg.OBJ:	..\ctlreg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlreg.obj ..\ctlreg.cpp



$(OUTPUTDIR)\ctltrack.OBJ:	..\ctltrack.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctltrack.obj ..\ctltrack.cpp



$(OUTPUTDIR)\ctlview.OBJ:	..\ctlview.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlview.obj ..\ctlview.cpp



$(OUTPUTDIR)\olepset.OBJ:	..\olepset.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olepset.obj ..\olepset.cpp



$(OUTPUTDIR)\ctlpbag.OBJ:	..\ctlpbag.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlpbag.obj ..\ctlpbag.cpp



$(OUTPUTDIR)\olepro32.OBJ:	..\olepro32.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olepro32.obj ..\olepro32.cpp



$(OUTPUTDIR)\ppgcolor.OBJ:	..\ppgcolor.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ppgcolor.obj ..\ppgcolor.cpp



$(OUTPUTDIR)\ppgfont.OBJ:	..\ppgfont.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ppgfont.obj ..\ppgfont.cpp



$(OUTPUTDIR)\ppgpict.OBJ:	..\ppgpict.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ppgpict.obj ..\ppgpict.cpp



$(OUTPUTDIR)\ppgstock.OBJ:	..\ppgstock.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ppgstock.obj ..\ppgstock.cpp



$(OUTPUTDIR)\ctlinl.OBJ:	..\ctlinl.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlinl.obj ..\ctlinl.cpp


$(SYMROOT).SYM: ..\stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO ..\stdafx.h



