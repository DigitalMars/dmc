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

PROJ		= snndcpp
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

HEADERS		= ..\INCLUDE\mt.h ..\INCLUDE\ehsup.h ..\heap32\heap.h  \
		..\heap32\pool.h ..\..\INCLUDE\rtlheap.h ..\ios\filesys.h ..\heap32\multpool.h  \
		..\..\INCLUDE\typeinfo.h 

DEFFILE		= snndcpp.DEF

!IF $(DEBUG)
O	= snndcpp
!IF EXIST (snndcpp)
CREATEO	=
!ELSE
CREATEO	= if not exist $(O)\*.* md $(O)
!ENDIF
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		=  snndcpp\precomp 
SYMS		= $(SYMROOT).SYM 
LIBS		= 

CFLAGS		=  -cpp -HIprecomp.h -Ae -Ar -r -mn -S -NL -5 -a8 -c -H -HDsnndcpp -HO- -wx -w- -w12 -gf 
CFLAGS_1	=  -cpp -Ae -Ar -Aa -r -mn -S -NL -5 -a8 -c -wx -w- -w12 -gf
DEFINES_1	= -D_MT -D_DEBUG -D_CRTBLD -D_MBCS=1
CFLAGS_2	=  -cpp -Ae -Ar -Aa -r -mn -S -NL -5 -a8 -c -wx -w- -w12 -gf
DEFINES_2	= -D_MT -D_DEBUG -D_CRTBLD -D_MBCS=1
CFLAGS_3	=  -cpp -Ae -Ar -Aa -r -mn -S -NL -5 -a8 -c -wx -w- -w12 -gf
DEFINES_3	= -D_MT -D_DEBUG -D_CRTBLD -D_MBCS=1
LFLAGS		=  /CO /NOI /DE /A:512
DEFINES		= -D_MT -D_DEBUG -D_CRTBLD -D_MBCS=1
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
DEFINES_1	= -D_MBCS=1
CFLAGS_2	=  -mn -o+time -D_WINDOWS -3 -a8 -c
DEFINES_2	= -D_MBCS=1
CFLAGS_3	=  -mn -o+time -D_WINDOWS -3 -a8 -c
DEFINES_3	= -D_MBCS=1
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

OBJS		=  $(O)\vector.OBJ  $(O)\vecnew.OBJ  $(O)\typeinfo.OBJ  \
		 $(O)\strstrea.OBJ  $(O)\streambf.OBJ  $(O)\stream.OBJ  $(O)\stdiostr.OBJ  \
		 $(O)\siosync.OBJ  $(O)\rtti.OBJ  $(O)\pool.OBJ  $(O)\ostreamx.OBJ  \
		 $(O)\ostreami.OBJ  $(O)\ostreamf.OBJ  $(O)\ostream.OBJ  $(O)\multpool.OBJ  \
		 $(O)\manip.OBJ  $(O)\istreamx.OBJ  $(O)\istreami.OBJ  $(O)\istreamf.OBJ  \
		 $(O)\istream.OBJ  $(O)\iostream.OBJ  $(O)\ios.OBJ  $(O)\heapinit.OBJ  \
		 $(O)\heap.OBJ  $(O)\generr.OBJ  $(O)\fstream.OBJ  $(O)\except.OBJ  \
		 $(O)\ehdata.OBJ  $(O)\dbp.OBJ  $(O)\ctrig.OBJ  $(O)\cstrm.OBJ  \
		 $(O)\corenew.OBJ  $(O)\coperat.OBJ  $(O)\complex.OBJ  $(O)\_newnear.OBJ  \
		 $(O)\_new.OBJ  $(O)\_new2.OBJ  $(O)\_new3.OBJ  $(O)\_handler.OBJ  \
		 $(O)\_delnear.OBJ  $(O)\_delete2.OBJ  $(O)\_delete3.OBJ  \
		 $(O)\adelete.OBJ  $(O)\adelete2.OBJ  $(O)\adelete3.OBJ  \
		 $(O)\nothrow.OBJ  $(O)\anew.OBJ  $(O)\anew2.OBJ  $(O)\anew3.OBJ 

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -osnndcpp\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -osnndcpp\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -osnndcpp\$(*B).sym $*.hxx

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
snndcpp\precomp.PCO+
snndcpp\vector.OBJ+
snndcpp\vecnew.OBJ+
snndcpp\typeinfo.OBJ+
snndcpp\strstrea.OBJ+
snndcpp\streambf.OBJ+
snndcpp\stream.OBJ+
snndcpp\stdiostr.OBJ+
snndcpp\siosync.OBJ+
snndcpp\rtti.OBJ+
snndcpp\pool.OBJ+
snndcpp\ostreamx.OBJ+
snndcpp\ostreami.OBJ+
snndcpp\ostreamf.OBJ+
snndcpp\ostream.OBJ+
snndcpp\multpool.OBJ+
snndcpp\manip.OBJ+
snndcpp\istreamx.OBJ+
snndcpp\istreami.OBJ+
snndcpp\istreamf.OBJ+
snndcpp\istream.OBJ+
snndcpp\iostream.OBJ+
snndcpp\ios.OBJ+
snndcpp\heapinit.OBJ+
snndcpp\heap.OBJ+
snndcpp\generr.OBJ+
snndcpp\fstream.OBJ+
snndcpp\except.OBJ+
snndcpp\ehdata.OBJ+
snndcpp\dbp.OBJ+
snndcpp\ctrig.OBJ+
snndcpp\cstrm.OBJ+
snndcpp\corenew.OBJ+
snndcpp\coperat.OBJ+
snndcpp\complex.OBJ+
snndcpp\_newnear.OBJ+
snndcpp\_new.OBJ+
snndcpp\_new2.OBJ+
snndcpp\_new3.OBJ+
snndcpp\_handler.OBJ+
snndcpp\_delnear.OBJ+
snndcpp\_delete2.OBJ+
snndcpp\_delete3.OBJ+
snndcpp\adelete.OBJ+
snndcpp\adelete2.OBJ+
snndcpp\adelete3.OBJ+
snndcpp\nothrow.OBJ+
snndcpp\anew.OBJ+
snndcpp\anew2.OBJ
snndcpp\anew3.OBJ
$$SCW$$.LIB
NUL

snndcpp.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
..\core\vector.OBJ+
..\core\vecnew.OBJ+
..\core\typeinfo.OBJ+
..\ios\strstrea.OBJ+
..\ios\streambf.OBJ+
..\ios\stream.OBJ+
..\ios\stdiostr.OBJ+
..\ios\siosync.OBJ+
..\core\rtti.OBJ+
..\heap32\pool.OBJ+
..\ios\ostreamx.OBJ+
..\ios\ostreami.OBJ+
..\ios\ostreamf.OBJ+
..\ios\ostream.OBJ+
..\heap32\multpool.OBJ+
..\ios\manip.OBJ+
..\ios\istreamx.OBJ+
..\ios\istreami.OBJ+
..\ios\istreamf.OBJ+
..\ios\istream.OBJ+
..\ios\iostream.OBJ+
..\ios\ios.OBJ+
..\heap32\heapinit.OBJ+
..\heap32\heap.OBJ+
..\ios\generr.OBJ+
..\ios\fstream.OBJ+
..\core\except.OBJ+
..\core\ehdata.OBJ+
..\ios\dbp.OBJ+
..\core\ctrig.OBJ+
..\core\cstrm.OBJ+
..\core\corenew.OBJ+
..\core\coperat.OBJ+
..\core\complex.OBJ+
..\core\_newnear.OBJ+
..\core\_new.OBJ+
..\core\_new2.OBJ+
..\core\_new3.OBJ+
..\core\_handler.OBJ+
..\core\_delnear.OBJ+
..\core\_delete2.OBJ+
..\core\_delete3.OBJ+
..\core\adelete.OBJ+
..\core\adelete2.OBJ+
..\core\adelete3OBJ+
..\core\nothrow.OBJ+
..\core\anew.OBJ+
..\core\anew2.OBJ
..\core\anew3.OBJ
$$SCW$$.LIB
NUL

snndcpp.DEF;
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
 + $(O)\vector.OBJ + $(O)\vecnew.OBJ + $(O)\typeinfo.OBJ &
		 + $(O)\strstrea.OBJ + $(O)\streambf.OBJ + $(O)\stream.OBJ + $(O)\stdiostr.OBJ &
		 + $(O)\siosync.OBJ + $(O)\rtti.OBJ + $(O)\pool.OBJ + $(O)\ostreamx.OBJ &
		 + $(O)\ostreami.OBJ + $(O)\ostreamf.OBJ + $(O)\ostream.OBJ + $(O)\multpool.OBJ &
		 + $(O)\manip.OBJ + $(O)\istreamx.OBJ + $(O)\istreami.OBJ + $(O)\istreamf.OBJ &
		 + $(O)\istream.OBJ + $(O)\iostream.OBJ + $(O)\ios.OBJ + $(O)\heapinit.OBJ &
		 + $(O)\heap.OBJ + $(O)\generr.OBJ + $(O)\fstream.OBJ + $(O)\except.OBJ &
		 + $(O)\ehdata.OBJ + $(O)\dbp.OBJ + $(O)\ctrig.OBJ + $(O)\cstrm.OBJ &
		 + $(O)\corenew.OBJ + $(O)\coperat.OBJ + $(O)\complex.OBJ + $(O)\_newnear.OBJ &
		 + $(O)\_new2.OBJ + $(O)\_new.OBJ + $(O)\_new3.OBJ + $(O)\_handler.OBJ + $(O)\_delnear.OBJ + $(O)\_delete2.OBJ + $(O)\_delete3.OBJ &
		 + $(O)\adelete.OBJ + $(O)\adelete2.OBJ + $(O)\adelete3.OBJ + $(O)\nothrow.OBJ + $(O)\anew.OBJ + $(O)\anew2.OBJ + $(O)\anew3.OBJ
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
		-del snndcpp.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO

cleanres:

res:		cleanres $(RCFILES) all


link:
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 + $(O)\vector.OBJ + $(O)\vecnew.OBJ + $(O)\typeinfo.OBJ &
		 + $(O)\strstrea.OBJ + $(O)\streambf.OBJ + $(O)\stream.OBJ + $(O)\stdiostr.OBJ &
		 + $(O)\siosync.OBJ + $(O)\rtti.OBJ + $(O)\pool.OBJ + $(O)\ostreamx.OBJ &
		 + $(O)\ostreami.OBJ + $(O)\ostreamf.OBJ + $(O)\ostream.OBJ + $(O)\multpool.OBJ &
		 + $(O)\manip.OBJ + $(O)\istreamx.OBJ + $(O)\istreami.OBJ + $(O)\istreamf.OBJ &
		 + $(O)\istream.OBJ + $(O)\iostream.OBJ + $(O)\ios.OBJ + $(O)\heapinit.OBJ &
		 + $(O)\heap.OBJ + $(O)\generr.OBJ + $(O)\fstream.OBJ + $(O)\except.OBJ &
		 + $(O)\ehdata.OBJ + $(O)\dbp.OBJ + $(O)\ctrig.OBJ + $(O)\cstrm.OBJ &
		 + $(O)\corenew.OBJ + $(O)\coperat.OBJ + $(O)\complex.OBJ + $(O)\_newnear.OBJ &
		 + $(O)\_new2.OBJ + $(O)\_new.OBJ + $(O)\_new3.OBJ + $(O)\_handler.OBJ + $(O)\_delnear.OBJ + $(O)\_delete2.OBJ + $(O)\_delete3.OBJ &
		 + $(O)\adelete.OBJ + $(O)\adelete2.OBJ + $(O)\adelete3.OBJ + $(O)\nothrow.OBJ + $(O)\anew.OBJ + $(O)\anew2.OBJ + $(O)\anew3.OBJ
<<
		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (snndcpp.dpd)
!INCLUDE snndcpp.dpd
!ENDIF



$(O)\vector.OBJ:	..\core\vector.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\vector.obj ..\core\vector.cpp



$(O)\vecnew.OBJ:	..\core\vecnew.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\vecnew.obj ..\core\vecnew.cpp



$(O)\typeinfo.OBJ:	..\core\typeinfo.cpp
#		$(CC) -cpp -HIprecomp.h -Ae -Ar -S -NL -5 -c -HDsnndcpp -HO- -wx -w- -w12 $(DEFINES) $(INCLUDES) -o$(O)\typeinfo.obj ..\core\typeinfo.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\typeinfo.obj ..\core\typeinfo.cpp



$(O)\strstrea.OBJ:	..\ios\strstrea.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\strstrea.obj ..\ios\strstrea.cpp



$(O)\streambf.OBJ:	..\ios\streambf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\streambf.obj ..\ios\streambf.cpp



$(O)\stream.OBJ:	..\ios\stream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\stream.obj ..\ios\stream.cpp



$(O)\stdiostr.OBJ:	..\ios\stdiostr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\stdiostr.obj ..\ios\stdiostr.cpp



$(O)\siosync.OBJ:	..\ios\siosync.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\siosync.obj ..\ios\siosync.cpp



$(O)\rtti.OBJ:	..\core\rtti.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\rtti.obj ..\core\rtti.cpp



$(O)\pool.OBJ:	..\heap32\pool.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\pool.obj ..\heap32\pool.cpp



$(O)\ostreamx.OBJ:	..\ios\ostreamx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ostreamx.obj ..\ios\ostreamx.cpp



$(O)\ostreami.OBJ:	..\ios\ostreami.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ostreami.obj ..\ios\ostreami.cpp



$(O)\ostreamf.OBJ:	..\ios\ostreamf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ostreamf.obj ..\ios\ostreamf.cpp



$(O)\ostream.OBJ:	..\ios\ostream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ostream.obj ..\ios\ostream.cpp



$(O)\multpool.OBJ:	..\heap32\multpool.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\multpool.obj ..\heap32\multpool.cpp



$(O)\manip.OBJ:	..\ios\manip.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\manip.obj ..\ios\manip.cpp



$(O)\istreamx.OBJ:	..\ios\istreamx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\istreamx.obj ..\ios\istreamx.cpp



$(O)\istreami.OBJ:	..\ios\istreami.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\istreami.obj ..\ios\istreami.cpp



$(O)\istreamf.OBJ:	..\ios\istreamf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\istreamf.obj ..\ios\istreamf.cpp



$(O)\istream.OBJ:	..\ios\istream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\istream.obj ..\ios\istream.cpp



$(O)\iostream.OBJ:	..\ios\iostream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\iostream.obj ..\ios\iostream.cpp



$(O)\ios.OBJ:	..\ios\ios.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ios.obj ..\ios\ios.cpp



$(O)\heapinit.OBJ:	..\heap32\heapinit.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\heapinit.obj ..\heap32\heapinit.cpp



$(O)\heap.OBJ:	..\heap32\heap.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\heap.obj ..\heap32\heap.cpp



$(O)\generr.OBJ:	..\ios\generr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\generr.obj ..\ios\generr.cpp



$(O)\fstream.OBJ:	..\ios\fstream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\fstream.obj ..\ios\fstream.cpp



$(O)\except.OBJ:	..\core\except.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\except.obj ..\core\except.cpp



$(O)\ehdata.OBJ:	..\core\ehdata.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ehdata.obj ..\core\ehdata.cpp



$(O)\dbp.OBJ:	..\ios\dbp.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\dbp.obj ..\ios\dbp.cpp



$(O)\ctrig.OBJ:	..\core\ctrig.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\ctrig.obj ..\core\ctrig.cpp



$(O)\cstrm.OBJ:	..\core\cstrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\cstrm.obj ..\core\cstrm.cpp



$(O)\corenew.OBJ:	..\core\corenew.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\corenew.obj ..\core\corenew.cpp



$(O)\coperat.OBJ:	..\core\coperat.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\coperat.obj ..\core\coperat.cpp



$(O)\complex.OBJ:	..\core\complex.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\complex.obj ..\core\complex.cpp



$(O)\_newnear.OBJ:	..\core\_newnear.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_newnear.obj ..\core\_newnear.cpp



$(O)\_new.OBJ:	..\core\_new.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_new.obj ..\core\_new.cpp



$(O)\_new2.OBJ:	..\core\_new2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_new2.obj ..\core\_new2.cpp



$(O)\_new3.OBJ:	..\core\_new3.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_new3.obj ..\core\_new3.cpp



$(O)\_handler.OBJ:	..\core\_handler.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_handler.obj ..\core\_handler.cpp



$(O)\_delnear.OBJ:	..\core\_delnear.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_delnear.obj ..\core\_delnear.cpp



$(O)\_delete2.OBJ:	..\core\_delete2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_delete2.obj ..\core\_delete2.cpp



$(O)\_delete3.OBJ:	..\core\_delete3.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(O)\_delete3.obj ..\core\_delete3.cpp



$(O)\adelete.OBJ:	..\core\adelete.cpp
		$(CC) $(CFLAGS_3) $(DEFINES_3) $(INCLUDES) -o$(O)\adelete.obj ..\core\adelete.cpp



$(O)\adelete2.OBJ:	..\core\adelete2.cpp
		$(CC) $(CFLAGS_3) $(DEFINES_3) $(INCLUDES) -o$(O)\adelete2.obj ..\core\adelete2.cpp



$(O)\adelete3.OBJ:	..\core\adelete3.cpp
		$(CC) $(CFLAGS_3) $(DEFINES_3) $(INCLUDES) -o$(O)\adelete3.obj ..\core\adelete3.cpp



$(O)\nothrow.OBJ:	..\core\nothrow.cpp
		$(CC) $(CFLAGS_3) $(DEFINES_3) $(INCLUDES) -o$(O)\nothrow.obj ..\core\nothrow.cpp



$(O)\anew.OBJ:	..\core\anew.cpp
		$(CC) $(CFLAGS_2) $(DEFINES_2) $(INCLUDES) -o$(O)\anew.obj ..\core\anew.cpp



$(O)\anew2.OBJ:	..\core\anew2.cpp
		$(CC) $(CFLAGS_1) $(DEFINES_1) $(INCLUDES) -o$(O)\anew2.obj ..\core\anew2.cpp


$(O)\anew3.OBJ:	..\core\anew3.cpp
		$(CC) $(CFLAGS_1) $(DEFINES_1) $(INCLUDES) -o$(O)\anew3.obj ..\core\anew3.cpp


$(SYMROOT).SYM: ..\include\precomp.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO ..\include\precomp.h



