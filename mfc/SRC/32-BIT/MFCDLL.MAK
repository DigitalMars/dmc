# This is a part of the Microsoft Foundation Classes C++ library.
# Copyright (C) 1992-1995 Microsoft Corporation
# All rights reserved.
#
# This source code is only intended as a supplement to the
# Microsoft Foundation Classes Reference and related
# electronic documentation provided with the library.
# See these sources for detailed information regarding the
# Microsoft Foundation Classes product.

# MFC42[D].DLL is a DLL
#  which exports all the MFC classes
#
# If you need a private build of the MFC DLL, be sure to rename
#  "MFC42.DLL" to something more appropriate for your application.
# Please do not re-distribute a privately built version with the
#  name "MFC42.DLL".
#
# Use nmake /f mfcdll.mak LIBNAME=<your name> to do this.
#
# Note: LIBNAME must be 6 characters or less.

!ifndef LIBNAME
!error LIBNAME is not defined. LIBNAME=MFC42 builds the prebuilt DLL.
!endif

!if "$(DEBUG)" != "0"
CRTDLL=msvcrtd.lib
!else
CRTDLL=msvcrt.lib
!endif

TARGET=w
DLL=2
TARGDEFS=/D_AFX_CORE_IMPL
LFLAGS=/nodefaultlib
RCFLAGS=/r

!if "$(UNICODE)" == "1"
TARGTYPE=U
RCDEFINES=$(RCDEFINES) /D_UNICODE
!endif

!if "$(PLATFORM)" == "M68K"
PF=M
CRTDLL=msvcrt.lib
!ifndef MONOLITHIC
MONOLITHIC=0
!endif
!elseif "$(PLATFORM)" == "MPPC"
PF=P
!ifndef MONOLITHIC
MONOLITHIC=0
!endif
!endif

!if "$(DEBUG)" != "0"
# Debug DLL build
TARGTYPE=$(TARGTYPE)D
RCDEFINES=$(RCDEFINES) /D_DEBUG
LFLAGS=$(LFLAGS)
!ifndef MONOLITHIC
MONOLITHIC=0
!endif
!ELSE
# Release DLL build
RCDEFINES=$(RCDEFINES)
LFLAGS=$(LFLAGS)
!ifndef MONOLITHIC
MONOLITHIC=1
!endif
!ENDIF

!if "$(MONOLITHIC)" == "1"
TARGDEFS=$(TARGDEFS) /D_AFX_OLE_IMPL /D_AFX_DB_IMPL /D_AFX_NET_IMPL
RCDEFINES=$(RCDEFINES) /D_AFX_MONOLITHIC
!endif

CFNAME=$(LIBNAME)$(TARGTYPE)
TARG=$(LIBNAME)$(PF)$(TARGTYPE)
TARG_STATIC=MFCS42$(PF)$(TARGTYPE)

# OPT:noref keeps unreferenced functions (ie. no dead-code elimination)
!if "$(REGEN)" == "0"
LFLAGS=$(LFLAGS) /opt:ref
!else
LFLAGS=$(LFLAGS) /opt:noref
!endif

DEFFILE=$(PLATFORM)\$(TARG).DEF

!if "$(DEBUGTYPE)" == ""
DEBUGTYPE=cv
!endif

!if "$(CODEVIEW)" != "0"
!if "$(REGEN)" != "1"
LFLAGS=$(LFLAGS) /debug:full /debugtype:$(DEBUGTYPE)
!else
LFLAGS=$(LFLAGS) /debug:none
!endif
!if "$(NO_PDB)" != "1" && "$(REGEN)" != "1"
LFLAGS=$(LFLAGS) /pdb:$(TARG).pdb
!else
LFLAGS=$(LFLAGS) /pdb:none
!endif
!else
LFLAGS=$(LFLAGS) /debug:none
!if "$(INCREMENTAL)" != "1"
LFLAGS=$(LFLAGS) /incremental:no
!endif
!endif

!ifdef RELEASE # Release VERSION info
RCDEFINES=$(RCDEFINES) /DRELEASE
LFLAGS=$(LFLAGS) /release
!endif

LFLAGS=$(LFLAGS) /dll

!if "$(ORDER)" == "1"
!if exist($(PLATFORM)\$(TARG).prf)
DEFS=$(DEFS) /D_AFX_FUNCTION_ORDER
LFLAGS=$(LFLAGS) /order:@$(PLATFORM)\$(TARG).prf
!endif
!endif

!if "$(PLATFORM)" == "PPC"
LIBS=$(LIBS) int64.lib /force:multiple
!endif

dll_goal: create2.dir \
	$(TARG).dll ..\lib\$(TARG).lib ..\lib\$(TARG_STATIC).lib

#############################################################################
# import most rules and library files from normal makefile

!include makefile

create2.dir:
	@-if not exist $D\*.* mkdir $D

#############################################################################
# more flags and switches

!ifndef MACOS
LFLAGS=$(LFLAGS) /version:4.2 /base:0x5F400000
LIBS=$(CRTDLL) kernel32.lib gdi32.lib user32.lib uuid.lib daouuid.lib $(PROFLIB)
!else
!if "$(BASE)" == "M68K"
!error DLL build is not supported for 68K Macintosh
!endif

LFLAGS=$(LFLAGS) /mac:nomfilepad
RCFLAGS=$(RCFLAGS) /m

!if "$(DEBUG)" != "0"
LIBS=msvcwlmd.lib
!else
LIBS=msvcwlm.lib
!endif

LIBS=uuid.lib $(CRTDLL) $(LIBS) interfac.lib
!endif

#############################################################################

STATICLINK_OBJS=$D\stdafx.obj $D\nolib.obj \
	$D\appmodul.obj $D\dllmodul.obj $D\oleexp.obj

CPPFLAGS_STATIC=\
	$(CL_MODEL) $(CL_OPT) $(DEFS) $(OPT) $(EH) /Gy /Fd..\lib\$(TARG_STATIC).pdb

PCH_TARGET=$D\stdafx.obj

$D\stdafx.obj $D\stdafxs.pch: stdafx.cpp stdafx.h
	cl @<<
/c $(CPPFLAGS_STATIC) /Ycstdafx.h /Fp$D\stdafxs.pch stdafx.cpp
<<

$D\appmodul.obj: $D\stdafx.obj appmodul.cpp
	cl @<<
/c $(CPPFLAGS_STATIC) /Yustdafx.h /Fp$D\stdafxs.pch appmodul.cpp
<<

$D\dllmodul.obj: $D\stdafx.obj dllmodul.cpp
	cl @<<
/c $(CPPFLAGS_STATIC) /Yustdafx.h /Fp$D\stdafxs.pch dllmodul.cpp
<<

$D\oleexp.obj: $D\stdafx.obj oleexp.cpp
	cl @<<
/c $(CPPFLAGS_STATIC) /Yustdafx.h /Fp$D\stdafxs.pch oleexp.cpp
<<

$D\nolib.obj: $D\stdafx.obj nolib.cpp
	cl @<<
/c $(CPPFLAGS_STATIC) /Yustdafx.h /Fp$D\stdafxs.pch nolib.cpp
<<

#############################################################################
# Build target

$D\$(TARG).res: mfcdll.rc build_.h
	rc $(RCFLAGS) $(RCDEFINES) /fo $D\$(TARG).res mfcdll.rc
!ifdef MACOS
	mrc /DRESFILE=\"$@\" /o $@ attribut.r
!endif

$D\$(TARG).rsc: mfcdll.r
	mrc /DARCHITECTURE=$(MACOS_ARCH) /DLIBNAME=\"Microsoft$(CFNAME)Lib\" \
		/DJDATE=$(JULIAN) /DJDATESTR=\"$(JULIAN)\" \
		/DCURRENTVER=0x04008000 /DOLDDEFVER=0x04008000 \
		/o $D\$(TARG).rsc mfcdll.r

DLL_OBJS=$(OBJECT) $(OBJDIAG) $(INLINES) $(FILES) $(COLL1) $(COLL2) $(MISC) \
	$(WINDOWS) $(DIALOG) $(WINMISC) $(DOCVIEW) $(APPLICATION) $(OLEREQ) \
	$(INTERNET)

!if "$(MONOLITHIC)" == "1"
DLL_OBJS=$(DLL_OBJS) $(SOCKETS) $(OLEDLL) $(DAO) $(DB)
!endif

DLL_OBJS=$(DLL_OBJS) $D\dllinit.obj

DLL_RESOURCES=$D\$(TARG).res
!ifdef MACOS
DLL_RESOURCES=$(DLL_RESOURCES) $D\$(TARG).rsc
!endif

$(TARG).dll ..\lib\$(TARG).lib: $(DLL_OBJS) $(DEFFILE) $(DLL_RESOURCES)
	link @<<
$(LFLAGS)
$(LIBS)
$(DLL_OBJS)
$(DLL_RESOURCES)
/def:$(DEFFILE)
/out:$(TARG).DLL
/map:$D\$(TARG).MAP
/implib:..\lib\$(TARG).LIB
!ifdef MACOS
/mac:type=shlb /mac:creator=cfmg
/mac:init=WlmConnectionInit
/import:currentver=67141632,oldcodever=67141632,oldapiver=67141632
!endif
<<

..\lib\$(TARG_STATIC).lib: $(STATICLINK_OBJS)
	lib /out:$@ $(STATICLINK_OBJS)

#############################################################################
