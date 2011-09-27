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

PROJ		= snddcpp
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

HEADERS		= ..\..\..\..\..\sc\INCLUDE\stdlib.h ..\..\..\..\..\sc\INCLUDE\new.h ..\..\..\..\..\sc\INCLUDE\stddef.h  \
		..\..\..\..\..\sc\INCLUDE\iostream.h ..\..\..\..\..\sc\INCLUDE\string.h ..\..\..\..\..\sc\INCLUDE\complex.h ..\..\..\..\..\sc\INCLUDE\math.h  \
		..\..\..\..\..\sc\INCLUDE\fltpnt.h ..\..\..\..\..\sc\INCLUDE\fp.h ..\..\..\..\..\sc\INCLUDE\typeinfo.h ..\ios\filesys.h  \
		..\..\..\..\..\sc\INCLUDE\stdio.h ..\..\..\..\..\sc\INCLUDE\fcntl.h ..\..\..\..\..\sc\INCLUDE\fstream.h ..\..\..\..\..\sc\INCLUDE\share.h  \
		..\..\..\..\..\sc\INCLUDE\sys\stat.h ..\..\..\..\..\sc\INCLUDE\ctype.h ..\..\..\..\..\sc\INCLUDE\strstrea.h ..\..\..\..\..\sc\INCLUDE\iomanip.h  \
		..\..\..\..\..\sc\INCLUDE\generic.h ..\..\..\..\..\sc\INCLUDE\stdiostr.h ..\..\..\..\..\sc\INCLUDE\stream.h ..\..\..\..\..\sc\INCLUDE\limits.h  \
		..\..\INCLUDE\stdlib.h ..\..\INCLUDE\new.h ..\..\INCLUDE\stddef.h ..\..\INCLUDE\iostream.h  \
		..\..\INCLUDE\string.h ..\..\INCLUDE\complex.h ..\..\INCLUDE\math.h ..\..\INCLUDE\fltpnt.h  \
		..\..\INCLUDE\fp.h ..\INCLUDE\ehsup.h ..\..\..\..\..\sc\INCLUDE\windows.h ..\..\..\..\..\sc\INCLUDE\win32\scdefs.h  \
		..\..\..\..\..\sc\INCLUDE\win32\windows.h ..\..\INCLUDE\excpt.h ..\..\INCLUDE\stdarg.h ..\..\..\..\..\sc\INCLUDE\windef.h  \
		..\..\..\..\..\sc\INCLUDE\win32\windef.h ..\..\..\..\..\sc\INCLUDE\winnt.h ..\..\..\..\..\sc\INCLUDE\win32\winnt.h ..\..\INCLUDE\ctype.h  \
		..\..\..\..\..\sc\INCLUDE\pshpack4.h ..\..\..\..\..\sc\INCLUDE\win32\pshpack4.h ..\..\..\..\..\sc\INCLUDE\poppack.h ..\..\..\..\..\sc\INCLUDE\win32\poppack.h  \
		..\..\..\..\..\sc\INCLUDE\win32\pshpack1.h ..\..\..\..\..\sc\INCLUDE\winbase.h ..\..\..\..\..\sc\INCLUDE\win32\winbase.h ..\..\..\..\..\sc\INCLUDE\winerror.h  \
		..\..\..\..\..\sc\INCLUDE\win32\winerror.h ..\..\..\..\..\sc\INCLUDE\wingdi.h ..\..\..\..\..\sc\INCLUDE\win32\wingdi.h ..\..\..\..\..\sc\INCLUDE\pshpack1.h  \
		..\..\..\..\..\sc\INCLUDE\pshpack2.h ..\..\..\..\..\sc\INCLUDE\win32\pshpack2.h ..\..\..\..\..\sc\INCLUDE\winuser.h ..\..\..\..\..\sc\INCLUDE\win32\winuser.h  \
		..\..\..\..\..\sc\INCLUDE\winnls.h ..\..\..\..\..\sc\INCLUDE\win32\winnls.h ..\..\..\..\..\sc\INCLUDE\wincon.h ..\..\..\..\..\sc\INCLUDE\win32\wincon.h  \
		..\..\..\..\..\sc\INCLUDE\winver.h ..\..\..\..\..\sc\INCLUDE\win32\winver.h ..\..\..\..\..\sc\INCLUDE\winreg.h ..\..\..\..\..\sc\INCLUDE\win32\winreg.h  \
		..\..\..\..\..\sc\INCLUDE\winnetwk.h ..\..\..\..\..\sc\INCLUDE\win32\winnetwk.h ..\..\..\..\..\sc\INCLUDE\cderr.h ..\..\..\..\..\sc\INCLUDE\win32\cderr.h  \
		..\..\..\..\..\sc\INCLUDE\dde.h ..\..\..\..\..\sc\INCLUDE\win32\dde.h ..\..\..\..\..\sc\INCLUDE\ddeml.h ..\..\..\..\..\sc\INCLUDE\win32\ddeml.h  \
		..\..\..\..\..\sc\INCLUDE\dlgs.h ..\..\..\..\..\sc\INCLUDE\win32\dlgs.h ..\..\..\..\..\sc\INCLUDE\lzexpand.h ..\..\..\..\..\sc\INCLUDE\win32\lzexpand.h  \
		..\..\..\..\..\sc\INCLUDE\mmsystem.h ..\..\..\..\..\sc\INCLUDE\win32\mmsystem.h ..\..\..\..\..\sc\INCLUDE\nb30.h ..\..\..\..\..\sc\INCLUDE\win32\nb30.h  \
		..\..\..\..\..\sc\INCLUDE\rpc.h ..\..\..\..\..\sc\INCLUDE\win32\rpc.h ..\..\..\..\..\sc\INCLUDE\rpcbase.h ..\..\..\..\..\sc\INCLUDE\win32\rpcbase.h  \
		..\..\..\..\..\sc\INCLUDE\rpcdce.h ..\..\..\..\..\sc\INCLUDE\win32\rpcdce.h ..\..\..\..\..\sc\INCLUDE\rpcdcep.h ..\..\..\..\..\sc\INCLUDE\win32\rpcdcep.h  \
		..\..\..\..\..\sc\INCLUDE\rpcnsi.h ..\..\..\..\..\sc\INCLUDE\win32\rpcnsi.h ..\..\..\..\..\sc\INCLUDE\rpcnterr.h ..\..\..\..\..\sc\INCLUDE\win32\rpcnterr.h  \
		..\..\..\..\..\sc\INCLUDE\shellapi.h ..\..\..\..\..\sc\INCLUDE\win32\shellapi.h ..\..\..\..\..\sc\INCLUDE\winperf.h ..\..\..\..\..\sc\INCLUDE\win32\winperf.h  \
		..\..\..\..\..\sc\INCLUDE\winsock.h ..\..\..\..\..\sc\INCLUDE\win32\winsock.h ..\..\..\..\..\sc\INCLUDE\commdlg.h ..\..\..\..\..\sc\INCLUDE\win32\commdlg.h  \
		..\..\..\..\..\sc\INCLUDE\winspool.h ..\..\..\..\..\sc\INCLUDE\win32\winspool.h ..\..\..\..\..\sc\INCLUDE\prsht.h ..\..\..\..\..\sc\INCLUDE\win32\prsht.h  \
		..\..\..\..\..\sc\INCLUDE\ole2.h ..\..\..\..\..\sc\INCLUDE\win32\ole2.h ..\..\..\..\..\sc\INCLUDE\pshpack8.h ..\..\..\..\..\sc\INCLUDE\win32\pshpack8.h  \
		..\..\..\..\..\sc\INCLUDE\objbase.h ..\..\..\..\..\sc\INCLUDE\win32\objbase.h ..\..\..\..\..\sc\INCLUDE\rpcndr.h ..\..\..\..\..\sc\INCLUDE\win32\rpcndr.h  \
		..\..\..\..\..\sc\INCLUDE\rpcnsip.h ..\..\..\..\..\sc\INCLUDE\win32\rpcnsip.h ..\..\..\..\..\sc\INCLUDE\wtypes.h ..\..\..\..\..\sc\INCLUDE\win32\wtypes.h  \
		..\..\..\..\..\SC\INCLUDE\win32\unknwn.h ..\..\INCLUDE\limits.h ..\..\INCLUDE\stream.h ..\..\INCLUDE\stdiostr.h  \
		..\..\INCLUDE\generic.h ..\..\INCLUDE\iomanip.h ..\..\INCLUDE\strstrea.h ..\..\INCLUDE\sys\stat.h  \
		..\..\INCLUDE\share.h ..\..\INCLUDE\typeinfo.h ..\..\INCLUDE\fstream.h ..\..\INCLUDE\fcntl.h  \
		..\..\..\..\..\SC\INCLUDE\win32\imm.h ..\..\..\..\..\SC\INCLUDE\imm.h ..\..\..\..\..\SC\INCLUDE\win32\mcx.h ..\..\..\..\..\SC\INCLUDE\mcx.h  \
		..\..\..\..\..\SC\INCLUDE\win32\winsvc.h ..\..\..\..\..\SC\INCLUDE\winsvc.h ..\..\..\..\..\SC\INCLUDE\win32\oleidl.h ..\..\..\..\..\SC\INCLUDE\oleidl.h  \
		..\..\..\..\..\SC\INCLUDE\win32\oaidl.h ..\..\..\..\..\SC\INCLUDE\oaidl.h ..\..\..\..\..\SC\INCLUDE\win32\oleauto.h ..\..\..\..\..\SC\INCLUDE\oleauto.h  \
		..\..\..\..\..\SC\INCLUDE\win32\cguid.h ..\..\..\..\..\SC\INCLUDE\cguid.h ..\..\..\..\..\SC\INCLUDE\win32\objidl.h ..\..\..\..\..\SC\INCLUDE\objidl.h  \
		..\..\..\..\..\SC\INCLUDE\unknwn.h ..\..\INCLUDE\stdio.h ..\..\INCLUDE\heapstat.h 

DEFFILE		= snddcpp.DEF

!IF $(DEBUG)
O	= SNDDCPP
!IF EXIST (SNDDCPP)
CREATEO	=
!ELSE
CREATEO	= if not exist $(O)\*.* md $(O)
!ENDIF
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		=  SNDDCPP\precomp 
SYMS		= $(SYMROOT).SYM 
LIBS		= 

CFLAGS		=  -Ae -Ar -r -mn -NL -ND -5 -a8 -c -H -HDSNDDCPP -HO- -wx -w- -w12 -g 
CFLAGS_1	=  -Ae -Ar -Aa -r -mn -NL -ND -5 -a8 -c -H -HDSNDDCPP -HO- -wx -w- -w12 -g
DEFINES_1	= -D_MT -D_MBCS -D_DEBUG -D_CRTBLD -D_DLL
CFLAGS_2	=  -Ae -Ar -Aa -r -mn -NL -ND -5 -a8 -c -H -HDSNDDCPP -HO- -wx -w- -w12 -g
DEFINES_2	= -D_MT -D_MBCS -D_DEBUG -D_CRTBLD -D_DLL
CFLAGS_3	=  -Ae -Ar -Aa -r -mn -NL -ND -5 -a8 -c -H -HDSNDDCPP -HO- -wx -w- -w12 -g
DEFINES_3	= -D_MT -D_MBCS -D_DEBUG -D_CRTBLD -D_DLL
LFLAGS		=  /CO /NOI /DE /A:512
DEFINES		= -D_MT -D_DLL -D_DEBUG -D_CRTBLD -D_DLL -D_MBCS=1
!ELSE
O	= .
CREATEO	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= 

CFLAGS		=  -mn -o+time -D_WINDOWS -3 -a8 -c 
CFLAGS_1	=  -mn -o+time -D_WINDOWS -3 -a8 -c
DEFINES_1	= 
CFLAGS_2	=  -mn -o+time -D_WINDOWS -3 -a8 -c
DEFINES_2	= 
CFLAGS_3	=  -mn -o+time -D_WINDOWS -3 -a8 -c
DEFINES_3	= 
LFLAGS		=  /NOI /DE /E /PACKF /A:512
DEFINES		= -D_MBCS=1
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

INCLUDES	= -I..\INCLUDE -I..\..\INCLUDE

INCLUDEDOBJS	= 

OBJS		=  $(O)\_delnear.OBJ  $(O)\_delete2.OBJ  $(O)\_delete3.OBJ  $(O)\_handler.OBJ  $(O)\_new.OBJ  \
		 $(O)\_new2.OBJ  $(O)\_new3.OBJ  $(O)\_newnear.OBJ  $(O)\complex.OBJ  $(O)\coperat.OBJ  \
		 $(O)\corenew.OBJ  $(O)\cstrm.OBJ  $(O)\ctrig.OBJ  $(O)\dbp.OBJ  \
		 $(O)\ehdata.OBJ  $(O)\fstream.OBJ  $(O)\generr.OBJ  $(O)\ios.OBJ  \
		 $(O)\iostream.OBJ  $(O)\istream.OBJ  $(O)\istreamf.OBJ  $(O)\istreami.OBJ  \
		 $(O)\istreamx.OBJ  $(O)\manip.OBJ  $(O)\ostream.OBJ  $(O)\ostreamf.OBJ  \
		 $(O)\ostreami.OBJ  $(O)\ostreamx.OBJ  $(O)\siosync.OBJ  $(O)\stdiostr.OBJ  \
		 $(O)\stream.OBJ  $(O)\streambf.OBJ  $(O)\strstrea.OBJ  $(O)\typeinfo.OBJ  \
		 $(O)\vecnew.OBJ  $(O)\vector.OBJ  $(O)\adelete.OBJ  $(O)\adelete2.OBJ  $(O)\adelete3.OBJ  $(O)\nothrow.OBJ  $(O)\anew.OBJ  $(O)\anew3.OBJ  \
		 $(O)\anew2.OBJ 

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oSNDDCPP\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oSNDDCPP\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oSNDDCPP\$(*B).sym $*.hxx

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
SNDDCPP\precomp.PCO+
SNDDCPP\_delnear.OBJ+
SNDDCPP\_delete2.OBJ+
SNDDCPP\_delete3.OBJ+
SNDDCPP\_handler.OBJ+
SNDDCPP\_new.OBJ+
SNDDCPP\_new2.OBJ+
SNDDCPP\_new3.OBJ+
SNDDCPP\_newnear.OBJ+
SNDDCPP\complex.OBJ+
SNDDCPP\coperat.OBJ+
SNDDCPP\corenew.OBJ+
SNDDCPP\cstrm.OBJ+
SNDDCPP\ctrig.OBJ+
SNDDCPP\dbp.OBJ+
SNDDCPP\ehdata.OBJ+
SNDDCPP\fstream.OBJ+
SNDDCPP\generr.OBJ+
SNDDCPP\ios.OBJ+
SNDDCPP\iostream.OBJ+
SNDDCPP\istream.OBJ+
SNDDCPP\istreamf.OBJ+
SNDDCPP\istreami.OBJ+
SNDDCPP\istreamx.OBJ+
SNDDCPP\manip.OBJ+
SNDDCPP\ostream.OBJ+
SNDDCPP\ostreamf.OBJ+
SNDDCPP\ostreami.OBJ+
SNDDCPP\ostreamx.OBJ+
SNDDCPP\siosync.OBJ+
SNDDCPP\stdiostr.OBJ+
SNDDCPP\stream.OBJ+
SNDDCPP\streambf.OBJ+
SNDDCPP\strstrea.OBJ+
SNDDCPP\typeinfo.OBJ+
SNDDCPP\vecnew.OBJ+
SNDDCPP\vector.OBJ+
SNDDCPP\adelete.OBJ+
SNDDCPP\adelete2.OBJ+
SNDDCPP\adelete3.OBJ+
SNDDCPP\nothrow.OBJ+
SNDDCPP\anew.OBJ+
SNDDCPP\anew2.OBJ+
SNDDCPP\anew3.OBJ
$$SCW$$.LIB
NUL

snddcpp.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
..\core\_delnear.OBJ+
..\core\_delete2.OBJ+
..\core\_delete3.OBJ+
..\core\_handler.OBJ+
..\core\_new.OBJ+
..\core\_new2.OBJ+
..\core\_new3.OBJ+
..\core\_newnear.OBJ+
..\core\complex.OBJ+
..\core\coperat.OBJ+
..\core\corenew.OBJ+
..\core\cstrm.OBJ+
..\core\ctrig.OBJ+
..\ios\dbp.OBJ+
..\core\ehdata.OBJ+
..\ios\fstream.OBJ+
..\ios\generr.OBJ+
..\ios\ios.OBJ+
..\ios\iostream.OBJ+
..\ios\istream.OBJ+
..\ios\istreamf.OBJ+
..\ios\istreami.OBJ+
..\ios\istreamx.OBJ+
..\ios\manip.OBJ+
..\ios\ostream.OBJ+
..\ios\ostreamf.OBJ+
..\ios\ostreami.OBJ+
..\ios\ostreamx.OBJ+
..\ios\siosync.OBJ+
..\ios\stdiostr.OBJ+
..\ios\stream.OBJ+
..\ios\streambf.OBJ+
..\ios\strstrea.OBJ+
..\core\typeinfo.OBJ+
..\core\vecnew.OBJ+
..\core\vector.OBJ+
..\core\adelete.OBJ+
..\core\adelete2.OBJ+
..\core\adelete3.OBJ+
..\core\nothrow.OBJ+
..\core\anew.OBJ+
..\core\anew2.OBJ+
..\core\anew3.OBJ
$$SCW$$.LIB
NUL

snddcpp.DEF;
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
	REM Output to $(O)

createdir:
	$(CREATEO)
	$(CREATETARGETDIR)
	
$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) 
			-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 + $(O)\_delnear.OBJ + $(O)\_delete2.OBJ + $(O)\_delete3.OBJ + $(O)\_handler.OBJ + $(O)\_new.OBJ &
		 + $(O)\_new2.OBJ + $(O)\_new3.OBJ + $(O)\_newnear.OBJ + $(O)\complex.OBJ + $(O)\coperat.OBJ &
		 + $(O)\corenew.OBJ + $(O)\cstrm.OBJ + $(O)\ctrig.OBJ + $(O)\dbp.OBJ &
		 + $(O)\ehdata.OBJ + $(O)\fstream.OBJ + $(O)\generr.OBJ + $(O)\ios.OBJ &
		 + $(O)\iostream.OBJ + $(O)\istream.OBJ + $(O)\istreamf.OBJ + $(O)\istreami.OBJ &
		 + $(O)\istreamx.OBJ + $(O)\manip.OBJ + $(O)\ostream.OBJ + $(O)\ostreamf.OBJ &
		 + $(O)\ostreami.OBJ + $(O)\ostreamx.OBJ + $(O)\siosync.OBJ + $(O)\stdiostr.OBJ &
		 + $(O)\stream.OBJ + $(O)\streambf.OBJ + $(O)\strstrea.OBJ + $(O)\typeinfo.OBJ &
		 + $(O)\vecnew.OBJ + $(O)\vector.OBJ + $(O)\adelete.OBJ + $(O)\adelete2.OBJ + $(O)\adelete3.OBJ + $(O)\nothrow.OBJ + $(O)\anew.OBJ + $(O)\anew3.OBJ &
		 + $(O)\anew2.OBJ
<<
			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(TARGETDIR)\$(PROJ).CLE
		-del $(O)\SCPH.SYM
		-del snddcpp.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO

cleanres:

res:		cleanres $(RCFILES) all


link:
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 + $(O)\_delnear.OBJ + $(O)\_delete2.OBJ + $(O)\_delete3.OBJ + $(O)\_handler.OBJ + $(O)\_new.OBJ &
		 + $(O)\_new2.OBJ + $(O)\_new3.OBJ + $(O)\_newnear.OBJ + $(O)\complex.OBJ + $(O)\coperat.OBJ &
		 + $(O)\corenew.OBJ + $(O)\cstrm.OBJ + $(O)\ctrig.OBJ + $(O)\dbp.OBJ &
		 + $(O)\ehdata.OBJ + $(O)\fstream.OBJ + $(O)\generr.OBJ + $(O)\ios.OBJ &
		 + $(O)\iostream.OBJ + $(O)\istream.OBJ + $(O)\istreamf.OBJ + $(O)\istreami.OBJ &
		 + $(O)\istreamx.OBJ + $(O)\manip.OBJ + $(O)\ostream.OBJ + $(O)\ostreamf.OBJ &
		 + $(O)\ostreami.OBJ + $(O)\ostreamx.OBJ + $(O)\siosync.OBJ + $(O)\stdiostr.OBJ &
		 + $(O)\stream.OBJ + $(O)\streambf.OBJ + $(O)\strstrea.OBJ + $(O)\typeinfo.OBJ &
		 + $(O)\vecnew.OBJ + $(O)\vector.OBJ + $(O)\adelete.OBJ + $(O)\adelete2.OBJ + $(O)\adelete3.OBJ + $(O)\nothrow.OBJ + $(O)\anew.OBJ + $(O)\anew3.OBJ &
		 + $(O)\anew2.OBJ
<<
		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (snddcpp.dpd)
!INCLUDE snddcpp.dpd
!ENDIF



$(O)\_delnear.OBJ:	..\core\_delnear.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_delnear.obj ..\core\_delnear.cpp



$(O)\_delete2.OBJ:	..\core\_delete2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_delete2.obj ..\core\_delete2.cpp



$(O)\_delete3.OBJ:	..\core\_delete3.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_delete3.obj ..\core\_delete3.cpp



$(O)\_handler.OBJ:	..\core\_handler.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_handler.obj ..\core\_handler.cpp



$(O)\_new.OBJ:	..\core\_new.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_new.obj ..\core\_new.cpp



$(O)\_new2.OBJ:	..\core\_new2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_new2.obj ..\core\_new2.cpp



$(O)\_new3.OBJ:	..\core\_new3.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_new3.obj ..\core\_new3.cpp



$(O)\_newnear.OBJ:	..\core\_newnear.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_newnear.obj ..\core\_newnear.cpp



$(O)\complex.OBJ:	..\core\complex.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\complex.obj ..\core\complex.cpp



$(O)\coperat.OBJ:	..\core\coperat.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\coperat.obj ..\core\coperat.cpp



$(O)\corenew.OBJ:	..\core\corenew.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\corenew.obj ..\core\corenew.cpp



$(O)\cstrm.OBJ:	..\core\cstrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\cstrm.obj ..\core\cstrm.cpp



$(O)\ctrig.OBJ:	..\core\ctrig.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ctrig.obj ..\core\ctrig.cpp



$(O)\dbp.OBJ:	..\ios\dbp.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\dbp.obj ..\ios\dbp.cpp



$(O)\ehdata.OBJ:	..\core\ehdata.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ehdata.obj ..\core\ehdata.cpp



$(O)\fstream.OBJ:	..\ios\fstream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\fstream.obj ..\ios\fstream.cpp



$(O)\generr.OBJ:	..\ios\generr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\generr.obj ..\ios\generr.cpp



$(O)\ios.OBJ:	..\ios\ios.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ios.obj ..\ios\ios.cpp



$(O)\iostream.OBJ:	..\ios\iostream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\iostream.obj ..\ios\iostream.cpp



$(O)\istream.OBJ:	..\ios\istream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\istream.obj ..\ios\istream.cpp



$(O)\istreamf.OBJ:	..\ios\istreamf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\istreamf.obj ..\ios\istreamf.cpp



$(O)\istreami.OBJ:	..\ios\istreami.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\istreami.obj ..\ios\istreami.cpp



$(O)\istreamx.OBJ:	..\ios\istreamx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\istreamx.obj ..\ios\istreamx.cpp



$(O)\manip.OBJ:	..\ios\manip.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\manip.obj ..\ios\manip.cpp



$(O)\ostream.OBJ:	..\ios\ostream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ostream.obj ..\ios\ostream.cpp



$(O)\ostreamf.OBJ:	..\ios\ostreamf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ostreamf.obj ..\ios\ostreamf.cpp



$(O)\ostreami.OBJ:	..\ios\ostreami.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ostreami.obj ..\ios\ostreami.cpp



$(O)\ostreamx.OBJ:	..\ios\ostreamx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ostreamx.obj ..\ios\ostreamx.cpp



$(O)\siosync.OBJ:	..\ios\siosync.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\siosync.obj ..\ios\siosync.cpp



$(O)\stdiostr.OBJ:	..\ios\stdiostr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\stdiostr.obj ..\ios\stdiostr.cpp



$(O)\stream.OBJ:	..\ios\stream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\stream.obj ..\ios\stream.cpp



$(O)\streambf.OBJ:	..\ios\streambf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\streambf.obj ..\ios\streambf.cpp



$(O)\strstrea.OBJ:	..\ios\strstrea.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\strstrea.obj ..\ios\strstrea.cpp



$(O)\typeinfo.OBJ:	..\core\typeinfo.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\typeinfo.obj ..\core\typeinfo.cpp



$(O)\vecnew.OBJ:	..\core\vecnew.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\vecnew.obj ..\core\vecnew.cpp



$(O)\vector.OBJ:	..\core\vector.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\vector.obj ..\core\vector.cpp



$(O)\adelete.OBJ:	..\core\adelete.cpp
		$(CC) $(CFLAGS_1) $(DEFINES_1) $(INCLUDES) -o$(O)\adelete.obj ..\core\adelete.cpp



$(O)\adelete2.OBJ:	..\core\adelete2.cpp
		$(CC) $(CFLAGS_1) $(DEFINES_1) $(INCLUDES) -o$(O)\adelete2.obj ..\core\adelete2.cpp



$(O)\adelete3.OBJ:	..\core\adelete3.cpp
		$(CC) $(CFLAGS_1) $(DEFINES_1) $(INCLUDES) -o$(O)\adelete3.obj ..\core\adelete3.cpp



$(O)\nothrow.OBJ:	..\core\nothrow.cpp
		$(CC) $(CFLAGS_1) $(DEFINES_1) $(INCLUDES) -o$(O)\nothrow.obj ..\core\nothrow.cpp



$(O)\anew.OBJ:	..\core\anew.cpp
		$(CC) $(CFLAGS_2) $(DEFINES_2) $(INCLUDES) -o$(O)\anew.obj ..\core\anew.cpp



$(O)\anew2.OBJ:	..\core\anew2.cpp
		$(CC) $(CFLAGS_3) $(DEFINES_3) $(INCLUDES) -o$(O)\anew2.obj ..\core\anew2.cpp


$(O)\anew3.OBJ:	..\core\anew3.cpp
		$(CC) $(CFLAGS_3) $(DEFINES_3) $(INCLUDES) -o$(O)\anew3.obj ..\core\anew3.cpp


$(SYMROOT).SYM: ..\include\precomp.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO ..\include\precomp.h



