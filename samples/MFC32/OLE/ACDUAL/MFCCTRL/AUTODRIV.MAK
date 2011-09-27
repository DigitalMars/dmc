# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=autodriv - Win32 (80x86) Debug
!MESSAGE No configuration specified.  Defaulting to autodriv - Win32 (80x86)\
 Debug.
!ENDIF 

!IF "$(CFG)" != "autodriv - Win32 (80x86) Debug" && "$(CFG)" !=\
 "autodriv - Win32 (80x86) Release" && "$(CFG)" !=\
 "autodriv - Win32 (80x86) Unicode Debug" && "$(CFG)" !=\
 "autodriv - Win32 (80x86) Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Autodriv.mak" CFG="autodriv - Win32 (80x86) Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "autodriv - Win32 (80x86) Debug" (based on "Win32 (x86) Application")
!MESSAGE "autodriv - Win32 (80x86) Release" (based on\
 "Win32 (x86) Application")
!MESSAGE "autodriv - Win32 (80x86) Unicode Debug" (based on\
 "Win32 (x86) Application")
!MESSAGE "autodriv - Win32 (80x86) Unicode Release" (based on\
 "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "autodriv - Win32 (80x86) Unicode Release"
MTL=mktyplib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "autodriv - Win32 (80x86) Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\Autodriv.exe" "$(OUTDIR)\Autodriv.bsc"

CLEAN : 
	-@erase "$(OUTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Autodriv.pch"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(OUTDIR)\Autodriv.bsc"
	-@erase "$(INTDIR)\autoclik.sbr"
	-@erase "$(INTDIR)\autodriv.sbr"
	-@erase "$(INTDIR)\stdafx.sbr"
	-@erase "$(INTDIR)\initIIDs.sbr"
	-@erase "$(INTDIR)\autoddlg.sbr"
	-@erase "$(OUTDIR)\Autodriv.exe"
	-@erase "$(INTDIR)\autoclik.obj"
	-@erase "$(INTDIR)\autodriv.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\initIIDs.obj"
	-@erase "$(INTDIR)\autoddlg.obj"
	-@erase "$(INTDIR)\autodriv.res"
	-@erase "$(INTDIR)\Autodriv.ilk"
	-@erase "$(INTDIR)\Autodriv.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Autodriv.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=.\WinDebug/
# ADD BASE RSC /l 0x1 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/autodriv.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Autodriv.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/autoclik.sbr" \
	"$(INTDIR)/autodriv.sbr" \
	"$(INTDIR)/stdafx.sbr" \
	"$(INTDIR)/initIIDs.sbr" \
	"$(INTDIR)/autoddlg.sbr"

"$(OUTDIR)\Autodriv.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/Autodriv.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Autodriv.exe" 
LINK32_OBJS= \
	"$(INTDIR)/autoclik.obj" \
	"$(INTDIR)/autodriv.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/initIIDs.obj" \
	"$(INTDIR)/autoddlg.obj" \
	"$(INTDIR)/autodriv.res"

"$(OUTDIR)\Autodriv.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "$(OUTDIR)\Autodriv.exe" "$(OUTDIR)\Autodriv.bsc"

CLEAN : 
	-@erase "$(OUTDIR)\Autodriv.bsc"
	-@erase "$(INTDIR)\autoclik.sbr"
	-@erase "$(INTDIR)\Autodriv.pch"
	-@erase "$(INTDIR)\stdafx.sbr"
	-@erase "$(INTDIR)\autodriv.sbr"
	-@erase "$(INTDIR)\initIIDs.sbr"
	-@erase "$(INTDIR)\autoddlg.sbr"
	-@erase "$(OUTDIR)\Autodriv.exe"
	-@erase "$(INTDIR)\autodriv.obj"
	-@erase "$(INTDIR)\initIIDs.obj"
	-@erase "$(INTDIR)\autoddlg.obj"
	-@erase "$(INTDIR)\autoclik.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\autodriv.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Autodriv.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=.\WinRel/
# ADD BASE RSC /l 0x1 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/autodriv.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Autodriv.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/autoclik.sbr" \
	"$(INTDIR)/stdafx.sbr" \
	"$(INTDIR)/autodriv.sbr" \
	"$(INTDIR)/initIIDs.sbr" \
	"$(INTDIR)/autoddlg.sbr"

"$(OUTDIR)\Autodriv.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/Autodriv.pdb" /machine:I386 /out:"$(OUTDIR)/Autodriv.exe" 
LINK32_OBJS= \
	"$(INTDIR)/autodriv.obj" \
	"$(INTDIR)/initIIDs.obj" \
	"$(INTDIR)/autoddlg.obj" \
	"$(INTDIR)/autoclik.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/autodriv.res"

"$(OUTDIR)\Autodriv.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32__8"
# PROP BASE Intermediate_Dir "Win32__8"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UniDebug"
# PROP Intermediate_Dir "UniDebug"
OUTDIR=.\UniDebug
INTDIR=.\UniDebug

ALL : "$(OUTDIR)\Autodriv.exe" "$(OUTDIR)\Autodriv.bsc"

CLEAN : 
	-@erase "$(OUTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Autodriv.pch"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(OUTDIR)\Autodriv.bsc"
	-@erase "$(INTDIR)\stdafx.sbr"
	-@erase "$(INTDIR)\autoclik.sbr"
	-@erase "$(INTDIR)\autodriv.sbr"
	-@erase "$(INTDIR)\initIIDs.sbr"
	-@erase "$(INTDIR)\autoddlg.sbr"
	-@erase "$(OUTDIR)\Autodriv.exe"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\autoclik.obj"
	-@erase "$(INTDIR)\autodriv.obj"
	-@erase "$(INTDIR)\initIIDs.obj"
	-@erase "$(INTDIR)\autoddlg.obj"
	-@erase "$(INTDIR)\autodriv.res"
	-@erase "$(INTDIR)\Autodriv.ilk"
	-@erase "$(INTDIR)\Autodriv.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /FR /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_UNICODE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Autodriv.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\UniDebug/
CPP_SBRS=.\UniDebug/
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/autodriv.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Autodriv.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/stdafx.sbr" \
	"$(INTDIR)/autoclik.sbr" \
	"$(INTDIR)/autodriv.sbr" \
	"$(INTDIR)/initIIDs.sbr" \
	"$(INTDIR)/autoddlg.sbr"

"$(OUTDIR)\Autodriv.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=/nologo /entry:"wWinMainCRTStartup" /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/Autodriv.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Autodriv.exe" 
LINK32_OBJS= \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/autoclik.obj" \
	"$(INTDIR)/autodriv.obj" \
	"$(INTDIR)/initIIDs.obj" \
	"$(INTDIR)/autoddlg.obj" \
	"$(INTDIR)/autodriv.res"

"$(OUTDIR)\Autodriv.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win32__0"
# PROP BASE Intermediate_Dir "Win32__0"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UniRel"
# PROP Intermediate_Dir "UniRel"
OUTDIR=.\UniRel
INTDIR=.\UniRel

ALL : "$(OUTDIR)\Autodriv.exe" "$(OUTDIR)\Autodriv.bsc"

CLEAN : 
	-@erase "$(OUTDIR)\Autodriv.bsc"
	-@erase "$(INTDIR)\stdafx.sbr"
	-@erase "$(INTDIR)\autoclik.sbr"
	-@erase "$(INTDIR)\Autodriv.pch"
	-@erase "$(INTDIR)\autodriv.sbr"
	-@erase "$(INTDIR)\initIIDs.sbr"
	-@erase "$(INTDIR)\autoddlg.sbr"
	-@erase "$(OUTDIR)\Autodriv.exe"
	-@erase "$(INTDIR)\autoclik.obj"
	-@erase "$(INTDIR)\autodriv.obj"
	-@erase "$(INTDIR)\initIIDs.obj"
	-@erase "$(INTDIR)\autoddlg.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\autodriv.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /FR /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_UNICODE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Autodriv.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\UniRel/
CPP_SBRS=.\UniRel/
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/autodriv.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Autodriv.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/stdafx.sbr" \
	"$(INTDIR)/autoclik.sbr" \
	"$(INTDIR)/autodriv.sbr" \
	"$(INTDIR)/initIIDs.sbr" \
	"$(INTDIR)/autoddlg.sbr"

"$(OUTDIR)\Autodriv.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=/nologo /entry:"wWinMainCRTStartup" /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/Autodriv.pdb" /machine:I386\
 /out:"$(OUTDIR)/Autodriv.exe" 
LINK32_OBJS= \
	"$(INTDIR)/autoclik.obj" \
	"$(INTDIR)/autodriv.obj" \
	"$(INTDIR)/initIIDs.obj" \
	"$(INTDIR)/autoddlg.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/autodriv.res"

"$(OUTDIR)\Autodriv.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

MTL_PROJ=

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "autodriv - Win32 (80x86) Debug"
# Name "autodriv - Win32 (80x86) Release"
# Name "autodriv - Win32 (80x86) Unicode Debug"
# Name "autodriv - Win32 (80x86) Unicode Release"

!IF  "$(CFG)" == "autodriv - Win32 (80x86) Debug"

!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Release"

!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Debug"

!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\stdafx.cpp
DEP_CPP_STDAF=\
	".\Stdafx.h"\
	".\..\server\acdual.h"\
	

!IF  "$(CFG)" == "autodriv - Win32 (80x86) Debug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Autodriv.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\stdafx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Autodriv.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Release"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Autodriv.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\stdafx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Autodriv.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Debug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_UNICODE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Autodriv.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\stdafx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Autodriv.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Release"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_UNICODE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Autodriv.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\stdafx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Autodriv.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\autodriv.cpp

!IF  "$(CFG)" == "autodriv - Win32 (80x86) Debug"

DEP_CPP_AUTOD=\
	".\Stdafx.h"\
	".\Autodriv.h"\
	".\Autoclik.h"\
	".\Autoddlg.h"\
	".\..\server\acdual.h"\
	

"$(INTDIR)\autodriv.obj" : $(SOURCE) $(DEP_CPP_AUTOD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"

"$(INTDIR)\autodriv.sbr" : $(SOURCE) $(DEP_CPP_AUTOD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"


!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Release"

DEP_CPP_AUTOD=\
	".\Stdafx.h"\
	".\Autodriv.h"\
	".\Autoclik.h"\
	".\Autoddlg.h"\
	".\..\server\acdual.h"\
	

"$(INTDIR)\autodriv.obj" : $(SOURCE) $(DEP_CPP_AUTOD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"

"$(INTDIR)\autodriv.sbr" : $(SOURCE) $(DEP_CPP_AUTOD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"


!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Debug"

DEP_CPP_AUTOD=\
	".\Stdafx.h"\
	".\..\server\acdual.h"\
	".\Autodriv.h"\
	".\Autoclik.h"\
	".\Autoddlg.h"\
	

"$(INTDIR)\autodriv.obj" : $(SOURCE) $(DEP_CPP_AUTOD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"

"$(INTDIR)\autodriv.sbr" : $(SOURCE) $(DEP_CPP_AUTOD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"


!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Release"

DEP_CPP_AUTOD=\
	".\Stdafx.h"\
	".\Autodriv.h"\
	".\Autoclik.h"\
	".\Autoddlg.h"\
	".\..\server\acdual.h"\
	

"$(INTDIR)\autodriv.obj" : $(SOURCE) $(DEP_CPP_AUTOD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"

"$(INTDIR)\autodriv.sbr" : $(SOURCE) $(DEP_CPP_AUTOD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\autoddlg.cpp

!IF  "$(CFG)" == "autodriv - Win32 (80x86) Debug"

DEP_CPP_AUTODD=\
	".\Stdafx.h"\
	".\Autodriv.h"\
	".\Autoclik.h"\
	".\Autoddlg.h"\
	".\..\server\acdual.h"\
	

"$(INTDIR)\autoddlg.obj" : $(SOURCE) $(DEP_CPP_AUTODD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"

"$(INTDIR)\autoddlg.sbr" : $(SOURCE) $(DEP_CPP_AUTODD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"


!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Release"

DEP_CPP_AUTODD=\
	".\Stdafx.h"\
	".\Autodriv.h"\
	".\Autoclik.h"\
	".\Autoddlg.h"\
	".\..\server\acdual.h"\
	

"$(INTDIR)\autoddlg.obj" : $(SOURCE) $(DEP_CPP_AUTODD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"

"$(INTDIR)\autoddlg.sbr" : $(SOURCE) $(DEP_CPP_AUTODD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"


!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Debug"

DEP_CPP_AUTODD=\
	".\Stdafx.h"\
	".\..\server\acdual.h"\
	".\Autodriv.h"\
	".\Autoclik.h"\
	".\Autoddlg.h"\
	

"$(INTDIR)\autoddlg.obj" : $(SOURCE) $(DEP_CPP_AUTODD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"

"$(INTDIR)\autoddlg.sbr" : $(SOURCE) $(DEP_CPP_AUTODD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"


!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Release"

DEP_CPP_AUTODD=\
	".\Stdafx.h"\
	".\Autodriv.h"\
	".\Autoclik.h"\
	".\Autoddlg.h"\
	".\..\server\acdual.h"\
	

"$(INTDIR)\autoddlg.obj" : $(SOURCE) $(DEP_CPP_AUTODD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"

"$(INTDIR)\autoddlg.sbr" : $(SOURCE) $(DEP_CPP_AUTODD) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\autodriv.rc
DEP_RSC_AUTODR=\
	".\res\autodriv.ico"\
	".\res\autodriv.rc2"\
	

!IF  "$(CFG)" == "autodriv - Win32 (80x86) Debug"


"$(INTDIR)\autodriv.res" : $(SOURCE) $(DEP_RSC_AUTODR) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Release"


"$(INTDIR)\autodriv.res" : $(SOURCE) $(DEP_RSC_AUTODR) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Debug"


"$(INTDIR)\autodriv.res" : $(SOURCE) $(DEP_RSC_AUTODR) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Release"


"$(INTDIR)\autodriv.res" : $(SOURCE) $(DEP_RSC_AUTODR) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\autoclik.cpp

!IF  "$(CFG)" == "autodriv - Win32 (80x86) Debug"

DEP_CPP_AUTOC=\
	".\Stdafx.h"\
	".\Autoclik.h"\
	".\..\server\acdual.h"\
	

"$(INTDIR)\autoclik.obj" : $(SOURCE) $(DEP_CPP_AUTOC) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"

"$(INTDIR)\autoclik.sbr" : $(SOURCE) $(DEP_CPP_AUTOC) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"


!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Release"

DEP_CPP_AUTOC=\
	".\Stdafx.h"\
	".\Autoclik.h"\
	".\..\server\acdual.h"\
	

"$(INTDIR)\autoclik.obj" : $(SOURCE) $(DEP_CPP_AUTOC) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"

"$(INTDIR)\autoclik.sbr" : $(SOURCE) $(DEP_CPP_AUTOC) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"


!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Debug"

DEP_CPP_AUTOC=\
	".\Stdafx.h"\
	".\..\server\acdual.h"\
	".\Autoclik.h"\
	

"$(INTDIR)\autoclik.obj" : $(SOURCE) $(DEP_CPP_AUTOC) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"

"$(INTDIR)\autoclik.sbr" : $(SOURCE) $(DEP_CPP_AUTOC) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"


!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Release"

DEP_CPP_AUTOC=\
	".\Stdafx.h"\
	".\Autoclik.h"\
	".\..\server\acdual.h"\
	

"$(INTDIR)\autoclik.obj" : $(SOURCE) $(DEP_CPP_AUTOC) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"

"$(INTDIR)\autoclik.sbr" : $(SOURCE) $(DEP_CPP_AUTOC) "$(INTDIR)"\
 "$(INTDIR)\Autodriv.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\initIIDs.cpp
DEP_CPP_INITI=\
	".\..\server\acdual.h"\
	

!IF  "$(CFG)" == "autodriv - Win32 (80x86) Debug"

# SUBTRACT CPP /YX /Yc /Yu

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\initIIDs.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\initIIDs.sbr" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Release"

# SUBTRACT CPP /YX /Yc /Yu

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\initIIDs.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\initIIDs.sbr" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Debug"

# SUBTRACT CPP /YX /Yc /Yu

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_UNICODE" /FR"$(INTDIR)/" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\initIIDs.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\initIIDs.sbr" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "autodriv - Win32 (80x86) Unicode Release"

# SUBTRACT CPP /YX /Yc /Yu

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_UNICODE" /FR"$(INTDIR)/" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\initIIDs.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\initIIDs.sbr" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
