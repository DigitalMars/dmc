# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=MDIBind - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to MDIBind - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MDIBind - Win32 Release" && "$(CFG)" !=\
 "MDIBind - Win32 Debug" && "$(CFG)" != "MDIBind - Win32 Unicode Debug" &&\
 "$(CFG)" != "MDIBind - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "MDIBind.mak" CFG="MDIBind - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MDIBind - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MDIBind - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "MDIBind - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "MDIBind - Win32 Unicode Release" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "MDIBind - Win32 Debug"
MTL=mktyplib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MDIBind - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\MDIBind.exe"

CLEAN : 
	-@erase "$(INTDIR)\DSNDlg.obj"
	-@erase "$(INTDIR)\gridfrm.obj"
	-@erase "$(INTDIR)\gridview.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\maskfrm.obj"
	-@erase "$(INTDIR)\maskview.obj"
	-@erase "$(INTDIR)\mcsdsc.obj"
	-@erase "$(INTDIR)\MDIBind.obj"
	-@erase "$(INTDIR)\MDIBind.pch"
	-@erase "$(INTDIR)\MDIBind.res"
	-@erase "$(INTDIR)\MDIDoc.obj"
	-@erase "$(INTDIR)\msdgrid.obj"
	-@erase "$(INTDIR)\msmask.obj"
	-@erase "$(INTDIR)\RDCDlg.obj"
	-@erase "$(INTDIR)\RDCFrm.obj"
	-@erase "$(INTDIR)\RDCView.obj"
	-@erase "$(INTDIR)\rdocol.obj"
	-@erase "$(INTDIR)\rdocols.obj"
	-@erase "$(INTDIR)\rdoreslt.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\MDIBind.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=fl32.exe
# ADD BASE F90 /I "Release/"
# ADD F90 /I "Release/"
F90_OBJS=.\Release/
F90_PROJ=/I "Release/" /Fo"Release/" 

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MDIBind.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MDIBind.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MDIBind.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/MDIBind.pdb" /machine:I386 /out:"$(OUTDIR)/MDIBind.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DSNDlg.obj" \
	"$(INTDIR)\gridfrm.obj" \
	"$(INTDIR)\gridview.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\maskfrm.obj" \
	"$(INTDIR)\maskview.obj" \
	"$(INTDIR)\mcsdsc.obj" \
	"$(INTDIR)\MDIBind.obj" \
	"$(INTDIR)\MDIBind.res" \
	"$(INTDIR)\MDIDoc.obj" \
	"$(INTDIR)\msdgrid.obj" \
	"$(INTDIR)\msmask.obj" \
	"$(INTDIR)\RDCDlg.obj" \
	"$(INTDIR)\RDCFrm.obj" \
	"$(INTDIR)\RDCView.obj" \
	"$(INTDIR)\rdocol.obj" \
	"$(INTDIR)\rdocols.obj" \
	"$(INTDIR)\rdoreslt.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\MDIBind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\MDIBind.exe"

CLEAN : 
	-@erase "$(INTDIR)\DSNDlg.obj"
	-@erase "$(INTDIR)\gridfrm.obj"
	-@erase "$(INTDIR)\gridview.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\maskfrm.obj"
	-@erase "$(INTDIR)\maskview.obj"
	-@erase "$(INTDIR)\mcsdsc.obj"
	-@erase "$(INTDIR)\MDIBind.obj"
	-@erase "$(INTDIR)\MDIBind.pch"
	-@erase "$(INTDIR)\MDIBind.res"
	-@erase "$(INTDIR)\MDIDoc.obj"
	-@erase "$(INTDIR)\msdgrid.obj"
	-@erase "$(INTDIR)\msmask.obj"
	-@erase "$(INTDIR)\RDCDlg.obj"
	-@erase "$(INTDIR)\RDCFrm.obj"
	-@erase "$(INTDIR)\RDCView.obj"
	-@erase "$(INTDIR)\rdocol.obj"
	-@erase "$(INTDIR)\rdocols.obj"
	-@erase "$(INTDIR)\rdoreslt.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\MDIBind.exe"
	-@erase "$(OUTDIR)\MDIBind.ilk"
	-@erase "$(OUTDIR)\MDIBind.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=fl32.exe
# ADD BASE F90 /I "Debug/"
# ADD F90 /I "Debug/"
F90_OBJS=.\Debug/
F90_PROJ=/I "Release/" /Fo"Release/" 

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MDIBind.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MDIBind.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MDIBind.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/MDIBind.pdb" /debug /machine:I386 /out:"$(OUTDIR)/MDIBind.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DSNDlg.obj" \
	"$(INTDIR)\gridfrm.obj" \
	"$(INTDIR)\gridview.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\maskfrm.obj" \
	"$(INTDIR)\maskview.obj" \
	"$(INTDIR)\mcsdsc.obj" \
	"$(INTDIR)\MDIBind.obj" \
	"$(INTDIR)\MDIBind.res" \
	"$(INTDIR)\MDIDoc.obj" \
	"$(INTDIR)\msdgrid.obj" \
	"$(INTDIR)\msmask.obj" \
	"$(INTDIR)\RDCDlg.obj" \
	"$(INTDIR)\RDCFrm.obj" \
	"$(INTDIR)\RDCView.obj" \
	"$(INTDIR)\rdocol.obj" \
	"$(INTDIR)\rdocols.obj" \
	"$(INTDIR)\rdoreslt.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\MDIBind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MDIBind_"
# PROP BASE Intermediate_Dir "MDIBind_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UniDebug"
# PROP Intermediate_Dir "UniDebug"
# PROP Target_Dir ""
OUTDIR=.\UniDebug
INTDIR=.\UniDebug

ALL : "$(OUTDIR)\MDIBind.exe"

CLEAN : 
	-@erase "$(INTDIR)\DSNDlg.obj"
	-@erase "$(INTDIR)\gridfrm.obj"
	-@erase "$(INTDIR)\gridview.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\maskfrm.obj"
	-@erase "$(INTDIR)\maskview.obj"
	-@erase "$(INTDIR)\mcsdsc.obj"
	-@erase "$(INTDIR)\MDIBind.obj"
	-@erase "$(INTDIR)\MDIBind.pch"
	-@erase "$(INTDIR)\MDIBind.res"
	-@erase "$(INTDIR)\MDIDoc.obj"
	-@erase "$(INTDIR)\msdgrid.obj"
	-@erase "$(INTDIR)\msmask.obj"
	-@erase "$(INTDIR)\RDCDlg.obj"
	-@erase "$(INTDIR)\RDCFrm.obj"
	-@erase "$(INTDIR)\RDCView.obj"
	-@erase "$(INTDIR)\rdocol.obj"
	-@erase "$(INTDIR)\rdocols.obj"
	-@erase "$(INTDIR)\rdoreslt.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\MDIBind.exe"
	-@erase "$(OUTDIR)\MDIBind.ilk"
	-@erase "$(OUTDIR)\MDIBind.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=fl32.exe
# ADD BASE F90 /I "MDIBind_/"
# ADD F90 /I "UniDebug/"
F90_OBJS=.\UniDebug/
F90_PROJ=/I "Release/" /Fo"Release/" 

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MDIBind.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\UniDebug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MDIBind.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MDIBind.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/MDIBind.pdb" /debug /machine:I386 /out:"$(OUTDIR)/MDIBind.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DSNDlg.obj" \
	"$(INTDIR)\gridfrm.obj" \
	"$(INTDIR)\gridview.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\maskfrm.obj" \
	"$(INTDIR)\maskview.obj" \
	"$(INTDIR)\mcsdsc.obj" \
	"$(INTDIR)\MDIBind.obj" \
	"$(INTDIR)\MDIBind.res" \
	"$(INTDIR)\MDIDoc.obj" \
	"$(INTDIR)\msdgrid.obj" \
	"$(INTDIR)\msmask.obj" \
	"$(INTDIR)\RDCDlg.obj" \
	"$(INTDIR)\RDCFrm.obj" \
	"$(INTDIR)\RDCView.obj" \
	"$(INTDIR)\rdocol.obj" \
	"$(INTDIR)\rdocols.obj" \
	"$(INTDIR)\rdoreslt.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\MDIBind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MDIBind0"
# PROP BASE Intermediate_Dir "MDIBind0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UniRel"
# PROP Intermediate_Dir "UniRel"
# PROP Target_Dir ""
OUTDIR=.\UniRel
INTDIR=.\UniRel

ALL : "$(OUTDIR)\MDIBind.exe"

CLEAN : 
	-@erase "$(INTDIR)\DSNDlg.obj"
	-@erase "$(INTDIR)\gridfrm.obj"
	-@erase "$(INTDIR)\gridview.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\maskfrm.obj"
	-@erase "$(INTDIR)\maskview.obj"
	-@erase "$(INTDIR)\mcsdsc.obj"
	-@erase "$(INTDIR)\MDIBind.obj"
	-@erase "$(INTDIR)\MDIBind.pch"
	-@erase "$(INTDIR)\MDIBind.res"
	-@erase "$(INTDIR)\MDIDoc.obj"
	-@erase "$(INTDIR)\msdgrid.obj"
	-@erase "$(INTDIR)\msmask.obj"
	-@erase "$(INTDIR)\RDCDlg.obj"
	-@erase "$(INTDIR)\RDCFrm.obj"
	-@erase "$(INTDIR)\RDCView.obj"
	-@erase "$(INTDIR)\rdocol.obj"
	-@erase "$(INTDIR)\rdocols.obj"
	-@erase "$(INTDIR)\rdoreslt.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\MDIBind.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=fl32.exe
# ADD BASE F90 /I "MDIBind0/"
# ADD F90 /I "UniRel/"
F90_OBJS=.\UniRel/
F90_PROJ=/I "Release/" /Fo"Release/" 

.for{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

.f90{$(F90_OBJS)}.obj:
   $(F90) $(F90_PROJ) $<  

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MDIBind.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\UniRel/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MDIBind.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MDIBind.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/MDIBind.pdb" /machine:I386 /out:"$(OUTDIR)/MDIBind.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DSNDlg.obj" \
	"$(INTDIR)\gridfrm.obj" \
	"$(INTDIR)\gridview.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\maskfrm.obj" \
	"$(INTDIR)\maskview.obj" \
	"$(INTDIR)\mcsdsc.obj" \
	"$(INTDIR)\MDIBind.obj" \
	"$(INTDIR)\MDIBind.res" \
	"$(INTDIR)\MDIDoc.obj" \
	"$(INTDIR)\msdgrid.obj" \
	"$(INTDIR)\msmask.obj" \
	"$(INTDIR)\RDCDlg.obj" \
	"$(INTDIR)\RDCFrm.obj" \
	"$(INTDIR)\RDCView.obj" \
	"$(INTDIR)\rdocol.obj" \
	"$(INTDIR)\rdocols.obj" \
	"$(INTDIR)\rdoreslt.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\MDIBind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

################################################################################
# Begin Target

# Name "MDIBind - Win32 Release"
# Name "MDIBind - Win32 Debug"
# Name "MDIBind - Win32 Unicode Debug"
# Name "MDIBind - Win32 Unicode Release"

!IF  "$(CFG)" == "MDIBind - Win32 Release"

!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"

!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\MDIBind.cpp
DEP_CPP_MDIBI=\
	".\gridfrm.h"\
	".\gridview.h"\
	".\MainFrm.h"\
	".\maskfrm.h"\
	".\maskview.h"\
	".\mcsdsc.h"\
	".\MDIBind.h"\
	".\MDIDoc.h"\
	".\msdgrid.h"\
	".\msmask.h"\
	".\RDCFrm.h"\
	".\RDCView.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\MDIBind.obj" : $(SOURCE) $(DEP_CPP_MDIBI) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\MDIBind.obj" : $(SOURCE) $(DEP_CPP_MDIBI) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\MDIBind.obj" : $(SOURCE) $(DEP_CPP_MDIBI) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\MDIBind.obj" : $(SOURCE) $(DEP_CPP_MDIBI) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MDIBind.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MDIBind.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MDIBind.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MDIBind.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MDIBind.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MDIBind.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MDIBind.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MDIBind.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\DSNDlg.h"\
	".\gridview.h"\
	".\MainFrm.h"\
	".\maskview.h"\
	".\mcsdsc.h"\
	".\MDIBind.h"\
	".\MDIDoc.h"\
	".\msdgrid.h"\
	".\msmask.h"\
	".\RDCDlg.h"\
	".\RDCView.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MDIDoc.cpp
DEP_CPP_MDIDO=\
	".\mcsdsc.h"\
	".\MDIBind.h"\
	".\MDIDoc.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\MDIDoc.obj" : $(SOURCE) $(DEP_CPP_MDIDO) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\MDIDoc.obj" : $(SOURCE) $(DEP_CPP_MDIDO) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\MDIDoc.obj" : $(SOURCE) $(DEP_CPP_MDIDO) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\MDIDoc.obj" : $(SOURCE) $(DEP_CPP_MDIDO) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\RDCView.cpp
DEP_CPP_RDCVI=\
	".\gridfrm.h"\
	".\MainFrm.h"\
	".\maskfrm.h"\
	".\mcsdsc.h"\
	".\MDIBind.h"\
	".\MDIDoc.h"\
	".\RDCView.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\RDCView.obj" : $(SOURCE) $(DEP_CPP_RDCVI) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\RDCView.obj" : $(SOURCE) $(DEP_CPP_RDCVI) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\RDCView.obj" : $(SOURCE) $(DEP_CPP_RDCVI) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\RDCView.obj" : $(SOURCE) $(DEP_CPP_RDCVI) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MDIBind.rc
DEP_RSC_MDIBIN=\
	".\res\grid.ico"\
	".\res\mask.ico"\
	".\res\MDIBind.ico"\
	".\res\MDIBind.rc2"\
	".\res\MDIDoc.ico"\
	".\res\Toolbar.bmp"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\MDIBind.res" : $(SOURCE) $(DEP_RSC_MDIBIN) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\MDIBind.res" : $(SOURCE) $(DEP_RSC_MDIBIN) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\MDIBind.res" : $(SOURCE) $(DEP_RSC_MDIBIN) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\MDIBind.res" : $(SOURCE) $(DEP_RSC_MDIBIN) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\maskview.cpp
DEP_CPP_MASKV=\
	".\maskview.h"\
	".\mcsdsc.h"\
	".\MDIBind.h"\
	".\MDIDoc.h"\
	".\msmask.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\maskview.obj" : $(SOURCE) $(DEP_CPP_MASKV) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\maskview.obj" : $(SOURCE) $(DEP_CPP_MASKV) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\maskview.obj" : $(SOURCE) $(DEP_CPP_MASKV) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\maskview.obj" : $(SOURCE) $(DEP_CPP_MASKV) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\maskfrm.cpp
DEP_CPP_MASKF=\
	".\maskfrm.h"\
	".\MDIBind.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\maskfrm.obj" : $(SOURCE) $(DEP_CPP_MASKF) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\maskfrm.obj" : $(SOURCE) $(DEP_CPP_MASKF) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\maskfrm.obj" : $(SOURCE) $(DEP_CPP_MASKF) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\maskfrm.obj" : $(SOURCE) $(DEP_CPP_MASKF) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\gridview.cpp
DEP_CPP_GRIDV=\
	".\gridview.h"\
	".\mcsdsc.h"\
	".\MDIBind.h"\
	".\MDIDoc.h"\
	".\msdgrid.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\gridview.obj" : $(SOURCE) $(DEP_CPP_GRIDV) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\gridview.obj" : $(SOURCE) $(DEP_CPP_GRIDV) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\gridview.obj" : $(SOURCE) $(DEP_CPP_GRIDV) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\gridview.obj" : $(SOURCE) $(DEP_CPP_GRIDV) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\gridfrm.cpp
DEP_CPP_GRIDF=\
	".\gridfrm.h"\
	".\MDIBind.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\gridfrm.obj" : $(SOURCE) $(DEP_CPP_GRIDF) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\gridfrm.obj" : $(SOURCE) $(DEP_CPP_GRIDF) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\gridfrm.obj" : $(SOURCE) $(DEP_CPP_GRIDF) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\gridfrm.obj" : $(SOURCE) $(DEP_CPP_GRIDF) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\RDCFrm.cpp
DEP_CPP_RDCFR=\
	".\MDIBind.h"\
	".\RDCFrm.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\RDCFrm.obj" : $(SOURCE) $(DEP_CPP_RDCFR) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\RDCFrm.obj" : $(SOURCE) $(DEP_CPP_RDCFR) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\RDCFrm.obj" : $(SOURCE) $(DEP_CPP_RDCFR) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\RDCFrm.obj" : $(SOURCE) $(DEP_CPP_RDCFR) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\msdgrid.cpp
DEP_CPP_MSDGR=\
	".\msdgrid.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\msdgrid.obj" : $(SOURCE) $(DEP_CPP_MSDGR) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\msdgrid.obj" : $(SOURCE) $(DEP_CPP_MSDGR) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\msdgrid.obj" : $(SOURCE) $(DEP_CPP_MSDGR) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\msdgrid.obj" : $(SOURCE) $(DEP_CPP_MSDGR) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\msmask.cpp
DEP_CPP_MSMAS=\
	".\msmask.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\msmask.obj" : $(SOURCE) $(DEP_CPP_MSMAS) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\msmask.obj" : $(SOURCE) $(DEP_CPP_MSMAS) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\msmask.obj" : $(SOURCE) $(DEP_CPP_MSMAS) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\msmask.obj" : $(SOURCE) $(DEP_CPP_MSMAS) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mcsdsc.cpp
DEP_CPP_MCSDS=\
	".\mcsdsc.h"\
	".\rdoreslt.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\mcsdsc.obj" : $(SOURCE) $(DEP_CPP_MCSDS) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\mcsdsc.obj" : $(SOURCE) $(DEP_CPP_MCSDS) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\mcsdsc.obj" : $(SOURCE) $(DEP_CPP_MCSDS) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\mcsdsc.obj" : $(SOURCE) $(DEP_CPP_MCSDS) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\RDCDlg.cpp
DEP_CPP_RDCDL=\
	".\MainFrm.h"\
	".\mcsdsc.h"\
	".\MDIBind.h"\
	".\MDIDoc.h"\
	".\RDCDlg.h"\
	".\RDCFrm.h"\
	".\RDCView.h"\
	".\rdocol.h"\
	".\rdocols.h"\
	".\rdoreslt.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\RDCDlg.obj" : $(SOURCE) $(DEP_CPP_RDCDL) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\RDCDlg.obj" : $(SOURCE) $(DEP_CPP_RDCDL) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\RDCDlg.obj" : $(SOURCE) $(DEP_CPP_RDCDL) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\RDCDlg.obj" : $(SOURCE) $(DEP_CPP_RDCDL) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DSNDlg.cpp
DEP_CPP_DSNDL=\
	".\DSNDlg.h"\
	".\MDIBind.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\DSNDlg.obj" : $(SOURCE) $(DEP_CPP_DSNDL) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\DSNDlg.obj" : $(SOURCE) $(DEP_CPP_DSNDL) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\DSNDlg.obj" : $(SOURCE) $(DEP_CPP_DSNDL) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\DSNDlg.obj" : $(SOURCE) $(DEP_CPP_DSNDL) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rdocol.cpp
DEP_CPP_RDOCO=\
	".\rdocol.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\rdocol.obj" : $(SOURCE) $(DEP_CPP_RDOCO) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\rdocol.obj" : $(SOURCE) $(DEP_CPP_RDOCO) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\rdocol.obj" : $(SOURCE) $(DEP_CPP_RDOCO) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\rdocol.obj" : $(SOURCE) $(DEP_CPP_RDOCO) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rdocols.cpp
DEP_CPP_RDOCOL=\
	".\rdocol.h"\
	".\rdocols.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\rdocols.obj" : $(SOURCE) $(DEP_CPP_RDOCOL) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\rdocols.obj" : $(SOURCE) $(DEP_CPP_RDOCOL) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\rdocols.obj" : $(SOURCE) $(DEP_CPP_RDOCOL) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\rdocols.obj" : $(SOURCE) $(DEP_CPP_RDOCOL) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rdoreslt.cpp
DEP_CPP_RDORE=\
	".\rdocols.h"\
	".\rdoreslt.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDIBind - Win32 Release"


"$(INTDIR)\rdoreslt.obj" : $(SOURCE) $(DEP_CPP_RDORE) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Debug"


"$(INTDIR)\rdoreslt.obj" : $(SOURCE) $(DEP_CPP_RDORE) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Debug"


"$(INTDIR)\rdoreslt.obj" : $(SOURCE) $(DEP_CPP_RDORE) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ELSEIF  "$(CFG)" == "MDIBind - Win32 Unicode Release"


"$(INTDIR)\rdoreslt.obj" : $(SOURCE) $(DEP_CPP_RDORE) "$(INTDIR)"\
 "$(INTDIR)\MDIBind.pch"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
################################################################################
# Section MDIBind : {00028C00-0000-0000-0000-000000000046}
# 	0:9:Picture.h:C:\mfc42\nonship\MDIBind\Picture.h
# 	0:8:Font.cpp:C:\mfc42\nonship\MDIBind\Font.cpp
# 	0:6:Font.h:C:\mfc42\nonship\MDIBind\Font.h
# 	0:11:Picture.cpp:C:\mfc42\nonship\MDIBind\Picture.cpp
# 	0:13:MsDgridCtrl.h:C:\mfc42\nonship\MDIBind\MsDgridCtrl.h
# 	0:15:MsDgridCtrl.cpp:C:\mfc42\nonship\MDIBind\MsDgridCtrl.cpp
# 	2:21:DefaultSinkHeaderFile:msdgrid.h
# 	2:16:DefaultSinkClass:CMsDgridCtrl
# End Section
################################################################################
################################################################################
# Section MDIBind : {4F5F8265-8AAF-11CE-A77F-08002B2F4E98}
# 	2:5:Class:CMcsDsc
# 	2:10:HeaderFile:mcsdsc.h
# 	2:8:ImplFile:mcsdsc.cpp
# End Section
################################################################################
################################################################################
# Section MDIBind : {C932BA85-4374-101B-A56C-00AA003668DC}
# 	0:9:Picture.h:C:\mfc42\nonship\MDIBind\Picture.h
# 	0:8:Font.cpp:C:\mfc42\nonship\MDIBind\Font.cpp
# 	0:6:Font.h:C:\mfc42\nonship\MDIBind\Font.h
# 	0:11:Picture.cpp:C:\mfc42\nonship\MDIBind\Picture.cpp
# 	0:8:Msmask.h:C:\mfc42\nonship\MDIBind\Msmask.h
# 	0:10:Msmask.cpp:C:\mfc42\nonship\MDIBind\Msmask.cpp
# 	2:21:DefaultSinkHeaderFile:msmask.h
# 	2:16:DefaultSinkClass:CMsmask
# End Section
################################################################################
################################################################################
# Section OLE Controls
# 	{00028C00-0000-0000-0000-000000000046}
# 	{C932BA85-4374-101B-A56C-00AA003668DC}
# 	{4F5F826A-8AAF-11CE-A77F-08002B2F4E98}
# End Section
################################################################################
################################################################################
# Section MDIBind : {4F5F826A-8AAF-11CE-A77F-08002B2F4E98}
# 	0:12:rdoColumns.h:C:\mfc42\nonship\MDIBind\rdoColumns.h
# 	0:25:rdoPreparedStatements.cpp:C:\mfc42\nonship\MDIBind\rdoPreparedStatements.cpp
# 	0:8:Font.cpp:C:\mfc42\nonship\MDIBind\Font.cpp
# 	0:13:rdoColumn.cpp:C:\mfc42\nonship\MDIBind\rdoColumn.cpp
# 	0:14:rdoResultset.h:C:\mfc42\nonship\MDIBind\rdoResultset.h
# 	0:6:Font.h:C:\mfc42\nonship\MDIBind\Font.h
# 	0:11:rdoColumn.h:C:\mfc42\nonship\MDIBind\rdoColumn.h
# 	0:15:rdoParameters.h:C:\mfc42\nonship\MDIBind\rdoParameters.h
# 	0:8:McsDsc.h:C:\mfc42\nonship\MDIBind\McsDsc.h
# 	0:10:rdoTable.h:C:\mfc42\nonship\MDIBind\rdoTable.h
# 	0:17:rdoConnection.cpp:C:\mfc42\nonship\MDIBind\rdoConnection.cpp
# 	0:23:rdoPreparedStatements.h:C:\mfc42\nonship\MDIBind\rdoPreparedStatements.h
# 	0:15:rdoConnection.h:C:\mfc42\nonship\MDIBind\rdoConnection.h
# 	0:18:rdoEnvironment.cpp:C:\mfc42\nonship\MDIBind\rdoEnvironment.cpp
# 	0:17:rdoParameters.cpp:C:\mfc42\nonship\MDIBind\rdoParameters.cpp
# 	0:10:McsDsc.cpp:C:\mfc42\nonship\MDIBind\McsDsc.cpp
# 	0:14:rdoColumns.cpp:C:\mfc42\nonship\MDIBind\rdoColumns.cpp
# 	0:14:rdoParameter.h:C:\mfc42\nonship\MDIBind\rdoParameter.h
# 	0:11:rdoTables.h:C:\mfc42\nonship\MDIBind\rdoTables.h
# 	0:12:rdoTable.cpp:C:\mfc42\nonship\MDIBind\rdoTable.cpp
# 	0:18:rdoConnections.cpp:C:\mfc42\nonship\MDIBind\rdoConnections.cpp
# 	0:13:rdoTables.cpp:C:\mfc42\nonship\MDIBind\rdoTables.cpp
# 	0:16:rdoEnvironment.h:C:\mfc42\nonship\MDIBind\rdoEnvironment.h
# 	0:16:rdoConnections.h:C:\mfc42\nonship\MDIBind\rdoConnections.h
# 	0:24:rdoPreparedStatement.cpp:C:\mfc42\nonship\MDIBind\rdoPreparedStatement.cpp
# 	0:16:rdoResultset.cpp:C:\mfc42\nonship\MDIBind\rdoResultset.cpp
# 	0:17:rdoResultsets.cpp:C:\mfc42\nonship\MDIBind\rdoResultsets.cpp
# 	0:22:rdoPreparedStatement.h:C:\mfc42\nonship\MDIBind\rdoPreparedStatement.h
# 	0:16:rdoParameter.cpp:C:\mfc42\nonship\MDIBind\rdoParameter.cpp
# 	0:15:rdoResultsets.h:C:\mfc42\nonship\MDIBind\rdoResultsets.h
# 	2:21:DefaultSinkHeaderFile:mcsdsc.h
# 	2:16:DefaultSinkClass:CMcsDsc
# End Section
################################################################################
################################################################################
# Section MDIBind : {00028C02-0000-0000-0000-000000000046}
# 	2:5:Class:CMsDgridCtrl
# 	2:10:HeaderFile:msdgrid.h
# 	2:8:ImplFile:msdgrid.cpp
# End Section
################################################################################
################################################################################
# Section MDIBind : {B541C03A-63BC-11CE-920C-08002B369A33}
# 	2:5:Class:CrdoColumn
# 	2:10:HeaderFile:rdocol.h
# 	2:8:ImplFile:rdocol.cpp
# End Section
################################################################################
################################################################################
# Section MDIBind : {2E746492-6ED1-11CE-9223-08002B369A33}
# 	2:5:Class:CrdoColumns
# 	2:10:HeaderFile:rdocols.h
# 	2:8:ImplFile:rdocols.cpp
# End Section
################################################################################
################################################################################
# Section MDIBind : {B541C03E-63BC-11CE-920C-08002B369A33}
# 	2:5:Class:CrdoResultset
# 	2:10:HeaderFile:rdoreslt.h
# 	2:8:ImplFile:rdoreslt.cpp
# End Section
################################################################################
################################################################################
# Section MDIBind : {C932BA86-4374-101B-A56C-00AA003668DC}
# 	2:5:Class:CMsmask
# 	2:10:HeaderFile:msmask.h
# 	2:8:ImplFile:msmask.cpp
# End Section
################################################################################
