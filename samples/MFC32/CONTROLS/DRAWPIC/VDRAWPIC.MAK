# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=DrawPic - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to DrawPic - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "DrawPic - Win32 Release" && "$(CFG)" !=\
 "DrawPic - Win32 Debug" && "$(CFG)" != "DrawPic - Win32 Unicode Debug" &&\
 "$(CFG)" != "DrawPic - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Drawpic.mak" CFG="DrawPic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DrawPic - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DrawPic - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DrawPic - Win32 Unicode Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DrawPic - Win32 Unicode Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "DrawPic - Win32 Debug"
MTL=mktyplib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DrawPic - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP BASE Target_Ext "ocx"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# PROP Target_Ext "ocx"
OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Drawpic.ocx" "$(OUTDIR)\regsvr32.trg"

CLEAN : 
	-@erase "$(INTDIR)\DrawPic.obj"
	-@erase "$(INTDIR)\Drawpic.pch"
	-@erase "$(INTDIR)\DrawPic.res"
	-@erase "$(INTDIR)\DrawPic.tlb"
	-@erase "$(INTDIR)\DrawPicC.obj"
	-@erase "$(INTDIR)\DrawPicP.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\Drawpic.exp"
	-@erase "$(OUTDIR)\Drawpic.lib"
	-@erase "$(OUTDIR)\Drawpic.ocx"
	-@erase "$(OUTDIR)\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)/Drawpic.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/DrawPic.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Drawpic.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/Drawpic.pdb" /machine:I386 /def:".\DrawPic.def"\
 /out:"$(OUTDIR)/Drawpic.ocx" /implib:"$(OUTDIR)/Drawpic.lib" 
DEF_FILE= \
	".\DrawPic.def"
LINK32_OBJS= \
	"$(INTDIR)\DrawPic.obj" \
	"$(INTDIR)\DrawPic.res" \
	"$(INTDIR)\DrawPicC.obj" \
	"$(INTDIR)\DrawPicP.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\Drawpic.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Registering OLE control...
OutDir=.\Release
TargetPath=.\Release\Drawpic.ocx
InputPath=.\Release\Drawpic.ocx
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   regsvr32 /s /c "$(TargetPath)"
   echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg"

# End Custom Build

!ELSEIF  "$(CFG)" == "DrawPic - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP BASE Target_Ext "ocx"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# PROP Target_Ext "ocx"
OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Drawpic.ocx" "$(OUTDIR)\regsvr32.trg"

CLEAN : 
	-@erase "$(INTDIR)\DrawPic.obj"
	-@erase "$(INTDIR)\Drawpic.pch"
	-@erase "$(INTDIR)\DrawPic.res"
	-@erase "$(INTDIR)\DrawPic.tlb"
	-@erase "$(INTDIR)\DrawPicC.obj"
	-@erase "$(INTDIR)\DrawPicP.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Drawpic.exp"
	-@erase "$(OUTDIR)\Drawpic.ilk"
	-@erase "$(OUTDIR)\Drawpic.lib"
	-@erase "$(OUTDIR)\Drawpic.ocx"
	-@erase "$(OUTDIR)\Drawpic.pdb"
	-@erase "$(OUTDIR)\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)/Drawpic.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/DrawPic.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Drawpic.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/Drawpic.pdb" /debug /machine:I386 /def:".\DrawPic.def"\
 /out:"$(OUTDIR)/Drawpic.ocx" /implib:"$(OUTDIR)/Drawpic.lib" 
DEF_FILE= \
	".\DrawPic.def"
LINK32_OBJS= \
	"$(INTDIR)\DrawPic.obj" \
	"$(INTDIR)\DrawPic.res" \
	"$(INTDIR)\DrawPicC.obj" \
	"$(INTDIR)\DrawPicP.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\Drawpic.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Registering OLE control...
OutDir=.\Debug
TargetPath=.\Debug\Drawpic.ocx
InputPath=.\Debug\Drawpic.ocx
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   regsvr32 /s /c "$(TargetPath)"
   echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg"

# End Custom Build

!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP BASE Target_Ext "ocx"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Dir ""
# PROP Target_Ext "ocx"
OUTDIR=.\DebugU
INTDIR=.\DebugU
# Begin Custom Macros
OutDir=.\DebugU
# End Custom Macros

ALL : "$(OUTDIR)\Drawpic.ocx" "$(OUTDIR)\regsvr32.trg"

CLEAN : 
	-@erase "$(INTDIR)\DrawPic.obj"
	-@erase "$(INTDIR)\Drawpic.pch"
	-@erase "$(INTDIR)\DrawPic.res"
	-@erase "$(INTDIR)\DrawPic.tlb"
	-@erase "$(INTDIR)\DrawPicC.obj"
	-@erase "$(INTDIR)\DrawPicP.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Drawpic.exp"
	-@erase "$(OUTDIR)\Drawpic.ilk"
	-@erase "$(OUTDIR)\Drawpic.lib"
	-@erase "$(OUTDIR)\Drawpic.ocx"
	-@erase "$(OUTDIR)\Drawpic.pdb"
	-@erase "$(OUTDIR)\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)/Drawpic.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\DebugU/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/DrawPic.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Drawpic.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/Drawpic.pdb" /debug /machine:I386 /def:".\DrawPic.def"\
 /out:"$(OUTDIR)/Drawpic.ocx" /implib:"$(OUTDIR)/Drawpic.lib" 
DEF_FILE= \
	".\DrawPic.def"
LINK32_OBJS= \
	"$(INTDIR)\DrawPic.obj" \
	"$(INTDIR)\DrawPic.res" \
	"$(INTDIR)\DrawPicC.obj" \
	"$(INTDIR)\DrawPicP.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\Drawpic.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Registering OLE control...
OutDir=.\DebugU
TargetPath=.\DebugU\Drawpic.ocx
InputPath=.\DebugU\Drawpic.ocx
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   regsvr32 /s /c "$(TargetPath)"
   echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg"

# End Custom Build

!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseU"
# PROP BASE Intermediate_Dir "ReleaseU"
# PROP BASE Target_Dir ""
# PROP BASE Target_Ext "ocx"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Target_Dir ""
# PROP Target_Ext "ocx"
OUTDIR=.\ReleaseU
INTDIR=.\ReleaseU
# Begin Custom Macros
OutDir=.\ReleaseU
# End Custom Macros

ALL : "$(OUTDIR)\Drawpic.ocx" "$(OUTDIR)\regsvr32.trg"

CLEAN : 
	-@erase "$(INTDIR)\DrawPic.obj"
	-@erase "$(INTDIR)\Drawpic.pch"
	-@erase "$(INTDIR)\DrawPic.res"
	-@erase "$(INTDIR)\DrawPic.tlb"
	-@erase "$(INTDIR)\DrawPicC.obj"
	-@erase "$(INTDIR)\DrawPicP.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\Drawpic.exp"
	-@erase "$(OUTDIR)\Drawpic.lib"
	-@erase "$(OUTDIR)\Drawpic.ocx"
	-@erase "$(OUTDIR)\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)/Drawpic.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\ReleaseU/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/DrawPic.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Drawpic.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/Drawpic.pdb" /machine:I386 /def:".\DrawPic.def"\
 /out:"$(OUTDIR)/Drawpic.ocx" /implib:"$(OUTDIR)/Drawpic.lib" 
DEF_FILE= \
	".\DrawPic.def"
LINK32_OBJS= \
	"$(INTDIR)\DrawPic.obj" \
	"$(INTDIR)\DrawPic.res" \
	"$(INTDIR)\DrawPicC.obj" \
	"$(INTDIR)\DrawPicP.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\Drawpic.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Registering OLE control...
OutDir=.\ReleaseU
TargetPath=.\ReleaseU\Drawpic.ocx
InputPath=.\ReleaseU\Drawpic.ocx
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   regsvr32 /s /c "$(TargetPath)"
   echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg"

# End Custom Build

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

# Name "DrawPic - Win32 Release"
# Name "DrawPic - Win32 Debug"
# Name "DrawPic - Win32 Unicode Debug"
# Name "DrawPic - Win32 Unicode Release"

!IF  "$(CFG)" == "DrawPic - Win32 Release"

!ELSEIF  "$(CFG)" == "DrawPic - Win32 Debug"

!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "DrawPic - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)/Drawpic.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Drawpic.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "DrawPic - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)/Drawpic.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Drawpic.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Debug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)/Drawpic.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Drawpic.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Release"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)/Drawpic.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Drawpic.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DrawPic.cpp
DEP_CPP_DRAWP=\
	".\DrawPic.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "DrawPic - Win32 Release"


"$(INTDIR)\DrawPic.obj" : $(SOURCE) $(DEP_CPP_DRAWP) "$(INTDIR)"\
 "$(INTDIR)\Drawpic.pch"


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Debug"


"$(INTDIR)\DrawPic.obj" : $(SOURCE) $(DEP_CPP_DRAWP) "$(INTDIR)"\
 "$(INTDIR)\Drawpic.pch"


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Debug"


"$(INTDIR)\DrawPic.obj" : $(SOURCE) $(DEP_CPP_DRAWP) "$(INTDIR)"\
 "$(INTDIR)\Drawpic.pch"


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Release"


"$(INTDIR)\DrawPic.obj" : $(SOURCE) $(DEP_CPP_DRAWP) "$(INTDIR)"\
 "$(INTDIR)\Drawpic.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DrawPic.def

!IF  "$(CFG)" == "DrawPic - Win32 Release"

!ELSEIF  "$(CFG)" == "DrawPic - Win32 Debug"

!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DrawPic.rc

!IF  "$(CFG)" == "DrawPic - Win32 Release"

DEP_RSC_DRAWPI=\
	".\Drawpic.ico"\
	".\Drawpicc.bmp"\
	
NODEP_RSC_DRAWPI=\
	".\Release\DrawPic.tlb"\
	

"$(INTDIR)\DrawPic.res" : $(SOURCE) $(DEP_RSC_DRAWPI) "$(INTDIR)"\
 "$(INTDIR)\DrawPic.tlb"
   $(RSC) /l 0x409 /fo"$(INTDIR)/DrawPic.res" /i "Release" /d "NDEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Debug"

DEP_RSC_DRAWPI=\
	".\Debug\DrawPic.tlb"\
	".\Drawpic.ico"\
	".\Drawpicc.bmp"\
	

"$(INTDIR)\DrawPic.res" : $(SOURCE) $(DEP_RSC_DRAWPI) "$(INTDIR)"\
 "$(INTDIR)\DrawPic.tlb"
   $(RSC) /l 0x409 /fo"$(INTDIR)/DrawPic.res" /i "Debug" /d "_DEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Debug"

DEP_RSC_DRAWPI=\
	".\Drawpic.ico"\
	".\Drawpicc.bmp"\
	
NODEP_RSC_DRAWPI=\
	".\DebugU\DrawPic.tlb"\
	

"$(INTDIR)\DrawPic.res" : $(SOURCE) $(DEP_RSC_DRAWPI) "$(INTDIR)"\
 "$(INTDIR)\DrawPic.tlb"
   $(RSC) /l 0x409 /fo"$(INTDIR)/DrawPic.res" /i "DebugU" /d "_DEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Release"

DEP_RSC_DRAWPI=\
	".\Drawpic.ico"\
	".\Drawpicc.bmp"\
	
NODEP_RSC_DRAWPI=\
	".\ReleaseU\DrawPic.tlb"\
	

"$(INTDIR)\DrawPic.res" : $(SOURCE) $(DEP_RSC_DRAWPI) "$(INTDIR)"\
 "$(INTDIR)\DrawPic.tlb"
   $(RSC) /l 0x409 /fo"$(INTDIR)/DrawPic.res" /i "ReleaseU" /d "NDEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DrawPic.odl

!IF  "$(CFG)" == "DrawPic - Win32 Release"


"$(OUTDIR)\DrawPic.tlb" : $(SOURCE) "$(OUTDIR)"
   $(MTL) /nologo /D "NDEBUG" /tlb "$(OUTDIR)/DrawPic.tlb" /win32 $(SOURCE)


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Debug"


"$(OUTDIR)\DrawPic.tlb" : $(SOURCE) "$(OUTDIR)"
   $(MTL) /nologo /D "_DEBUG" /tlb "$(OUTDIR)/DrawPic.tlb" /win32 $(SOURCE)


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Debug"


"$(OUTDIR)\DrawPic.tlb" : $(SOURCE) "$(OUTDIR)"
   $(MTL) /nologo /D "_DEBUG" /tlb "$(OUTDIR)/DrawPic.tlb" /win32 $(SOURCE)


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Release"


"$(OUTDIR)\DrawPic.tlb" : $(SOURCE) "$(OUTDIR)"
   $(MTL) /nologo /D "NDEBUG" /tlb "$(OUTDIR)/DrawPic.tlb" /win32 $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DrawPicC.cpp
DEP_CPP_DRAWPIC=\
	".\DrawPic.h"\
	".\DrawPicC.h"\
	".\DrawPicP.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "DrawPic - Win32 Release"


"$(INTDIR)\DrawPicC.obj" : $(SOURCE) $(DEP_CPP_DRAWPIC) "$(INTDIR)"\
 "$(INTDIR)\Drawpic.pch"


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Debug"


"$(INTDIR)\DrawPicC.obj" : $(SOURCE) $(DEP_CPP_DRAWPIC) "$(INTDIR)"\
 "$(INTDIR)\Drawpic.pch"


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Debug"


"$(INTDIR)\DrawPicC.obj" : $(SOURCE) $(DEP_CPP_DRAWPIC) "$(INTDIR)"\
 "$(INTDIR)\Drawpic.pch"


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Release"


"$(INTDIR)\DrawPicC.obj" : $(SOURCE) $(DEP_CPP_DRAWPIC) "$(INTDIR)"\
 "$(INTDIR)\Drawpic.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DrawPicP.cpp
DEP_CPP_DRAWPICP=\
	".\DrawPic.h"\
	".\DrawPicP.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "DrawPic - Win32 Release"


"$(INTDIR)\DrawPicP.obj" : $(SOURCE) $(DEP_CPP_DRAWPICP) "$(INTDIR)"\
 "$(INTDIR)\Drawpic.pch"


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Debug"


"$(INTDIR)\DrawPicP.obj" : $(SOURCE) $(DEP_CPP_DRAWPICP) "$(INTDIR)"\
 "$(INTDIR)\Drawpic.pch"


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Debug"


"$(INTDIR)\DrawPicP.obj" : $(SOURCE) $(DEP_CPP_DRAWPICP) "$(INTDIR)"\
 "$(INTDIR)\Drawpic.pch"


!ELSEIF  "$(CFG)" == "DrawPic - Win32 Unicode Release"


"$(INTDIR)\DrawPicP.obj" : $(SOURCE) $(DEP_CPP_DRAWPICP) "$(INTDIR)"\
 "$(INTDIR)\Drawpic.pch"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
