ORIGIN		= Digital Mars C++
ORIGIN_VER	= Version 7.22
VERSION		= RELEASE

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 0
NDEBUG		= 1
!ENDIF

PROJ		= dlluni
APPTYPE		= WIN32 LIB
PROJTYPE	= LIB

CC		= SC
CPP		= SPP
MAKE		= SMAKE
RC		= RCC
HC		= HC31
ASM		= SC
DISASM		= OBJ2ASM
LNK		= LINK
DLLS		= 

HEADERS		= ..\include\semlock.h ..\include\ehsup.h ..\include\mt.h 

DEFFILE		= dlluni.DEF

!IF $(DEBUG)
OUTPUTDIR	= snduni
!IF EXIST (snduni)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= .
!IF EXIST (.)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		=  snduni\precomp 
SYMS		= $(SYMROOT).SYM 
LIBS		= 

CFLAGS		=  -HIprecomp.h -Ae -Ar -r -mn -NL -ND -5 -a8 -Nc -c -H -HDsnduni -HO- -wx -g 
LFLAGS		=  /CO /NOI /DE /XN /A:512
DEFINES		= -D_MT -DUNICODE -D_UNICODE=1
!ELSE
OUTPUTDIR	= dlluni
!IF EXIST (dlluni)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= .
!IF EXIST (.)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		=  dlluni\precomp 
SYMS		= $(SYMROOT).SYM 
LIBS		= 

CFLAGS		=  -HIprecomp.h -Ae -Ar -r -mn -o+time -D_WINDLL -NL -5 -a8 -Nc -c -H -HDdlluni -HO- -wx 
LFLAGS		=  /NOI /DE /E /PACKF /XN /A:512
DEFINES		= -D_MT -DUNICODE -D_UNICODE=1
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

INCLUDES	= -I..\include -I..\..\include

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\wgetenv.OBJ $(OUTPUTDIR)\wputenv.OBJ \
		   $(OUTPUTDIR)\wcschr.OBJ $(OUTPUTDIR)\wcsdup.OBJ \
		   $(OUTPUTDIR)\wcsnicmp.OBJ $(OUTPUTDIR)\wcscpy.OBJ 

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -odlluni\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -odlluni\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -odlluni\$(*B).sym $*.hxx

.ASM.OBJ:
	$(ASM) $(AFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.asm


all:	noteout createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done


all2:	createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done

noteout:
	REM Output to $(OUTPUTDIR)

createdir:
	$(CREATEOUTPUTDIR)
	$(CREATETARGETDIR)
	
$(TARGETDIR)\$(PROJ).lib: $(SYMS) $(OBJS)
		lib -c $(TARGETDIR)\$(PROJ).lib \
				@<<
			$(OBJS)
<<

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(TARGETDIR)\$(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del dlluni.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO

cleanres:

res:		cleanres $(RCFILES) all



!IF EXIST (dlluni.dpd)
!INCLUDE dlluni.dpd
!ENDIF



$(OUTPUTDIR)\wgetenv.OBJ:	..\win32\wgetenv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wgetenv.obj ..\win32\wgetenv.c



$(OUTPUTDIR)\wputenv.OBJ:	..\core\wputenv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wputenv.obj ..\core\wputenv.c



$(OUTPUTDIR)\wcschr.OBJ:	..\core\wcschr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcschr.obj ..\core\wcschr.c



$(OUTPUTDIR)\wcsdup.OBJ:	..\core\wcsdup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsdup.obj ..\core\wcsdup.c



$(OUTPUTDIR)\wcsnicmp.OBJ:	..\core\wcsnicmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsnicmp.obj ..\core\wcsnicmp.c



$(OUTPUTDIR)\wcscpy.OBJ:	..\core\wcscpy.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcscpy.obj ..\core\wcscpy.c


$(SYMROOT).SYM: ..\include\precomp.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO ..\include\precomp.h



