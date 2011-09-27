# This is a part of the Microsoft Foundation Classes C++ library.
# Copyright (C) 1992-1995 Microsoft Corporation
# All rights reserved.
#
# This source code is only intended as a supplement to the
# Microsoft Foundation Classes Reference and related
# electronic documentation provided with the library.
# See these sources for detailed information regarding the
# Microsoft Foundation Classes product.

# MFCD42[D].DLL is a DLL
#  which exports all the MFC/DB classes
#
# If you need a private build of the MFC DB DLL, be sure to rename
#  "MFCD42.DLL" to something more appropriate for you application.
# Please do not re-distribute a privately built version with the
#  name "MFCD42.DLL".
#
# Use nmake /f mfcdb.mak LIBNAME=<my name> to do this.
#
# Note: LIBNAME must be 6 characters or less.

!ifndef LIBNAME
!error LIBNAME is not defined. LIBNAME=MFCD42 builds the prebuilt DLL.
!endif

!if "$(DEBUG)" != "0"
CRTDLL=msvcrtd.lib
!else
CRTDLL=msvcrt.lib
!endif

TARGET=w
DLL=2
TARG=$(LIBNAME)
TARGDEFS=/D_AFX_DB_IMPL
MFCDLL=MFC42
MFCOLE=MFCO42
LFLAGS=/nodefaultlib

!if "$(PLATFORM)" == "M68K"
TARG=$(TARG)M
MFCDLL=MFC42M
MFCOLE=MFCO42M
MACOS=1
!elseif "$(PLATFORM)" == "MPPC"
TARG=$(TARG)P
MFCDLL=MFC42P
MFCOLE=MFCO42P
MACOS=1
!endif

!if "$(UNICODE)" == "1"
TARG=$(TARG)U
MFCDLL=$(MFCDLL)U
MFCOLE=$(MFCOLE)U
!endif

!if "$(DEBUG)" != "0"
# Debug DLL build
RCDEFINES=/D_DEBUG
LFLAGS=$(LFLAGS)
PCH_FILE=$D\stdafxd.pch
PCH_CPP=dlldb
TARG=$(TARG)D
MFCDLL=$(MFCDLL)D
MFCOLE=$(MFCOLE)D

!ELSE
# Release DLL build
RCDEFINES=
LFLAGS=$(LFLAGS)
PCH_FILE=$D\stdafxd.pch
PCH_CPP=dlldb
!ENDIF

DEFFILE=$(PLATFORM)\$(TARG).DEF

MFCDLL=..\lib\$(MFCDLL).lib
MFCOLE=..\lib\$(MFCOLE).lib

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
LFLAGS=$(LFLAGS) /debug:none /incremental:no
!endif

!ifdef RELEASE # Release VERSION info
RCDEFINES=$(RCDEFINES) /DRELEASE
LFLAGS=$(LFLAGS) /release
!endif

LFLAGS=$(LFLAGS) /dll

# OPT:noref keeps unreferenced functions (ie. no dead-code elimination)
!if "$(REGEN)" == "1"
LFLAGS=$(LFLAGS) /opt:noref
!else
LFLAGS=$(LFLAGS) /opt:ref
!endif

!ifndef MACOS
LFLAGS=$(LFLAGS) /version:4.2 /base:0x5F700000

LIBS=$(MFCDLL) $(MFCOLE) $(CRTDLL) kernel32.lib gdi32.lib user32.lib \
	uuid.lib daouuid.lib
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
LIBS=$(MFCDLL) $(CRTDLL) $(LIBS) interfac.lib
!endif


!if "$(ORDER)" == "1"
!if exist($(PLATFORM)\$(TARG).prf)
DEFS=$(DEFS) /D_AFX_FUNCTION_ORDER
LFLAGS=$(LFLAGS) /order:@$(PLATFORM)\$(TARG).prf
!endif
!endif

dll_goal: create2.dir $(TARG).dll ..\lib\$(TARG).lib

#############################################################################
# import most rules and library files from normal makefile

!include makefile

create2.dir:
	@-if not exist $D\*.* mkdir $D

#############################################################################
# Build target

$D\$(TARG).res: mfcdb.rc build_.h
	rc /r $(RCFLAGS) $(RCDEFINES) /fo $D\$(TARG).res mfcdb.rc
!ifdef MACOS
	mrc /DRESFILE=\"$@\" /o $@ attribut.r
!endif

$D\$(TARG).rsc: mfcdll.r
!if "$(DEBUG)" == "0"
	mrc /DARCHITECTURE=$(MACOS_ARCH) /DLIBNAME=\"MicrosoftMFCD42Lib\" \
		/DCURRENTVER=0x04008000 /DOLDDEFVER=0x04008000 \
		/DJDATE=$(JULIAN) -DJDATESTR=\"$(JULIAN)\" /o $D\$(TARG).rsc mfcdll.r
!else
	mrc /DARCHITECTURE=$(MACOS_ARCH) /DLIBNAME=\"MicrosoftMFCD42DLib\" \
		/DCURRENTVER=0x04008000 /DOLDDEFVER=0x04008000 \
		/DJDATE=$(JULIAN) -DJDATESTR=\"$(JULIAN)\" /o $D\$(TARG).rsc mfcdll.r
!endif

DLL_OBJS=$(DB) $(DAO)

!ifdef MACOS
MRC_RESOURCES=$D\$(TARG).rsc
!else
MRC_RESOURCES=
!endif

$(TARG).dll ..\lib\$(TARG).lib: $D\$(PCH_CPP).obj $(DLL_OBJS) $(DEFFILE) $D\$(TARG).res $(MRC_RESOURCES)
	link @<<
$(LFLAGS)
$(LIBS)
$D\$(PCH_CPP).obj
$(DLL_OBJS)
$D\$(TARG).res
/def:$(DEFFILE)
/out:$(TARG).DLL
/map:$D\$(TARG).MAP
/implib:..\lib\$(TARG).LIB
!ifdef MACOS
/mac:type=shlb /mac:creator=cfmg
/mac:init=WlmConnectionInit
/import:currentver=67141632,oldcodever=67141632,oldapiver=67141632
$D\$(TARG).rsc
!endif
<<
	if exist ..\lib\$(TARG).exp del ..\lib\$(TARG).exp

#############################################################################
