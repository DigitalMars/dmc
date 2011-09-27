# Microsoft Developer Studio Generated NMAKE File, Format Version 42001
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=ExtBind - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to ExtBind - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ExtBind - Win32 Release" && "$(CFG)" !=\
 "ExtBind - Win32 Debug" && "$(CFG)" != "ExtBind - Win32 Unicode Debug" &&\
 "$(CFG)" != "ExtBind - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "ExtBind.mak" CFG="ExtBind - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ExtBind - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ExtBind - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ExtBind - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "ExtBind - Win32 Unicode Release" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "ExtBind - Win32 Unicode Release"
MTL=mktyplib.exe
CPP=cl.exe
F90=fl32.exe
RSC=rc.exe

!IF  "$(CFG)" == "ExtBind - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\ExtBind.exe"

CLEAN : 
	-@erase "$(INTDIR)\extbind.obj"
	-@erase "$(INTDIR)\ExtBind.pch"
	-@erase "$(INTDIR)\extbind.res"
	-@erase "$(INTDIR)\mcsdsc.obj"
	-@erase "$(INTDIR)\modeldlg.obj"
	-@erase "$(INTDIR)\msacal70.obj"
	-@erase "$(INTDIR)\msdgrid.obj"
	-@erase "$(INTDIR)\msmask.obj"
	-@erase "$(INTDIR)\nwinddlg.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(OUTDIR)\ExtBind.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE F90 /Ox /I "Release/" /c /nologo
# ADD F90 /Ox /I "Release/" /c /nologo
F90_PROJ=/Ox /I "Release/" /c /nologo /Fo"Release/" 
F90_OBJS=.\Release/
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ExtBind.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/extbind.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ExtBind.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/ExtBind.pdb" /machine:I386 /out:"$(OUTDIR)/ExtBind.exe" 
LINK32_OBJS= \
	"$(INTDIR)\extbind.obj" \
	"$(INTDIR)\extbind.res" \
	"$(INTDIR)\mcsdsc.obj" \
	"$(INTDIR)\modeldlg.obj" \
	"$(INTDIR)\msacal70.obj" \
	"$(INTDIR)\msdgrid.obj" \
	"$(INTDIR)\msmask.obj" \
	"$(INTDIR)\nwinddlg.obj" \
	"$(INTDIR)\stdafx.obj"

"$(OUTDIR)\ExtBind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ExtBind - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\ExtBind.exe"

CLEAN : 
	-@erase "$(INTDIR)\extbind.obj"
	-@erase "$(INTDIR)\ExtBind.pch"
	-@erase "$(INTDIR)\extbind.res"
	-@erase "$(INTDIR)\mcsdsc.obj"
	-@erase "$(INTDIR)\modeldlg.obj"
	-@erase "$(INTDIR)\msacal70.obj"
	-@erase "$(INTDIR)\msdgrid.obj"
	-@erase "$(INTDIR)\msmask.obj"
	-@erase "$(INTDIR)\nwinddlg.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\ExtBind.exe"
	-@erase "$(OUTDIR)\ExtBind.ilk"
	-@erase "$(OUTDIR)\ExtBind.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE F90 /Zi /I "Debug/" /c /nologo
# ADD F90 /Zi /I "Debug/" /c /nologo
F90_PROJ=/Zi /I "Debug/" /c /nologo /Fo"Debug/" /Fd"Debug/ExtBind.pdb" 
F90_OBJS=.\Debug/
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ExtBind.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/extbind.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ExtBind.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/ExtBind.pdb" /debug /machine:I386 /out:"$(OUTDIR)/ExtBind.exe" 
LINK32_OBJS= \
	"$(INTDIR)\extbind.obj" \
	"$(INTDIR)\extbind.res" \
	"$(INTDIR)\mcsdsc.obj" \
	"$(INTDIR)\modeldlg.obj" \
	"$(INTDIR)\msacal70.obj" \
	"$(INTDIR)\msdgrid.obj" \
	"$(INTDIR)\msmask.obj" \
	"$(INTDIR)\nwinddlg.obj" \
	"$(INTDIR)\stdafx.obj"

"$(OUTDIR)\ExtBind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Unicode Debug"
# PROP BASE Intermediate_Dir "Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UniDebug"
# PROP Intermediate_Dir "UniDebug"
# PROP Target_Dir ""
OUTDIR=.\UniDebug
INTDIR=.\UniDebug

ALL : "$(OUTDIR)\ExtBind.exe"

CLEAN : 
	-@erase "$(INTDIR)\extbind.obj"
	-@erase "$(INTDIR)\ExtBind.pch"
	-@erase "$(INTDIR)\extbind.res"
	-@erase "$(INTDIR)\mcsdsc.obj"
	-@erase "$(INTDIR)\modeldlg.obj"
	-@erase "$(INTDIR)\msacal70.obj"
	-@erase "$(INTDIR)\msdgrid.obj"
	-@erase "$(INTDIR)\msmask.obj"
	-@erase "$(INTDIR)\nwinddlg.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\ExtBind.exe"
	-@erase "$(OUTDIR)\ExtBind.ilk"
	-@erase "$(OUTDIR)\ExtBind.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE F90 /Zi /I "Unicode Debug/" /c /nologo
# ADD F90 /Zi /I "UniDebug/" /c /nologo
F90_PROJ=/Zi /I "UniDebug/" /c /nologo /Fo"UniDebug/" /Fd"UniDebug/ExtBind.pdb"\
 
F90_OBJS=.\UniDebug/
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ExtBind.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\UniDebug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/extbind.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ExtBind.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/ExtBind.pdb" /debug /machine:I386 /out:"$(OUTDIR)/ExtBind.exe" 
LINK32_OBJS= \
	"$(INTDIR)\extbind.obj" \
	"$(INTDIR)\extbind.res" \
	"$(INTDIR)\mcsdsc.obj" \
	"$(INTDIR)\modeldlg.obj" \
	"$(INTDIR)\msacal70.obj" \
	"$(INTDIR)\msdgrid.obj" \
	"$(INTDIR)\msmask.obj" \
	"$(INTDIR)\nwinddlg.obj" \
	"$(INTDIR)\stdafx.obj"

"$(OUTDIR)\ExtBind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Unicode Release"
# PROP BASE Intermediate_Dir "Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UniRel"
# PROP Intermediate_Dir "UniRel"
# PROP Target_Dir ""
OUTDIR=.\UniRel
INTDIR=.\UniRel

ALL : "$(OUTDIR)\ExtBind.exe"

CLEAN : 
	-@erase "$(INTDIR)\extbind.obj"
	-@erase "$(INTDIR)\ExtBind.pch"
	-@erase "$(INTDIR)\extbind.res"
	-@erase "$(INTDIR)\mcsdsc.obj"
	-@erase "$(INTDIR)\modeldlg.obj"
	-@erase "$(INTDIR)\msacal70.obj"
	-@erase "$(INTDIR)\msdgrid.obj"
	-@erase "$(INTDIR)\msmask.obj"
	-@erase "$(INTDIR)\nwinddlg.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(OUTDIR)\ExtBind.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE F90 /Ox /I "Unicode Release/" /c /nologo
# ADD F90 /Ox /I "UniRel/" /c /nologo
F90_PROJ=/Ox /I "UniRel/" /c /nologo /Fo"UniRel/" 
F90_OBJS=.\UniRel/
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ExtBind.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\UniRel/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/extbind.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ExtBind.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/ExtBind.pdb" /machine:I386 /out:"$(OUTDIR)/ExtBind.exe" 
LINK32_OBJS= \
	"$(INTDIR)\extbind.obj" \
	"$(INTDIR)\extbind.res" \
	"$(INTDIR)\mcsdsc.obj" \
	"$(INTDIR)\modeldlg.obj" \
	"$(INTDIR)\msacal70.obj" \
	"$(INTDIR)\msdgrid.obj" \
	"$(INTDIR)\msmask.obj" \
	"$(INTDIR)\nwinddlg.obj" \
	"$(INTDIR)\stdafx.obj"

"$(OUTDIR)\ExtBind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

################################################################################
# Begin Target

# Name "ExtBind - Win32 Release"
# Name "ExtBind - Win32 Debug"
# Name "ExtBind - Win32 Unicode Debug"
# Name "ExtBind - Win32 Unicode Release"

!IF  "$(CFG)" == "ExtBind - Win32 Release"

!ELSEIF  "$(CFG)" == "ExtBind - Win32 Debug"

!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\stdafx.cpp
DEP_CPP_STDAF=\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "ExtBind - Win32 Release"

# ADD CPP /Yc

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ExtBind.pch" /Yc /Fo"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ExtBind.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "ExtBind - Win32 Debug"

# ADD CPP /Yc

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ExtBind.pch" /Yc /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ExtBind.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Debug"

# ADD BASE CPP /Yc
# ADD CPP /Yc

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ExtBind.pch" /Yc\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ExtBind.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Release"

# ADD BASE CPP /Yc
# ADD CPP /Yc

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ExtBind.pch" /Yc\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ExtBind.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\nwinddlg.cpp
DEP_CPP_NWIND=\
	".\extbind.h"\
	".\mcsdsc.h"\
	".\modeldlg.h"\
	".\msacal70.h"\
	".\msdgrid.h"\
	".\msmask.h"\
	".\nwinddlg.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "ExtBind - Win32 Release"


"$(INTDIR)\nwinddlg.obj" : $(SOURCE) $(DEP_CPP_NWIND) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Debug"


"$(INTDIR)\nwinddlg.obj" : $(SOURCE) $(DEP_CPP_NWIND) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Debug"


"$(INTDIR)\nwinddlg.obj" : $(SOURCE) $(DEP_CPP_NWIND) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Release"


"$(INTDIR)\nwinddlg.obj" : $(SOURCE) $(DEP_CPP_NWIND) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\msmask.cpp
DEP_CPP_MSMAS=\
	".\msmask.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "ExtBind - Win32 Release"


"$(INTDIR)\msmask.obj" : $(SOURCE) $(DEP_CPP_MSMAS) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Debug"


"$(INTDIR)\msmask.obj" : $(SOURCE) $(DEP_CPP_MSMAS) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Debug"


"$(INTDIR)\msmask.obj" : $(SOURCE) $(DEP_CPP_MSMAS) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Release"


"$(INTDIR)\msmask.obj" : $(SOURCE) $(DEP_CPP_MSMAS) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\msdgrid.cpp
DEP_CPP_MSDGR=\
	".\msdgrid.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "ExtBind - Win32 Release"


"$(INTDIR)\msdgrid.obj" : $(SOURCE) $(DEP_CPP_MSDGR) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Debug"


"$(INTDIR)\msdgrid.obj" : $(SOURCE) $(DEP_CPP_MSDGR) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Debug"


"$(INTDIR)\msdgrid.obj" : $(SOURCE) $(DEP_CPP_MSDGR) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Release"


"$(INTDIR)\msdgrid.obj" : $(SOURCE) $(DEP_CPP_MSDGR) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\msacal70.cpp
DEP_CPP_MSACA=\
	".\msacal70.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "ExtBind - Win32 Release"


"$(INTDIR)\msacal70.obj" : $(SOURCE) $(DEP_CPP_MSACA) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Debug"


"$(INTDIR)\msacal70.obj" : $(SOURCE) $(DEP_CPP_MSACA) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Debug"


"$(INTDIR)\msacal70.obj" : $(SOURCE) $(DEP_CPP_MSACA) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Release"


"$(INTDIR)\msacal70.obj" : $(SOURCE) $(DEP_CPP_MSACA) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\modeldlg.cpp
DEP_CPP_MODEL=\
	".\extbind.h"\
	".\mcsdsc.h"\
	".\modeldlg.h"\
	".\msacal70.h"\
	".\msdgrid.h"\
	".\msmask.h"\
	".\nwinddlg.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "ExtBind - Win32 Release"


"$(INTDIR)\modeldlg.obj" : $(SOURCE) $(DEP_CPP_MODEL) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Debug"


"$(INTDIR)\modeldlg.obj" : $(SOURCE) $(DEP_CPP_MODEL) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Debug"


"$(INTDIR)\modeldlg.obj" : $(SOURCE) $(DEP_CPP_MODEL) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Release"


"$(INTDIR)\modeldlg.obj" : $(SOURCE) $(DEP_CPP_MODEL) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mcsdsc.cpp
DEP_CPP_MCSDS=\
	".\mcsdsc.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "ExtBind - Win32 Release"


"$(INTDIR)\mcsdsc.obj" : $(SOURCE) $(DEP_CPP_MCSDS) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Debug"


"$(INTDIR)\mcsdsc.obj" : $(SOURCE) $(DEP_CPP_MCSDS) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Debug"


"$(INTDIR)\mcsdsc.obj" : $(SOURCE) $(DEP_CPP_MCSDS) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Release"


"$(INTDIR)\mcsdsc.obj" : $(SOURCE) $(DEP_CPP_MCSDS) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\extbind.cpp
DEP_CPP_EXTBI=\
	".\extbind.h"\
	".\mcsdsc.h"\
	".\modeldlg.h"\
	".\msacal70.h"\
	".\msdgrid.h"\
	".\msmask.h"\
	".\nwinddlg.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "ExtBind - Win32 Release"


"$(INTDIR)\extbind.obj" : $(SOURCE) $(DEP_CPP_EXTBI) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Debug"


"$(INTDIR)\extbind.obj" : $(SOURCE) $(DEP_CPP_EXTBI) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Debug"


"$(INTDIR)\extbind.obj" : $(SOURCE) $(DEP_CPP_EXTBI) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Release"


"$(INTDIR)\extbind.obj" : $(SOURCE) $(DEP_CPP_EXTBI) "$(INTDIR)"\
 "$(INTDIR)\ExtBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\extbind.rc
DEP_RSC_EXTBIN=\
	".\res\extbind.bmp"\
	".\res\extbind.ico"\
	".\res\extbind.rc2"\
	".\resource.hm"\
	

!IF  "$(CFG)" == "ExtBind - Win32 Release"


"$(INTDIR)\extbind.res" : $(SOURCE) $(DEP_RSC_EXTBIN) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Debug"


"$(INTDIR)\extbind.res" : $(SOURCE) $(DEP_RSC_EXTBIN) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Debug"


"$(INTDIR)\extbind.res" : $(SOURCE) $(DEP_RSC_EXTBIN) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ExtBind - Win32 Unicode Release"


"$(INTDIR)\extbind.res" : $(SOURCE) $(DEP_RSC_EXTBIN) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
