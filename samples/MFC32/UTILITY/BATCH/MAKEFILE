#===========================================================================;
#
#   Batch -- Batch Compression Sample Application
#
#
#############################################################################
#
#   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
#   KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
#   IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
#   PURPOSE.
#
#   Copyright (c) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
#
#############################################################################

CPPFLAGS = /D_WINDOWS /W3 /GX /MT
LINKFLAGS = -subsystem:windows -incremental:yes

!if "$(DEBUG)" != "0"
OBJ_DIR=Win32
CPPFLAGS=$(CPPFLAGS) /D_DEBUG /Fd$(OBJ_DIR)\ /Fo$(OBJ_DIR)\ /Fp$(OBJ_DIR)\batch.pch \
	/FR"$(OBJ_DIR)/" /Od /Zi 
LINKFLAGS=$(LINKFLAGS) -debug:full -out:$(OBJ_DIR)\batch.exe
LIBS=vfw32.lib msacm32.lib
RCDEFS=/l0x409 /D_DEBUG /fo$(OBJ_DIR)\batch.res
!else
OBJ_DIR=WinRel
CPPFLAGS=$(CPPFLAGS) /DNDEBUG /Fd$(OBJ_DIR)\ /Fo$(OBJ_DIR)\ /Fp$(OBJ_DIR)\batch.pch \
	/FR"$(OBJ_DIR)/" /O2
LINKFLAGS=$(LINKFLAGS) -debug:none -out:$(OBJ_DIR)\batch.exe
LIBS=vfw32.lib msacm32.lib
RCDEFS=/l0x409 /DNDEBUG /fo$(OBJ_DIR)\batch.res
!endif

#############################################################################
# Targets

all: $(OBJ_DIR) $(OBJ_DIR)\batch.exe batch.bsc

$(OBJ_DIR):
	@if not exist $(OBJ_DIR) md $(OBJ_DIR)

#############################################################################
# Special inference rules (applied to all .cpp files except stdafx.cpp)

.cpp{$(OBJ_DIR)}.obj:
   $(CPP) $(CPPFLAGS) /Yustdafx.h /c $<

#############################################################################
# file list

OBJS= \
	$(OBJ_DIR)\stdafx.obj  \
	$(OBJ_DIR)\batch.obj \
	$(OBJ_DIR)\batchdoc.obj \
	$(OBJ_DIR)\batchvw.obj \
	$(OBJ_DIR)\mainfrm.obj \
	$(OBJ_DIR)\compress.obj \
	$(OBJ_DIR)\interlea.obj \
	$(OBJ_DIR)\destdird.obj \
	  # TODO: add more files here

RESOURCES = \
	$(OBJ_DIR)\batch.res


#############################################################################
# Final linking

$(OBJ_DIR)\batch.exe: $(OBJ_DIR) $(OBJS) $(RESOURCES)
        link $(LINKFLAGS) @<<
$(OBJS)
$(LIBS)
$(RESOURCES)
<<

batch.bsc: $(OBJ_DIR)\*.sbr
        bscmake /o batch.bsc $(OBJ_DIR)\*.sbr

#############################################################################
# Compiling resources


$(OBJ_DIR)\batch.res: resource.h batch.rc batch.rc2 *.ico *.bmp
	rc /r $(RCDEFS) batch.rc


#############################################################################
# Special rules

# 'batch.pch' contains pre-compiled header
# 'stdafx.obj' to contain pre-compiled types
$(OBJ_DIR)\stdafx.obj : stdafx.h
	$(CPP) $(CPPFLAGS) /Ycstdafx.h /c stdafx.cpp

# Note: no explicit dependency on resource.h
$(OBJ_DIR)\batch.obj: \
	batch.h \
	mainfrm.h \
	batchdoc.h \
	batchvw.h \
	$(OBJ_DIR)\batch.pch

$(OBJ_DIR)\mainfrm.obj: batch.h mainfrm.h $(OBJ_DIR)\batch.pch

$(OBJ_DIR)\batchdoc.obj: batch.h \
	batchdoc.h \
	$(OBJ_DIR)\batch.pch

$(OBJ_DIR)\batchvw.obj: batch.h \
	batchdoc.h \
	batchvw.h \
	$(OBJ_DIR)\batch.pch

$(OBJ_DIR)\compress.obj: batch.h compress.h $(OBJ_DIR)\batch.pch

$(OBJ_DIR)\interlea.obj: batch.h interlea.h $(OBJ_DIR)\batch.pch

$(OBJ_DIR)\destdird.obj: batch.h destdird.h $(OBJ_DIR)\batch.pch

clean:
	-erase $(OBJ_DIR)\batch.exe
	-erase $(OBJ_DIR)\batch.res
	-erase $(OBJ_DIR)\batch.pch
	-erase $(OBJ_DIR)\batch.map
	-erase $(OBJ_DIR)\batch.sym
	-erase $(OBJ_DIR)\batch.pdb
	-erase $(OBJ_DIR)\*.obj

#############################################################################
