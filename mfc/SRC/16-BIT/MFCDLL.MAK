# This is a part of the Microsoft Foundation Classes C++ library.
# Copyright (C) 1992-1993 Microsoft Corporation
# All rights reserved.
#
# This source code is only intended as a supplement to the
# Microsoft Foundation Classes Reference and Microsoft
# QuickHelp documentation provided with the library.
# See these sources for detailed information regarding the
# Microsoft Foundation Classes product.
#

# SMFC25[D].DLL is a large model DLL built with MFC large model [debug] DLL
#  which exports all the MFC classes
# If you need a private build of the MFC DLL, be sure to rename
#  "SMFC25.DLL" to something more appropriate for your application.
# Please do not re-distribute a privately built version with the
#  name "SMFC25.DLL".


TARGET=W
DLL=2
TARG=SMFC25
MSTARG=MFC250
MASMOPT=
LFLAGS=/NOD /NOE /DE /PACKDATA /NOPACKCODE /B /NOLOGO
# /NOPACKCODE => keep MFC library segmentation

!if "$(DEBUG)"!="0"
# Debug DLL build
TARG=$(TARG)D
MSTARG=$(MSTARG)D
AFXDLL_OBJ=afxdlld.obj
RCDEFINES=-D_DEBUG
LFLAGS=$(LFLAGS)
!ifndef NO_PCH
PCH_OBJ=$D\stdafx.obj
!else
PCH_OBJ=
!endif

!ELSE
# Release DLL build
AFXDLL_OBJ=afxdll.obj
RCDEFINES=
LFLAGS=$(LFLAGS) /AL:16
PCH_OBJ=
!ENDIF

!ifdef RELEASE # Release VERSION info
RCDEFINES=$(RCDEFINES) -DRELEASE
!endif

dll_goal: create2.dir ..\..\bin\$(TARG).dll ..\..\lib\$(TARG).lib ..\..\lib\$(MSTARG).lib

#############################################################################
# import most rules and library files from normal makefile

!include makefile

OBJ_LIB=$D.lib

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

$D\$(TARG).res: mfcdll.rc
	rcc $(RCDEFINES) -o$D\$(TARG).res mfcdll.rc

DLLOBJS=$(OBJECT) $(INLINES) $(FILES) $(COLLECTIONS1) $(COLLECTIONS2) \
	$(MISC) $(WINDOWS) $(DIALOG) $(WINMISC) $(DOCVIEW) $(APPLICATION) \
	$(VBX_OBJ) $(OLEREQ)

..\..\bin\$(TARG).dll ..\..\lib\$(TARG).lib: $(PCH_FILE) $(DLLOBJS) $D\dllinit.obj $(TARG).def $D\$(TARG).res
    @-if exist ..\..\lib\$(MSTARG).lib del ..\..\lib\$(MSTARG).lib
	@echo Creating DLL: $(TARG).DLL
	link  @<<$(TARG).LNK
$D\dllinit.obj+
$(PCH_OBJ)+
$(DLLOBJS: =+
) $(LFLAGS) /IMPL:..\..\lib\$(TARG).lib /RC -31 -t :$D\$(TARG).res,
..\..\bin\$(TARG).dll,
$D\$(TARG).map /MAP:ADDRESS,
swl libw commdlg shell,
$(TARG).def;
<<

#############################################################################
# Special rules for building static linked library part

# NonShared diagnostics and other hooks (see OBJDIAG in MAKEFILE)
SLIB_OBJ= $(OBJ_LIB)\dumpinit.obj $(OBJ_LIB)\dumpout.obj \
	$(OBJ_LIB)\afxasert.obj $(OBJ_LIB)\afxmem.obj $(OBJ_LIB)\afxabort.obj

# _AFXDLL defined (but not _WINDLL)
TARGOPTS_LIB=$(TARGOPTS_LIB) -WA-r  -2
CPPFLAGS_LIB=$(CL_MODEL) -wx -a $(TARGOPTS_LIB) $(DEFS) $(OPT)
PCH_LIB=$(OBJ_LIB)\stdafx.sym

$(OBJ_LIB)\stdafx.sym: stdafx.h $(HDRS)
	$(CPP) -c -cpp stdafx.h @<<$(TARG).OPT
-HF$(PCH_LIB) $(CPPFLAGS_LIB) -o$(OBJ_LIB)\
<<


.cpp{$(OBJ_LIB)}.obj:
	$(CPP) -c $< @<<$(TARG).OPT
$(CPPFLAGS_LIB) -H -HD$(OBJ_LIB) -o$(OBJ_LIB)\ 
<<

# AFXDLL.OBJ and AFXDLLD.OBJ are pre-built using the following MASM rules
afxdll.obj: afxdll.asm
	masm /Mx $(MASM_OPT) afxdll.asm,afxdll.obj;
afxdlld.obj: afxdll.asm
	masm /Mx $(MASM_OPT) /D_DEBUG afxdll.asm,afxdlld.obj;
	
	
# import libraries for Built Shared DLLS
..\..\lib\$(MSTARG).lib: $(PCH_LIB) $(MSTARG).def $(AFXDLL_OBJ) $(SLIB_OBJ)
	if exist $@ del $@
	implib /NOWEP $@ $(MSTARG).def
	lib /NOEXT /PAGESIZE:16 @<<$(MSTARG).RSP
$@
$(AFXDLL_OBJ)&
$(SLIB_OBJ)
;
<<
	if exist ..\..\lib\$(MSTARG).bak del ..\..\lib\$(MSTARG).bak

# $(TARG).lib was created at link stage, simply  add objs to it.
	lib /NOEXT /PAGESIZE:16 @<<$(TARG).RSP
..\..\lib\$(TARG).lib 
$(AFXDLL_OBJ)&
$(SLIB_OBJ)
;
<<
	if exist ..\..\lib\$(TARG).bak del ..\..\lib\$(TARG).bak
	
#############################################################################
	

