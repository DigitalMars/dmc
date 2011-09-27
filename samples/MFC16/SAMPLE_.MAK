# This is a part of the Microsoft Foundation Classes C++ library.
# Copyright (C) 1992 Microsoft Corporation
# All rights reserved.
#
# This source code is only intended as a supplement to the
# Microsoft Foundation Classes Reference and Microsoft
# QuickHelp and/or WinHelp documentation provided with the library.
# See these sources for detailed information regarding the
# Microsoft Foundation Classes product.

# Common include for building MFC Sample programs
#
#  typical usage
#       PROJ = foo
#       OBJS = foo.obj bar.obj ...
#       !INCLUDE ..\SAMPLE_.MAK
#
#  ROOT specifies the location of the sc\samples\mfc25 directory,
#  relative to the project directory. Because the MFC tutorial samples
#  have an intermediate STEP<n> subdirectory, they use
#       ROOT=..\..
#  instead of the default
#       ROOT=..
#
# NOTE: do not include 'stdafx.obj' in the OBJS list - the correctly
#    built version will be included for you
#
# Options to SMAKE:
#     "AFXDLL=1" => build a DLL client app (default static MFC library)
#     "DEBUG=0" => use retail (default debug)
#     "OPT" => use to set custom compile options

!ifndef PROJ
	!ERROR You forgot to define 'PROJ' !!
!endif
!ifndef OBJS
	!ERROR You forgot to define 'OBJS' !!
!endif

!ifndef ROOT
ROOT=..
!endif

!if "$(DEBUG)"!="0"
DEBUGSUF=D
!endif

!if "$(AFXDLL)"!="1"
# static link library variant - the default
STDAFX=stdafx
CPPMAIN_FLAGS=-ml -a -WA -2 -Nc
LIBS=swl
AFXLIB=lafxcw$(DEBUGSUF)
!else
# dynamic link library variant 
STDAFX=stdafx
CPPMAIN_FLAGS=-ml -a -WA-r -2 -Nc -D_AFXDLL
RCFLAGS=$(RCFLAGS) -D_AFXDLL
LIBS=swl
AFXLIB=smfc25$(DEBUGSUF)
!ifdef USES_OLE
AFXLIB=$(AFXLIB) smfco25$(DEBUGSUF)
!endif
!ifdef USES_DB
AFXLIB=$(AFXLIB) smfcd25$(DEBUGSUF)
!endif
!ifdef USES_SOCK
AFXLIB=$(AFXLIB) smfcn25$(DEBUGSUF)
!endif
!endif #//AFXDLL

!ifdef USES_OLE
# special settings for OLE enabled apps
WIN31ONLY=1
LIBS=$(LIBS) mfcoleui compobj storage ole2 ole2disp
!endif

!ifdef USES_DB
LIBS=$(LIBS) odbc
!endif

!ifdef USES_SOCK
LIBS=$(LIBS) winsock
!endif

!if "$(DEBUG)"!="0"
# debug variant - the default
CPPMAIN_FLAGS=-D_DEBUG $(CPPMAIN_FLAGS) -g -C
LINKFLAGS=/NOD /DE /CO
STDAFX=$(STDAFX)
!else
# retail variant
CPPMAIN_FLAGS=$(CPPMAIN_FLAGS) -o+speed
LINKFLAGS=/AL:16 /NOD /DE 
!endif #//DEBUG

!if "$(OPT)"!=""
CPPMAIN_FLAGS=$(CPPMAIN_FLAGS) $(OPT)
!endif

LIBS=$(AFXLIB) $(LIBS) libw commdlg shell $(EXTRA_LIBS)
CPPFLAGS=$(CPPMAIN_FLAGS) -w -H -HD$(ROOT) $(EXTRA_FLAGS)

#############################################################################

!ifndef RESFILE
RESFILE=$(PROJ)
!endif

$(PROJ).exe: $(RESFILE).res $(PROJ).def $(OBJS)
	link @<<
$(LINKFLAGS) /nologo /RC -31 -k -t :$(RESFILE).res $(OBJS),
$(PROJ),NUL,$(LIBS),$(PROJ).def;
<<

$(OBJS): $(ROOT)\$(STDAFX).sym

$(RESFILE).res:  resource.h
	rcc $(RCFLAGS) $(RESFILE).rc

clean::
	-erase $(PROJ).exe 
	-erase $(RESFILE).res
	-erase *.sym
	-erase *.map
	-erase *.obj

#############################################################################
# Shared SYM files (shared by all samples)

$(ROOT)\$(STDAFX).sym :: $(ROOT)\stdafx.h 
	@echo.
	@echo Building shared Pre-Compiled Header/Type files.
	@echo.
	$(CPP) $(CPPMAIN_FLAGS) -c -cpp -w -HF$(ROOT)\$(STDAFX).sym -o$(ROOT)\$(STDAFX).obj $(ROOT)\stdafx.h

cleanall: clean
	-erase $(ROOT)\$(STDAFX).sym
	-erase $(ROOT)\$(STDAFX).obj

#############################################################################
