# This is a part of the Microsoft Foundation Classes C++ library.
# Copyright (C) 1996 Microsoft Corporation
# All rights reserved.
#
# This source code is only intended as a supplement to the
# Microsoft Foundation Classes Reference and related
# electronic documentation provided with the library.
# See these sources for detailed information regarding the
# Microsoft Foundation Classes product.

# Default PLATFORM depending on host environment
!ifndef PLATFORM
!ifndef PROCESSOR_ARCHITECTURE
!error PLATFORM must be set to intended target
!endif
!if "$(PROCESSOR_ARCHITECTURE)" == "x86"
PLATFORM=INTEL
!endif
!if "$(PROCESSOR_ARCHITECTURE)" == "MIPS"
PLATFORM=MIPS
!endif
!if "$(PROCESSOR_ARCHITECTURE)" == "ALPHA"
PLATFORM=ALPHA
!endif
!if "$(PROCESSOR_ARCHITECTURE)" == "PPC"
PLATFORM=PPC
!endif
!endif

# Default to DEBUG mode
!ifndef DEBUG
DEBUG=1
!endif

# Default to NOT DLL
!ifndef DLL
DLL=0
!endif

# Default Codeview Info
!ifndef CODEVIEW
!if "$(DEBUG)" == "1"
CODEVIEW=1
!else
CODEVIEW=0
!endif
!endif

# BROWSEONLY is default 0 and implies BROWSE=1 if BROWSEONLY=1
!ifndef BROWSEONLY
BROWSEONLY=0
!endif

!if "$(BROWSEONLY)" != "0"
!undef BROWSE
BROWSE=1
!endif

# Default to no BROWSE info
!ifndef BROWSE
BROWSE=0
!endif

# Default to _MBCS build
!ifndef MBCS
MBCS=1
!endif

# Default to multithreading support
!ifndef MT
MT=1
!endif

!if "$(MT)" != "1"
!error This library must be built with threadding enabled.
!endif

# TYPE = Library Type Designator
#       N = normal C library
#   D = for use with MFC USRDLL library
#       E = for use with MFC Extension DLL library

!if "$(DLL)" == "0"
TYPE=N
!if "$(DEBUG)" == "1"
TARGOPTS=$(TARGOPTS) /MTd
!else
TARGOPTS=$(TARGOPTS) /MT
!endif
!endif

!if "$(DLL)" == "1"
!error The _USRDLL configuration is not supported by this library.
!endif

!if "$(DLL)" == "2"
!if "$(DEBUG)" == "1"
TARGOPTS=$(TARGOPTS) /MDd
!else
TARGOPTS=$(TARGOPTS) /MD
!endif
TARGDEFS=$(TARGDEFS) /D_WINDLL /D_AFXDLL /D_DLL /D_AFX_CORE_IMPL
TYPE=E
!endif

#############################################################################
# normalize cases of parameters, or error check

!if "$(CPU)" == "MIPS"
!if "$(PLATFORM)" != "MIPS"
!error Must set PLATFORM=MIPS for MIPS builds
!endif
!endif

!if "$(CPU)" == "ALPHA"
!if "$(PLATFORM)" != "ALPHA"
!error Must set PLATFORM=ALPHA for ALPHA builds
!endif
!endif

#############################################################################
# Parse options

#
# DEBUG OPTIONS
#
!if "$(DEBUG)" != "0"

DEBUGSUF=D
DEBDEFS=/D_DEBUG
DEBOPTS=/Od

!endif

#
# NON-DEBUG OPTIONS
#
!if "$(DEBUG)" == "0"

DEBUGSUF=
DEBDEFS=

!if "$(PLATFORM)" == "INTEL"
DEBOPTS=/O1 /Gy
!endif
!if "$(PLATFORM)" == "MIPS"
DEBOPTS=/O1 /Gy
!endif
!if "$(PLATFORM)" == "ALPHA"
DEBOPTS=/O1 /Gy
!endif
!if "$(PLATFORM)" == "PPC"
DEBOPTS=/O1 /Gy
!endif
!endif

#
# PLATFORM options
#
CPP=cl
LIB32=lib
LINK32=link

!if "$(PLATFORM)" == "INTEL"
CL_MODEL=/D_X86_
!endif

!if "$(PLATFORM)" == "MIPS"
CL_MODEL=/D_MIPS_
!endif

!if "$(PLATFORM)" == "ALPHA"
CL_MODEL=/D_ALPHA_
!endif

!if "$(PLATFORM)" == "PPC"
CL_MODEL=/D_PPC_
!if "$(PROCESSOR_ARCHITECTURE)" == "x86"
CPP=mcl
!endif
!endif

!if "$(CPP)" == ""
!error PLATFORM must be one of INTEL, MIPS, ALPHA, PPC
!endif


#REVIEW: does ISAPI support UNICODE?

!if "$(UNICODE)" == "1"
MODEL=ISU
TARGDEFS=$(TARGDEFS) /D_UNICODE
!else
MODEL=IS
!if "$(MBCS)" != "0"
TARGDEFS=$(TARGDEFS) /D_MBCS
!endif
!endif

#
# Object File Directory
#
!if "$(OBJ)" == ""
D=$$$(TYPE)$(MODEL)$(DEBUGSUF)    # subdirectory specific to variant
!else
D=$(OBJ)                                 # User specified directory
!endif

GOAL=$(TYPE)AFX$(MODEL)$(DEBUGSUF)

#
# CODEVIEW options
#
!if "$(CODEVIEW)" == "1"
!if "$(NO_PDB)" == "1"
CVOPTS=/Z7
!else
CVOPTS=/Zi /Fd..\lib\$(GOAL).pdb
!endif
!endif

#
# COMPILER OPTIONS
#
!if "$(PLATFORM)" == "INTEL"
CL_OPT=/W4 /WX /Zl /GX $(DEBOPTS) $(CVOPTS) $(TARGOPTS)
!endif

!if "$(PLATFORM)" == "MIPS"
CL_OPT=/W4 /WX /Zl /GX $(DEBOPTS) $(CVOPTS) $(TARGOPTS)
!endif

!if "$(PLATFORM)" == "ALPHA"
CL_OPT=/W4 /WX /Zl /GX $(DEBOPTS) $(CVOPTS) $(TARGOPTS)
!endif

!if "$(PLATFORM)" == "PPC"
CL_OPT=/W4 /Zl $(DEBOPTS) $(CVOPTS) $(TARGOPTS)
!endif

!if "$(BROWSE)" != "0"
CL_OPT=/FR$D\ $(CL_OPT)
!endif

!if "$(BROWSEONLY)" != "0"
CL_OPT=/Zs $(CL_OPT)
!else
CL_OPT=/Fo$D\ $(CL_OPT)
!endif

DEFS=$(DEFS) $(DEBDEFS) $(TARGDEFS)

#############################################################################
# Library Components

OBJS=$D\isapi.obj $D\inetcall.obj

#############################################################################
# Standard tools

#############################################################################
# Set CPPFLAGS for use with .cpp.obj and .c.obj rules
# Define rule for use with OBJ directory
# C++ uses a PCH file

CPPFLAGS=$(CPPFLAGS) $(CL_MODEL) $(CL_OPT) $(DEFS) $(OPT)

PCH_FILE=

.SUFFIXES: .cpp

.cpp{$D}.obj:
	$(CPP) @<<
$(CPPFLAGS) /c $<
<<
!if "$(BROWSE)" != "0"
	copy /b $*.sbr+pchmark.bin $*.sbr >NUL
!endif

.cpp{$D}.sbr:
	$(CPP) @<<
$(CPPFLAGS) /c $<
<<
	copy /b $*.sbr+pchmark.bin $*.sbr >NUL

#############################################################################
# Goals to build

GOALS=create.dir
!if "$(BROWSEONLY)" == "0"
GOALS=$(GOALS) ..\lib\$(GOAL).lib
!endif
!if "$(BROWSE)" != "0"
GOALS=$(GOALS) $(GOAL).bsc
!endif

goal: $(GOALS)

create.dir:
	@-if not exist $D\*.* mkdir $D

clean:
	-if exist $D\*.obj erase $D\*.obj
	-if exist $D\*.pch erase $D\*.pch
	-if exist $D\*.res erase $D\*.res
	-if exist $D\*.rsc erase $D\*.rsc
	-if exist $D\*.map erase $D\*.map
	-if exist $D\*.* rmdir $D
	-if exist ..\lib\$(GOAL).pdb del ..\lib\$(GOAL).pdb

#############################################################################
# Build the library from the up-to-date objs

SBRS=$(CPP_OBJS:.obj=.sbr)

!if "$(BROWSEONLY)" != "0"

# Build final browse database
$(GOAL).bsc: $(SBRS)
	bscmake /n /Iu /El /o$@ @<<
$(SBRS)
<<

!else #BROWSEONLY

# Build final library
..\lib\$(GOAL).lib: $(OBJS)
	@-if exist $@ erase $@
	@$(LIB32) /out:$@ @<<
$(OBJS)
<<

# Recurse to build browse database
$(GOAL).bsc: $(SBRS)
	$(MAKE) /f makefile. @<<
BROWSEONLY=1 PLATFORM=$(PLATFORM) DEBUG=$(DEBUG) CODEVIEW=$(CODEVIEW) \
DLL=$(DLL) OBJ=$(OBJ) OPT=$(OPT)
<<

!endif #!BROWSEONLY

#############################################################################
