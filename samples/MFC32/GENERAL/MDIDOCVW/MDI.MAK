# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=MDI - Win32 UDebug
!MESSAGE No configuration specified.  Defaulting to MDI - Win32 UDebug.
!ENDIF 

!IF "$(CFG)" != "MDI - Win32 Release" && "$(CFG)" != "MDI - Win32 Debug" &&\
 "$(CFG)" != "MDI - Win32 UDebug" && "$(CFG)" != "MDI - Win32 URelease"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "mdi.mak" CFG="MDI - Win32 UDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MDI - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MDI - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "MDI - Win32 UDebug" (based on "Win32 (x86) Application")
!MESSAGE "MDI - Win32 URelease" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "MDI - Win32 Debug"
MTL=mktyplib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MDI - Win32 Release"

# PROP BASE Use_MFC 6
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

ALL : "$(OUTDIR)\mdi.exe"

CLEAN : 
	-@erase "$(INTDIR)\BncDoc.obj"
	-@erase "$(INTDIR)\BncFrm.obj"
	-@erase "$(INTDIR)\BncVw.obj"
	-@erase "$(INTDIR)\HelloDoc.obj"
	-@erase "$(INTDIR)\HelloFrm.obj"
	-@erase "$(INTDIR)\HelloVw.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MDI.obj"
	-@erase "$(INTDIR)\mdi.pch"
	-@erase "$(INTDIR)\MDI.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\mdi.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/mdi.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MDI.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/mdi.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/mdi.pdb" /machine:I386 /out:"$(OUTDIR)/mdi.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BncDoc.obj" \
	"$(INTDIR)\BncFrm.obj" \
	"$(INTDIR)\BncVw.obj" \
	"$(INTDIR)\HelloDoc.obj" \
	"$(INTDIR)\HelloFrm.obj" \
	"$(INTDIR)\HelloVw.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MDI.obj" \
	"$(INTDIR)\MDI.res" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\mdi.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"

# PROP BASE Use_MFC 6
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

ALL : "$(OUTDIR)\mdi.exe"

CLEAN : 
	-@erase "$(INTDIR)\BncDoc.obj"
	-@erase "$(INTDIR)\BncFrm.obj"
	-@erase "$(INTDIR)\BncVw.obj"
	-@erase "$(INTDIR)\HelloDoc.obj"
	-@erase "$(INTDIR)\HelloFrm.obj"
	-@erase "$(INTDIR)\HelloVw.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MDI.obj"
	-@erase "$(INTDIR)\mdi.pch"
	-@erase "$(INTDIR)\MDI.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\mdi.exe"
	-@erase "$(OUTDIR)\mdi.ilk"
	-@erase "$(OUTDIR)\mdi.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/mdi.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MDI.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/mdi.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/mdi.pdb" /debug /machine:I386 /out:"$(OUTDIR)/mdi.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BncDoc.obj" \
	"$(INTDIR)\BncFrm.obj" \
	"$(INTDIR)\BncVw.obj" \
	"$(INTDIR)\HelloDoc.obj" \
	"$(INTDIR)\HelloFrm.obj" \
	"$(INTDIR)\HelloVw.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MDI.obj" \
	"$(INTDIR)\MDI.res" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\mdi.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MDI___Wi"
# PROP BASE Intermediate_Dir "MDI___Wi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UDebug"
# PROP Intermediate_Dir "UDebug"
# PROP Target_Dir ""
OUTDIR=.\UDebug
INTDIR=.\UDebug

ALL : "$(OUTDIR)\mdi.exe"

CLEAN : 
	-@erase "$(INTDIR)\BncDoc.obj"
	-@erase "$(INTDIR)\BncFrm.obj"
	-@erase "$(INTDIR)\BncVw.obj"
	-@erase "$(INTDIR)\HelloDoc.obj"
	-@erase "$(INTDIR)\HelloFrm.obj"
	-@erase "$(INTDIR)\HelloVw.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MDI.obj"
	-@erase "$(INTDIR)\mdi.pch"
	-@erase "$(INTDIR)\MDI.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\mdi.exe"
	-@erase "$(OUTDIR)\mdi.ilk"
	-@erase "$(OUTDIR)\mdi.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_UNICODE" /Fp"$(INTDIR)/mdi.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\UDebug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MDI.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/mdi.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /entry:"wWinMainCRTStartup" /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/mdi.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/mdi.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BncDoc.obj" \
	"$(INTDIR)\BncFrm.obj" \
	"$(INTDIR)\BncVw.obj" \
	"$(INTDIR)\HelloDoc.obj" \
	"$(INTDIR)\HelloFrm.obj" \
	"$(INTDIR)\HelloVw.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MDI.obj" \
	"$(INTDIR)\MDI.res" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\mdi.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MDI___W0"
# PROP BASE Intermediate_Dir "MDI___W0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "URelease"
# PROP Intermediate_Dir "URelease"
# PROP Target_Dir ""
OUTDIR=.\URelease
INTDIR=.\URelease

ALL : "$(OUTDIR)\mdi.exe"

CLEAN : 
	-@erase "$(INTDIR)\BncDoc.obj"
	-@erase "$(INTDIR)\BncFrm.obj"
	-@erase "$(INTDIR)\BncVw.obj"
	-@erase "$(INTDIR)\HelloDoc.obj"
	-@erase "$(INTDIR)\HelloFrm.obj"
	-@erase "$(INTDIR)\HelloVw.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MDI.obj"
	-@erase "$(INTDIR)\mdi.pch"
	-@erase "$(INTDIR)\MDI.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\mdi.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_UNICODE" /Fp"$(INTDIR)/mdi.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c\
 
CPP_OBJS=.\URelease/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MDI.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/mdi.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /entry:"wWinMainCRTStartup" /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/mdi.pdb" /machine:I386 /out:"$(OUTDIR)/mdi.exe"\
 
LINK32_OBJS= \
	"$(INTDIR)\BncDoc.obj" \
	"$(INTDIR)\BncFrm.obj" \
	"$(INTDIR)\BncVw.obj" \
	"$(INTDIR)\HelloDoc.obj" \
	"$(INTDIR)\HelloFrm.obj" \
	"$(INTDIR)\HelloVw.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MDI.obj" \
	"$(INTDIR)\MDI.res" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\mdi.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "MDI - Win32 Release"
# Name "MDI - Win32 Debug"
# Name "MDI - Win32 UDebug"
# Name "MDI - Win32 URelease"

!IF  "$(CFG)" == "MDI - Win32 Release"

!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"

!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"

!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "MDI - Win32 Release"

!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"

!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"

!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MDI.cpp
DEP_CPP_MDI_C=\
	".\BncDoc.h"\
	".\BncFrm.h"\
	".\BncVw.h"\
	".\HelloDoc.h"\
	".\HelloFrm.h"\
	".\HelloVw.h"\
	".\MainFrm.h"\
	".\MDI.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDI - Win32 Release"


"$(INTDIR)\MDI.obj" : $(SOURCE) $(DEP_CPP_MDI_C) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"


"$(INTDIR)\MDI.obj" : $(SOURCE) $(DEP_CPP_MDI_C) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"


"$(INTDIR)\MDI.obj" : $(SOURCE) $(DEP_CPP_MDI_C) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"


"$(INTDIR)\MDI.obj" : $(SOURCE) $(DEP_CPP_MDI_C) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDI - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/mdi.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\mdi.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/mdi.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\mdi.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_UNICODE" /Fp"$(INTDIR)/mdi.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\mdi.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_UNICODE" /Fp"$(INTDIR)/mdi.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\mdi.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\MainFrm.h"\
	".\MDI.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDI - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HelloFrm.cpp
DEP_CPP_HELLO=\
	".\HelloFrm.h"\
	".\MDI.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDI - Win32 Release"


"$(INTDIR)\HelloFrm.obj" : $(SOURCE) $(DEP_CPP_HELLO) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"


"$(INTDIR)\HelloFrm.obj" : $(SOURCE) $(DEP_CPP_HELLO) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"


"$(INTDIR)\HelloFrm.obj" : $(SOURCE) $(DEP_CPP_HELLO) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"


"$(INTDIR)\HelloFrm.obj" : $(SOURCE) $(DEP_CPP_HELLO) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HelloDoc.cpp
DEP_CPP_HELLOD=\
	".\HelloDoc.h"\
	".\MDI.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDI - Win32 Release"


"$(INTDIR)\HelloDoc.obj" : $(SOURCE) $(DEP_CPP_HELLOD) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"


"$(INTDIR)\HelloDoc.obj" : $(SOURCE) $(DEP_CPP_HELLOD) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"


"$(INTDIR)\HelloDoc.obj" : $(SOURCE) $(DEP_CPP_HELLOD) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"


"$(INTDIR)\HelloDoc.obj" : $(SOURCE) $(DEP_CPP_HELLOD) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MDI.rc
DEP_RSC_MDI_R=\
	".\mdi.ico"\
	".\res\HelloDoc.ico"\
	".\res\idr_boun.ico"\
	".\res\MDI.rc2"\
	".\res\Toolbar.bmp"\
	

!IF  "$(CFG)" == "MDI - Win32 Release"


"$(INTDIR)\MDI.res" : $(SOURCE) $(DEP_RSC_MDI_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"


"$(INTDIR)\MDI.res" : $(SOURCE) $(DEP_RSC_MDI_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"


"$(INTDIR)\MDI.res" : $(SOURCE) $(DEP_RSC_MDI_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"


"$(INTDIR)\MDI.res" : $(SOURCE) $(DEP_RSC_MDI_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HelloVw.cpp
DEP_CPP_HELLOV=\
	".\HelloDoc.h"\
	".\HelloVw.h"\
	".\MDI.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDI - Win32 Release"


"$(INTDIR)\HelloVw.obj" : $(SOURCE) $(DEP_CPP_HELLOV) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"


"$(INTDIR)\HelloVw.obj" : $(SOURCE) $(DEP_CPP_HELLOV) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"


"$(INTDIR)\HelloVw.obj" : $(SOURCE) $(DEP_CPP_HELLOV) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"


"$(INTDIR)\HelloVw.obj" : $(SOURCE) $(DEP_CPP_HELLOV) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BncVw.cpp
DEP_CPP_BNCVW=\
	".\BncDoc.h"\
	".\BncVw.h"\
	".\MDI.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDI - Win32 Release"


"$(INTDIR)\BncVw.obj" : $(SOURCE) $(DEP_CPP_BNCVW) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"


"$(INTDIR)\BncVw.obj" : $(SOURCE) $(DEP_CPP_BNCVW) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"


"$(INTDIR)\BncVw.obj" : $(SOURCE) $(DEP_CPP_BNCVW) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"


"$(INTDIR)\BncVw.obj" : $(SOURCE) $(DEP_CPP_BNCVW) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BncFrm.cpp
DEP_CPP_BNCFR=\
	".\BncFrm.h"\
	".\MDI.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDI - Win32 Release"


"$(INTDIR)\BncFrm.obj" : $(SOURCE) $(DEP_CPP_BNCFR) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"


"$(INTDIR)\BncFrm.obj" : $(SOURCE) $(DEP_CPP_BNCFR) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"


"$(INTDIR)\BncFrm.obj" : $(SOURCE) $(DEP_CPP_BNCFR) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"


"$(INTDIR)\BncFrm.obj" : $(SOURCE) $(DEP_CPP_BNCFR) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BncDoc.cpp
DEP_CPP_BNCDO=\
	".\BncDoc.h"\
	".\MDI.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MDI - Win32 Release"


"$(INTDIR)\BncDoc.obj" : $(SOURCE) $(DEP_CPP_BNCDO) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"


"$(INTDIR)\BncDoc.obj" : $(SOURCE) $(DEP_CPP_BNCDO) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 UDebug"


"$(INTDIR)\BncDoc.obj" : $(SOURCE) $(DEP_CPP_BNCDO) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ELSEIF  "$(CFG)" == "MDI - Win32 URelease"


"$(INTDIR)\BncDoc.obj" : $(SOURCE) $(DEP_CPP_BNCDO) "$(INTDIR)"\
 "$(INTDIR)\mdi.pch"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
