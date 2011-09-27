# This is a part of the Microsoft Foundation Classes C++ library.
# Copyright (C) 1993 Microsoft Corporation
# All rights reserved.
#
# This source code is only intended as a supplement to the
# Microsoft Foundation Classes Reference and Microsoft
# QuickHelp documentation provided with the library.
# See these sources for detailed information regarding the
# Microsoft Foundation Classes product.
#

# SMFCD25[D].DLL is a large model DLL built with MFC large model [debug] DLL
#  which exports all the MFC database classes
# If you need a private build of the MFC DLL, be sure to rename
#  "SMFCD25.DLL" to something more appropriate for your application.
# Please do not re-distribute a privately built version with the
#  name "SMFCD25.DLL".


MODEL=l
TARGET=w
DLL=2
DEXT=.db
TARG=SMFCD25
MSTARG=MFCD250
TARGDEFS=-D_AFXDLLEX

LFLAGS=/NOD /NOE /DE /PACKDATA /NOPACKCODE /B /NOLOGO
# /NOPACKCODE => keep MFC library segmentation

!if "$(DEBUG)"!="0"
# Debug DLL build
TARG=$(TARG)D
MSTARG=$(MSTARG)D
OBJ_LIB=$D
AFXDLL_OBJ=afxdlld.obj
RCDEFINES=-D_DEBUG
LFLAGS=$(LFLAGS)
MFCDLL=..\..\lib\smfc25d.lib
PCH_FILE=$D\stdafx.sym
PCH_CPP=stdafx
!ifndef NO_PCH
PCH_OBJ=$D\stdafx.obj
!else
PCH_OBJ=
!endif

!ELSE
# Release DLL build
OBJ_LIB=$D
AFXDLL_OBJ=afxdll.obj
RCDEFINES=
LFLAGS=$(LFLAGS) /AL:16
MFCDLL=..\..\lib\smfc25.lib
PCH_FILE=$D\stdafx.sym
PCH_CPP=stdafx
PCH_OBJ=

!ENDIF

!ifdef RELEASE # Release VERSION info
RCDEFINES=$(RCDEFINES) -DRELEASE
!endif

dll_goal: create2.dir ..\..\bin\$(TARG).dll ..\..\lib\$(TARG).lib ..\..\lib\$(MSTARG).lib

#############################################################################
# import most rules and library files from normal makefile

!include makefile

!if "$(CODEVIEW)"=="1"
TARGOPTS_LIB=-g -C
LFLAGS=$(LFLAGS) /CO
!endif

create2.dir:
	@-if not exist $D\*.* mkdir $D
    @-if not exist ..\..\bin\*.* mkdir ..\..\bin
	@-if not exist $(OBJ_LIB)\*.* mkdir $(OBJ_LIB)

#############################################################################
# Debug target

$D\$(TARG).res: mfcdb.rc
	rcc $(RCDEFINES) -o$D\$(TARG).res mfcdb.rc

DLLOBJS=$(DB)

..\..\bin\$(TARG).dll ..\..\lib\$(TARG).lib: $(PCH_FILE) $D\dlldb.obj $(DLLOBJS) $(TARG).def $D\$(TARG).res
	@echo Creating DLL: $(TARG).DLL
	link @<<$(TARG).LNK
$D\dlldb.obj+
$(PCH_OBJ)+
$(DLLOBJS: =+
) $(LFLAGS) /IMPL:..\..\lib\$(TARG).lib /RC -31 -t :$D\$(TARG).res,
..\..\bin\$(TARG).dll,
$D\$(TARG).map /MAP:ADDRESS,
$(MFCDLL) swl libw commdlg shell odbc,
$(TARG).def;
<<

# import library
..\..\lib\$(MSTARG).lib: $(MSTARG).def $(AFXDLL_OBJ) $(SLIB_OBJ)
	if exist $@ del $@
	implib /NOWEP $@ $(MSTARG).def

#############################################################################
